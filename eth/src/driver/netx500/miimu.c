
#include "miimu.h"

#include "nec_cb12.h"
#include "netx_io_areas.h"
#include "options.h"


/* MIIMU parameters */
#define MIIMU_SUPPRESS_PREAMBLE		0
#define MIIMU_SEND_PREAMBLE		1
#define MIIMU_MDCFREQ_HIGH		0
#define MIIMU_MDCFREQ_LOW		1
#define MIIMU_RTA_0_BITS		0
#define MIIMU_RTA_1_BITS		1


/* This is for netX500. */
#define ETH_MAX_INSTANCES 4


#if ASIC_TYP==56

static HOSTADEF(INT_PHY) * const atIntPhyAreas[2] =
{
	(HOSTADEF(INT_PHY) * const)HOSTADDR(int_phy_ctrl0),
	(HOSTADEF(INT_PHY) * const)HOSTADDR(int_phy_ctrl1)
};


unsigned long Miimu_ReadPhy(unsigned int uiPhyAdr, unsigned int uiRegisterAddress)
{
	unsigned long ulValue;
	HOSTDEF(ptMiimuArea)
	HOSTADEF(INT_PHY) * ptIntPhyArea;


	ulValue = 0xffffffff;

	/* Does the address select an external PHY? */

	/* The internal PHY uses address 0 and 1. All others are external. */
	if( uiPhyAdr>=2 )
	{
		ulValue  = HOSTMSK(miimu_snrdy);
		ulValue |= HOSTMSK(miimu_preamble);
/* TODO: try to use fast mode. */
		ulValue |= HOSTMSK(miimu_mdc_period);
		ulValue |= HOSTMSK(miimu_rta);
		ulValue |= uiRegisterAddress << HOSTSRT(miimu_regaddr);
		ulValue |= uiPhyAdr << HOSTSRT(miimu_phyaddr);
		ptMiimuArea->ulMiimu = ulValue;

		/* Wait until the operation finished. */
		do
		{
			ulValue  = ptMiimuArea->ulMiimu;
			ulValue &= HOSTMSK(miimu_snrdy);
		} while( ulValue!=0 );

		ulValue   = ptMiimuArea->ulMiimu;
		ulValue  &= HOSTMSK(miimu_data);
		ulValue >>= HOSTSRT(miimu_data);

	}
	else
	{
		/* NOTE: This is a little shortcut to prevent extraction of the complete
		 * phy address from the config register. Both internal phys share address
		 * bits 4..1 . The first phy has address[0]=0, the second one address[0]=1.
		 */
		ptIntPhyArea = atIntPhyAreas[uiPhyAdr&1];

		ulValue  = HOSTMSK(int_phy_ctrl_miimu_snrdy);
		ulValue |= HOSTMSK(int_phy_ctrl_miimu_preamble);
/* TODO: try to use fast mode. */
		ulValue |= HOSTMSK(int_phy_ctrl_miimu_mdc_period);
		ulValue |= HOSTMSK(int_phy_ctrl_miimu_rta);
		ulValue |= uiRegisterAddress << HOSTSRT(int_phy_ctrl_miimu_regaddr);
		ulValue |= uiPhyAdr << HOSTSRT(int_phy_ctrl_miimu_phyaddr);
		ptIntPhyArea->ulInt_phy_ctrl_miimu = ulValue;

		/* Wait until the operation finished. */
		do
		{
			ulValue  = ptIntPhyArea->ulInt_phy_ctrl_miimu;
			ulValue &= HOSTMSK(int_phy_ctrl_miimu_snrdy);
		} while( ulValue!=0 );

		ulValue   = ptIntPhyArea->ulInt_phy_ctrl_miimu;
		ulValue  &= HOSTMSK(int_phy_ctrl_miimu_data);
		ulValue >>= HOSTSRT(int_phy_ctrl_miimu_data);
	}

	return ulValue;
}


void Miimu_WritePhy(unsigned int uiPhyAdr, unsigned int uiRegisterAddress, unsigned int uiData)
{
	unsigned long ulValue;
	HOSTDEF(ptMiimuArea)
	HOSTADEF(INT_PHY) * ptIntPhyArea;


	if( uiPhyAdr<0x10 )
	{
		ulValue  = HOSTMSK(miimu_snrdy);
		ulValue |= HOSTMSK(miimu_preamble);
		ulValue |= HOSTMSK(miimu_opmode);
/* TODO: try to use fast mode. */
		ulValue |= HOSTMSK(miimu_mdc_period);
		ulValue |= HOSTMSK(miimu_rta);
		ulValue |= uiRegisterAddress << HOSTSRT(miimu_regaddr);
		ulValue |= uiPhyAdr << HOSTSRT(miimu_phyaddr);
		ulValue |= uiData << HOSTSRT(miimu_data);
		ptMiimuArea->ulMiimu = ulValue;

		/* Wait until the operation finished. */
		do
		{
			ulValue  = ptMiimuArea->ulMiimu;
			ulValue &= HOSTMSK(miimu_snrdy);
		} while( ulValue!=0 );
	}
	else
	{
		/* NOTE: This is a little shortcut to prevent extraction of the complete
		 * phy address from the config register. Both internal phys share address
		 * bits 4..1 . The first phy has address[0]=0, the second one address[0]=1.
		 */
		ptIntPhyArea = atIntPhyAreas[uiPhyAdr&1];

		ulValue  = HOSTMSK(int_phy_ctrl_miimu_snrdy);
		ulValue |= HOSTMSK(int_phy_ctrl_miimu_preamble);
		ulValue |= HOSTMSK(int_phy_ctrl_miimu_opmode);
/* TODO: try to use fast mode. */
		ulValue |= HOSTMSK(int_phy_ctrl_miimu_mdc_period);
		ulValue |= HOSTMSK(int_phy_ctrl_miimu_rta);
		ulValue |= uiRegisterAddress << HOSTSRT(int_phy_ctrl_miimu_regaddr);
		ulValue |= uiPhyAdr << HOSTSRT(int_phy_ctrl_miimu_phyaddr);
		ulValue |= uiData << HOSTSRT(int_phy_ctrl_miimu_data);
		ptIntPhyArea->ulInt_phy_ctrl_miimu = ulValue;

		/* Wait until the operation finished. */
		do
		{
			ulValue  = ptIntPhyArea->ulInt_phy_ctrl_miimu;
			ulValue &= HOSTMSK(int_phy_ctrl_miimu_snrdy);
		} while( ulValue!=0 );
	}
}

