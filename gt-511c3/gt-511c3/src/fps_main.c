#ifndef __FPS_MAIN_C__
#define __FPS_MAIN_C__

#include "common.h"
#include "gpio.h"
#include "uart.h"
#include "fps.h"
#include "fps_main.h"

/*Global instance of fps context*/
fps_ctx_t ctx_g; 


void fps_process_command_response(uint8 *response_ptr, uint16 length) {
  
  if(FPS_ACK == *((uint16 *)&response_ptr[8])) {
    fps_process_ack(response_ptr, length);
 
  } else if(FPS_NACK == *((uint16 *)&response_ptr[8])) {
    fps_process_nack(*((uint32 *)&response_ptr[4]));
  }

}/*fps_process_command_response*/


void response_cb(unsigned char *rsp_ptr, unsigned int rsp_len) {

  if(ctx_g.offset == (ctx_g.response_length - 1)) {
    /*store last character*/
    memcpy((void *)&ctx_g.response_ptr[ctx_g.offset], rsp_ptr, rsp_len);
    ctx_g.offset += rsp_len;

#ifdef __DEBUG__
    fprintf(stderr, "response length is %d\n", ctx_g.offset);
    unsigned int idx = 0;

    for(idx = 0; idx < ctx_g.offset ; idx++) {
      fprintf(stderr, "0x%0.2X ", ctx_g.response_ptr[idx]);
    }
    fprintf(stderr,"\n");
#endif
    /*Set the response signal so that Context of thread is released*/ 
    ctx_g.sig_mask ^= FPS_RESPONSE_SIG;
    ctx_g.sig_mask |= FPS_RESPONSE_SIG;

  } else {
    if(NULL != ctx_g.response_ptr) {
      /*store the response*/
      memcpy((void *)&ctx_g.response_ptr[ctx_g.offset], rsp_ptr, rsp_len);
      ctx_g.offset += rsp_len;
    } else {
      fprintf(stderr, "Data has been received after de-allocating the allocated buffer\n");
    }
  }

}/*response_cb*/

uint32 fps_display_option(void) {
  uint32 cc = 0;
 
  printf("|================================================|\n"); 
  printf("|....1) OPEN.....................................|\n"); 
  printf("|....2) SENSOR ON................................|\n"); 
  printf("|....3) SENSOR OFF...............................|\n"); 
  printf("|....4) PRESS FINGER.............................|\n"); 
  printf("|....5) CAPTURE FINGER...........................|\n"); 
  printf("|....6) CLOSE....................................|\n"); 
  printf("|....7) GET IMAGE................................|\n"); 
  printf("|....8) GET RAW IMAGE............................|\n"); 
  printf("|....9) SET BAUD RATE............................|\n"); 
  printf("|================================================|\n"); 
  scanf("%d", &cc);
  return(cc);

}/*fps_display_option*/

