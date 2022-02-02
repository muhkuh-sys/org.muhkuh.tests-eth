local class = require 'pl.class'
local TestClass = require 'test_class'
local TestClassEth = class(TestClass)

function TestClassEth:_init(strTestName, uiTestCase, tLogWriter, strLogLevel)
  self:super(strTestName, uiTestCase, tLogWriter, strLogLevel)

  local pl = require'pl.import_into'()
  self.pl = pl
  self.bit = require 'bit'
  self.vstruct = require 'vstruct'

  local atInterface = {
    ['None']     = ${INTERFACE_None},
    ['INTPHY0']  = ${INTERFACE_INTPHY0},
    ['INTPHY1']  = ${INTERFACE_INTPHY1},
    ['EXTPHY0']  = ${INTERFACE_EXTPHY0},
    ['EXTPHY1']  = ${INTERFACE_EXTPHY1},
    ['LVDS0']    = ${INTERFACE_LVDS0},
    ['LVDS1']    = ${INTERFACE_LVDS1}
  }
  self.atInterface = atInterface
  local strInterfaces = table.concat(pl.tablex.keys(atInterface), ',')

  local atInterfaceFunction = {
    ['None']       = ${INTERFACE_FUNCTION_None},
    ['EchoServer'] = ${INTERFACE_FUNCTION_EchoServer},
    ['EchoClient'] = ${INTERFACE_FUNCTION_EchoClient}
  }
  self.atInterfaceFunction = atInterfaceFunction
  local strInterfaceFunctions = table.concat(pl.tablex.keys(atInterfaceFunction), ',')

  local atEthernetPortFlags = {
    ['Permanent'] = ${ETHERNET_PORT_FLAG_Permanent}
  }
  self.atEthernetPortFlags = atEthernetPortFlags
  local strEthernetPortFlags = table.concat(pl.tablex.keys(atEthernetPortFlags), ',')

  self.tStructure_EthernetPortConfiguration = self.vstruct.compile([[
    ulVerbose:u4
    ulLinkUpTimeout:u4
    ulMaximumTransferTime:u4
    2*{
      acName:s16
      ulInterface:u4
      ulFunction:u4
      ulFlags:u4
      ulIp:u4
      ulGatewayIp:u4
      ulNetmask:u4
      ulRemoteIp:u4
      usLinkUpDelay:u2
      usLocalPort:u2
      usRemotePort:u2
      aucMac: {6*u1}
    }
  ]])

  local P = self.P
  self:__parameter {
    P:P('plugin', 'A pattern for the plugin to use.'):
      required(false),

    P:P('plugin_options', 'Plugin options as a JSON object.'):
      required(false),

    P:U32('link_up_timeout', 'The maximum time to wait for a link on all ports in ms.'):
      default(4000):
      required(true),

    P:U32('maximum_transfer_time', 'The maximum time to wait for the transfer test to finish in ms.'):
      default(6000):
      required(true),

    P:P('port0_name', 'The name of port 0.'):
      default('CH0'):
      required(true),

    P:SC('port0_interface', 'The interface for port 0.'):
      default('None'):
      required(true):
      constraint(strInterfaces),

    P:SC('port0_function', 'The function for port 0.'):
      default('None'):
      required(true):
      constraint(strInterfaceFunctions),

    P:MC('port0_flags', 'The flags for port 0.'):
      required(false):
      constraint(strEthernetPortFlags),

    P:P('port0_mac', 'The MAC for port 0.'):
      default('00:02:a2:20:20:00'):
      required(true):
      constraint(self.parseMac),

    P:P('port0_ip', 'The IP for port 0.'):
      default('192.168.64.20'):
      required(true):
      constraint(self.parseIp),

    P:P('port0_gateway_ip', 'The IP of the gateway for port 0.'):
      default('192.168.64.1'):
      required(true):
      constraint(self.parseIp),

    P:P('port0_netmask', 'The netmask for port 0.'):
      default('255.255.255.0'):
      required(true):
      constraint(self.parseIp),

    P:U16('port0_link_up_delay', 'The delay after a link up in ms.'):
      default(1000):
      required(true),

    P:U16('port0_local_port', 'The local port number of port 0.'):
      default(1024):
      required(true),

    P:P('port0_remote_ip', 'The IP of the communication partner for port 0.'):
      default('192.168.64.20'):
      required(true):
      constraint(self.parseIp),

    P:U16('port0_remote_port', 'The local port number of port 0.'):
      default(1024):
      required(true),


    P:P('port1_name', 'The name of port 1.'):
      default('CH1'):
      required(true),

    P:SC('port1_interface', 'The interface for port 1.'):
      default('None'):
      required(true):
      constraint(strInterfaces),

    P:SC('port1_function', 'The function for port 1.'):
      default('None'):
      required(true):
      constraint(strInterfaceFunctions),

    P:MC('port1_flags', 'The flags for port 1.'):
      required(false):
      constraint(strEthernetPortFlags),

    P:P('port1_mac', 'The MAC for port 1.'):
      default('00:02:a2:20:20:01'):
      required(true):
      constraint(self.parseMac),

    P:P('port1_ip', 'The IP for port 1.'):
      default('192.168.64.21'):
      required(true):
      constraint(self.parseIp),

    P:P('port1_gateway_ip', 'The IP of the gateway for port 1.'):
      default('192.168.64.1'):
      required(true):
      constraint(self.parseIp),

    P:P('port1_netmask', 'The netmask for port 1.'):
      default('255.255.255.0'):
      required(true):
      constraint(self.parseIp),

    P:U16('port1_link_up_delay', 'The delay after a link up in ms.'):
      default(1000):
      required(true),

    P:U16('port1_local_port', 'The local port number of port 1.'):
      default(1025):
      required(true),

    P:P('port1_remote_ip', 'The IP of the communication partner for port 1.'):
      default('192.168.64.20'):
      required(true):
      constraint(self.parseIp),

    P:U16('port1_remote_port', 'The local port number of port 1.'):
      default(1024):
      required(true)
  }
