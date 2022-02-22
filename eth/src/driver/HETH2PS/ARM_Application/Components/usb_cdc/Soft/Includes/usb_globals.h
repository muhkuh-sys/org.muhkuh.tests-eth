#ifndef __USB_GLOBALS_H
#define __USB_GLOBALS_H

#include "usb_requests_common.h"

/* --------------------------------------------------------------------------- */

#define VendorID                0x1939
#define ProductID               0x0fff
#define ProductRel              0x0001

/* --------------------------------------------------------------------------- */

/* max packet size for end points */
#define Usb_Ep0_PacketSize      0x40
#define Usb_Ep1_PacketSize      0x40
#define Usb_Ep2_PacketSize      0x40
#define Usb_Ep3_PacketSize      0x40

/* end point buffers */
#define Usb_Ep0_Buffer          0x0000
#define Usb_Ep1_Buffer          0x0080
#define Usb_Ep2_Buffer          0x0100
#define Usb_Ep3_Buffer          0x0180

/* --------------------------------------------------------------------------- */

typedef enum
{
  USB_State_Attached = 0,
  USB_State_Powered,
  USB_State_Default,
  USB_State_AddressChangePending,
  USB_State_Address,
  USB_State_Configured,
  USB_State_Suspended
} USB_State_t;

typedef enum
{
        USB_DescriptorType_Device                       = 0x01,
        USB_DescriptorType_Configuration                = 0x02,
        USB_DescriptorType_String                       = 0x03,
        USB_DescriptorType_Interface                    = 0x04,
        USB_DescriptorType_Endpoint                     = 0x05,
        USB_DescriptorType_DeviceQualifier              = 0x06,
        USB_DescriptorType_OtherSpeedConfiguration      = 0x07,
        USB_DescriptorType_InterfacePower               = 0x08
} USB_DescriptorType_t;

typedef enum
{
        USB_EndpointState_Unconfigured                  = 0x00,
        USB_EndpointState_Running,
        USB_EndpointState_Halted
} USB_EndpointState_t;

typedef enum
{
        USB_SetupTransaction_NoOutTransaction           = 0,
        USB_SetupTransaction_OutTransaction             = 1
} USB_SetupTransaction_t;

typedef enum
{
        USB_ReceiveEndpoint_Running                     = 0,
        USB_ReceiveEndpoint_Blocked                     = 1
} USB_ReceiveEndpoint_t;

typedef enum
{
        USB_SendEndpoint_Idle                           = 0,
        USB_SendEndpoint_Running                        = 1
} USB_SendEndpoint_t;

typedef enum
{
	USB_CDC_ConnectionState_Idle			= 0,
	USB_CDC_ConnectionState_Connected		= 1
} USB_CDC_ConnectionState_t;

/* --------------------------------------------------------------------------- */

extern USB_State_t globalState;

extern unsigned int currentConfig;
extern unsigned int uiCurrentInterface;

/* buffer for setup packets */
extern unsigned char setupBuffer[Usb_Ep0_PacketSize];
extern unsigned char receiveBuffer[Usb_Ep2_PacketSize];
extern unsigned char sendBuffer[Usb_Ep3_PacketSize];

/* decoded packet */
extern setupPacket_t tSetupPkt;
/* out transaction needed */
extern USB_SetupTransaction_t tOutTransactionNeeded;

extern USB_ReceiveEndpoint_t tReceiveEpState;
extern USB_SendEndpoint_t tSendEpState;
extern unsigned int uiLastPacketSize;

/* new address for pending address change */
extern unsigned int uiNewAddress;

extern USB_EndpointState_t endpoint0State;
extern USB_EndpointState_t endpoint1State;
extern USB_EndpointState_t endpoint2State;
extern USB_EndpointState_t endpoint3State;

/* CDC Connection State */
extern USB_CDC_ConnectionState_t tCdcConnectionState;

/* --------------------------------------------------------------------------- */

/* Prototypes */
int usb_receiveChar(void);
USB_CDC_ConnectionState_t usb_pollConnection(void);
void usb_init(void);
void usb_sendChar(unsigned char uData);
void usb_sendFinish(void);
void usb_sendBuffer(void);
void usb_sendPendingPacket(void);
void usb_pingpong(void);
void usb_handleReset(void);
void usb_configurePipes(void);
void usb_unconfigurePipes(void);
void usb_activateInputPipe(void);
void usb_func(void);

#endif  /* __USB_GLOBALS_H */
