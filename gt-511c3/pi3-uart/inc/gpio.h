#ifndef __GPIO_H__
#define __GPIO_H__

#include <common.h>


#define GPIO_BASE             0x00200000
#define GPIO_IO               (PERIPHERAL_BASE + GPIO_BASE)

#define SHIFTS                2

#define GPFSEL0               (0x00 >> SHIFTS)
#define GPFSEL1               (0x04 >> SHIFTS)
#define GPFSEL2               (0x08 >> SHIFTS)
#define GPFSEL3               (0x0C >> SHIFTS)
#define GPFSEL4               (0x10 >> SHIFTS)
#define GPFSEL5               (0x14 >> SHIFTS)
/*Reserved named as GPFSEL6*/
#define GPFSEL6               (0x18 >> SHIFTS)

#define GPSET0                (0x1C >> SHIFTS)
#define GPSET1                (0x20 >> SHIFTS)
/*Reserved*/
#define GPSET2                (0x24 >> SHIFTS)

#define GPCLR0                (0x28 >> SHIFTS)
#define GPCLR1                (0x2C >> SHIFTS)

/*Reserved*/
#define GPCLR2                (0x30 >> SHIFTS)


#define GPLEV0                (0x34 >> SHIFTS)
#define GPLEV1                (0x38 >> SHIFTS)
#define GPLEV2                (0x3C >> SHIFTS)

#define GPEDS0                (0x40 >> SHIFTS)
#define GPEDS1                (0x44 >> SHIFTS)
#define GPEDS2                (0x48 >> SHIFTS)

#define GPREN0                (0x4C >> SHIFTS)
#define GPREN1                (0x50 >> SHIFTS)
#define GPREN2                (0x54 >> SHIFTS)

#define GPFEN0                (0x58 >> SHIFTS)
#define GPFEN1                (0x5C >> SHIFTS)
#define GPFEN2                (0x60 >> SHIFTS)

#define GPHEN0                (0x64 >> SHIFTS)
#define GPHEN1                (0x68 >> SHIFTS)
#define GPHEN2                (0x6C >> SHIFTS)


#define GPLEN0                (0x70 >> SHIFTS)
#define GPLEN1                (0x74 >> SHIFTS)
#define GPLEN2                (0x78 >> SHIFTS)

#define GPAREN0               (0x7C >> SHIFTS)
#define GPAREN1               (0x80 >> SHIFTS)
#define GPAREN2               (0x84 >> SHIFTS)

#define GPAFEN0               (0x88 >> SHIFTS)
#define GPAFEN1               (0x8C >> SHIFTS)
#define GPAFEN2               (0x90 >> SHIFTS)

#define GPPUD                 (0x94 >> SHIFTS)
#define GPPUDCLK0             (0x98 >> SHIFTS)
#define GPPUDCLK1             (0x9C >> SHIFTS)




void GPIO_PUT32(unsigned int addr, unsigned int value);


unsigned int GPIO_GET32(unsigned int addr);

int gpio_memmap (void);

#endif /*GPIO_H__*/
