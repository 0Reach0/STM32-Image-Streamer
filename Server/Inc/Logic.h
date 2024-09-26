#include <stdint.h>
#include <SFML/Graphics.hpp>


 #define BAUD_RATE  115200
 #define CHARACTER_SIZE 8
 #define STOP_BIT_ONE  boost::asio::serial_port_base::stop_bits::one
 #define START_CONVERSATION 0xA 
 #define END_CONVERSATION 0xB

#define CREATE_WINDOW  0x11
#define RESIZE_WINDOW  0x12
#define FILL_COLOR     0x13
#define DRAW_PIXEL     0x14
#define DRAW_IMAGE     0x15
#define RESET_SCREEN   0x16
#define CLOSE_WINDOW   0x17
#define WRITE_TEXT     0x18 

#define STOP_COMMAND "DISCONNECT"


void init_usart();

int8_t listening_create_window();
int8_t listening_resize_window();
int8_t listening_fill_color();
int8_t listening_draw_pixel();
int8_t listening_draw_image();
int8_t listening_write_text();

int8_t create_window(uint16_t, uint16_t);
int8_t resize_window(uint16_t, uint16_t);
int8_t fill_color(int8_t, int8_t,int8_t);
int8_t draw_pixel(uint16_t, uint16_t, int8_t , int8_t , int8_t );
int8_t draw_image(uint16_t , uint16_t ,uint16_t , uint16_t,uint16_t *);
int8_t write_text(uint16_t , uint16_t,uint8_t ,uint8_t ,uint8_t , uint8_t  *);
int8_t reset_screen();
int8_t close_window();
int8_t sent_ready();
int8_t end_conversation();
void sent_error();
void sent_succesful();
void send_end_conversation();


struct RGB {
    uint8_t r, g, b;
};


RGB rgb565_to_rgb888(uint16_t color);
void add_pixel(sf::Vertex);
