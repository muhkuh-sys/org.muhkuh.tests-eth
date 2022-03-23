local __APPLICATION__ = 'patch_eth_standalone'
local __VERSION__ = '0.0.1'

local argparse = require 'argparse'
local dkjson = require 'dkjson'
local mhash = require 'mhash'
local pl = require'pl.import_into'()
local vstruct = require 'vstruct'

local atLogLevels = {
  'debug',
  'info',
  'warning',
  'error',
  'fatal'
}
local atTranslateLogLevels = {
  ['debug'] = 'debug',
  ['info'] = 'info',
  ['warning'] = 'warning',
  ['error'] = 'error',
  ['fatal'] = 'fatal'
}

local atFileOutputTypes = {
  ['BIN'] = 'BIN',
  ['JSON'] = 'JSON',
  ['TXT'] = 'TXT'
}
local atFileOutputKeys = pl.tablex.keys(atFileOutputTypes)
table.sort(atFileOutputKeys)

local atInterfaceName2Index = {
  INTERFACE_None            = 0,
  INTERFACE_ETHMAC_INTPHY0  = 1,
  INTERFACE_ETHMAC_INTPHY1  = 2,
  INTERFACE_ETHMAC_EXTPHY0  = 3,
  INTERFACE_ETHMAC_EXTPHY1  = 4,
  INTERFACE_ETH2PS_INTPHY0  = 5,
  INTERFACE_ETH2PS_INTPHY1  = 6,
  INTERFACE_ETH2PS_EXTSPE0  = 7
}
local atInterfaceIndex2Name = {}
for k,v in pairs(atInterfaceName2Index) do
  atInterfaceIndex2Name[v] = k
end

local atInterfaceFunctionName2Index = {
  INTERFACE_FUNCTION_None       = 0,
  INTERFACE_FUNCTION_EchoServer = 1,
  INTERFACE_FUNCTION_EchoClient = 2
}
local atInterfaceFunctionIndex2Name = {}
for k,v in pairs(atInterfaceFunctionName2Index) do
  atInterfaceFunctionIndex2Name[v] = k
end

local atEthernetPortFlagsName2Bit = {
  ETHERNET_PORT_FLAG_Permanent       = 0x00000001,
  ETHERNET_PORT_FLAG_LinkDownAllowed = 0x00000002
}

local atExpectedLinkAttributesName2Index = {
  EXPECTED_LINK_ATTRIBUTES_ANY      = 0,
  EXPECTED_LINK_ATTRIBUTES_10_HALF  = 1,
  EXPECTED_LINK_ATTRIBUTES_10_FULL  = 2,
  EXPECTED_LINK_ATTRIBUTES_100_HALF = 3,
  EXPECTED_LINK_ATTRIBUTES_100_FULL = 4
}
local atExpectedLinkAttributesIndex2Name = {}
for k,v in pairs(atExpectedLinkAttributesName2Index) do
  atExpectedLinkAttributesIndex2Name[v] = k
end

-- Read a maximum of 512kB from the binary input file.
local sizBinaryMaxInBytes = 512*1024

local tRapaStruct = vstruct.compile([[
  ulMagic:u4
  ulStructureVersion:u4
  ulVerbose:u4
  ulLinkUpTimeout:u4
  ulMaximumTransferTime:u4
  atPortConfiguration:{
    2*{
      acName:z16
      ulInterface:u4
      ulFunction:u4
      ulFlags:u4
      ulExpectedLinkAttributes:u4
      ulIp:u4
      ulGatewayIp:u4
      ulNetmask:u4
      ulRemoteIp:u4
      ulNumberOfTestPackets:u4
      usLinkUpDelay:u2
      usLocalPort:u2
      usRemotePort:u2
      aucMac:s6
    }
  }
]])
local sizRapaStruct = 148

------------------------------------------------------------------------------
--
-- Parser and formatter for the data.
--
local function formatStructureVersion(ulInput)
  return string.format('%d.%d', (ulInput>>16)&0xffff, ulInput&0xffff)
end

local function parseStructureVersion(strInput)
  local ulResult
  local ulVersionHi, ulVersionLo = string.match(strInput, '(%d+)%.(%d+)')
  if ulVersionHi~=nil then
    ulResult = tonumber(ulVersionHi)*0x00010000 | tonumber(ulVersionLo)
  end
  return ulResult
end

