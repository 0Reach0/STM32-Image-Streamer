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



int8_t draw_image(uint16_t x, uint16_t y, uint16_t width, uint16_t height,uint16_t * image, uint16_t size){
     if(!send_command(DRAW_IMAGE)) return 0;
    uint8_t data[4+4+4+4];
    data[0] = (x >> 8) & 0xFF;  
    data[1] = x & 0xFF;   
    data[2] = (y >> 8) & 0xFF;  
    data[3] = y & 0xFF;
    data[4] = (width >> 8) & 0xFF;  
    data[5] = width & 0xFF;   
    data[6] = (height >> 8) & 0xFF;  
    data[7] = height & 0xFF;
    data[8] = size>>24 & 0xFF;
    data[9] = size >>16 & 0xFF;
    data[10] = size>>8 & 0xFF;
    data[11] =size &0xFF;

    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart1, &data, sizeof(data), 1000);
       if (status == HAL_OK) {
            if(!check_answer(READY)) return 0;
       }
       else return 0;
    status = HAL_UART_Transmit(&huart1, image, size,1000);
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

uint16_t test_image[] ={0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFF7C, 0xE400, 0xE400, 0xE300, 0xE320, 0xFF5B, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xE3E0, 0xEC00, 0xFC80, 0xFB80, 0xEB20, 0xE300, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xE3E0, 0xFC60, 0xFC60, 0xFB60, 0xFB80, 0xE300, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFBD, 0xF6D7, 0xFFFF, 0xFFFF, 0xE484, 0xEC20, 0xFC60, 0xFB60, 0xEB20, 0xEBC4, 0xFFFF, 0xFFFF, 0xF6D7, 0xFFBD, 0xFFFF, 0xF6F7, 0xE3E0, 0xE3E0, 0xE400, 0xED6C, 0xF6B5, 0xEC20, 0xFC60, 0xFB60, 0xF320, 0xF655, 0xED8C, 0xE400, 0xE3E0, 0xE400, 0xFEF7, 0xE3E0, 0xEC20, 0xFC80, 0xF440, 0xEC00, 0xF400, 0xEC00, 0xFCC1, 0xFBC1, 0xEB40, 0xEC40, 0xEC00, 0xF440, 0xFC80, 0xEC20, 0xE3E0, 0xE300, 0xFBA0, 0xFC00, 0xFC60, 0xFC80, 0xFC60, 0xFE87, 0xFF8B, 0xFFAB, 0xFE47, 0xFC60, 0xFC80, 0xFC60, 0xFC00, 0xFBA0, 0xE300, 0xE320, 0xFB60, 0xFB80, 0xFB60, 0xFBA0, 0xFD44, 0xFF8B, 0xFF8B, 0xFF6A, 0xFEE7, 0xFD02, 0xFBA0, 0xFB60, 0xFB80, 0xFB60, 0xE320, 0xEBC4, 0xE320, 0xE300, 0xF340, 0xF340, 0xFDA6, 0xFF8B, 0xFF6A, 0xFEC6, 0xFEC6, 0xFD44, 0xF340, 0xF340, 0xE300, 0xE320, 0xE3C4, 0xFFFF, 0xFFBE, 0xF5B1, 0xECAA, 0xFB80, 0xFD43, 0xFF6B, 0xFEC6, 0xFEC6, 0xFEE7, 0xFD02, 0xFB80, 0xECAA, 0xF5B1, 0xFFBE, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xED6B, 0xFC40, 0xFC60, 0xFD84, 0xFEE6, 0xFEE6, 0xFD84, 0xFC60, 0xFC40, 0xED4B, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xED8C, 0xE3E0, 0xFC40, 0xFC40, 0xFB60, 0xFB80, 0xFB80, 0xFB60, 0xFC40, 0xFC40, 0xE3E0, 0xED8C, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFDE, 0xE3E0, 0xFC60, 0xFC80, 0xFB80, 0xF340, 0xF340, 0xF340, 0xF340, 0xFB80, 0xFC80, 0xFC60, 0xE3E0, 0xFFDE, 0xFFFF, 0xFFFF, 0xFF9C, 0xE400, 0xFC80, 0xFBC0, 0xFB80, 0xE300, 0xECCB, 0xECEC, 0xE300, 0xFB80, 0xFBC0, 0xFC80, 0xE400, 0xFF7B, 0xFFFF, 0xFFFF, 0xFFFF, 0xE400, 0xE3C0, 0xFB60, 0xEB20, 0xE300, 0xFFFF, 0xFFFF, 0xE300, 0xEB20, 0xFB60, 0xE3A0, 0xE400, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xE3C4, 0xE300, 0xE320, 0xF675, 0xFFFF, 0xFFFF, 0xF675, 0xE320, 0xE300, 0xEBC4, 0xFFFF, 0xFFFF, 0xFFFF} ;

int8_t Test()
{
    HAL_Delay(1000);
    create_window(100, 100);
    HAL_Delay(100);
    fill_color(150,100,20);
    HAL_Delay(100);
    fill_color(200,200,20);
    HAL_Delay(100);
    resize_window(100,100);
    for(int i =0; i < 100; i ++)
    {
        draw_pixel(i, 10, 0,0,0 );
        draw_pixel(i, 11, 0,0,0 );
         draw_pixel(i, 12, 0,0,0 );
    }
    stop_conversation();
    HAL_Delay(100);
    start_conversation();
    HAL_Delay(1000);
    create_window(500, 100);
    HAL_Delay(100);
    fill_color(150,100,20);
    HAL_Delay(100);
    char *text = "Hello World!!!\0";
    write_text(30,30, 10,100,23, text);
    HAL_Delay(1000);
    fill_color(10,200,100);
    draw_image(50,50, 16, 16, test_image, sizeof(test_image));
}
