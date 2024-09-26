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



void init(char * path_to_port)
{
    serial.open(path_to_port);
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


void start(char * path_to_port)
{
    char data[10];
    init(path_to_port);
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

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cout << "Only 1 argument :  <PATH_TO_PORT>\n for example </dev/ttyUSB1>";
        return 0;
    }
    
    start(argv[1]);
}