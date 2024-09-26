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

uint16_t test_image[] ={ 0x1002, 0x00E2, 0x00A0, 0x0800, 0x3800, 0x1800, 0x2000, 0xC208, 0x0860, 0x0000, 0xE71D, 0xD5F9, 0xFDF9, 0x9B8F, 0xDF7D, 0xAEDC, 0x2001, 0xD1CB, 0x9207, 0x1800, 0xD28D, 0xC1AB, 0xA9C8, 0xF9AA, 0xF96B, 0x4062, 0x8E57, 0x01A3, 0x0060, 0x1800, 0x3801, 0xCDF9, 0x1800, 0xE168, 0xF9E9, 0x3800, 0x68E3, 0x8803, 0xD045, 0xF865, 0xF8A9, 0x3062, 0x0080, 0x0800, 0x2080, 0xB208, 0x8823, 0x0963, 0x2000, 0x38E0, 0xE967, 0xC927, 0x0080, 0x2000, 0xF8EA, 0xF2AB, 0xE108, 0x0820, 0x4001, 0xD9AB, 0xA1C6, 0xFA8B, 0xF969, 0x0020, 0x4062, 0x00A0, 0xC188, 0xF9CE, 0x0841, 0x0020, 0xB907, 0xC186, 0xD28B, 0x0800, 0xC9CB, 0xF9AC, 0x9143, 0xD165, 0xF803, 0xC9C9, 0x1800, 0x4002, 0xD26D, 0xE0A9, 0xC8A9, 0x2000, 0x5980, 0xC900, 0x0840, 0x1040, 0xE92A, 0xF807, 0xF22A, 0xF269, 0xF927, 0xA987, 0x00C3, 0x6802, 0xC20A, 0xC147, 0xF9CD, 0xB9C5, 0xC5EA, 0xFECC, 0x0080, 0x6AA6, 0xC9CA, 0xF8C9, 0xF968, 0xC124, 0xA248, 0x0000, 0x6592, 0x2000, 0x4800, 0x8902, 0xA080, 0xDA64, 0xEE89, 0xAFE6, 0xFD93, 0x5000, 0x3060, 0x4101, 0x4800, 0x78E2, 0x4902, 0x0800, 0xFE79, 0xC617, 0x0041, 0x0021, 0x3124, 0x2000, 0xD54B, 0xE68A, 0xC48E, 0x1080, 0x00A0, 0x0042, 0x1804, 0x2001, 0x2000, 0x1840, 0xC617, 0xBE58, 0x0041, 0x1000, 0x3800, 0x5800, 0x8160, 0x8240, 0x38E0, 0x3060, 0x2000, 0x2802, 0x5209, 0xCE59, 0xD699, 0xC637, 0xA657, 0x1923, 0x1800, 0x9967, 0xE1EA, 0xF24A, 0x98E3, 0x7901, 0x0000, 0x4800, 0xE28D, 0xB126, 0x2800, 0x1101, 0xAE98, 0xAE38, 0x8D55, 0x0800, 0xC2CB, 0xD0C4, 0xD002, 0xF1CA, 0xBA0B, 0x3000, 0x18C3, 0x5822, 0xC926, 0xE946, 0xD208, 0x2800, 0x0020, 0xBEFC, 0x94D4, 0x2800, 0xFB90, 0xF988, 0xF967, 0xB947, 0x4003, 0x0802, 0x1001, 0x3000, 0x7020, 0xFAEC, 0xE8A4, 0xFA8D, 0x4863, 0x9DD8, 0xACD5, 0x3000, 0x9822, 0xC8E5, 0xC1A7, 0x4000, 0x0800, 0xCEDD, 0x0800, 0x49A7, 0x4881, 0xA945, 0xF9CA, 0xC8A7, 0x3000, 0xA5B7, 0x0020, 0x2000, 0xBAAC, 0xAA29, 0x58E2, 0x0800, 0x08C3, 0xADF8, 0xC658, 0x0800, 0x2000, 0x89A8, 0xD3B0, 0x3000, 0x4125, 0xC699, 0xBF3A, 0x0000, 0x3821, 0x2000, 0x0800, 0x0080, 0xC699, 0xDEBB, 0xB719, 0xCDD7, 0x4003, 0x2000, 0x0020, 0x0080, 0xBF19, 0xBE36 };
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