void fps_process_request(void) {

  switch(*ctx_g.command_state_ptr) {

    case OPEN_ST:
      fps_process_open_req();
      break;

    case CLOSE_ST:
      fps_process_close_req();
      break;

    case BAUD_RATE_CHANGE_ST:
      fps_process_baudrate_change_req(FPS_CHANGE_BAUDRATE_F);
      break;

    case USB_INTERNAL_CHECK_ST:
      fps_process_usb_internal_check_req();
      break;

    case SET_IAP_MODE_ST:
      fps_process_set_iap_mode_req();
      break;

    case LED_ON_ST:
      fps_process_led_on_req();
      break;

    case LED_OFF_ST:
      fps_process_led_off_req();
      break;

    case GET_ENROLL_COUNT_ST:
      fps_process_enroll_count_req();
      break;

    case CHECK_ENROLL_ST:
      fps_process_check_enroll_req();
      break;

    case ENROLL_START_ST:
      fps_process_enroll_start_req();
      break;

    case ENROLL1_ST:
      fps_process_enroll1_req();
      break;

    case ENROLL2_ST:
      fps_process_enroll2_req();
      break;

    case ENROLL3_ST:
      fps_process_enroll3_req(FPS_ENROLL3_F);
      break;

    case FINGER_PRESSED_ST:
      fps_process_finger_pressed_req(FPS_IS_PRESS_FINGER_F);
      break;

    case DELETE_ID_ST:
      fps_process_delete_id_req(FPS_DELETE_ID_F);
      break;

    case DELETE_ALL_ST:
      fps_process_delete_all_req(FPS_DELETE_ALL_F);
      break;

    case VERIFY_ST:
      fps_process_verify_req(FPS_VERIFY_F);
      break;

    case VERIFY_TEMPLATE_ST:
      fps_process_verify_template_req(FPS_VERIFY_TEMPLATE_F);
      break;

    case IDENTIFY_TEMPLATE_ST:
      fps_process_identify_template_req(FPS_IDENTIFY_TEMPLATE_F);
      break;

    case CAPTURE_FINGER_ST:
      fps_process_capture_finger_req(FPS_CAPTURE_FINGER_F);
      printf("capturing Finger\n");
      break;

    case MAKE_TEMPLATE_ST:
      fps_process_make_template_req(FPS_MAKE_TEMPLATE_F);
      break;

    case GET_IMAGE_ST:
      fps_process_get_image_req(FPS_GET_IMAGE_F);
      printf("retrieving image\n");
      break;

    case GET_RAW_IMAGE_ST:
      fps_process_get_raw_image_req(FPS_GET_RAW_IMAGE_F);
      printf("retrieving RAW image\n");
      break;

    case GET_TEMPLATE_ST:
      fps_process_get_template_req(FPS_GET_TEMPLATE_F);
      break;

    case SET_TEMPLATE_ST:
      fps_process_set_template_req(FPS_SET_TEMPLATE_F);
      break;

    case DONE_ST:
      ctx_g.is_command_in_progress = 0;
      memset((void *)&ctx_g, 0, sizeof(ctx_g));
      break;
    default:
      fprintf(stderr, "Default case for process_request 0x%X\n", *ctx_g.command_state_ptr);
      break;
  }/*End of Switch*/

}/*fps_process_request*/

void fps_process_set_template_req(uint32 command) {
  fps_build_send_command(command, 0, RESPONSE_LENGTH);

}/*fps_process_set_template_req*/

void fps_process_get_template_req(uint32 command) {
  fps_build_send_command(command, 0, (RESPONSE_LENGTH + 506));
  
}/*fps_process_get_template_req*/

void fps_process_get_raw_image_req(uint32 command) {
  fps_build_send_command(command, 0, (RESPONSE_LENGTH + DATA_PACKET_FIX_LEN + 19200));
  
}/*fps_process_get_raw_image_req*/

void fps_process_get_image_req(uint32 command) {
  //fps_build_send_command(command, 0, (RESPONSE_LENGTH + DATA_PACKET_FIX_LEN + 51840));
  fps_build_send_command(command, 0, (RESPONSE_LENGTH + DATA_PACKET_FIX_LEN + 52116));
  //fps_build_send_command(command, 0, (RESPONSE_LENGTH + DATA_PACKET_FIX_LEN + 3220));
  
}/*fps_process_get_image_req*/

void fps_process_make_template_req(uint32 command) {
  fps_build_send_command(command, 0, (RESPONSE_LENGTH + 506));

}/*fps_process_make_template_req*/

void fps_process_capture_finger_req(uint32 command) {
  fps_build_send_command(command, 1, RESPONSE_LENGTH);
  
}/*fps_process_capture_finger_req*/

void fps_process_identify_template_req(uint32 command) {
  fps_build_send_command(command, 0, RESPONSE_LENGTH);
  
}/*fps_process_identify_template_req*/


void fps_process_verify_template_req(uint32 command) {
  fps_build_send_command(command, 0, RESPONSE_LENGTH);
  
}/*fps_process_verify_template_req*/

void fps_process_verify_req(uint32 command) {
  fps_build_send_command(command, 0, RESPONSE_LENGTH);

}/*fps_process_verify_req*/