end



function TestClassEth.parseMac(tValue)
  local fIsValid = false
  local strMessage


  if type(tValue)=='string' then
    -- Does this look like a MAC?
    local tMac0, tMac1, tMac2, tMac3, tMac4, tMac5 = string.match(tValue, '^(%x%x):(%x%x):(%x%x):(%x%x):(%x%x):(%x%x)$')
    if tMac0==nil then
      strMessage = 'The MAC has an invalid format, it should be xx:xx:xx:xx:xx:xx .'
    else
      local ucMac0 = tonumber(tMac0, 16)
      local ucMac1 = tonumber(tMac1, 16)
      local ucMac2 = tonumber(tMac2, 16)
      local ucMac3 = tonumber(tMac3, 16)
      local ucMac4 = tonumber(tMac4, 16)
      local ucMac5 = tonumber(tMac5, 16)

      tValue = { ucMac0, ucMac1, ucMac2, ucMac3, ucMac4, ucMac5 }
      fIsValid = true
    end
  else
    strMessage = string.format('The value for the MAC must be a string, but it is "%s".', type(tValue))
  end

  return fIsValid, tValue, strMessage
end



function TestClassEth.parseIp(tValue)
  local fIsValid = false
  local strMessage


  if type(tValue)=='string' then
    -- Does this look like an IP?
    local tIp0, tIp1, tIp2, tIp3 = string.match(tValue, '^(%d+)%.(%d+)%.(%d+)%.(%d+)$')
    if tIp0==nil then
      strMessage = 'The IP has an invalid format, it should be d.d.d.d .'
    else
      local ucIp0 = tonumber(tIp0)
      local ucIp1 = tonumber(tIp1)
      local ucIp2 = tonumber(tIp2)
      local ucIp3 = tonumber(tIp3)

      if ucIp0<0 or ucIp0>255 or ucIp1<0 or ucIp1>255 or ucIp2<0 or ucIp2>255 or ucIp3<0 or ucIp3>255 then
        strMessage = 'The componentes of the IP must be 0<=d<=255 .'
      else
        tValue = ucIp0 + 0x00000100*ucIp1 + 0x00010000*ucIp2 + 0x01000000*ucIp3
        fIsValid = true
      end
    end
  else
    strMessage = string.format('The value for the IP must be a string, but it is "%s".', type(tValue))
  end

  return fIsValid, tValue, strMessage
