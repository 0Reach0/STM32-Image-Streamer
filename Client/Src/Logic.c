#include "Logic.h"
#include "stm32f4xx_hal.h"


const uint8_t CREATE_WINDOW  = 0x11;
const uint8_t RESIZE_WINDOW  = 0x12;
const uint8_t FILL_COLOR     = 0x13;
const uint8_t DRAW_PIXEL     = 0x14;
const uint8_t DRAW_IMAGE     = 0x15;
const uint8_t RESET_SCREEN   = 0x16;
const uint8_t CLOSE_WINDOW   = 0x17;
const uint8_t WRITE_TEXT     = 0x18;



int8_t send_command(uint8_t command)
{
    uint8_t data = command;     

    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart1, &data, sizeof(data), 10000);
    if (status == HAL_OK) {
        if(check_answer(READY)) return 1;
        return 0;  
    } else {
        return 0; 
    }
}





int8_t check_answer(uint8_t command)
{
    HAL_StatusTypeDef  status;
    uint8_t data;
    status = HAL_UART_Receive(&huart1, &data, sizeof(data), 1000);
       if (status == HAL_OK & data == command) {
        return 1;  
    } else {
        return 0; 
    }
}



int8_t create_window(uint16_t x, uint16_t y){
    if(!send_command(CREATE_WINDOW)) return 0;
    uint8_t data[4];

    data[0] = (x >> 8) & 0xFF;  
    data[1] = x & 0xFF;         
    data[2] = (y >> 8) & 0xFF;  
    data[3] = y & 0xFF;         
    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart1, &data, sizeof(data), 1000);
    if (status == HAL_OK) {
        if(!check_answer(SUCCESFUL_OPERATION)) return 0;
        return 1;  
    } else {
        return 0; 
    }
}





int8_t resize_window(uint16_t x, uint16_t y){
    if(!send_command(RESIZE_WINDOW)) return 0;
    uint8_t data[4];

    data[0] = (x >> 8) & 0xFF;  
    data[1] = x & 0xFF;         
    data[2] = (y >> 8) & 0xFF;  
    data[3] = y & 0xFF;         

    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart1, &data, sizeof(data), 1000);

    if (status == HAL_OK) {
        if(!check_answer(SUCCESFUL_OPERATION)) return 0;
        return 1;  
    } else {
        return 0; 
    }
}




int8_t fill_color(uint8_t r, uint8_t g,uint8_t b){
    if(!send_command(FILL_COLOR)) return 0;
    uint8_t data[3];

    data[0] = r;
    data[1] = g;
    data[2] = b;
    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart1, (const uint8_t *)&data, sizeof(data), 1000);

    if (status == HAL_OK) {
        if(!check_answer(SUCCESFUL_OPERATION)) return 0;
        return 1;  
    } else {
        return 0; 
    }

}



int8_t draw_pixel(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b){
    if(!send_command(DRAW_PIXEL)) return 0;
    uint8_t data[7];

    data[0] = (x >> 8) & 0xFF;  
    data[1] = x & 0xFF;         
    data[2] = (y >> 8) & 0xFF;  
    data[3] = y & 0xFF;
    data[4] = r;
    data[5] = g;         
    data[6] = b;

    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart1, &data, sizeof(data), 1000);

    if (status == HAL_OK) {
        if(!check_answer(SUCCESFUL_OPERATION)) return 0;
        return 1;  
    } else {
        return 0; 
    }

}



int8_t draw_image(uint16_t x, uint16_t y,uint16_t * image){
     if(!send_command(DRAW_IMAGE)) return 0;
    uint8_t data[7];

    data[0] = (x >> 8) & 0xFF;  
    data[1] = x & 0xFF;         
    data[2] = (y >> 8) & 0xFF;  
    data[3] = y & 0xFF;
    data[4] = sizeof(image)>>8 & 0xFF;
    data[5] = sizeof(image) &0xFF;

    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart1, &data, sizeof(data), 1000);
       if (status == HAL_OK) {
            if(!check_answer(READY)) return 0;
       }
       else return 0;
        
    status = HAL_UART_Transmit(&huart1, image, sizeof(image),1000);
    if (status == HAL_OK) {
        if(!check_answer(SUCCESFUL_OPERATION)) return 0;
        return 1;  
    } else {
        return 0; 
    }


}

int8_t write_text(uint16_t x, uint16_t y,uint8_t r,uint8_t g,uint8_t b, uint8_t* text){
     if(!send_command(WRITE_TEXT)) return 0;
    uint8_t data[4+3+2];

    data[0] = (x >> 8) & 0xFF;  
    data[1] = x & 0xFF;         
    data[2] = (y >> 8) & 0xFF;  
    data[3] = y & 0xFF;
    data[4] = r;
    data[5] = g;
    data[6] = b;
    data[7] =  (strlen(text))>>8 & 0xFF;
    data[8] =  (strlen(text)) &0xFF;

    HAL_Delay(1000);
    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart1, &data, sizeof(data), 1000);
       if (status == HAL_OK) {
            if(!check_answer(READY)) return 0;
       }
       else return 0;
        
    status = HAL_UART_Transmit(&huart1,  text, strlen(text),1000);
       if (status == HAL_OK) {
        if(!check_answer(READY)) return 0;
    } else {
        return 0; 
    }
    if (status == HAL_OK) {
        if(!check_answer(SUCCESFUL_OPERATION)) return 0;
        return 1;  
    } else {
        return 0; 
    }


}


int8_t reset_screen(){
    if(!send_command(RESET_SCREEN)) return 0;
    

}




int8_t close_window(){
    if(!send_command(CLOSE_WINDOW)) return 0;
    

}



int8_t start_conversation(){
    if(!send_command(START_CONVERSATION)) return 0;
    


}


int8_t stop_conversation()
{
        if(!send_command(END_CONVERSATION)) return 0;
    
}