#else
void miimu_dummy_read(unsigned uiInstance)
{
	HOSTDEF(ptMiimuArea);
	unsigned int uiPhyAddress;
	unsigned long ulValue;


	if( uiInstance<ETH_MAX_INSTANCES )
	{
		/* Get the PHY address.
		 * For the internal PHYs of the netX500 this is 0 and 1.
		 */
		uiPhyAddress = uiInstance;

		ulValue  = HOSTMSK(miimu_reg_miimu_snrdy);
		ulValue |= HOSTMSK(miimu_reg_phy_nres);
		ulValue |= MIIMU_SEND_PREAMBLE << HOSTSRT(miimu_reg_miimu_preamble);
		ulValue |= MIIMU_MDCFREQ_HIGH << HOSTSRT(miimu_reg_miimu_mdc_period);
		ulValue |= MIIMU_RTA_0_BITS << HOSTSRT(miimu_reg_miimu_rta);
		ulValue |= DRV_CB12_CONTROL << HOSTSRT(miimu_reg_miimu_regaddr);
		ulValue |= uiPhyAddress << HOSTSRT(miimu_reg_miimu_phyaddr);
		ptMiimuArea->ulMiimu_reg = ulValue;

		/* wait for command done */
		do
		{
			ulValue  = ptMiimuArea->ulMiimu_reg;
			ulValue &= HOSTMSK(miimu_reg_miimu_snrdy);
		} while( ulValue!=0 );
	}
}


unsigned long Miimu_ReadPhy(unsigned uiInstance, unsigned int uMiimuReqAddr)
{
	HOSTDEF(ptMiimuArea);
	unsigned int uiPhyAddress;
	unsigned long ulValue;


	ulValue = 0;
	if( uiInstance<ETH_MAX_INSTANCES )
	{
		/* Get the PHY address.
		 * For the internal PHYs of the netX500 this is 0 and 1.
		 */
		uiPhyAddress = uiInstance;

		ulValue  = HOSTMSK(miimu_reg_miimu_snrdy);
		ulValue |= HOSTMSK(miimu_reg_phy_nres);
		ulValue |= MIIMU_SEND_PREAMBLE << HOSTSRT(miimu_reg_miimu_preamble);
		ulValue |= MIIMU_MDCFREQ_HIGH << HOSTSRT(miimu_reg_miimu_mdc_period);
		ulValue |= MIIMU_RTA_0_BITS << HOSTSRT(miimu_reg_miimu_rta);
		ulValue |= uMiimuReqAddr << HOSTSRT(miimu_reg_miimu_regaddr);
		ulValue |= uiPhyAddress << HOSTSRT(miimu_reg_miimu_phyaddr);
		ptMiimuArea->ulMiimu_reg = ulValue;

		/* wait for command done */
		do
		{
			ulValue  = ptMiimuArea->ulMiimu_reg;
		} while( (ulValue&HOSTMSK(miimu_reg_miimu_snrdy))!=0 );

		/* extract the data part */
		ulValue  &= HOSTMSK(miimu_reg_miimu_data);
		ulValue >>= HOSTSRT(miimu_reg_miimu_data);
	}

	return ulValue;
}


void Miimu_WritePhy(unsigned uiInstance, unsigned int uMiimuReqAddr, unsigned int uiData)
{
	HOSTDEF(ptMiimuArea);
	unsigned int uiPhyAddress;
	unsigned long ulValue;


	if( uiInstance<ETH_MAX_INSTANCES )
	{
		/* Get the PHY address.
		 * For the internal PHYs of the netX500 this is 0 and 1.
		 */
		uiPhyAddress = uiInstance;

		ulValue  = HOSTMSK(miimu_reg_miimu_snrdy);
		ulValue |= HOSTMSK(miimu_reg_phy_nres);
		ulValue |= HOSTMSK(miimu_reg_miimu_opmode);
		ulValue |= MIIMU_SEND_PREAMBLE << HOSTSRT(miimu_reg_miimu_preamble);
		ulValue |= MIIMU_MDCFREQ_HIGH << HOSTSRT(miimu_reg_miimu_mdc_period);
		ulValue |= uMiimuReqAddr << HOSTSRT(miimu_reg_miimu_regaddr);
		ulValue |= uiPhyAddress << HOSTSRT(miimu_reg_miimu_phyaddr);
		ulValue |= uiData << HOSTSRT(miimu_reg_miimu_data);
		ptMiimuArea->ulMiimu_reg = ulValue;

		/* wait for command done */
		do
		{
			ulValue  = ptMiimuArea->ulMiimu_reg;
		} while( (ulValue&HOSTMSK(miimu_reg_miimu_snrdy))!=0 );
	}
}

#endif