void fps_process_delete_all_req(uint32 command) {
  fps_build_send_command(command, 0, RESPONSE_LENGTH);
  
}/*fps_process_delete_all_req*/

void fps_process_delete_id_req(uint32 command) {
  fps_build_send_command(command, 0, RESPONSE_LENGTH);
  
}/*fps_process_delete_id_req*/

void fps_process_finger_pressed_req(uint32 command) {
  fps_build_send_command(command, 0, RESPONSE_LENGTH);
  
}/*fps_process_finger_pressed_req*/

void fps_build_send_command(uint32 command, uint32 param, uint32 rsp_length) {
  /*Response with Serial Number*/
  uint32 parameter;
  uint16 device_id   = 0x0001;
  uint16 fps_command;
  uint16 out_len     = 0;
  uint8 *fps_cmd_ptr = NULL;
  fps_ctx_t *ctx_ptr = &ctx_g;
  /*parameter is the index that needs to be provided as part of command*/ 
  parameter = param;
  fps_command = command;
 
  fps_cmd_ptr = fps_build_request(parameter, 
                                  device_id, 
                                  fps_command, 
                                  &out_len);

  /*Fixed Response length*/
  ctx_ptr->response_length  = rsp_length;
  ctx_ptr->command_length   = out_len;

  ctx_ptr->offset           = 0x00;
  ctx_ptr->response_ptr     = (uint8 *)malloc(ctx_ptr->response_length);
  
  /*Initializing to zero*/ 
  memset((void *)ctx_ptr->response_ptr, 0, sizeof(ctx_ptr->response_length));
 
  /*Sending Data to UART*/ 
  uart_write(fps_cmd_ptr, out_len);

}/*fps_build_send_command*/

void fps_process_enroll3_req(uint32 command) {
  fps_build_send_command(command, 0, RESPONSE_LENGTH);

}/*fps_process_enroll3_req*/

void fps_process_enroll2_req(void) {
  /*Response with Serial Number*/
  uint32 parameter;
  uint16 device_id   = 0x0001;
  uint16 fps_command;
  uint16 out_len     = 0;
  uint8 *fps_cmd_ptr = NULL;
  fps_ctx_t *ctx_ptr = &ctx_g;
  /*parameter is the index that needs to be provided as part of command*/ 
  parameter = 0;
  fps_command = FPS_ENROLL2_F;
 
  fps_cmd_ptr = fps_build_request(parameter, 
                                  device_id, 
                                  fps_command, 
                                  &out_len);

  /*Fixed Response length*/
  ctx_ptr->response_length  = RESPONSE_LENGTH;
  ctx_ptr->command_length   = out_len;

  ctx_ptr->offset           = 0x00;
  ctx_ptr->response_ptr     = (uint8 *)malloc(ctx_ptr->response_length);
  
  /*Initializing to zero*/ 
  memset((void *)ctx_ptr->response_ptr, 0, sizeof(ctx_ptr->response_length));
 
  /*Sending Data to UART*/ 
  uart_write(fps_cmd_ptr, out_len);

}/*fps_process_enroll2_req*/

void fps_process_enroll1_req(void) {
  /*Response with Serial Number*/
  uint32 parameter;
  uint16 device_id   = 0x0001;
  uint16 fps_command;
  uint16 out_len     = 0;
  uint8 *fps_cmd_ptr = NULL;
  fps_ctx_t *ctx_ptr = &ctx_g;
  /*parameter is the index that needs to be provided as part of command*/ 
  parameter = 0;
  fps_command = FPS_ENROLL1_F;
 
  fps_cmd_ptr = fps_build_request(parameter, 
                                  device_id, 
                                  fps_command, 
                                  &out_len);

  /*Fixed Response length*/
  ctx_ptr->response_length  = RESPONSE_LENGTH;
  ctx_ptr->command_length   = out_len;

  ctx_ptr->offset           = 0x00;
  ctx_ptr->response_ptr     = (uint8 *)malloc(ctx_ptr->response_length);
  
  /*Initializing to zero*/ 
  memset((void *)ctx_ptr->response_ptr, 0, sizeof(ctx_ptr->response_length));
 
  /*Sending Data to UART*/ 
  uart_write(fps_cmd_ptr, out_len);

}/*fps_process_enroll1_req*/

