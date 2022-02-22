#ifndef __HW_DEFINES_H
#define __HW_DEFINES_H

#include "regdef_netx51.h" /* used for addresses and structures */

#define __NETX51 1

#define _HW_CONCAT(a,b) a ## b
#define HW_MSK(bf)           _HW_CONCAT(MSK_NX51_, bf)
#define HW_SRT(bf)           _HW_CONCAT(SRT_NX51_, bf)
#define HW_MSK_USED_BITS(bf) _HW_CONCAT(MSK_USED_BITS_NX51_, bf)
#define HW_DFLT_BF_VAL(bf)   _HW_CONCAT(DFLT_BF_VAL_NX51_, bf)
#define HW_DFLT_VAL(reg)     _HW_CONCAT(DFLT_VAL_NX51_, reg)
#define HW_REGADR(reg)       _HW_CONCAT(Adr_NX51_, reg)
#define HW_AREAADR(area)     _HW_CONCAT(Addr_NX51_, area)
#define HW_TYPE(area)        _HW_CONCAT(NX51_, area)

#define HW_BFGET(bf, value) (((value) & HW_MSK(bf)) >> HW_SRT(bf))
#define HW_BFSET(bf, value) ((uint32_t)(value) << HW_SRT(bf))

#define HW_DEV_FREQUENCY 100000000L // 100 Mhz

#define HW_PTR_ASIC_CTRL(var)          static NX51_ASIC_CTRL_AREA_T*            const var   = (NX51_ASIC_CTRL_AREA_T*) Addr_NX51_asic_ctrl;

#define HW_PTR_CANCTRL(var)            static NX51_CANCTRL_AREA_T*              const var[] = \
  {\
    (NX51_CANCTRL_AREA_T*) Addr_NX51_canctrl \
  };

#define HW_PTR_UART(var)               static NX51_UART_AREA_T*                 const var[] = \
  {\
    (NX51_UART_AREA_T*) Addr_NX51_uart0,\
    (NX51_UART_AREA_T*) Addr_NX51_uart1,\
    (NX51_UART_AREA_T*) Addr_NX51_uart2,\
  };

#define HW_PTR_USB_DEV_CTRL(var)       static NX51_USB_DEV_CTRL_AREA_T*         const var   = (NX51_USB_DEV_CTRL_AREA_T*) Addr_NX51_usb_dev_ctrl;
#define HW_PTR_USB_DEV_ENUM_RAM(var)   static uint32_t*                         const var   = (uint32_t*) Addr_NX51_usb_dev_enum_ram;
#define HW_PTR_USB_DEV_FIFO_CTRL(var)  static NX51_USB_DEV_FIFO_CTRL_AREA_T*    const var   = (NX51_USB_DEV_FIFO_CTRL_AREA_T*) Addr_NX51_usb_dev_fifo_ctrl;
#define HW_PTR_USB_DEV_FIFO(var)       static NX51_USB_DEV_FIFO_AREA_T*         const var   = (NX51_USB_DEV_FIFO_AREA_T*) Addr_NX51_usb_dev_fifo;

/*** Access Macros ***/
#ifndef HW_WR32
#define HW_WR32(var,val) (var) = (val)
#endif

#ifndef HW_RD32
#define HW_RD32(var)      (var)
#endif

#ifndef HW_WR16
#define HW_WR16(var,val) (var) = (val)
#endif

#ifndef HW_RD16
#define HW_RD16(var)      (var)
#endif

#ifndef HW_WR8
#define HW_WR8(var,val)  (var) = (val)
#endif

#ifndef HW_RD8
#define HW_RD8(var)       (var)
#endif

/* Copy data from netX to Host */
#ifndef HW_RDMEM
#define HW_RDMEM(dst, src, len) memcpy(dst, src, len)
#endif

/* Copy data from Host to netX */
#ifndef HW_WRMEM
#define HW_WRMEM(dst, src, len) memcpy(dst, src, len)
#endif

#endif /* __HW_DEFINES_H */
