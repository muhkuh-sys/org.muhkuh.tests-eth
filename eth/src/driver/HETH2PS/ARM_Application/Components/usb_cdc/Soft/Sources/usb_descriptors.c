#include "usb_regmap.h"
#include "usb_io.h"
#include "usb_requests_common.h"
#include "usb_globals.h"
#include "usb_descriptors.h"

/* --------------------------------------------------------------------------- */

/* string descriptor */
const unsigned char Top_String_Descriptor[] = {
  0x04,           /* length */
  0x03,           /* descriptor type = string descriptor */
  0x09, 0x04      /* language #0 : English US */
};

/* stringDescriptor for the Serial Number */
const stringDescriptor_t strSerialNumber_eng =
{
  0x04,
  0x03,
  {
    '0', 0
  }
};

/* stringDescriptor for the Manufacturer */
const stringDescriptor_t strManufacturer_eng =
{
  0x1c,
  0x03,
  {
    'H', 0, 'i', 0, 'l', 0, 's', 0, 'c', 0, 'h', 0, 'e', 0, 'r', 0,
    ' ', 0, 'G', 0, 'm', 0, 'b', 0, 'H', 0
  }
};

/* stringDescriptor for the product name */
const stringDescriptor_t strProduct_eng =
{
  0x34,
  0x03,
  {
    'H', 0, 'i', 0, 'l', 0, 's', 0, 'c', 0, 'h', 0, 'e', 0, 'r', 0,
    ' ', 0, 'n', 0, 'e', 0, 't', 0, 'X', 0, ' ', 0, 'D', 0, 'e', 0,
    'v', 0, 'e', 0, 'l', 0, 'o', 0, 'p', 0, 'm', 0, 'e', 0, 'n', 0,
    't', 0
  }
};


const stringDescriptor_t strInterfaceFullDesc_eng =
{
  0x0a,
  0x03,
  {
    'i', 0, 'f', 0, 'f', 0, 'e', 0
  }
};


const stringDescriptor_t strInterfaceHiDesc_eng =
{
  0x0a,
  0x03,
  {
    'i', 0, 'f', 0, 'h', 0, 'e', 0
  }
};


const stringDescriptor_t *engStrings[] =
{
  &strManufacturer_eng,
  &strProduct_eng,
  &strSerialNumber_eng,
  &strInterfaceHiDesc_eng
};

/* --------------------------------------------------------------------------- */

/* Device Descriptor */
const unsigned char Device_Descriptor[] =
{
  0x12,                                   /* length of desc */
  0x01,                                   /* descriptor type */
  0x00, 0x02,                             /* USB spec revision level (BCD) */
  0x02,                                   /* device class */
  0x00,                                   /* device subclass */
  0x00,                                   /* device protocol */
  Usb_Ep0_PacketSize,                     /* max packet size */
  (VendorID&0xff), (VendorID>>8),         /* vendor ID */
  (ProductID&0xff), (ProductID>>8),       /* product ID */
  (ProductRel>>8), (ProductRel&0xff),     /* product release ID (BCD) */
  0x01,                                   /* index of manufacturer StringDesc */
  0x02,                                   /* index of product StringDesc */
  0x03,                                   /* index of serial number StringDesc */
  0x01                                    /* number of configurations */
};