void fps_process_enroll_start_req(void) {
  /*Response with Serial Number*/
  uint32 parameter;
  uint16 device_id   = 0x0001;
  uint16 fps_command;
  uint16 out_len     = 0;
  uint8 *fps_cmd_ptr = NULL;
  fps_ctx_t *ctx_ptr = &ctx_g;
  /*parameter is the index that needs to be provided as part of command*/ 
  parameter = 0;
  fps_command = FPS_ENROLL_START_F;
 
  fps_cmd_ptr = fps_build_request(parameter, 
                                  device_id, 
                                  fps_command, 
                                  &out_len);

  /*Fixed Response length*/
  ctx_ptr->response_length  = RESPONSE_LENGTH;
  ctx_ptr->command_length   = out_len;

  ctx_ptr->offset           = 0x00;
  ctx_ptr->response_ptr     = (uint8 *)malloc(ctx_ptr->response_length);
  
  /*Initializing to zero*/ 
  memset((void *)ctx_ptr->response_ptr, 0, sizeof(ctx_ptr->response_length));
 
  /*Sending Data to UART*/ 
  uart_write(fps_cmd_ptr, out_len);


}/*fps_process_enroll_start_req*/

void fps_process_check_enroll_req(void) {
  /*Response with Serial Number*/
  uint32 parameter;
  uint16 device_id   = 0x0001;
  uint16 fps_command;
  uint16 out_len     = 0;
  uint8 *fps_cmd_ptr = NULL;
  fps_ctx_t *ctx_ptr = &ctx_g;
  /*parameter is the index that needs to be provided as part of command*/ 
  parameter = 0;
  fps_command = FPS_CHECK_ENROLLED_F;
 
  fps_cmd_ptr = fps_build_request(parameter, 
                                  device_id, 
                                  fps_command, 
                                  &out_len);

  /*Fixed Response length*/
  ctx_ptr->response_length  = RESPONSE_LENGTH;
  ctx_ptr->command_length   = out_len;

  ctx_ptr->offset           = 0x00;
  ctx_ptr->response_ptr     = (uint8 *)malloc(ctx_ptr->response_length);
  
  /*Initializing to zero*/ 
  memset((void *)ctx_ptr->response_ptr, 0, sizeof(ctx_ptr->response_length));
 
  /*Sending Data to UART*/ 
  uart_write(fps_cmd_ptr, out_len);

}/*fps_process_check_enroll_req*/

void fps_process_enroll_count_req(void) {
  /*Response with Serial Number*/
  uint32 parameter;
  uint16 device_id   = 0x0001;
  uint16 fps_command;
  uint16 out_len     = 0;
  uint8 *fps_cmd_ptr = NULL;
  fps_ctx_t *ctx_ptr = &ctx_g;
  
  parameter = 0;
  fps_command = FPS_GET_ENROLL_COUNT_F;
 
  fps_cmd_ptr = fps_build_request(parameter, 
                                  device_id, 
                                  fps_command, 
                                  &out_len);

  /*Fixed Response length*/
  ctx_ptr->response_length  = RESPONSE_LENGTH;
  ctx_ptr->command_length   = out_len;

  ctx_ptr->offset           = 0x00;
  ctx_ptr->response_ptr     = (uint8 *)malloc(ctx_ptr->response_length);
  
  /*Initializing to zero*/ 
  memset((void *)ctx_ptr->response_ptr, 0, sizeof(ctx_ptr->response_length));
 
  /*Sending Data to UART*/ 
  uart_write(fps_cmd_ptr, out_len);
 
}/*fps_process_enroll_count_req*/


