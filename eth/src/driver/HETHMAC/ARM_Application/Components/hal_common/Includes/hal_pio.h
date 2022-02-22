#ifndef __HAL_PIO_H
#define __HAL_PIO_H

#include <stdint.h>
#include <stdbool.h>

/*****************************************************************************/
/*! PIO Mode                                                                 */
/*****************************************************************************/
typedef enum PIO_MODE_Etag {
  PIO_MODE_IN  = 0,
  PIO_MODE_OUT = 1
} PIO_MODE_E;

/*****************************************************************************/
/* Function prototypes                                                       */
/*****************************************************************************/
#ifndef HALDEC
#define HALDEC(name) name
#endif

void     HALDEC(PIO_SetMode)   (unsigned int uPioNum, PIO_MODE_E eMode);
int      HALDEC(PIO_GetInput)  (unsigned int uPioNum);
uint32_t HALDEC(PIO_GetIn)     (void);
int      HALDEC(PIO_GetOutput) (unsigned int uPioNum);
void     HALDEC(PIO_SetOutput) (unsigned int uPioNum, bool fEnable);
uint32_t HALDEC(PIO_GetLine)   (void);
void     HALDEC(PIO_SetLine)   (uint32_t ulVal);
void     HALDEC(PIO_SetOe)     (uint32_t ulOe);

#endif /* #ifndef __HAL_PIO_H */