local function formatVerbose(ulInput)
  return (ulInput~=0)
end

local function parseVerbose(strInput)
  local ulResult
  local strType = type(strInput)
  if strType=='boolean' then
    if strInput==true then
      ulResult = 1
    else
      ulResult = 0
    end

  elseif strType=='string' then
    local strInputLower = string.lower(strInput)
    if strInputLower=='true' then
      ulResult = 1
    elseif strInputLower=='false' then
      ulResult = 0
    end
  end

  return ulResult
end

local function formatInterface(ulInput)
  return atInterfaceIndex2Name[ulInput]
end

local function parseInterface(strInput)
  return atInterfaceName2Index[strInput]
end

local function formatFunction(ulInput)
  return atInterfaceFunctionIndex2Name[ulInput]
end

local function parseFunction(strInput)
  return atInterfaceFunctionName2Index[strInput]
end

local function formatFlags(ulInput)
  local atResult = {}
  for strName, ulBit in pairs(atEthernetPortFlagsName2Bit) do
    if (ulInput&ulBit)~=0 then
      table.insert(atResult, strName)
    end
  end
  table.sort(atResult)
  return table.concat(atResult, ',')
end

local function parseFlags(strInput)
  local ulResult = 0

  -- Split the input string by commata.
  local atFlags = pl.stringx.split(strInput, ',')
  for _, strFlag in ipairs(atFlags) do
    -- Translate the flag.
    local strFlagStripped = pl.stringx.strip(strFlag)
    local ulBit = atEthernetPortFlagsName2Bit[strFlagStripped]
    if ulBit==nil then
      error(string.format('Unknown flag: "%s"', strFlagStripped))
    end
    ulResult = ulResult | ulBit
  end

  return ulResult
end

local function formatExpectedLinkAttributes(ulInput)
  return atExpectedLinkAttributesIndex2Name[ulInput]
end

local function parseExpectedLinkAttributes(strInput)
  return atExpectedLinkAttributesName2Index[strInput]
end

local function formatIp(ulInput)
  return string.format('%d.%d.%d.%d', ulInput&0xff, (ulInput>>8)&0xff, (ulInput>>16)&0xff, (ulInput>>24)&0xff)
end

local function parseIp(strInput)
  local ulResult
  local ip0, ip1, ip2, ip3 = string.match(strInput, '(%d+)%.(%d+)%.(%d+).(%d+)')
  if ip0~=nil then
    ulResult = tonumber(ip0) | tonumber(ip1)<<8 | tonumber(ip2)<<16 | tonumber(ip3)<<24
  end
  return ulResult
end

local function formatMAC(strInput)
  return string.format('%02x:%02x:%02x:%02x:%02x:%02x', string.byte(strInput, 1, 6))
end

local function parseMAC(strInput)
  local strResult
  local m0, m1, m2, m3, m4, m5 = string.match(strInput, '(%x%x):(%x%x):(%x%x):(%x%x):(%x%x):(%x%x)')
  if m0~=nil then
    strResult = string.char(
      tonumber(m0, 16),
      tonumber(m1, 16),
      tonumber(m2, 16),
      tonumber(m3, 16),
      tonumber(m4, 16),
      tonumber(m5, 16)
    )
  end
  return strResult
end

local atFormatFunctions = {
  ulMagic = nil,
  ulStructureVersion = formatStructureVersion,
  ulVerbose = formatVerbose,
  ulLinkUpTimeout = nil,
  ulMaximumTransferTime = nil,
  atPortConfiguration = {
    {
      acName = nil,
      ulInterface = formatInterface,
      ulFunction = formatFunction,
      ulFlags = formatFlags,
      ulExpectedLinkAttributes = formatExpectedLinkAttributes,
      ulIp = formatIp,
      ulGatewayIp = formatIp,
      ulNetmask = formatIp,
      ulRemoteIp = formatIp,
      ulNumberOfTestPackets = nil,
      usLinkUpDelay = nil,
      usLocalPort = nil,
      usRemotePort = nil,
      aucMac = formatMAC
    },
    {
      acName = nil,
      ulInterface = formatInterface,
      ulFunction = formatFunction,
      ulFlags = formatFlags,
      ulExpectedLinkAttributes = formatExpectedLinkAttributes,
      ulIp = formatIp,
      ulGatewayIp = formatIp,
      ulNetmask = formatIp,
      ulRemoteIp = formatIp,
      ulNumberOfTestPackets = nil,
      usLinkUpDelay = nil,
      usLocalPort = nil,
      usRemotePort = nil,
      aucMac = formatMAC
    }
  }
}