end



function TestClassEth:run()
  local atParameter = self.atParameter
  local tLog = self.tLog
  local bit = self.bit
  local pl = self.pl

  ----------------------------------------------------------------------
  --
  -- Parse the parameters and collect all options.
  --
  local strPluginPattern = atParameter['plugin']:get()
  local strPluginOptions = atParameter['plugin_options']:get()

  -- Parse the interface.
  local strInterface = atParameter['port0_interface']:get()
  local ulPort0_Interface = self.atInterface[strInterface]
  if ulPort0_Interface==nil then
    tLog.error('Unknown interface "%s" specified for port 0.', strInterface)
    error('Unknown interface for port 0.')
  end
  strInterface = atParameter['port1_interface']:get()
  local ulPort1_Interface = self.atInterface[strInterface]
  if ulPort1_Interface==nil then
    tLog.error('Unknown interface "%s" specified for port 1.', strInterface)
    error('Unknown interface for port 1.')
  end

  if ulPort0_Interface == ulPort1_Interface then
    error('Identical interface of port 1 and port 2.')
  end

  local port0_name,port1_name
  port0_name = atParameter['port0_name']:get()
  port1_name = atParameter['port1_name']:get()

  if port0_name == port1_name then
    error('Identical name of port 1 and port 2.')
  end

  local port0_ip,port1_ip
  port0_ip = atParameter['port0_ip']:get()
  port1_ip = atParameter['port1_ip']:get()

  if port0_ip == port1_ip then
    error('Identical ip address of port 1 and port 2.')
  end

  local port0_mac,port1_mac
  port0_mac = atParameter['port0_mac']:get()
  port1_mac = atParameter['port1_mac']:get()

  print('TEST :',port0_mac,port1_mac)

  if pl.tablex.compare(port0_mac, port1_mac,'==') then
    error('Identical mac address of port 1 and port 2.')
  end

  local strInterfaceFunction = atParameter['port0_function']:get()
  local ulPort0_InterfaceFunction = self.atInterfaceFunction[strInterfaceFunction]
  if ulPort0_InterfaceFunction==nil then
    tLog.error('Unknown interface function "%s" specified for port 0.', strInterfaceFunction)
    error('Unknown interface function for port 0.')
  end
  strInterfaceFunction = atParameter['port1_function']:get()
  local ulPort1_InterfaceFunction = self.atInterfaceFunction[strInterfaceFunction]
  if ulPort1_InterfaceFunction==nil then
    tLog.error('Unknown interface function "%s" specified for port 1.', strInterfaceFunction)
    error('Unknown interface function for port 1.')
  end

  local ulPort0_Flags = 0
  local atFlags = atParameter['port0_flags']:get()
  if atFlags~=nil then
    for _, strFlag in pairs(atFlags) do
      local ulValue = self.atEthernetPortFlags[strFlag]
      if ulValue==nil then
        tLog.error('Unknown flag "%s" specified at port 0.', strFlag)
        error('Unknown flag.')
      end
      ulPort0_Flags = bit.bor(ulPort0_Flags, ulValue)
    end
  end
  local ulPort1_Flags = 0
  atFlags = atParameter['port1_flags']:get()
  if atFlags~=nil then
    for _, strFlag in pairs(atFlags) do
      local ulValue = self.atEthernetPortFlags[strFlag]
      if ulValue==nil then
        tLog.error('Unknown flag "%s" specified at port 1.', strFlag)
        error('Unknown flag.')
      end
      ulPort1_Flags = bit.bor(ulPort1_Flags, ulValue)
    end
  end

  -- Combine the parameters.
  local strEthernetPortConfiguration
  local atConfig = {
    ['ulVerbose'] = 1,
    ['ulLinkUpTimeout'] = atParameter['link_up_timeout']:get(),
    ['ulMaximumTransferTime'] = atParameter['maximum_transfer_time']:get(),

    {
      ['acName'] = port0_name,
      ['ulInterface'] = ulPort0_Interface,
      ['ulFunction'] = ulPort0_InterfaceFunction,
      ['ulFlags'] = ulPort0_Flags,
      ['ulIp'] = port0_ip,
      ['ulGatewayIp'] = atParameter['port0_gateway_ip']:get(),
      ['ulNetmask'] = atParameter['port0_netmask']:get(),
      ['ulRemoteIp'] = atParameter['port0_remote_ip']:get(),
      ['usLinkUpDelay'] = atParameter['port0_link_up_delay']:get(),
      ['usLocalPort'] = atParameter['port0_local_port']:get(),
      ['usRemotePort'] = atParameter['port0_remote_port']:get(),
      ['aucMac'] = port0_mac
    },
    {
      ['acName'] = port1_name,
      ['ulInterface'] = ulPort1_Interface,
      ['ulFunction'] = ulPort1_InterfaceFunction,
      ['ulFlags'] = ulPort1_Flags,
      ['ulIp'] = port1_ip,
      ['ulGatewayIp'] = atParameter['port1_gateway_ip']:get(),
      ['ulNetmask'] = atParameter['port1_netmask']:get(),
      ['ulRemoteIp'] = atParameter['port1_remote_ip']:get(),
      ['usLinkUpDelay'] = atParameter['port1_link_up_delay']:get(),
      ['usLocalPort'] = atParameter['port1_local_port']:get(),
      ['usRemotePort'] = atParameter['port1_remote_port']:get(),
      ['aucMac'] = port1_mac
    }
  }

  -- Convert the data to a struct.
  strEthernetPortConfiguration = self.tStructure_EthernetPortConfiguration:write(atConfig)

  ----------------------------------------------------------------------
  --
  -- Open the connection to the netX.
  -- (or re-use an existing connection.)
  --
  local atPluginOptions = {}
  if strPluginOptions~=nil then
    local tJson, uiPos, strJsonErr = json.decode(strPluginOptions)
    if tJson==nil then
      tLog.warning('Ignoring invalid plugin options. Error parsing the JSON: %d %s', uiPos, strJsonErr)
    else
      atPluginOptions = tJson
    end
  end
  local tPlugin = _G.tester:getCommonPlugin(strPluginPattern, atPluginOptions)
  if tPlugin==nil then
    local strPluginOptions = pl.pretty.write(atPluginOptions)
    local strError = string.format('Failed to establish a connection to the netX with pattern "%s" and options "%s".', strPluginPattern, strPluginOptions)
    error(strError)
  end

  local astrBinaryName = {
    [romloader.ROMLOADER_CHIPTYP_NETX4000_RELAXED] = '4000',
    [romloader.ROMLOADER_CHIPTYP_NETX4000_FULL]    = '4000',
    [romloader.ROMLOADER_CHIPTYP_NETX4100_SMALL]   = '4000',
    [romloader.ROMLOADER_CHIPTYP_NETX500]          = '500',
    [romloader.ROMLOADER_CHIPTYP_NETX100]          = '500',
    [romloader.ROMLOADER_CHIPTYP_NETX90_MPW]       = '90_mpw',
    [romloader.ROMLOADER_CHIPTYP_NETX90]           = '90',
    [romloader.ROMLOADER_CHIPTYP_NETX90B]          = '90',
--    [romloader.ROMLOADER_CHIPTYP_NETX56]           = '56',
--    [romloader.ROMLOADER_CHIPTYP_NETX56B]          = '56',
--    [romloader.ROMLOADER_CHIPTYP_NETX50]           = '50',
--    [romloader.ROMLOADER_CHIPTYP_NETX10]           = '10'
  }

  -- Get the binary for the ASIC.
  local tAsicTyp = tPlugin:GetChiptyp()
  local strBinary = astrBinaryName[tAsicTyp]
  if strBinary==nil then
    tLog.error('No binary for chip type %s.', tAsicTyp)
    error('No matching binary found.')
  end
  local strNetxBinary = string.format('netx/eth_netx%s.bin', strBinary)

  local ulResult = tester:mbin_simple_run(tPlugin, strNetxBinary, strEthernetPortConfiguration)
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
