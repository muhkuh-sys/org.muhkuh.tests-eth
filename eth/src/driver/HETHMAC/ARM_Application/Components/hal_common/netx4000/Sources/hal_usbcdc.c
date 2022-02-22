#include "hal_usbcdc.h"
#include "hw_defines.h"
#include "compiler_spec.h"

#include <stdint.h>
#include <string.h>

/*****************************************************************************/
/*  Definitions                                                              */
/*****************************************************************************/

/* The following default definitions are compatible
 * with the standard netX USB drivers provided by Hilscher GmbH */

/* Vendor ID */
#ifndef USBCDC_VID
#define USBCDC_VID           0x1939
#endif

/* Product ID */
#ifndef USBCDC_PID
#define USBCDC_PID           0x0130
#endif

/* Product Release */
#ifndef USBCDC_PREL
#define USBCDC_PREL          0x0001
#endif

/* Product String */
#ifndef USBCDC_PRODUCT_STR
#define USBCDC_PRODUCT_STR       "Hilscher netX 51"
#endif

/* Vendor String */
#ifndef USBCDC_VENDOR_STR
#define USBCDC_VENDOR_STR        "Hilscher GmbH"
#endif

/* Serial String */
#ifndef USBCDC_SERIAL_STR
#define USBCDC_SERIAL_STR        "0"
#endif

/* USB defines */
#define USB_STRINGDESCR_VENDOR            0
#define USB_STRINGDESCR_PRODUCT           1
#define USB_STRINGDESCR_SERIAL            2

#define USB_DESCRIPTORTYPE_STRING         3

#define USB_DEVICESECR_ATTRIB_DEFAULT     0x80
#define USB_DEVICESECR_ATTRIB_SELFPOWERED 0x40

#define USB_STRINGDESCR_MAXSIZE           16

#ifndef min
  #define min(a,b)  ((a < b)? a : b)
#endif

/*! Structure of a String device descriptor in the enumeration RAM */
typedef __PACKED_PRE struct USB_SINGLE_STRING_DESCR_Ttag
{
  uint8_t bLen;                               /*!< Length of the descriptor */
  uint8_t bType;                              /*!< Descriptor type (3)      */
  char  szString[USB_STRINGDESCR_MAXSIZE];    /*!< ASCII string             */
} __PACKED_POST USB_SINGLE_STRING_DESCR_T;

/*! Structure of enumeration RAM */
typedef union USB_ENUM_RAM_Utag
{
  struct __PACKED_PRE USB_ENUM_RAM_Ttag
  {
    struct __PACKED_PRE USB_DEVICE_DESCRIPTOR_Ttag
    {
      uint16_t  usVendorId;
      uint16_t  usProductId;
      uint16_t  usReleaseNr;
      uint8_t   bCharacteristics;
      uint8_t   bMaxPower;
    } __PACKED_POST tDevice;

    USB_SINGLE_STRING_DESCR_T atStrings[3];

  } __PACKED_POST tEnum;

  uint32_t  aulRAM[16];
  uint8_t   abRAM[64];

} USB_ENUM_RAM_U;

#ifndef HALDEF
#define HALDEF(name) name
#endif

/*****************************************************************************/
/*  Variables                                                                */
/*****************************************************************************/

HW_PTR_USB_DEV_CTRL(s_ptUsbDevCtrl)
HW_PTR_USB_DEV_ENUM_RAM(s_pulUsbDevEnumRam)
HW_PTR_USB_DEV_FIFO_CTRL(s_ptUsbDevFifoCtrl)
HW_PTR_USB_DEV_FIFO(s_ptUsbDevFifo)


/*****************************************************************************/
/*  Functions                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*! Setup string descriptor in Enumeration memory
* \description
*   This function prepares one of three string descriptors of the USB Device unit
* \class
*   USBCDC
 * \params
 *   puEnumRam   Pointer to EnumRAM structure
 *   pszSource   String to insert
 *   uiIdx       Descriptor index (see USB_STRINGDESCR_XXX)                  */