local atParseFunctions = {
  ulMagic = nil,
  ulStructureVersion = parseStructureVersion,
  ulVerbose = parseVerbose,
  ulLinkUpTimeout = nil,
  ulMaximumTransferTime = nil,
  atPortConfiguration = {
    {
      acName = nil,
      ulInterface = parseInterface,
      ulFunction = parseFunction,
      ulFlags = parseFlags,
      ulExpectedLinkAttributes = parseExpectedLinkAttributes,
      ulIp = parseIp,
      ulGatewayIp = parseIp,
      ulNetmask = parseIp,
      ulRemoteIp = parseIp,
      ulNumberOfTestPackets = nil,
      usLinkUpDelay = nil,
      usLocalPort = nil,
      usRemotePort = nil,
      aucMac = parseMAC
    },
    {
      acName = nil,
      ulInterface = parseInterface,
      ulFunction = parseFunction,
      ulFlags = parseFlags,
      ulExpectedLinkAttributes = parseExpectedLinkAttributes,
      ulIp = parseIp,
      ulGatewayIp = parseIp,
      ulNetmask = parseIp,
      ulRemoteIp = parseIp,
      ulNumberOfTestPackets = nil,
      usLinkUpDelay = nil,
      usLocalPort = nil,
      usRemotePort = nil,
      aucMac = parseMAC
    }
  }
}


local function applyFunctions(atInput, atFunctions, atOutput, atPath)
  atOutput = atOutput or {}
  atPath = atPath or {}

  for strKey, tValue in pairs(atInput) do
    local atPathSub = pl.tablex.copy(atPath)
    table.insert(atPathSub, strKey)

    -- Does a function exist for this entry?
    local fnProcess = atFunctions[strKey]
    local strType = type(fnProcess)
    if strType=='function' then
      local tOut = fnProcess(tValue)
      if tOut==nil then
        error(string.format('Failed to process key %s with value %s.', table.concat(atPathSub, '.'), dkjson.encode(tValue)))
      end
      atOutput[strKey] = tOut

    -- Is this a table to recurse in?
    elseif strType=='table' then
      atOutput[strKey] = {}
      local atFunctionsSub
      if atFunctions~=nil then
        atFunctionsSub = atFunctions[strKey]
      end
      applyFunctions(atInput[strKey], atFunctionsSub, atOutput[strKey], atPathSub)

    -- Copy the value from input to output.
    else
      atOutput[strKey] = tValue
    end
  end

  return atOutput
end


local strPrettyPrintTemplate = [[
  ulMagic:               0x$(string.format('%08x', data.ulMagic))
  ulStructureVersion:    $(data.ulStructureVersion)
  ulVerbose:             $(tostring(data.ulVerbose))
  ulLinkUpTimeout:       $(data.ulLinkUpTimeout)ms
  ulMaximumTransferTime: $(data.ulMaximumTransferTime)ms

  PortConfiguration 1:
    acName:                   $(data.atPortConfiguration[1].acName)
    ulInterface:              $(data.atPortConfiguration[1].ulInterface)
    ulFunction:               $(data.atPortConfiguration[1].ulFunction)
    ulFlags:                  $(data.atPortConfiguration[1].ulFlags)
    ulExpectedLinkAttributes: $(data.atPortConfiguration[1].ulExpectedLinkAttributes)
    ulIp:                     $(data.atPortConfiguration[1].ulIp)
    ulGatewayIp:              $(data.atPortConfiguration[1].ulGatewayIp)
    ulNetmask:                $(data.atPortConfiguration[1].ulNetmask)
    ulRemoteIp:               $(data.atPortConfiguration[1].ulRemoteIp)
    ulNumberOfTestPackets:    $(data.atPortConfiguration[1].ulNumberOfTestPackets)
    usLinkUpDelay:            $(data.atPortConfiguration[1].usLinkUpDelay)
    usLocalPort:              $(data.atPortConfiguration[1].usLocalPort)
    usRemotePort:             $(data.atPortConfiguration[1].usRemotePort)
    aucMac:                   $(data.atPortConfiguration[1].aucMac)

  PortConfiguration 2:
    acName:                   $(data.atPortConfiguration[2].acName)
    ulInterface:              $(data.atPortConfiguration[2].ulInterface)
    ulFunction:               $(data.atPortConfiguration[2].ulFunction)
    ulFlags:                  $(data.atPortConfiguration[2].ulFlags)
    ulExpectedLinkAttributes: $(data.atPortConfiguration[2].ulExpectedLinkAttributes)
    ulIp:                     $(data.atPortConfiguration[2].ulIp)
    ulGatewayIp:              $(data.atPortConfiguration[2].ulGatewayIp)
    ulNetmask:                $(data.atPortConfiguration[2].ulNetmask)
    ulRemoteIp:               $(data.atPortConfiguration[2].ulRemoteIp)
    ulNumberOfTestPackets:    $(data.atPortConfiguration[2].ulNumberOfTestPackets)
    usLinkUpDelay:            $(data.atPortConfiguration[2].usLinkUpDelay)
    usLocalPort:              $(data.atPortConfiguration[2].usLocalPort)
    usRemotePort:             $(data.atPortConfiguration[2].usRemotePort)
    aucMac:                   $(data.atPortConfiguration[2].aucMac)
]]


