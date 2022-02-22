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

void     PIO_SetMode   (unsigned int uiPioNum, PIO_MODE_E eMode);
int      PIO_GetInput  (unsigned int uiPioNum);
uint32_t PIO_GetIn     (void);
int      PIO_GetOutput (unsigned int uiPioNum);
void     PIO_SetOutput (unsigned int uiPioNum, bool fEnable);
uint32_t PIO_GetLine   (void);
void     PIO_SetLine   (uint32_t ulVal);
void     PIO_SetOe     (uint32_t ulOe);

#endif /* #ifndef __HAL_PIO_H */
