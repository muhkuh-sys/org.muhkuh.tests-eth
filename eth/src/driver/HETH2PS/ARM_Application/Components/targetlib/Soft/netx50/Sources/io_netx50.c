#include "io.h"
#include "common_func_netx50.h"
#include <stdbool.h>


#define PIO_APPTRG_MIN 6
#define PIO_APPTRG_CNT 2


void io_apptrg_init(void)
{
  unsigned int uiIdx;

  for(uiIdx = 0; uiIdx < PIO_APPTRG_CNT; ++uiIdx)
  {
    NX50_PIO_SetOutput(uiIdx + PIO_APPTRG_MIN, 0);
    NX50_PIO_SetMode(uiIdx + PIO_APPTRG_MIN, NX50_PIO_MODE_OUT);
  }
}

void io_apptrg_set(unsigned int uiIdx, unsigned int uiVal)
{
  if( uiIdx >= PIO_APPTRG_CNT )
  {
    return;
  }

  NX50_PIO_SetOutput(PIO_APPTRG_MIN + uiIdx, uiVal);
}