/*****************************************************************************/
static void SetupStringDescriptor(USB_ENUM_RAM_U* puEnumRam, char* pszSource, unsigned int uiIdx)
{
  unsigned int                uiCopyCount;
  USB_SINGLE_STRING_DESCR_T*  ptDescriptor = &puEnumRam->tEnum.atStrings[uiIdx];

  uiCopyCount = strnlen(pszSource, USB_STRINGDESCR_MAXSIZE);

  memcpy(ptDescriptor->szString, pszSource, uiCopyCount);

  /* Length of a string descriptor is the length of the UNICODE string 
     + 2 Byte for the descriptor type and length */
  ptDescriptor->bLen  = (uint8_t)(uiCopyCount * sizeof(uint16_t) + 2 * sizeof(uint8_t));
  ptDescriptor->bType = USB_DESCRIPTORTYPE_STRING;
}


/*****************************************************************************/
/*! Initialize USB Device as CDC
* \description
*   This function resets and initializes the USB core for UART like usage.
* \class
*   USBCDC
 * \params
 *   none                                                                    */
/*****************************************************************************/
void HALDEF(USBCDC_Init)(void)
{
  unsigned long   ulValue;
  USB_ENUM_RAM_U  uEnumRam;

  /* Reset and disable core */
  s_ptUsbDevCtrl->ulUsb_dev_cfg  = HW_MSK(usb_dev_cfg_usb_dev_reset);
  s_ptUsbDevCtrl->ulUsb_dev_cfg &= ~HW_MSK(usb_dev_cfg_usb_dev_reset);

  /* reset all FIFOs (defaults to streaming mode with ZLP) */
  ulValue  = HW_DFLT_VAL(usb_dev_fifo_ctrl_conf);
  s_ptUsbDevFifoCtrl->ulUsb_dev_fifo_ctrl_conf = ulValue | HW_MSK(usb_dev_fifo_ctrl_conf_reset);
  s_ptUsbDevFifoCtrl->ulUsb_dev_fifo_ctrl_conf = ulValue; /* release the reset */

  memset(&uEnumRam, 0, sizeof(uEnumRam));


  uEnumRam.tEnum.tDevice.usVendorId       = USBCDC_VID;
  uEnumRam.tEnum.tDevice.usProductId      = USBCDC_PID;
  uEnumRam.tEnum.tDevice.usReleaseNr      = USBCDC_PREL;
  uEnumRam.tEnum.tDevice.bCharacteristics = USB_DEVICESECR_ATTRIB_DEFAULT |
                                            USB_DEVICESECR_ATTRIB_SELFPOWERED;
  uEnumRam.tEnum.tDevice.bMaxPower        = 0;

  /* Copy Product and Vendor Name */
  SetupStringDescriptor(&uEnumRam, USBCDC_VENDOR_STR,   USB_STRINGDESCR_VENDOR);
  SetupStringDescriptor(&uEnumRam, USBCDC_PRODUCT_STR,  USB_STRINGDESCR_PRODUCT);
  SetupStringDescriptor(&uEnumRam, USBCDC_SERIAL_STR,   USB_STRINGDESCR_SERIAL);


  for(ulValue = 0; ulValue < sizeof(uEnumRam.aulRAM) / 4; ulValue++)
    s_pulUsbDevEnumRam[ulValue] = uEnumRam.aulRAM[ulValue];

  s_ptUsbDevCtrl->ulUsb_dev_irq_mask = 0;

  /* Enable Core and enter PowerUp state */
  s_ptUsbDevCtrl->ulUsb_dev_cfg = HW_MSK(usb_dev_cfg_usb_core_enable);
}

void HALDEF(USBCDC_Connect)(void)
{
  s_ptUsbDevCtrl->ulUsb_dev_cfg |= HW_MSK(usb_dev_cfg_usb_phy_pullup_enable);
}



bool HALDEF(USBCDC_IsConnected)(void)
{
  unsigned long ulValue;

  /* check if the device is addressed and configured */
  ulValue  = s_ptUsbDevCtrl->ulUsb_dev_status;
  ulValue &= (HW_MSK(usb_dev_status_usb_configured) | HW_MSK(usb_dev_status_usb_addressed));

  if( (HW_MSK(usb_dev_status_usb_configured) | HW_MSK(usb_dev_status_usb_addressed)) == ulValue )
  {
    return true;
  }

  return false;
}

