#pragma once
 #include <stdint.h>
#include "stm32f4xx_hal.h"

 #define BAUD_RATE  115200
 #define CHARACTER_SIZE 8
 #define STOP_BIT_ONE one
 #define START_CONVERSATION 0xA 
 #define END_CONVERSATION 0xB
 #define SUCCESFUL_OPERATION 0x20
 #define READY  0x18
 #define ERROR  0X19




extern UART_HandleTypeDef huart1;



int8_t send_command(uint8_t);
int8_t check_answer(uint8_t);

int8_t create_window(uint16_t, uint16_t);
int8_t resize_window(uint16_t, uint16_t);
int8_t fill_color(uint8_t, uint8_t,uint8_t);
int8_t draw_pixel(uint16_t, uint16_t, uint8_t , uint8_t , uint8_t );
int8_t draw_image(uint16_t , uint16_t ,uint16_t *);
int8_t write_text(uint16_t , uint16_t, uint8_t,uint8_t,uint8_t,uint8_t *);
int8_t reset_screen();
int8_t close_window();
int8_t start_conversation();
int8_t stop_conversation();