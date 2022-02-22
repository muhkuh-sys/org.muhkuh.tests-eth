#ifndef __HAL_PHYLED_XPIC_H
#define __HAL_PHYLED_XPIC_H

typedef enum PHYLED_XPIC_PARAM_Etag
{
  PHYLED_XPIC_PARAM_MODE,
  PHYLED_XPIC_PARAM_TX_SR,
  PHYLED_XPIC_PARAM_TX_SR_BIT,
  PHYLED_XPIC_PARAM_TX_SR_INV,
  PHYLED_XPIC_PARAM_RX_SR,
  PHYLED_XPIC_PARAM_RX_SR_BIT,
  PHYLED_XPIC_PARAM_RX_SR_INV,
} PHYLED_XPIC_PARAM_E;

#define PHYLED_XPIC_MODE_OFF      0
#define PHYLED_XPIC_MODE_STATIC   1
#define PHYLED_XPIC_MODE_FLASHING 2
#define PHYLED_XPIC_MODE_COMBINED 3

void PhyledXpic_Initialize(void* pvUsr);
int  PhyledXpic_SetParam(unsigned int uiPort, PHYLED_XPIC_PARAM_E ePrmId, unsigned int uiValue);
void PhyledXpic_Start(void* pvUsr);

#endif /* __HAL_PHYLED_XPIC_H */
