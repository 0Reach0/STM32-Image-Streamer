#include "../Inc/General.h"
#include <mutex>
#include <thread> 
#include <boost/asio.hpp>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include "Logic.h"
#include <chrono>




extern const uint8_t COMMAND_OPCODE;
extern  const uint8_t DATA_OPCODE;
extern  const uint8_t SUCCESFUL_OPERATION;
extern const uint8_t READY;
extern const uint8_t ERROR;





boost::asio::io_service io;
boost::asio::serial_port serial(io);
sf::RenderWindow window;
std::vector<sf::Sprite> sprites;
std::vector<sf::Texture *> images;
std::vector<sf::Vertex> pixels;
std::vector<sf::Text> strings;
std::atomic<bool> isResize(false);
std::atomic<bool> isFillScreen(false);
std::atomic<sf::Color>fillColor(sf::Color(255, 255, 255));
std::atomic<bool> isActive(false);
std::atomic<bool> isRender(false);
std::atomic<uint16_t> widht (0);
std::atomic<uint16_t> height(0);
sf::Font font;



#define RESIZE_WINDOW 0x12
#define FILL_COLOR 0x13
#define DRAW_PIXE 0x14
#define DRAW_IMAGE 0x15
#define RESET_SCREEN 0x16
#define CLOSE_WINDOW 0x17



void init()
{
    serial.open(PATH_TO_PORT);
    init_usart();
    isActive.store(false);
    fillColor.store(sf::Color::Black);
     if (!font.loadFromFile("../Src/intro.ttf")) {
        std::cerr << "Cant load a font!" << std::endl;
    }
}


void receiver_process()
{
    uint8_t command;

    while(isActive)
    {    
        command = 0;   
         boost::asio::read(serial, boost::asio::buffer(&command, sizeof(command)));
         switch (command)
         {
         case CREATE_WINDOW:
            if(listening_create_window()) sent_succesful();
            break;
         case RESIZE_WINDOW:
            if(listening_resize_window()) sent_succesful();
            break;
        case FILL_COLOR:
            if(listening_fill_color()) sent_succesful();
            break;
        case DRAW_PIXEL:
            if(listening_draw_pixel()) sent_succesful();
            break;
        case DRAW_IMAGE:
            if(listening_draw_image()) sent_succesful();
            break;
        case WRITE_TEXT:
            if(listening_write_text()) sent_succesful();
            break;
        case RESET_SCREEN:
            if(reset_screen()) sent_succesful();
            break;
        case CLOSE_WINDOW:
            if(close_window()) sent_succesful();
            break;
        case END_CONVERSATION:
            if(end_conversation()) sent_succesful();
            break;
         default:
            sent_error();
            break;
         }
    }
}



void render_process()
{
    while(!isRender);
    
    window.create(sf::VideoMode(widht, height), "Window");
    window.setFramerateLimit(60);
    while(isRender & window.isOpen())
    {   
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(fillColor);

        if(isResize == true)
        {
            isResize.store(false);
            window.setSize(sf::Vector2u(widht, height));  
            sf::FloatRect visibleArea(0, 0, widht, height);
            window.setView(sf::View(visibleArea));
        }
        if(isFillScreen == true)
        {
            isFillScreen.store(false);
            strings.erase(strings.begin(), strings.end());
            images.erase(images.begin(), images.end());
            pixels.erase(pixels.begin(), pixels.end());
        }
        for(size_t i =0; i < strings.size(); i++)
        {
            window.draw(strings[i]);
        }
        for (size_t i = 0; i < pixels.size(); i++) {
             window.draw(&pixels[i], sizeof(pixels[i]), sf::Points); 
        }
        if(!sprites.empty())
        {
            for (int i = 0; i < sprites.size(); i++) {
             window.draw(sprites[i]);
         }
        }

        window.display(); 
    }
    images.erase(images.begin(), images.end());
    sprites.erase(sprites.begin(), sprites.end());
    pixels.erase(pixels.begin(), pixels.end());
    window.close();
}