local function prettyPrint(atInput)
  local template = require 'pl.template'
  local tEnv = {
    string = string,
    tostring = tostring,

    data = atInput
  }
  local tResult, strError = template.substitute(strPrettyPrintTemplate, tEnv)
  if tResult==nil then
    error(string.format('Failed to pretty print the data: %s', strError))
  end
  return tResult
end

------------------------------------------------------------------------------

local function mergeTables(tLog, atMainTable, atPatchTable, atPath)
  atPath = atPath or {}

  -- Loop over all elements in the patch table.
  for strKey, tPatchValue in pairs(atPatchTable) do
    local atPathSub = pl.tablex.copy(atPath)
    table.insert(atPathSub, strKey)
    local strPathSub = table.concat(atPathSub, '.')

    -- Get the corresponding entry in the main table.
    local tMainValue = atMainTable[strKey]
    if tMainValue==nil then
      error(string.format('The patch table entry does not exist in the main table: %s', strPathSub))
    end

    -- Get the type of the main and patch entry.
    local strMainType = type(tMainValue)
    local strPatchType = type(tPatchValue)
    -- Complain if the types differ.
    if strMainType~=strPatchType then
      error(string.format('Expected a %s at %s, but found a %s in the patch table.',
                          strMainType, strPathSub, strPatchType))
    end
    -- Recurse into subtables.
    if strMainType=='table' then
      mergeTables(tLog, tMainValue, tPatchValue, atPathSub)

    else
      tLog.debug('Patched %s = %s', strPathSub, dkjson.encode(tPatchValue))
      atMainTable[strKey] = tPatchValue
    end
  end
end


------------------------------------------------------------------------------


local tBootimageHeaderStruct = vstruct.compile([[
  ulMagic:u4
  ulBootControl:u4
  pfnExecutionAddress:u4
  ulApplicationChecksum:u4
  sizApplicationDword:u4
  pulApplicationLoadAddress:u4
  ulSignature:u4
  ulMemoryControl0:u4
  ulMemoryControl1:u4
  ulMemoryControl2:u4
  ulMemoryControl3:u4
  ulMemoryControl4:u4
  ulMiscAsicCtrl:u4
  ulUserData:u4
  ulSrcType:u4
  ulBootblockChecksum:u4
]])
local sizBootimageHeaderStruct = 64


local tHBootChunkHeaderStruct = vstruct.compile([[
  acName:s4
  ulSize:u4
]])
local sizHBootChunkHeaderStruct = 8


local function fix_aboot_image(tLog, strInputImage)
  -- Read the boot header from the input image.
  local tBootHeader = tBootimageHeaderStruct:read(strInputImage)

  -- Generate the application checksum.
  local ulApplicationChecksum = 0
  local sizInputImage = string.len(strInputImage)
  for uiOffset=sizBootimageHeaderStruct+1,sizInputImage do
    local uiShift = ((uiOffset-1)&3)<<3
    ulApplicationChecksum = (ulApplicationChecksum + string.byte(strInputImage, uiOffset)<<uiShift) & 0xffffffff
  end
  tBootHeader.ulApplicationChecksum = ulApplicationChecksum

  -- Update the header checksum.
  local strTempHeader = tBootimageHeaderStruct:write(tBootHeader)
  local ulBootblockChecksum = 0
  for uiOffset=1,sizBootimageHeaderStruct-4 do
    local uiShift = ((uiOffset-1)&3)<<3
    ulBootblockChecksum = (ulBootblockChecksum + string.byte(strTempHeader, uiOffset)<<uiShift) & 0xffffffff
  end
  ulBootblockChecksum = (ulBootblockChecksum - 1) ^ 0xffffffff
  tBootHeader.ulBootblockChecksum = ulBootblockChecksum

  return tBootimageHeaderStruct:write(tBootHeader) .. string.sub(strInputImage, 65)