void fps_process_led_off_req(void) {
  /*Response with Serial Number*/
  uint32 parameter;
  uint16 device_id   = 0x0001;
  uint16 fps_command;
  uint16 out_len     = 0;
  uint8 *fps_cmd_ptr = NULL;
  fps_ctx_t *ctx_ptr = &ctx_g;
  
  parameter = 0;
  fps_command = FPS_CMOS_LED_F;
 
  fps_cmd_ptr = fps_build_request(parameter, 
                                  device_id, 
                                  fps_command, 
                                  &out_len);

  /*Fixed Response length*/
  ctx_ptr->response_length  = RESPONSE_LENGTH;
  ctx_ptr->command_length   = out_len;

  ctx_ptr->offset           = 0x00;
  ctx_ptr->response_ptr     = (uint8 *)malloc(ctx_ptr->response_length);
  
  /*Initializing to zero*/ 
  memset((void *)ctx_ptr->response_ptr, 0, sizeof(ctx_ptr->response_length));
 
  /*Sending Data to UART*/ 
  uart_write(fps_cmd_ptr, out_len);

}/*fps_process_led_off_req*/


void fps_process_led_on_req(void) {
  /*Response with Serial Number*/
  uint32 parameter;
  uint16 device_id   = 0x0001;
  uint16 fps_command;
  uint16 out_len     = 0;
  uint8 *fps_cmd_ptr = NULL;
  fps_ctx_t *ctx_ptr = &ctx_g;
  
  parameter = 1;
  fps_command = FPS_CMOS_LED_F;
 
  fps_cmd_ptr = fps_build_request(parameter, 
                                  device_id, 
                                  fps_command, 
                                  &out_len);

  /*Fixed Response length*/
  ctx_ptr->response_length  = RESPONSE_LENGTH;
  ctx_ptr->command_length   = out_len;

  ctx_ptr->offset           = 0x00;
  ctx_ptr->response_ptr     = (uint8 *)malloc(ctx_ptr->response_length);
  
  /*Initializing to zero*/ 
  memset((void *)ctx_ptr->response_ptr, 0, sizeof(ctx_ptr->response_length));
 
  /*Sending Data to UART*/ 
  uart_write(fps_cmd_ptr, out_len);

}/*fps_process_led_on_req*/


void fps_process_set_iap_mode_req(void) {
  /*Response with Serial Number*/
  uint32 parameter;
  uint16 device_id   = 0x0001;
  uint16 fps_command;
  uint16 out_len     = 0;
  uint8 *fps_cmd_ptr = NULL;
  fps_ctx_t *ctx_ptr = &ctx_g;
  
  parameter = 0;
  fps_command = FPS_SET_IAP_MODE_F;
 
  fps_cmd_ptr = fps_build_request(parameter, 
                                  device_id, 
                                  fps_command, 
                                  &out_len);

  /*Fixed Response length*/
  ctx_ptr->response_length  = RESPONSE_LENGTH;
  ctx_ptr->command_length   = out_len;

  ctx_ptr->offset           = 0x00;
  ctx_ptr->response_ptr     = (uint8 *)malloc(ctx_ptr->response_length);
  
  /*Initializing to zero*/ 
  memset((void *)ctx_ptr->response_ptr, 0, sizeof(ctx_ptr->response_length));
 
  /*Sending Data to UART*/ 
  uart_write(fps_cmd_ptr, out_len);

}/*fps_process_set_iap_mode_req*/

void fps_process_usb_internal_check_req(void) {
  /*Response with Serial Number*/
  uint32 parameter;
  uint16 device_id   = 0x0001;
  uint16 fps_command;
  uint16 out_len     = 0;
  uint8 *fps_cmd_ptr = NULL;
  fps_ctx_t *ctx_ptr = &ctx_g;
  
  parameter = 0;
  fps_command = FPS_USB_INTERNAL_CHECK_F;
 
  fps_cmd_ptr = fps_build_request(parameter, 
                                  device_id, 
                                  fps_command, 
                                  &out_len);

  /*Fixed Response length*/
  ctx_ptr->response_length  = RESPONSE_LENGTH;
  ctx_ptr->command_length   = out_len;

  ctx_ptr->offset           = 0x00;
  ctx_ptr->response_ptr     = (uint8 *)malloc(ctx_ptr->response_length);
  
  /*Initializing to zero*/ 
  memset((void *)ctx_ptr->response_ptr, 0, sizeof(ctx_ptr->response_length));
 
  /*Sending Data to UART*/ 
  uart_write(fps_cmd_ptr, out_len);

}/*fps_process_usb_internal_check_req*/

