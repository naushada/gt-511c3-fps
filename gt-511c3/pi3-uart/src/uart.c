#ifndef __UART_PLL011_C__
#define __UART_PLL011_C__


#include <common.h>
#include <uart.h>
#include <gpio.h>

/* Global instance */
static peripheral_ctx_t uart0;
static rx_call_back uart_recv_cb;
static pthread_t    thread_id;

void uart_set_baudrate(unsigned int baudrate) {
  double baud_value = 0.0;
  unsigned int cr_value = 0;
  unsigned short int_value = 0x00;
  unsigned short fractional_value = 0x00;
  /*
    4)//(48000000 / (16 * 115200) = 26.0416
      //(0.0416 * 64 + 0.5) = 3

    3)//(48000000 / (16 * 9600)) = 312.5 = IBRD=312
      //(0.5 * 64 + 0.5) = 32
  */
  baud_value = (48000000 / (baudrate * 16.0));
  int_value = (unsigned short)baud_value;
  /*Four digits after decimal*/
  fractional_value =  ((baud_value - int_value) * 64) + 0.5;
 
#if 0 
  cr_value = UART0_GET32(CR);
  printf("CR value is %X\n", cr_value);
  cr_value &= ~CR_UARTEN;
  /*Disable the UART*/
  UART0_PUT32(CR, cr_value);
  printf("int_value %d fractional_value %d baud_value is %f\n", int_value, fractional_value, baud_value);
#endif
  UART0_PUT32(IBRD, int_value); 
  UART0_PUT32(FBRD, fractional_value);

#if 0
  UART0_PUT32(LCRH, (LCRH_WLEN8 |LCRH_FEN));

  /*Enabling the UART*/
  cr_value = UART0_GET32(CR);
  cr_value |= CR_UARTEN; 
  UART0_PUT32(CR, cr_value);
#endif
    
}/*uart_set_baudrate*/


void uart_register_cb(void (*Fn)(unsigned char *rsp_ptr, unsigned int len)) {
  unsigned int rc = 0;
  /*Initializing call back*/
  uart_recv_cb = Fn;
  /*Creating the Thread for receiving the response from UART*/
  rc = pthread_create(&thread_id, (void *)0, poll_receiver, (void *)0);

}/*uart_register_cb*/