end


local function fix_hboot_image(tLog, strInputImage)
  local atOutputImage = {}

  -- Add the header to the output image.
  table.insert(atOutputImage, string.sub(strInputImage, 1, sizBootimageHeaderStruct))

  -- Loop over all chunks.
  local uiPos = sizBootimageHeaderStruct+1
  local sizInputImage = string.len(strInputImage)
  while uiPos<sizInputImage do
    -- Check for end of image.
    local strChunkID = string.sub(strInputImage, uiPos, uiPos+3)
    if strChunkID==string.char(0x00, 0x00, 0x00, 0x00) or strChunkID==string.char(0xff, 0xff, 0xff, 0xff) then
      tLog.debug('Found HBOOT end of image at offset %d.', uiPos-1)
      table.insert(atOutputImage, string.sub(strInputImage, uiPos))
      break

    else
      -- Read the next chunk header.
      local tChunkHeader = tHBootChunkHeaderStruct:read(
        string.sub(strInputImage, uiPos, uiPos+sizHBootChunkHeaderStruct-1)
      )
      tLog.debug('Found chunk "%s" at offset 0x%08x with 0x%0x bytes.',
                 tChunkHeader.acName, uiPos-1, tChunkHeader.ulSize*4)
      local tH = mhash.mhash_state()
      tH:init(mhash.MHASH_SHA384)
      if tChunkHeader.acName=='SKIP' then
        tH:hash(string.sub(strInputImage, uiPos, uiPos+7))
        local strHashMy = string.sub(tH:hash_end(), 1, 4)
        local strHashImg = string.sub(strInputImage, uiPos+8, uiPos+11)
        if strHashMy==strHashImg then
          tLog.debug('Hash OK')
        else
          tLog.debug('Hash ERROR, fixing...')
          strHashImg = strHashMy
        end
        -- Add the "SKIP" header to the output image.
        table.insert(atOutputImage, string.sub(strInputImage, uiPos, uiPos+7))
        -- Add the hash to the output image.
        table.insert(atOutputImage, strHashImg)
        -- Add the "SKIP" data to the output image.
        table.insert(atOutputImage, string.sub(strInputImage, uiPos+12, uiPos+8+tChunkHeader.ulSize*4-1))

        -- Increase the position. This is 8 bytes for the header, then the skip size and finally the hash size.
        uiPos = uiPos + 8 + tChunkHeader.ulSize*4
      else
        local strChunk = string.sub(strInputImage, uiPos, uiPos + 4 + tChunkHeader.ulSize*4 - 1)
        -- Add the chunk to the output image.
        table.insert(atOutputImage, strChunk)
        -- Get the hash for the chunk.
        tH:hash(strChunk)
        local strHashMy = string.sub(tH:hash_end(), 1, 4)
        -- Compare the calculated hash with the image.
        local strHashImg = string.sub(strInputImage, uiPos+4+tChunkHeader.ulSize*4, uiPos+4+tChunkHeader.ulSize*4+3)
        if strHashMy==strHashImg then
          tLog.debug('Hash OK')
        else
          tLog.debug('Hash ERROR, fixing...')
          strHashImg = strHashMy
        end
        -- Add the hash to the output image.
        table.insert(atOutputImage, strHashImg)

        -- Increase the position.
        uiPos = uiPos + 4 + tChunkHeader.ulSize*4 + 4
      end
    end
  end

  return table.concat(atOutputImage)
end


local function bootimage_update(tLog, strInputImage)
  local tResult

  -- Parse the first 64 bytes.
  local tBootHeader = tBootimageHeaderStruct:read(strInputImage)
  local ulMagic = tBootHeader.ulMagic
  if ulMagic==0xf8beaf00 then
    -- Fix an ABOOT image.
    tResult = fix_aboot_image(tLog, strInputImage)

  elseif ulMagic==0xf3beaf00 then
    -- FIx a HBOOT image.
    tResult = fix_hboot_image(tLog, strInputImage)

  else
    error(string.format('Unknown bootimage magic: 0x%08x.', ulMagic))
  end

  return tResult
