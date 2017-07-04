#ifndef __GPIO_C__
#define __GPIO_C__

#include <common.h>
#include <gpio.h>

/* Global instance */
static peripheral_ctx_t gpio;

void GPIO_PUT32(unsigned int addr, unsigned int value) {
  *(gpio.base_addr + addr) = value;

}/*GPIO_PUT32*/

unsigned int GPIO_GET32(unsigned int addr) {
  return(*(gpio.base_addr + addr));

}/*GPIO_GET32*/

int gpio_memmap (void) {
  peripheral_ctx_t *pi3b_gpio = &gpio;

  if ((pi3b_gpio->mem_fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0) {
    fprintf(stderr, "Failed to open mem device\n");
    return -1;
  }
  
  /* Mapping GPIO Physical memeory into virtual memory */
  pi3b_gpio->map = mmap(NULL, /*Let the Kernel decisides for us*/
                        BLOCK_SIZE,
                        (PROT_READ | PROT_WRITE), /*The Mapped region is protected*/
                        MAP_SHARED, /*Shared accross Process*/
                        pi3b_gpio->mem_fd, /*File Descriptor of mapped region*/
                        GPIO_IO);

  if(pi3b_gpio->map == MAP_FAILED) {
    perror("mmap");
    return -1;		
  }	
  
  /* Virtual Address mapped by Kernel */
  pi3b_gpio->base_addr = (volatile unsigned int *)pi3b_gpio->map;
  return 0;
}/*gpio_memmap*/


#endif /*__GPIO_C__*/
