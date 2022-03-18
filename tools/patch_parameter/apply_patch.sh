#! /bin/bash

# This script generates the ping server for the SPE test.
./lua5.4 patch_eth_standalone.lua -v debug --patch ../../standalone_configs/rte_spe_switch.json ../../eth/targets/netx90/standalone_eth2ps/eth_eth2ps_netx90.img spe_pingserver.img