end


------------------------------------------------------------------------------


local tParser = argparse(__APPLICATION__, 'Patch Ethernet standalone binaries.')
tParser:flag('--version')
  :description('Show the version and exit.')
  :action(function()
    print(string.format('%s %s', __APPLICATION__, __VERSION__))
    print('Copyright (C) 2022 by Christoph Thelen (doc_bacardi@users.sourceforge.net)')
    os.exit(0)
  end)
tParser:argument('input')
  :argname('<IN_FILE>')
  :description('Read the input data from IN_FILE.')
  :target('strInputFile')
tParser:argument('output')
  :argname('<OUT_FILE>')
  :description('Write the output data to OUT_FILE.')
  :target('strOutputFile')
  :args('?')
tParser:option('-p --patch')
  :description('Patch the input data with values from the JSON file PATCH_FILE.')
  :argname('<PATCH_FILE>')
  :default(nil)
  :target('strPatchFile')
tParser:option('--output-type')
  :description(string.format('Do not guess the type of the output file but set it to OUT_TYPE. Possible values for OUT_TYPE are %s.', table.concat(atFileOutputKeys, ', ')))
  :argname('<OUT_TYPE>')
  :convert(atFileOutputTypes)
  :default(nil)
  :target('strOutputType')
tParser:option('-v --verbose')
  :description(string.format('Set the verbosity level to LEVEL. Possible values for LEVEL are %s.', table.concat(atLogLevels, ', ')))
  :argname('<LEVEL>')
  :convert(atTranslateLogLevels)
  :default('warning')
  :target('strLogLevel')
tParser:option('-l --logfile')
  :description('Write all output to FILE.')
  :argname('<FILE>')
  :default(nil)
  :target('strLogFileName')
tParser:mutex(
  tParser:flag('--color')
    :description('Use colors to beautify the console output. This is the default on Linux.')
    :action("store_true")
    :target('fUseColor'),
  tParser:flag('--no-color')
    :description('Do not use colors for the console output. This is the default on Windows.')
    :action("store_false")
    :target('fUseColor')
)
local tArgs = tParser:parse()

-----------------------------------------------------------------------------
--
-- Create a log writer.
--

local fUseColor = tArgs.fUseColor
if fUseColor==nil then
  if pl.path.is_windows==true then
    -- Running on windows. Do not use colors by default as cmd.exe
    -- does not support ANSI on all windows versions.
    fUseColor = false
  else
    -- Running on Linux. Use colors by default.
    fUseColor = true
  end
end

-- Collect all log writers.
local atLogWriters = {}

-- Create the console logger.
local tLogWriterConsole
if fUseColor==true then
  tLogWriterConsole = require 'log.writer.console.color'.new()
else
  tLogWriterConsole = require 'log.writer.console'.new()
end
table.insert(atLogWriters, tLogWriterConsole)

-- Create the file logger if requested.
local tLogWriterFile
if tArgs.strLogFileName~=nil then
  tLogWriterFile = require 'log.writer.file'.new{ log_name=tArgs.strLogFileName }
  table.insert(atLogWriters, tLogWriterFile)
end

-- Combine all writers.
local tLogWriter
if _G.LUA_VER_NUM==501 then
  tLogWriter = require 'log.writer.list'.new(unpack(atLogWriters))
else
  tLogWriter = require 'log.writer.list'.new(table.unpack(atLogWriters))
end

-- Set the logger level from the command line options.
local cLogWriter = require 'log.writer.filter'.new(tArgs.strLogLevel, tLogWriter)
local tLog = require "log".new(
  -- maximum log level
  "trace",
  cLogWriter,
  -- Formatter
  require "log.formatter.format".new()
)


------------------------------------------------------------------------------
--
-- Guess the type of the output file if it is not fixed.
-- This is rather simple: if no output file is set, then the default format is
-- "TXT". If the file suffix is ".json" then the type is "JSON". Otherwise it
-- is "BIN".
--
local strOutputType = tArgs.strOutputType
if tArgs.strOutputFile==nil then
  strOutputType = 'TXT'
