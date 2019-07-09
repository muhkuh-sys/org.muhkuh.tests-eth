

#ifndef __XC_CODES_RESET_H__
#define __XC_CODES_RESET_H__


extern const unsigned long XcCode_rpu_reset0[57];
extern const unsigned long XcCode_tpu_reset0[57];
extern const unsigned long XcCode_rpu_reset1[57];
extern const unsigned long XcCode_tpu_reset1[57];
extern const unsigned long XcCode_rpu_reset2[57];
extern const unsigned long XcCode_tpu_reset2[57];
extern const unsigned long XcCode_rpu_reset3[57];
extern const unsigned long XcCode_tpu_reset3[57];


typedef union
{
	unsigned long *apulCodes[8];
	struct
	{
		struct
		{
			const unsigned long *pulRpu;
			const unsigned long *pulTpu;
		} atUnits[4];
	} tCodes;
} XC_CODES_RESET_T;


#endif  /* __XC_CODES_RESET_H__ */