void fps_process_baudrate_change_req(uint32 command) {
  fps_build_send_command(command, 115200, RESPONSE_LENGTH);
    
}/*fps_process_baudrate_change_req*/

void fps_process_close_req(void) {
  /*Response with Serial Number*/
  uint32 parameter;
  uint16 device_id   = 0x0001;
  uint16 fps_command;
  uint16 out_len     = 0;
  uint8 *fps_cmd_ptr = NULL;
  fps_ctx_t *ctx_ptr = &ctx_g;
  
  parameter = 0;
  fps_command = FPS_CLOSE_F;
 
  fps_cmd_ptr = fps_build_request(parameter, 
                                  device_id, 
                                  fps_command, 
                                  &out_len);

  /*Fixed Response length*/
  ctx_ptr->response_length  = RESPONSE_LENGTH;
  ctx_ptr->command_length   = out_len;

  ctx_ptr->offset           = 0x00;
  ctx_ptr->response_ptr     = (uint8 *)malloc(ctx_ptr->response_length);
  
  /*Initializing to zero*/ 
  memset((void *)ctx_ptr->response_ptr, 0, sizeof(ctx_ptr->response_length));
 
  /*Sending Data to UART*/ 
  uart_write(fps_cmd_ptr, out_len);

}/*fps_process_close_req*/


void fps_process_open_req(void) {

  /*Response with Serial Number*/
  uint32 parameter;
  uint16 device_id   = 0x0001;
  uint16 fps_command;
  uint16 out_len     = 0;
  uint8 *fps_cmd_ptr = NULL;
  fps_ctx_t *ctx_ptr = &ctx_g;
  
  parameter = 1;
  fps_command = FPS_OPEN_F;
 
  fps_cmd_ptr = fps_build_request(parameter, 
                                  device_id, 
                                  fps_command, 
                                  &out_len);

  /*Fixed Response length*/
  ctx_ptr->response_length  = OPEN_RESPONSE_LENGTH;
  ctx_ptr->command_length   = out_len;

  ctx_ptr->offset           = 0x00;
  ctx_ptr->response_ptr     = (uint8 *)malloc(ctx_ptr->response_length);
  
  /*Initializing to zero*/ 
  memset((void *)ctx_ptr->response_ptr, 0, sizeof(ctx_ptr->response_length));
 
  /*Sending Data to UART*/ 
  uart_write(fps_cmd_ptr, out_len);
  
}/*fps_process_open_req*/


uint32 fps_get_command(void) {
   
  uint32 option;
  option = fps_display_option();

  switch(option) {
    case 1:
      fps_process_command(FPS_OPEN_F, 1, 0, NULL);
      break;

    case 2:
      /*LED ON*/
      fps_process_command(FPS_CMOS_LED_F, 1, 0, NULL);
      break;

    case 3:
      /*LED OFF*/
      fps_process_command(FPS_CMOS_LED_F, 0, 0, NULL);
      break;

    case 4:
      /*PRESS_FINGER*/
      fps_process_command(FPS_IS_PRESS_FINGER_F, 1, 0, NULL);
      break;

    case 5:
      /*CAPTURE FINGER*/
      fps_process_command(FPS_CAPTURE_FINGER_F, 1, 0, NULL);
      break;

    case 6:
      /*CLOSE*/
      fps_process_command(FPS_CLOSE_F, 1, 0, NULL);
      break;

    case 7:
      /*GET IMAGE*/
      fps_process_command(FPS_GET_IMAGE_F, 0, 0, NULL);
      break;

    case 8:
      /*GET RAW IMAGE*/
      fps_process_command(FPS_GET_RAW_IMAGE_F, 1, 0, NULL);
      break;

    case 9:
      /*GET RAW IMAGE*/
      fps_process_command(FPS_CHANGE_BAUDRATE_F, 115200, 0, NULL);
      break;

    default:
      break;

  }/*end of switch*/
   
}/*fps_get_command*/

