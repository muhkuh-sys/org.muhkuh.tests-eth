#! /bin/bash
set -e

# This script generates the ping server for the SPE test.
./lua5.4 patch_eth_standalone.lua -v debug --patch ethmac_pingserver.json           eth_ethmac_netx90.img   rte_pingserver_patched.img
./lua5.4 patch_eth_standalone.lua -v debug --patch rte_spe_switch_pingserver.json   eth_eth2ps_netx90.img   spe_pingserver_patched.img
./lua5.4 patch_eth_standalone.lua -v debug --patch rte_spe_bridge_app.json          eth_eth2ps_netx90.img  rte_spe_bridge_app_patched.img