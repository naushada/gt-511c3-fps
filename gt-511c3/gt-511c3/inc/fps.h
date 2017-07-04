#ifndef __FPS_H__
#define __FPS_H__


#define uint8   unsigned char
#define int8    char
#define uint16  unsigned short int
#define int16   short int
#define uint32  unsigned int
#define int32   int
#define uint64  unsigned long
#define int63   long

#define FPS_START_CODE1   0x55
#define FPS_START_CODE2   0xAA

/*Command Type*/
typedef enum {
  FPS_OPEN_F = 1,
  FPS_CLOSE_F,
  FPS_USB_INTERNAL_CHECK_F,
  FPS_CHANGE_BAUDRATE_F,
  /*For upgrading the Firmware of FPS - Finger Print Scanner*/
  FPS_SET_IAP_MODE_F,
  FPS_CMOS_LED_F = 0x12,
  FPS_GET_ENROLL_COUNT_F = 0x20,
  FPS_CHECK_ENROLLED_F,
  FPS_ENROLL_START_F,
  FPS_ENROLL1_F,
  FPS_ENROLL2_F,
  FPS_ENROLL3_F,
  FPS_IS_PRESS_FINGER_F,
  FPS_DELETE_ID_F = 0x40,
  FPS_DELETE_ALL_F,
  FPS_VERIFY_F = 0x50,
  FPS_IDENTIFY_F,
  FPS_VERIFY_TEMPLATE_F,
  FPS_IDENTIFY_TEMPLATE_F,
  FPS_CAPTURE_FINGER_F = 0x60,
  FPS_MAKE_TEMPLATE_F,
  FPS_GET_IMAGE_F,
  FPS_GET_RAW_IMAGE_F,
  FPS_GET_TEMPLATE_F = 0x70,
  FPS_SET_TEMPLATE_F,
  FPS_ACK = 0x30,
  FPS_NACK
  
}fps_command_type_t;

/*Error code*/

typedef enum {
  FPS_NACK_TIMEOUT = 0x1001,
  FPS_NACK_INVALID_BAUDRATE,
  /*The Specified ID is not between 0 - 199*/
  FPS_NACK_INVALID_POS,
  FPS_NACK_IS_NOT_USED,
  FPS_NACK_IS_ALREADY_USED,
  FPS_NACK_COMM_ERR,
  FPS_NACK_VERIFY_FAILED,
  FPS_NACK_IDENTIFY_FAILED,
  FPS_NACK_DB_IS_FULL,
  FPS_NACK_DB_IS_EMPTY,
  FPS_NACK_TURN_ERR,
  FPS_BAD_FINGER,
  FPS_NACK_ENROLL_FAILED,
  FPS_NACK_IS_NOT_SUPPORTED,
  /*Device Error, Especially If crypto-chip is trouble*/
  FPS_NACK_DEV_ERR,
  FPS_NACK_CAPTURE_CANCELLED,
  FPS_NACK_INVALID_PARAM,
  FPS_NACK_FINGER_IS_NOT_PRESSED,
  
}fps_err_code_t;


typedef struct {
  /*Value 0x55 is Fixed*/
  uint8   start_code1;
  /*Value 0xAA is Fixed*/
  uint8   start_code2;
  /*Device ID shall be fixed as = 0x0001*/
  uint8   device_id[2];

}fps_header_t;

/*Command/Request format*/

typedef struct {
  fps_header_t hdr;
  /*Parameter of command or response*/
  int8   param[4];
  /*Command code or response code (ACK/NACK)*/
  uint8   code[2];
  /*Check sum is the addition/sum of all above data member.*/
  uint8   chk_sum[2]; 
  
}fps_generic_packet_t;

typedef struct {
  fps_header_t hdr;
  uint8        data_length;
  /*The maximum data packet size could be 52,116 bytes*/
  uint8        *data;
  /*Check sum of all above bytes/data member*/
  uint8        chk_sum[2];
 
}fps_generic_data_packet_t;

typedef struct {
  uint32  firmware_version;
  uint32  iso_area_max_size;
  uint8   serial_number[16];
}fps_open_response_t;

typedef struct {
  uint8  templ[506];

}fps_make_template_response_t;

typedef struct {
  /*Image is of size 240 X 216*/
  uint8  image[240 * 216];
}fps_get_image_response_t;


/*Function Prototype*/
uint8 *fps_build_request(uint32 input_param, 
                         uint16 device_id, 
                         uint16 command, 
                         uint16 *out_len);


uint16 fps_checksum(uint8 *byte_ptr, uint16 len);

uint8 fps_process_nack(uint32 param);

uint8 fps_process_ack(uint8 *response_ptr, uint16 response_length);

#endif