int uart_memmap(void) {
  peripheral_ctx_t *pi3b_uart = &uart0;

  if ((pi3b_uart->mem_fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0) {
    fprintf(stderr, "Failed to open mem device\n");
    return -1;
  }
  
  /* Mapping GPIO Physical memeory into virtual memory */
  pi3b_uart->map = mmap(NULL, /*Let the Kernel decisides for us*/
                        BLOCK_SIZE,
                        (PROT_READ | PROT_WRITE), /*The Mapped region is protected*/
                        MAP_SHARED, /*Shared accross Process*/
                        pi3b_uart->mem_fd, /*File Descriptor of mapped region*/
                        UART_BASE);

  if (pi3b_uart->map == MAP_FAILED) {
    perror("mmap");
    return -1;		
  }	
  
  /* Virtual Address mapped by Kernel */
  pi3b_uart->base_addr = (volatile unsigned int *)pi3b_uart->map;
  return 0;
}/*memmap*/

void UART0_PUT32(unsigned int reg, unsigned int value) {
  *(uart0.base_addr + reg) = value;

}/*UART0_PUT32*/

unsigned int UART0_GET32(unsigned int reg) {
  return(*(uart0.base_addr + reg));

}/*UART0_GET32*/

void disable_uart(void) {
  unsigned int data = 0x00;
  data = UART0_GET32(CR);
  data &= ~(CR_UARTEN);
  UART0_PUT32(CR, data);

}/*disable_uart*/

void enable_uart(void) {
  unsigned int data = 0x00;
  data = UART0_GET32(CR);
  /*Clear the bit0 first*/
  data &= ~(CR_UARTEN);
  /*set the bit0 now*/
  data |= CR_UARTEN;
  UART0_PUT32(CR, data);

}/*disable_uart*/

void enable_tx(void) {
  unsigned int data = 0x00;
  data = UART0_GET32(CR);
  data &= ~(CR_TXE);
  data |= CR_TXE;
  UART0_PUT32(CR, data);

}/*enable_tx*/

void enable_rx(void) {
  unsigned int data = 0x00;
  data = UART0_GET32(CR);
  data &= ~(CR_RXE);
  data |= CR_RXE;
  UART0_PUT32(CR, data);

}/*enable_rx*/

void disable_tx(void) {
  unsigned int data = 0x00;
  data = UART0_GET32(CR);
  data &= ~(CR_TXE);
  UART0_PUT32(CR, data);

}/*disable_tx*/

void disable_rx(void) {
  unsigned int data = 0x00;
  data = UART0_GET32(CR);
  data &= ~(CR_RXE);
  UART0_PUT32(CR, data);

}/*disable_rx*/

void flush_fifo(void) {
  unsigned int data = 0x00;
  data = UART0_GET32(LCRH);
  data &= ~(LCRH_FEN);
  UART0_PUT32(LCRH, data);
  
}/*flush_fifo*/

void enable_fifo(void) {
  unsigned int data = 0x00;
  data = UART0_GET32(LCRH);
  data &= ~(LCRH_FEN);
  data |= LCRH_FEN;
  UART0_PUT32(LCRH, data);

}/*enable_fifo*/

void uart_write(unsigned char *data_ptr, unsigned char data_len) {
  unsigned int idx = 0;
  while(idx < data_len) {
    uart_putc(data_ptr[idx]);
    idx++;
  }
}/*uart_write*/

void uart_putc(unsigned char c) {
  unsigned int data = 0;

  while(1) {
    /*Wait Until TX-FIFO has a room for new byte*/
    if((UART0_GET32(FR) & FR_TXFF) == 0) break;
  }

  //data = UART0_GET32(DR);
  //data &= ~(DR_DATA);
  data = c;
  UART0_PUT32(DR, data);
}

void uart_init(void) {
  unsigned int data = 0;

  /*Disable the UART*/
  UART0_PUT32(CR, 0);
  
  data = GPIO_GET32(GPFSEL1);
  data &= ~(7<<12); //gpio14
  data |= 4<<12;    //alt0
  data &= ~(7<<15); //gpio15
  data |= 4<<15;    //alt0
  
  GPIO_PUT32(GPFSEL1, data);
  GPIO_PUT32(GPPUD, 0);
  
  for(data = 0; data < 150; data++) {
    asm volatile("nop");
  }
  GPIO_PUT32(GPPUDCLK0, ((1 << 14) | (1 << 15))); 
  
  for(data = 0; data < 150; data++) {
    asm volatile("nop");
  }
  
  GPIO_PUT32(GPPUDCLK0, 0); 

  UART0_PUT32(ICR,0x7FF);
  /*1)//(3000000 / (16 * 115200) = 1.627 ==> IBRD=1
      //(0.627*64)+0.5 = 40 ==> FBRD=40
      //int 1 frac 40
    2)//(3000000 / (16 * 9600)) = 19.2 ==> IBRD=19
      //(0.53125 * 64 + 0.5) = 34.5 ==>FBRD=34.5
      UART_CLK is 48MHz, look for the Kernel command line argument by dmesg tailf
    3)//(48000000 / (16 * 9600)) = 312.5 = IBRD=312
      //(0.5 * 64 + 0.5) = 32
    4)//(48000000 / (16 * 115200) = 26.0416
      //(0.0416 * 64 + 0.5) = 3
  */
  uart_set_baudrate(9600);
  //UART0_PUT32(IBRD, 312); 
  //UART0_PUT32(FBRD, 32);
  /*8Bits word no Parity and no stop bits*/
  UART0_PUT32(LCRH, (LCRH_WLEN8 |LCRH_FEN));
  //UART0_PUT32(LCRH, 0x60);

  UART0_PUT32(CR, (CR_TXE |CR_RXE |CR_UARTEN)); 
}/*uart_init*/

void *poll_receiver(void *thread_arg) {
  (void) thread_arg;
  unsigned char rsp_byte;
  FILE *fp = NULL;
  fp = fopen("./recv_char.txt", "w");
  while(1) {
    /*Wait until RX-FIFO is not empty*/
    if(!(UART0_GET32(FR) & FR_RXFE)) {

      rsp_byte = UART0_GET32(DR) & 0xFF;
      //fprintf(stderr, "0x%0.2X .... %c  \n", rsp_byte, rsp_byte);
      //fwrite(rsp_byte, 1, 1, fp);
      fprintf(fp, "0x%0.2X...%c\n", rsp_byte, rsp_byte);
      uart_recv_cb((unsigned char *)&rsp_byte, 1);
    } 
  }/*while(1)*/
  fclose(fp);
}/*poll_receiver*/

#endif /*__UART_PLL011_C__*/
