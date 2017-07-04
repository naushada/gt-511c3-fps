#ifndef __UART_PLL011_H__
#define __UART_PLL011_H__

#include <common.h>

/*Address Offset for UART_PLL011*/
#define BCM2709_UART_PLL011 0x00201000

#define UART_BASE  (PERIPHERAL_BASE + BCM2709_UART_PLL011)

/*UART Register's Address*/

#define DR      (0x00 >> 2)
#define RSRECR  (0x04 >> 2)
#define FR      (0x18 >> 2)
#define ILPR    (0x20 >> 2)
#define IBRD    (0x24 >> 2)
#define FBRD    (0x28 >> 2)
#define LCRH    (0x2C >> 2)
#define CR      (0x30 >> 2)
#define IFLS    (0x34 >> 2)
#define IMSC    (0x38 >> 2)
#define RIS     (0x3C >> 2)
#define MIS     (0x40 >> 2)
#define ICR     (0x44 >> 2)
#define DMACR   (0x48 >> 2)
#define ITCR    (0x80 >> 2)
#define ITIP    (0x84 >> 2)
#define ITOP    (0x88 >> 2)
#define TDR     (0x8C >> 2)

/*DR - Data Register Break up*/
#define DR_FE      (1 << 8)
#define DR_PE      (1 << 9)
#define DR_BE      (1 << 10)
#define DR_OE      (1 << 11)
#define DR_DATA    0xFF

/*RSRECR - Break up*/
#define RSRECR_FE  (1 << 0)
#define RSRECR_PE  (1 << 1)
#define RSRECR_BE  (1 << 2)
#define RSRECR_OE  (1 << 3)

/*FR Register's Break up*/

#define FR_RI      (1 << 8)
#define FR_TXFE    (1 << 7)
#define FR_RXFF    (1 << 6)
#define FR_TXFF    (1 << 5)
#define FR_RXFE    (1 << 4)
#define FR_BUSY    (1 << 3)
#define FR_DCD     (1 << 2)
#define FR_DSR     (1 << 1)
#define FR_CTS     (1 << 0)

/*ILPR Register's Break up*/
#define ILPR_ILPR       0xFFFFFFFF

/*IBRD Register's Break up*/
#define IBRD_IBRD       0x0000FFFF

/*FBRD Register's break up*/
#define FBRD_FBRD       0x0000FFFF

/*LCRH Register's details*/

#define LCRH_SPS   (1 << 7)
#define LCRH_WLEN8 (3 << 5)
#define LCRH_FEN   (1 << 4)
#define LCRH_STP2  (1 << 3)
#define LCRH_EPS   (1 << 2)
#define LCRH_PEN   (1 << 1)
#define LCRH_BRK   (1 << 0)

/*CR Register's Break UP*/
#define CR_CTSEN   (1 << 15)
#define CR_RTSEN   (1 << 14)
#define CR_OUT2    (1 << 13)
#define CR_OUT1    (1 << 12)
#define CR_RTS     (1 << 11)
#define CR_DTR     (1 << 10)
#define CR_RXE     (1 << 9)
#define CR_TXE     (1 << 8)
#define CR_LBE     (1 << 7)
#define CR_SIRLP   (1 << 2)
#define CR_SIREN   (1 << 1)
#define CR_UARTEN  (1 << 0)

/*IFLS Register's break up*/
#define IFLS_RXIFPSEL   (7 << 9)
#define IFLS_TXIFPSEL   (7 << 6)
#define IFLS_RXIFLSEL   (7 << 3)
#define IFLS_TXIFLSEL   (7 << 0)

/*IMSC Register's break up*/
#define IMSC_OEIM       (1 << 10)
#define IMSC_BEIM       (1 << 9)
#define IMSC_PEIM       (1 << 8)
#define IMSC_FEIM       (1 << 7)
#define IMSC_RTIM       (1 << 6)
#define IMSC_TXIM       (1 << 5)
#define IMSC_RXIM       (1 << 4)
#define IMSC_DSRMIM     (1 << 3)
#define IMSC_DCDMIM     (1 << 2)
#define IMSC_CTSMIM     (1 << 1)
#define IMSC_RIMIM      (1 << 0)

/*RIS Regisater's break up*/
#define RIS_OERIS      (1 << 10)
#define RIS_BERIS      (1 << 9)
#define RIS_PERIS      (1 << 8)
#define RIS_FERIS      (1 << 7)
#define RIS_RTRIS      (1 << 6)
#define RIS_TXRIS      (1 << 5)
#define RIS_RXRIS      (1 << 4)
#define RIS_DSRRMIS    (1 << 3)
#define RIS_DCDRMIS    (1 << 2)
#define RIS_CTSRMIS    (1 << 1)
#define RIS_RIRMIS     (1 << 0)

/*MIS Register's break up*/
#define MIS_OEMIS      (1 << 10)
#define MIS_BEMIS      (1 << 9)
#define MIS_PEMIS      (1 << 8)
#define MIS_FEMIS      (1 << 7)
#define MIS_RTMIS      (1 << 6)
#define MIS_TXMIS      (1 << 5)
#define MIS_RXMIS      (1 << 4)
#define MIS_DSRMMIS    (1 << 3)
#define MIS_DCDMMIS    (1 << 2)
#define MIS_CTSMMIS    (1 << 1)
#define MIS_RIMMIS     (1 << 0)

/*ICR Register's break up*/
#define ICR_OEIC       (1 << 10)
#define ICR_BEIC       (1 << 9)
#define ICR_PEIC       (1 << 8)
#define ICR_FEIC       (1 << 7)
#define ICR_RTIC       (1 << 6)
#define ICR_TXIC       (1 << 5)
#define ICR_RXIC       (1 << 4)
#define ICR_DSRMIC     (1 << 3)
#define ICR_DCDMIC     (1 << 2)
#define ICR_CTSMIC     (1 << 1)
#define ICR_RIMIC      (1 << 0)

/*ITCR register's break up*/
#define ITCR_ITCR1     (1 << 1)
#define ITCR_ITCR0     (1 << 0)

/*ITIP Register's break up*/
#define ITIP_ITIP3     (1 << 3)
#define ITIP_ITIP0     (1 << 0)

/*ITOP Register's break up*/
#define ITOP_ITOP11    (1 << 11)
#define ITOP_ITOP10    (1 << 10)
#define ITOP_ITOP9     (1 << 9)
#define ITOP_ITOP8     (1 << 8)
#define ITOP_ITOP7     (1 << 7)
#define ITOP_ITIP6     (1 << 6)
#define ITOP_ITIP3     (1 << 3)
#define ITOP_ITIP0     (1 << 0)

/*TDR Register's break up*/
#define TDR_TDR10_0    0x000003FF

typedef void (*rx_call_back)(unsigned char *rsp_data, unsigned int rsp_len);

int uart_memmap(void);

void UART0_PUT32(unsigned int reg, unsigned int value);

unsigned int UART0_GET32(unsigned int reg);

void disable_uart(void);

void enable_uart(void);

void enable_tx(void);

void enable_rx(void);

void disable_tx(void);

void disable_rx(void);

void flush_fifo(void);

void enable_fifo(void);

void uart_putc(unsigned char c);

void uart_init(void);

void *poll_receiver(void *thread_arg);

void uart_write(unsigned char *data_ptr, unsigned char data_len);

void uart_register_cb(void (*Fn)(unsigned char *rsp_ptr, unsigned int len));

void uart_set_baudrate(unsigned int baudrate);

#endif /*__UART_PLL011_H__*/

