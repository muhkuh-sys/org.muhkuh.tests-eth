

#ifndef __XC_CODES_ETH_STD_MAC_H__
#define __XC_CODES_ETH_STD_MAC_H__


extern const unsigned long XcCode_rpu_eth0[123];
extern const unsigned long XcCode_tpu_eth0[219];
extern const unsigned long XcCode_xpec_eth_std_mac0[1568];

extern const unsigned long XcCode_rpu_eth1[123];
extern const unsigned long XcCode_tpu_eth1[219];
extern const unsigned long XcCode_xpec_eth_std_mac1[1568];

extern const unsigned long XcCode_rpu_eth2[123];
extern const unsigned long XcCode_tpu_eth2[219];
extern const unsigned long XcCode_xpec_eth_std_mac2[1550];

extern const unsigned long XcCode_rpu_eth3[123];
extern const unsigned long XcCode_tpu_eth3[219];
extern const unsigned long XcCode_xpec_eth_std_mac3[1550];


typedef union
{
	unsigned long *apulCodes[12];
	struct
	{
		struct
		{
			const unsigned long *pulRpu;
			const unsigned long *pulTpu;
			const unsigned long *pulXpec;
		} atUnits[4];
	} tCodes;
} XC_CODES_ETH_STD_MAC_T;


#endif  /* __XC_CODES_ETH_STD_MAC_H__ */

