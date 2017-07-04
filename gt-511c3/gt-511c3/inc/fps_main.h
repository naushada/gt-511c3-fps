#ifndef __FPS_MAIN_H__
#define __FPS_MAIN_H__

#include <fps.h>

#define RESPONSE_LENGTH        12
#define COMMAND_LENGTH         RESPONSE_LENGTH
#define DATA_PACKET_FIX_LEN    (4 + 2)
/*+2 for check sum, 28 for 16+4+4+2+1+1*/
#define OPEN_RESPONSE_LENGTH   (RESPONSE_LENGTH + DATA_PACKET_FIX_LEN + 24)


typedef enum {
  OPEN_ST    = (1 << 0xF),
  CLOSE_ST,
  BAUD_RATE_CHANGE_ST,
  USB_INTERNAL_CHECK_ST,
  SET_IAP_MODE_ST,
  LED_ON_ST,
  LED_OFF_ST,
  GET_ENROLL_COUNT_ST,
  CHECK_ENROLL_ST,
  ENROLL_START_ST,
  ENROLL1_ST,
  ENROLL2_ST,
  ENROLL3_ST,
  FINGER_PRESSED_ST,
  DELETE_ID_ST,
  DELETE_ALL_ST,
  VERIFY_ST,
  VERIFY_TEMPLATE_ST,
  IDENTIFY_TEMPLATE_ST,
  CAPTURE_FINGER_ST,
  MAKE_TEMPLATE_ST,
  GET_IMAGE_ST,
  GET_RAW_IMAGE_ST,
  GET_TEMPLATE_ST,
  SET_TEMPLATE_ST,
  DONE_ST    
}fps_state_t;

typedef enum {
  FPS_RESPONSE_SIG = (1 << 1),
  FPS_COMMAND_SIG  = (1 << 2),
  FPS_DONE_SIG     = (1 << 3)

}fps_command_sig_t;

typedef struct {

  fps_state_t command_state[sizeof(fps_state_t)];
  fps_state_t *command_state_ptr;
  uint32      in_parameter;
  uint32      command_data_len;
  uint8       *command_data_ptr;
  uint16      response_length;
  uint16      command_length;
  uint8       *response_ptr;
  uint32      offset;
  uint16      command;
  uint8       is_command_in_progress;
  fps_command_sig_t sig_mask;

}fps_ctx_t; 



void fps_process_open_req(void);

void fps_process_scan_finger_req(void);

void fps_process_request(void);

void fps_process_next_state(void);

void fps_init_done_state(void);

void fps_process_done_req(void);

uint32 fps_display_option(void);

void response_cb(unsigned char *rsp_ptr, unsigned int rsp_len);

void fps_process_command_response(uint8 *response_ptr, uint16 length);

void fps_process_command(uint32 command, uint32 in_parameter, uint32 data_len, uint8 *data);

void fps_process_set_template_req(uint32 command);
void fps_process_get_template_req(uint32 command);
void fps_process_get_raw_image_req(uint32 command);
void fps_process_get_image_req(uint32 command);
void fps_process_make_template_req(uint32 command);
void fps_process_capture_finger_req(uint32 command);
void fps_process_identify_template_req(uint32 command);
void fps_process_verify_template_req(uint32 command);
void fps_process_verify_req(uint32 command);
void fps_process_delete_all_req(uint32 command);

void fps_process_delete_id_req(uint32 command);

void fps_process_finger_pressed_req(uint32 command);

void fps_build_send_command(uint32 command, uint32 param, uint32 rsp_length);

void fps_process_enroll3_req(uint32 command);

void fps_process_enroll2_req(void);

void fps_process_enroll1_req(void);

void fps_process_enroll_start_req(void);

void fps_process_check_enroll_req(void);

void fps_process_enroll_count_req(void);

void fps_process_led_off_req(void);

void fps_process_led_on_req(void);

void fps_process_set_iap_mode_req(void);

void fps_process_usb_internal_check_req(void);

void fps_process_baudrate_change_req(uint32 command);

void fps_process_close_req(void);

void fps_save_image(uint8 *file_name, uint8 *response_ptr, uint16 response_length);

#endif /*__FPS_MAIN_H__*/