/* Configuration Descriptor */
const unsigned char Configuration_Descriptor[] =
{
  0x09,                                   /* length of desc */
  0x02,                                   /* descriptor type */
  0x3e, 0x00,                             /* total length of returned data */
  0x02,                                   /* number of interfaces */
  0x01,                                   /* number of this configuration */
  0x04,                                   /* index of config StringDescriptor */
  0xc0,                                   /* attribute (self powered, no wakeup-support) */
  0x00,                                   /* max power (in 2mA units)*/

  /* interface 0 descriptor */
  0x09,                                   /* length of desc */
  0x04,                                   /* descriptor type = interface descriptor */
  0x00,                                   /* interface number */
  0x00,                                   /* alternate setting */
  0x01,                                   /* number of (non-zero) EPs */
  0x02,                                   /* interface class */
  0x02,                                   /* interface subclass */
  0x01,                                   /* interface protocol */
  0x00,                                   /* interface StringDescriptor index */

  /* something */
  0x05,                                   /* length of desc */
  0x24,                                   /* descriptor type = CS_INTERFACE */
  0x00,                                   /* subtype = Header Functional Descriptor */
  0x01, 0x01,                             /* USB CDC Version 01.01 */

  /* something */
  0x05,                                   /* length of desc */
  0x24,                                   /* descriptor type = CS_INTERFACE */
  0x01,                                   /* subtype = Call Management Descriptor */
  0x00,                                   /* no call management */
  0x01,                                   /* ignored for 'no call management' */

  /* something */
  0x04,                                   /* length of desc */
  0x24,                                   /* descriptor type = CS_INTERFACE */
  0x02,                                   /* subtype = Abstract Control Management Descriptor */
  0x02,                                   /* set features, see 'usbcdc11.pdf', page47 */

  /* endpoint 1 descriptor */
  0x07,                                   /* length */
  0x05,                                   /* descriptor type = endpoint descriptor */
  0x81,                                   /* endpoint 0x01 */
  0x03,                                   /* interrupt */
  Usb_Ep1_PacketSize, 0x00,               /* max packet size */
  0xff,                                   /* polling interval */

  /* interface 1 descriptor */
  0x09,                                   /* length of desc */
  0x04,                                   /* descriptor type = interface descriptor */
  0x01,                                   /* interface number */
  0x00,                                   /* alternate setting */
  0x02,                                   /* number of (non-zero) EPs */
  0x0a,                                   /* interface class for CDC data channel */
  0x00,                                   /* interface subclass */
  0x00,                                   /* interface protocol */
  0x00,                                   /* interface StringDescriptor index */

  /* endpoint 2 descriptor */
  0x07,                                   /* length */
  0x05,                                   /* descriptor type = endpoint descriptor */
  0x83,                                   /* endpoint 0x83 */
  0x02,                                   /* bulk endpoint */
  Usb_Ep2_PacketSize, 0x00,               /* max packet size */
  0x01,                                   /* polling interval (ignored for bulk) */

  /* endpoint 3 descriptor */
  0x07,                                   /* length */
  0x05,                                   /* descriptor type = endpoint descriptor */
  0x02,                                   /* endpoint 0x02 */
  0x02,                                   /* bulk endpoint */
  Usb_Ep3_PacketSize, 0x00,               /* max packet size */
  0x01                                    /* polling interval (ignored for bulk) */
};

void sendDescriptor(unsigned int reqiredLength, unsigned int descriptorLength, const unsigned char *descriptor)
{
  unsigned int sendLength;


  if( reqiredLength < descriptorLength)
  {
    sendLength = reqiredLength;
  }
  else
  {
    sendLength = descriptorLength;
  }

  usb_io_sendDataPacket(sendLength, descriptor);
}


packet_handler_stall_req_t getDescriptor(setupPacket_t *pPacket)
{
  USB_DescriptorType_t descriptorType;
  unsigned int descriptorIndex;
  unsigned int uiLength;


  descriptorIndex = (pPacket->tHeader.wValue&0x0f);
  descriptorType = (USB_DescriptorType_t)(pPacket->tHeader.wValue>>8);
  uiLength = pPacket->tHeader.wLength;

  /* GetDescriptor Request */
  switch( descriptorType )
  {
  case USB_DescriptorType_Device:
    sendDescriptor(uiLength, sizeof(Device_Descriptor), Device_Descriptor);
    return PKT_HANDLER_Do_Not_Send_Stall;

  case USB_DescriptorType_Configuration:
    sendDescriptor(uiLength, sizeof(Configuration_Descriptor), Configuration_Descriptor);
    return PKT_HANDLER_Do_Not_Send_Stall;

  case USB_DescriptorType_String:
    /* language index is wIndex, string index is the descriptor index */
    sendStringDescriptor(pPacket->tHeader.wIndex, descriptorIndex, uiLength);
    return PKT_HANDLER_Do_Not_Send_Stall;
/*
  case USB_DescriptorType_DeviceQualifier:
    sendDescriptor(uiLength, sizeof(Device_Qualifier), Device_Qualifier);
    return PKT_HANDLER_Do_Not_Send_Stall;
*/
  default:
    /* Other requests are not supported */
    return PKT_HANDLER_Send_Stall;
  }
}


void sendStringDescriptor(unsigned int languageId, unsigned int stringIdx, unsigned int wLength)
{
  if( stringIdx==0 )
  {
    /* config */
    sendDescriptor(wLength, sizeof(Top_String_Descriptor), Top_String_Descriptor);
  }
  else
  {
    --stringIdx;

    if( languageId==0x0409 && stringIdx<4 )
    {
      /* send english strings */
      sendDescriptor(wLength, engStrings[stringIdx]->length, (const unsigned char*)(*(engStrings+stringIdx)));
    }
    else
    {
      /* unsupported language => STALL */
      usb_io_sendStall();
    }
  }
}