void start()
{
    char data[10];
    init();
    boost::system::error_code ec;
    uint8_t signal;

    start :
    while(true)
    {
        std::size_t bytes_read = boost::asio::read(serial, boost::asio::buffer(&signal, sizeof(signal)), boost::asio::transfer_exactly(sizeof(signal)), ec);
        if(ec){
            sent_error();
        }
        else{
            if(signal == START_CONVERSATION)
            {
                if(sent_ready()) break;
                else sent_error();
            }

            else sent_error();
        }
    }
    
    isActive.store(true);
    std::thread receive(receiver_process);
    std::thread render(render_process);
    render.detach();
    receive.detach();
    
    while(true)
    {
        if(isRender == false && isActive == false)
        {   
            std::this_thread::sleep_for(std::chrono::milliseconds(1000)); 
            goto start;
        }
        if(data == STOP_COMMAND)
        {
            isActive.store(false);
            isRender.store(false);
            break;
        };
    }
}

int main()
{
    start();
    // uint16_t image[] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFF7C, 0xE400, 0xE400, 0xE300, 0xE320, 0xFF5B, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xE3E0, 0xEC00, 0xFC80, 0xFB80, 0xEB20, 0xE300, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xE3E0, 0xFC60, 0xFC60, 0xFB60, 0xFB80, 0xE300, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFBD, 0xF6D7, 0xFFFF, 0xFFFF, 0xE484, 0xEC20, 0xFC60, 0xFB60, 0xEB20, 0xEBC4, 0xFFFF, 0xFFFF, 0xF6D7, 0xFFBD, 0xFFFF, 0xF6F7, 0xE3E0, 0xE3E0, 0xE400, 0xED6C, 0xF6B5, 0xEC20, 0xFC60, 0xFB60, 0xF320, 0xF655, 0xED8C, 0xE400, 0xE3E0, 0xE400, 0xFEF7, 0xE3E0, 0xEC20, 0xFC80, 0xF440, 0xEC00, 0xF400, 0xEC00, 0xFCC1, 0xFBC1, 0xEB40, 0xEC40, 0xEC00, 0xF440, 0xFC80, 0xEC20, 0xE3E0, 0xE300, 0xFBA0, 0xFC00, 0xFC60, 0xFC80, 0xFC60, 0xFE87, 0xFF8B, 0xFFAB, 0xFE47, 0xFC60, 0xFC80, 0xFC60, 0xFC00, 0xFBA0, 0xE300, 0xE320, 0xFB60, 0xFB80, 0xFB60, 0xFBA0, 0xFD44, 0xFF8B, 0xFF8B, 0xFF6A, 0xFEE7, 0xFD02, 0xFBA0, 0xFB60, 0xFB80, 0xFB60, 0xE320, 0xEBC4, 0xE320, 0xE300, 0xF340, 0xF340, 0xFDA6, 0xFF8B, 0xFF6A, 0xFEC6, 0xFEC6, 0xFD44, 0xF340, 0xF340, 0xE300, 0xE320, 0xE3C4, 0xFFFF, 0xFFBE, 0xF5B1, 0xECAA, 0xFB80, 0xFD43, 0xFF6B, 0xFEC6, 0xFEC6, 0xFEE7, 0xFD02, 0xFB80, 0xECAA, 0xF5B1, 0xFFBE, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xED6B, 0xFC40, 0xFC60, 0xFD84, 0xFEE6, 0xFEE6, 0xFD84, 0xFC60, 0xFC40, 0xED4B, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xED8C, 0xE3E0, 0xFC40, 0xFC40, 0xFB60, 0xFB80, 0xFB80, 0xFB60, 0xFC40, 0xFC40, 0xE3E0, 0xED8C, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFDE, 0xE3E0, 0xFC60, 0xFC80, 0xFB80, 0xF340, 0xF340, 0xF340, 0xF340, 0xFB80, 0xFC80, 0xFC60, 0xE3E0, 0xFFDE, 0xFFFF, 0xFFFF, 0xFF9C, 0xE400, 0xFC80, 0xFBC0, 0xFB80, 0xE300, 0xECCB, 0xECEC, 0xE300, 0xFB80, 0xFBC0, 0xFC80, 0xE400, 0xFF7B, 0xFFFF, 0xFFFF, 0xFFFF, 0xE400, 0xE3C0, 0xFB60, 0xEB20, 0xE300, 0xFFFF, 0xFFFF, 0xE300, 0xEB20, 0xFB60, 0xE3A0, 0xE400, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xE3C4, 0xE300, 0xE320, 0xF675, 0xFFFF, 0xFFFF, 0xF675, 0xE320, 0xE300, 0xEBC4, 0xFFFF, 0xFFFF, 0xFFFF} ;
    // create_window(100,100);
    // draw_image(50,50, 16,16, image);
}