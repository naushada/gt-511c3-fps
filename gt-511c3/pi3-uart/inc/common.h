#ifndef __COMMON_H__
#define __COMMON_H__

#include <assert.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#define PERIPHERAL_BASE       0x3F000000
#define BLOCK_SIZE            (4 * 1024)


typedef struct {
  /*Physical Address of GPIO*/
  unsigned long         physical_base_address;
  int                   mem_fd;
  /*Kernel Address map for Physical Address of GPIO*/
  void                  *map;
  volatile unsigned int *base_addr;
}peripheral_ctx_t;

#endif /*__COMMON_H__*/