elseif strOutputType==nil then
  tLog.debug('Guessing the type of the output file based on the file name "%s".', tArgs.strOutputFile)
  local strSuffix = pl.path.extension(tArgs.strOutputFile)
  if strSuffix=='.json' then
    strOutputType = 'JSON'
  elseif strSuffix=='.txt' then
    strOutputType = 'TXT'
  else
    strOutputType = 'BIN'
  end
  tLog.debug('  Guessed file type "%s".', strOutputType)
end



------------------------------------------------------------------------------
--
-- Read the input file.
--
tLog.info('Reading "%s".', tArgs.strInputFile)
local tFileIn, strError = io.open(tArgs.strInputFile, 'rb')
if tFileIn==nil then
  tLog.error('Failed to read the input file from "%s": %s', tArgs.strInputFile, strError)
  error('Failed to read the input file.')
end
local strInputData = tFileIn:read(sizBinaryMaxInBytes)
if strInputData==nil then
  tLog.error('Failed to read the input file "%s".', tArgs.strInputFile)
  error('Failed to read the input file.')
end
tFileIn:close()


------------------------------------------------------------------------------
--
-- Search the "RAPA" magic and extract the structure.
--
local uiStructureStart = string.find(strInputData, 'RAPA', 1, true)
if uiStructureStart==nil then
  local strMsg = 'Failed to find the magic in the input binary. Is this really an Ethernet standalone test?'
  tLog.error(strMsg)
  error(strMsg)
end
-- Are enough bytes left after the magic for the structure?
local strRapaData = string.sub(strInputData, uiStructureStart, uiStructureStart+sizRapaStruct-1)
local sizRapaData = string.len(strRapaData)
if sizRapaData~=sizRapaStruct then
  local strMsg = string.format('Not enough structure data found. Expected %d bytes, but got only %d.',
                               sizRapaStruct, sizRapaData)
  tLog.error(strMsg)
  error(strMsg)
end
-- Parse the structure data.
local tRapaData = tRapaStruct:read(strRapaData)

-- Apply all format functions.
local atFormattedOutput = applyFunctions(tRapaData, atFormatFunctions)


------------------------------------------------------------------------------
--
-- Do something with the data.
--
if tArgs.strPatchFile~=nil then
  tLog.info('Patching with "%s"...', tArgs.strPatchFile)

  -- Read the complete file.
  local strPatchData, strPatchError = pl.utils.readfile(tArgs.strPatchFile, false)
  if strPatchData==nil then
    tLog.error('Failed to read the patch file "%s": %s.', tArgs.strPatchFile, tostring(strPatchError))
    error('Failed to read the patch file.')
  end

  -- Convert the JSON data to a table.
  local tPatchData, strJsonError = dkjson.decode(strPatchData)
  if tPatchData==nil then
    tLog.error('Failed to parse the JSON data: %s', strJsonError)
    error('Patch file is no valid JSON.')
  end

  mergeTables(tLog, atFormattedOutput, tPatchData)
end


------------------------------------------------------------------------------
--
-- Write the data to the selected output.
--
local strOutputData
if strOutputType=='BIN' then
  -- Convert the formatted data back to unformatted data.
  local atUnformattedData = applyFunctions(atFormattedOutput, atParseFunctions)

  -- Convert the unformatted data to binary.
  local strStructData = tRapaStruct:write(atUnformattedData)

  -- Replace the structure data in the input binary.
  strOutputData = string.sub(strInputData, 1, uiStructureStart-1) ..
                  strStructData ..
                  string.sub(strInputData, uiStructureStart+sizRapaStruct)

  -- Update the checksums in the boot image.
  strOutputData = bootimage_update(tLog, strOutputData)

elseif strOutputType=='JSON' then
  strOutputData = dkjson.encode(atFormattedOutput)

else
  pl.pretty.dump(atFormattedOutput)
  strOutputData = prettyPrint(atFormattedOutput)
end

if tArgs.strOutputFile==nil then
  print(strOutputData)

else
  tLog.info('Writing "%s".', tArgs.strOutputFile)
  if strOutputType=='BIN' then
    local tFile = io.open(tArgs.strOutputFile, 'wb')
    tFile:write(strOutputData)
    tFile:close()

  else
    local tFile = io.open(tArgs.strOutputFile, 'w')
    tFile:write(strOutputData)
    tFile:close()
  end
end