void fps_process_command(uint32 command, uint32 in_parameter, uint32 data_len, uint8 *data) {

  fps_ctx_t *ctx_ptr = &ctx_g;
  memset((void *)ctx_ptr, 0, sizeof(ctx_g));

  switch(command) {

    case FPS_OPEN_F:
    {
      /*Ensure sequential execution of Command*/
      ctx_ptr->is_command_in_progress = 1;

      /*Initializing the STATE*/
      fps_state_t  command_state[] = {OPEN_ST,
                                      DONE_ST};

      memcpy(ctx_ptr->command_state, command_state, sizeof(command_state));
      ctx_ptr->command_state_ptr   = ctx_ptr->command_state;
      ctx_ptr->in_parameter        = in_parameter;
      ctx_ptr->command             = command;

      fps_process_request(); 
    }
      break;
    case FPS_CMOS_LED_F:
    {
      /*Ensure sequential execution of Command*/
      ctx_ptr->is_command_in_progress = 1;

      /*Initializing the STATE*/
      fps_state_t  command_state[] = {LED_ON_ST,
                                      DONE_ST};
      if(0 == in_parameter) {
        command_state[0] = LED_OFF_ST;
      }

      memcpy(ctx_ptr->command_state, command_state, sizeof(command_state));
      ctx_ptr->command_state_ptr   = ctx_ptr->command_state;
      ctx_ptr->in_parameter        = in_parameter;
      ctx_ptr->command             = command;

      fps_process_request(); 
    } 
      break;
    case FPS_CLOSE_F:
    {
      /*Ensure sequential execution of Command*/
      ctx_ptr->is_command_in_progress = 1;

      /*Initializing the STATE*/
      fps_state_t  command_state[] = {CLOSE_ST,
                                      DONE_ST};

      memcpy(ctx_ptr->command_state, command_state, sizeof(command_state));
      ctx_ptr->command_state_ptr   = ctx_ptr->command_state;
      ctx_ptr->in_parameter        = in_parameter;
      ctx_ptr->command             = command;

      fps_process_request(); 
    }
      break;
    case FPS_SET_IAP_MODE_F:
    {
      /*Ensure sequential execution of Command*/
      ctx_ptr->is_command_in_progress = 1;

      /*Initializing the STATE*/
      fps_state_t  command_state[] = {SET_IAP_MODE_ST,
                                      DONE_ST};

      memcpy(ctx_ptr->command_state, command_state, sizeof(command_state));
      ctx_ptr->command_state_ptr   = ctx_ptr->command_state;
      ctx_ptr->in_parameter        = in_parameter;
      ctx_ptr->command             = command;

      fps_process_request(); 
    }
      break;
    case FPS_IS_PRESS_FINGER_F:
    {
      /*Ensure sequential execution of Command*/
      ctx_ptr->is_command_in_progress = 1;

      /*Initializing the STATE*/
      fps_state_t  command_state[] = {FINGER_PRESSED_ST,
                                      DONE_ST};

      memcpy(ctx_ptr->command_state, command_state, sizeof(command_state));
      ctx_ptr->command_state_ptr   = ctx_ptr->command_state;
      ctx_ptr->in_parameter        = in_parameter;
      ctx_ptr->command             = command;

      fps_process_request(); 
    }
      break;
    case FPS_DELETE_ID_F:
    {
      /*Ensure sequential execution of Command*/
      ctx_ptr->is_command_in_progress = 1;

      /*Initializing the STATE*/
      fps_state_t  command_state[] = {DELETE_ID_ST,
                                      DONE_ST};

      memcpy(ctx_ptr->command_state, command_state, sizeof(command_state));
      ctx_ptr->command_state_ptr   = ctx_ptr->command_state;
      ctx_ptr->in_parameter        = in_parameter;
      ctx_ptr->command             = command;

      fps_process_request(); 
    }
      break;
    case FPS_CAPTURE_FINGER_F:
    {
      /*Ensure sequential execution of Command*/
      ctx_ptr->is_command_in_progress = 1;

      /*Initializing the STATE*/
      fps_state_t  command_state[] = {FINGER_PRESSED_ST,
                                      LED_ON_ST,
                                      CAPTURE_FINGER_ST,
                                      GET_IMAGE_ST,  
                                      DONE_ST};

      memcpy(ctx_ptr->command_state, command_state, sizeof(command_state));
      ctx_ptr->command_state_ptr   = ctx_ptr->command_state;
      ctx_ptr->in_parameter        = in_parameter;
      ctx_ptr->command             = command;

      fps_process_request();
    }
      break;

    case FPS_GET_RAW_IMAGE_F:
    {
      /*Ensure sequential execution of Command*/
      ctx_ptr->is_command_in_progress = 1;

      /*Initializing the STATE*/
      fps_state_t  command_state[] = {GET_RAW_IMAGE_ST,
                                      DONE_ST};

      memcpy(ctx_ptr->command_state, command_state, sizeof(command_state));
      ctx_ptr->command_state_ptr   = ctx_ptr->command_state;
      ctx_ptr->in_parameter        = in_parameter;
      ctx_ptr->command             = command;

      fps_process_request(); 
    }
      break;
    case FPS_CHANGE_BAUDRATE_F:
    {
      /*Ensure sequential execution of Command*/
      ctx_ptr->is_command_in_progress = 1;

      /*Initializing the STATE*/
      fps_state_t  command_state[] = {BAUD_RATE_CHANGE_ST,
                                      DONE_ST};

      memcpy(ctx_ptr->command_state, command_state, sizeof(command_state));
      ctx_ptr->command_state_ptr   = ctx_ptr->command_state;
      ctx_ptr->in_parameter        = in_parameter;
      ctx_ptr->command             = command;

      fps_process_request(); 
    }
      break;
 
    default:
      fprintf(stderr, "No Case match\n");
      break;

  }/*End of Switch*/

}/*fps_process_command*/

