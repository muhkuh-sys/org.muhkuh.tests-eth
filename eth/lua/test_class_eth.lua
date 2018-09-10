local class = require 'pl.class'
local TestClassEth = class()

function TestClassEth:_init(strTestName)
  self.parameters = require 'parameters'
  self.pl = require'pl.import_into'()

  self.CFG_strTestName = strTestName

  self.CFG_aParameterDefinitions = {
--[[
    {
      name="expected_boot_mode",
      default=nil,
      help="The expected boot mode.",
      mandatory=true,
      validate=parameters.test_uint32,
      constrains=nil
    },
    {
      name="expected_strapping_options",
      default=0,
      help="The expected strapping options.",
      mandatory=false,
      validate=parameters.test_uint32,
      constrains=nil
    },
    {
      name="expected_chip_id",
      default=nil,
      help="The expected chip ID.",
      mandatory=true,
      validate=parameters.test_choice_single,
      constrains="NETX500,NETX100,NETX50,NETX10,NETX51A_NETX50_COMPATIBILITY_MODE,NETX51B_NETX50_COMPATIBILITY_MODE,NETX51A,NETX51B,NETX52A,NETX52B,NETX4000_RELAXED,NETX4000_FULL,NETX4000_SMALL,NETX90_MPW,NETX90"
    }
--]]
  }
end



function TestClassEth:run(aParameters, tLog)
  ----------------------------------------------------------------------
  --
  -- Parse the parameters and collect all options.
  --
--  local ulExpectedBootMode         = tonumber(aParameters["expected_boot_mode"])
--  local ulExpectedStrappingOptions = tonumber(aParameters["expected_strapping_options"])
--  local strExpectedChipId          = aParameters["expected_chip_id"]
--  local ulExpectedChipId = self.bootpins.atChipID[strExpectedChipId]
--  if ulExpectedChipId==nil then
--    error('Unknown chip ID: "%s"', strExpectedChipId)
--  end

  ----------------------------------------------------------------------
  --
  -- Open the connection to the netX.
  -- (or re-use an existing connection.)
  --
  local tPlugin = tester.getCommonPlugin()
  if tPlugin==nil then
    error("No plug-in selected, nothing to do!")
  end

  local astrBinaryName = {
    [romloader.ROMLOADER_CHIPTYP_NETX4000_RELAXED] = '4000',
    [romloader.ROMLOADER_CHIPTYP_NETX4000_FULL]    = '4000',
    [romloader.ROMLOADER_CHIPTYP_NETX4100_SMALL]   = '4000',
--    [romloader.ROMLOADER_CHIPTYP_NETX500]          = '500',
--    [romloader.ROMLOADER_CHIPTYP_NETX100]          = '500',
--    [romloader.ROMLOADER_CHIPTYP_NETX90_MPW]       = '90_mpw',
--    [romloader.ROMLOADER_CHIPTYP_NETX90]           = '90',
--    [romloader.ROMLOADER_CHIPTYP_NETX56]           = '56',
--    [romloader.ROMLOADER_CHIPTYP_NETX56B]          = '56',
--    [romloader.ROMLOADER_CHIPTYP_NETX50]           = '50',
--    [romloader.ROMLOADER_CHIPTYP_NETX10]           = '10'
  }

  -- Get the binary for the ASIC.
  local tAsicTyp = tPlugin:GetChiptyp()
  local strBinary = astrBinaryName[tAsicTyp]
  if strBinary==nil then
    error('No binary for chip type %s.', tAsicTyp)
  end
  local strNetxBinary = string.format('netx/eth_netx%s.bin', strBinary)

  local aParameter = {
    0
  }
  local ulResult = tester.mbin_simple_run(nil, tPlugin, strNetxBinary, aParameter)
  if ulResult~=0 then
    error('The test failed with return code:' .. ulResult)
  end

  print("")
  print(" #######  ##    ## ")
  print("##     ## ##   ##  ")
  print("##     ## ##  ##   ")
  print("##     ## #####    ")
  print("##     ## ##  ##   ")
  print("##     ## ##   ##  ")
  print(" #######  ##    ## ")
  print("")
end


return TestClassEth