int HALDEF(USBCDC_BufSpace)(void)
{
  uint32_t ulFifoStat = s_ptUsbDevFifoCtrl->ulUsb_dev_fifo_ctrl_uart_ep_tx_stat;
  ulFifoStat = (ulFifoStat & HW_MSK(usb_dev_fifo_ctrl_uart_ep_tx_stat_fill_level))
                          >> HW_SRT(usb_dev_fifo_ctrl_uart_ep_tx_stat_fill_level);

  return 320 - ulFifoStat;
}

int HALDEF(USBCDC_BufLevel)(void)
{
  uint32_t ulFillLevel;

  ulFillLevel   = s_ptUsbDevFifoCtrl->ulUsb_dev_fifo_ctrl_uart_ep_rx_stat;
  ulFillLevel  &= HW_MSK(usb_dev_fifo_ctrl_control_ep_rx_stat_fill_level);
  ulFillLevel >>= HW_SRT(usb_dev_fifo_ctrl_control_ep_rx_stat_fill_level);

  return (int)ulFillLevel;
}

unsigned char HALDEF(USBCDC_BufRecGet)(void)
{
  unsigned char bRet;
  unsigned long ulFillLevel;

  /* wait for character */
  do
  {
    /* avoid deadlock if USB is disconnected during communication */
    if( !USBCDC_IsConnected() )
      return 0;

    ulFillLevel   = s_ptUsbDevFifoCtrl->ulUsb_dev_fifo_ctrl_uart_ep_rx_stat;
    ulFillLevel  &= HW_MSK(usb_dev_fifo_ctrl_control_ep_rx_stat_fill_level);
    ulFillLevel >>= HW_SRT(usb_dev_fifo_ctrl_control_ep_rx_stat_fill_level);

  } while(0 == ulFillLevel);
 
  /* copy one byte from the receive FIFO */
  bRet = (unsigned char)s_ptUsbDevFifo->ulUsb_dev_uart_rx_data;

  return bRet;
}

void HALDEF(USBCDC_BufSendPut)(unsigned char bValue)
{
  int fFifoFull;
  do
  {
    /* avoid deadlock if USB is disconnected during communication */
    if( !USBCDC_IsConnected() )
      return;

    fFifoFull  = s_ptUsbDevFifoCtrl->ulUsb_dev_fifo_ctrl_uart_ep_tx_stat;
    fFifoFull &= HW_MSK(usb_dev_fifo_ctrl_uart_ep_tx_stat_full);
  } while(fFifoFull) ;

  s_ptUsbDevFifo->ulUsb_dev_uart_tx_data = bValue;
}

void HALDEF(USBCDC_BufSendFlush)(void)
{
  int fFifoEmpty;
  do
  {
    /* avoid deadlock if USB is disconnected during communication */
    if( !USBCDC_IsConnected() )
      return;

    fFifoEmpty  = s_ptUsbDevFifoCtrl->ulUsb_dev_fifo_ctrl_uart_ep_tx_stat;
    fFifoEmpty &= HW_MSK(usb_dev_fifo_ctrl_uart_ep_tx_stat_empty);
  } while(!fFifoEmpty);
}

unsigned int HALDEF(USBCDC_BufRecPeek)(void)
{
  unsigned int  uiRet = 0;
  unsigned long ulFillLevel;

  ulFillLevel   = s_ptUsbDevFifoCtrl->ulUsb_dev_fifo_ctrl_uart_ep_rx_stat;
  ulFillLevel  &= HW_MSK(usb_dev_fifo_ctrl_control_ep_rx_stat_fill_level);
  ulFillLevel >>= HW_SRT(usb_dev_fifo_ctrl_control_ep_rx_stat_fill_level);

  if(ulFillLevel > 0)
  {
    uiRet = 1;
  }

  return uiRet;
}