void fps_save_image(uint8 *file_name, uint8 *response_ptr, uint16 response_length) {
  FILE *fp = NULL;

  int32 rc = -1;

  fp = fopen(file_name, "w");

  if(NULL == fp) {
    fprintf(stderr, "failed to open the file %s\n", file_name);
  }
  fprintf(stderr, "length is %d\n", response_length); 
  rc = fwrite((void *)&response_ptr[15], 1, (response_length - (12 + 6)), fp);
  fprintf(stderr, "Bytes %d written to file\n", rc);
  fflush(fp);
  fclose(fp);
  
}/*fps_save_image*/

void fps_free_mem(void) {
  if(NULL != ctx_g.response_ptr) {
    free(ctx_g.response_ptr);
    ctx_g.offset = 0;
    ctx_g.response_ptr = NULL;
  }
}

int main() {

  fps_ctx_t *ctx_ptr = &ctx_g;

  uart_memmap();
  gpio_memmap();
  uart_init();
  uart_register_cb(response_cb);

  while(1)
  {
    
    if(ctx_ptr->sig_mask & FPS_RESPONSE_SIG) {
      /*Process the Response received from FPS*/
      ctx_ptr->sig_mask ^= FPS_RESPONSE_SIG;
      fps_process_command_response(ctx_ptr->response_ptr, ctx_ptr->offset);
      fps_free_mem();

    } else if(ctx_ptr->sig_mask & FPS_COMMAND_SIG) {
      ctx_ptr->sig_mask ^= FPS_COMMAND_SIG;
      fps_process_request();

    } else {

      if(!ctx_ptr->is_command_in_progress) {
        fps_get_command();
      }
    }
  }
}
#endif /*__FPS_MAIN_C__*/
