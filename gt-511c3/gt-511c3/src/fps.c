#ifndef __FPS_C__
#define __FPS_C__
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <uart.h>
#include <fps.h>
#include <fps_main.h>

/*Extern Variable*/
extern fps_ctx_t ctx_g;

/*Global Data structure initialization*/

fps_generic_packet_t      command_packet_g;
fps_generic_packet_t      response_packet_g;
fps_generic_data_packet_t response_data_packet_g;

uint8 fps_process_ack(uint8 *response_ptr, uint16 response_length) {
  uint32 idx = 0;
  
  switch(*ctx_g.command_state_ptr) {
    case OPEN_ST:
    {
      /*Extract the Data Packet*/
      fps_open_response_t *data_packet_ptr = (fps_open_response_t *)&response_ptr[16];
      printf("\nFirmware Version 0x%X Iso Maz Area Size 0x%X \n", 
                data_packet_ptr->firmware_version, 
                data_packet_ptr->iso_area_max_size);
      printf("\nSerial Number is ==>");
 
      for(idx = 0; idx <16; idx++) {
        printf("%0.2X ", data_packet_ptr->serial_number[idx]);
      }
      printf("\n");
    }
      ctx_g.command_state_ptr++ ;
      ctx_g.sig_mask |= FPS_COMMAND_SIG;
      break;
    
    case CLOSE_ST:
      break;
    case USB_INTERNAL_CHECK_ST:
      break;

    case BAUD_RATE_CHANGE_ST:
      disable_uart();
      flush_fifo();
      uart_set_baudrate(115200);
      enable_rx();
      enable_tx();
      enable_fifo();
      enable_uart();
      ctx_g.command_state_ptr++ ;
      ctx_g.sig_mask |= FPS_COMMAND_SIG;
      break;
    case SET_IAP_MODE_ST:
      break;
    case LED_ON_ST:
      ctx_g.command_state_ptr++ ;
      ctx_g.sig_mask |= FPS_COMMAND_SIG;
      break;
    case LED_OFF_ST:
      ctx_g.command_state_ptr++ ;
      ctx_g.sig_mask |= FPS_COMMAND_SIG;
      break;
    case GET_ENROLL_COUNT_ST:
      break;
    case CHECK_ENROLL_ST:
      break;
    case ENROLL_START_ST:
      break;
    case ENROLL1_ST:
      break;
    case ENROLL2_ST:
      break;
    case ENROLL3_ST:
      break;
    case FINGER_PRESSED_ST:
    {
      uint32 param = *((uint32 *)&response_ptr[4]);
      if(param > 0) {
        ctx_g.command_state_ptr++;
        /*Check If Finger is pressed or not*/
      } else {
        ctx_g.command_state_ptr++;
      }
      ctx_g.sig_mask ^= FPS_COMMAND_SIG;
      ctx_g.sig_mask |= FPS_COMMAND_SIG;
    
    }
      break;
    case DELETE_ID_ST:
      break;
    case DELETE_ALL_ST:
      break;
    case VERIFY_ST:
      break;
    case VERIFY_TEMPLATE_ST:
      break;
    case IDENTIFY_TEMPLATE_ST:
      break;
    case CAPTURE_FINGER_ST:
      ctx_g.command_state_ptr++ ;
      ctx_g.sig_mask ^= FPS_COMMAND_SIG;
      ctx_g.sig_mask |= FPS_COMMAND_SIG;
      printf("Going into next state after capturing Finger\n");
      break;
    case MAKE_TEMPLATE_ST:
      break;
    case GET_IMAGE_ST:
      fps_save_image("/tmp/image", response_ptr, response_length);
      ctx_g.command_state_ptr++ ;
      ctx_g.sig_mask ^= FPS_COMMAND_SIG;
      ctx_g.sig_mask |= FPS_COMMAND_SIG;
      printf("After saving image\n");
      break;
    case GET_RAW_IMAGE_ST:
      fps_save_image("/tmp/raw_image", response_ptr, response_length);
      ctx_g.command_state_ptr++ ;
      ctx_g.sig_mask ^= FPS_COMMAND_SIG;
      ctx_g.sig_mask |= FPS_COMMAND_SIG;
      break;
    case GET_TEMPLATE_ST:
      break;
    case SET_TEMPLATE_ST:
      break;
    default:
      fprintf(stderr, "Default case hit\n");
      break;
  } 
  /*Determine from state machine whether Response has Data Packet or not*/
  return(0);

}/*fps_process_ack*/

