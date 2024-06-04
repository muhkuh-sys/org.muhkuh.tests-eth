local t = ...

-- Copy all additional files.
t:install{
  ['${prj_root}/eth/targets/lua/patch_eth_standalone.lua']                  = '${install_base}/',

  ['${prj_root}/eth/targets/netx90/standalone_eth2ps/eth_eth2ps_netx90.img']          = '${install_base}/',
  ['${prj_root}/eth/targets/netx90/standalone_ethmac/eth_ethmac_netx90.img']          = '${install_base}/',

  ['${prj_root}/standalone_configs/apply_patches.sh']                       = '${install_base}/',
  ['${prj_root}/standalone_configs/ethmac_pingserver.json']                 = '${install_base}/',
  ['${prj_root}/standalone_configs/rte_spe_bridge_app.json']                = '${install_base}/',
  ['${prj_root}/standalone_configs/rte_spe_switch_pingserver.json']         = '${install_base}/',
}

return true