uint8 fps_process_nack(uint32 param) {
  printf("0x%X ", param);
  switch(param) {
    case FPS_NACK_TIMEOUT:
    case FPS_NACK_INVALID_BAUDRATE:
    /*The Specified ID is not between 0 - 199*/
    case FPS_NACK_INVALID_POS:
    case FPS_NACK_IS_NOT_USED:
    case FPS_NACK_IS_ALREADY_USED:
    case FPS_NACK_COMM_ERR:
    case FPS_NACK_VERIFY_FAILED:
    case FPS_NACK_IDENTIFY_FAILED:
    case FPS_NACK_DB_IS_FULL:
    case FPS_NACK_DB_IS_EMPTY:
    case FPS_NACK_TURN_ERR:
    case FPS_BAD_FINGER:
    case FPS_NACK_ENROLL_FAILED:
    case FPS_NACK_IS_NOT_SUPPORTED:
    /*Device Error, Especially If crypto-chip is trouble*/
    case FPS_NACK_DEV_ERR:
    case FPS_NACK_CAPTURE_CANCELLED:
    case FPS_NACK_INVALID_PARAM:
    case FPS_NACK_FINGER_IS_NOT_PRESSED:
      //fps_process_request();
      break;
    default:
      break;
  } 
  return(0);

}/*fps_process_nack*/

uint16 fps_checksum(uint8 *byte_ptr, uint16 len) {
  uint8 idx = 0;
  uint16 chk_sum = 0;

  for(idx = 0; idx < len; idx++) {
    chk_sum += byte_ptr[idx];
  }
  return(chk_sum);

}/*fps_checksum*/


uint8 *fps_build_request(uint32  input_param, 
                         uint16  device_id, 
                         uint16  command, 
                         uint16  *out_len) {
  uint8 *byte_ptr = NULL;
  uint16 chk_sum = 0x00;
  
  fps_generic_packet_t *command_ptr = &command_packet_g;
  byte_ptr = (uint8 *)command_ptr;
  
  memset((void *)byte_ptr, 0, sizeof(fps_generic_packet_t));

  /*Preparing the command*/
  command_ptr->hdr.start_code1 = FPS_START_CODE1;
  *out_len += 1;
  command_ptr->hdr.start_code2 = FPS_START_CODE2;
  *out_len += 1;

  /*Packing of data as little endian*/
  command_ptr->hdr.device_id[0] = device_id & 0xFF;
  *out_len += 1;
  command_ptr->hdr.device_id[1] = (device_id >> 8) & 0xFF;
  *out_len += 1;
  memset((void *)command_ptr->param, 0, sizeof(command_ptr->param));

  /*if with_data_packet is zero, then response data 
    will not be present in response*/
  command_ptr->param[0] = input_param & 0xFF;
  *out_len += 1;
  command_ptr->param[1] = (input_param >> 8)  & 0xFF;
  *out_len += 1;
  command_ptr->param[2] = (input_param >> 16) & 0xFF;
  *out_len += 1;
  command_ptr->param[3] = (input_param >> 24) & 0xFF;
  *out_len += 1;

  /*Populating command code in little endian fashion*/
  memset((void *)command_ptr->code, 0, sizeof(command_ptr->code));
  command_ptr->code[0] = command & 0xFF;
  *out_len += 1;
  command_ptr->code[1] = (command >> 8) & 0xFF;
  *out_len += 1;

  chk_sum = fps_checksum(byte_ptr, *out_len);

  command_ptr->chk_sum[0] = chk_sum & 0xFF;
  *out_len += 1;

  command_ptr->chk_sum[1] = (chk_sum >> 8) & 0xFF;  
  *out_len += 1;

  return((uint8 *)command_ptr);
  
}/*fps_build_request*/

#endif
