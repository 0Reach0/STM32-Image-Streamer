#include <boost/asio.hpp> 
#include <iostream>
#include "../Inc/Logic.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <atomic>
#include <chrono>
#include <thread>


 const uint8_t COMMAND_OPCODE =0;
 const uint8_t DATA_OPCODE =1;
 const uint8_t SUCCESFUL_OPERATION =0x20 ;
 const uint8_t READY = 0x18;
 const uint8_t ERROR = 0X19;






extern std::atomic<sf::Color> fillColor;
extern std::atomic<bool> isActive;
extern std::atomic<bool> isRender;
extern std::atomic<bool> isResize;
extern boost::asio::serial_port serial;
extern std::vector<sf::Sprite> sprites;
extern std::vector<sf::Texture * > images;
extern std::vector<sf::Vertex> pixels;
extern sf::RenderWindow window;
extern std::atomic<uint16_t> widht ;
extern std::atomic<uint16_t> height;
extern std::atomic<bool> isFillScreen;
extern std::vector<sf::Text> strings;
extern sf::Font font;



void init_usart()
{
    serial.set_option(boost::asio::serial_port_base::baud_rate(BAUD_RATE));
    serial.set_option(boost::asio::serial_port_base::character_size(CHARACTER_SIZE));
    serial.set_option(boost::asio::serial_port_base::stop_bits(STOP_BIT_ONE));
}



int8_t listening_create_window()
{
    if(!sent_ready())
    {
        return 0;
    }
    
    const std::size_t expected_size = 4; 
    std::uint8_t  * data = new uint8_t[expected_size];    


    boost::system::error_code ec;
    std::size_t bytes_read = boost::asio::read(serial, boost::asio::buffer(data, expected_size), boost::asio::transfer_exactly(expected_size), ec);
    if (!ec) {
        if (bytes_read == expected_size) {
            uint16_t width = (static_cast<uint16_t>(data[0]) << 8) | data[1];
            uint16_t height = (static_cast<uint16_t>(data[2]) << 8) | data[3];
            create_window(width, height);           
        } else {
            std::cerr << "Read error: " << bytes_read << " bytes read" << std::endl;
            return 0;
        }
    } else {
        std::cerr << "Read error " << ec.message() << std::endl;
        return 0;
    }

   delete[] data;
    return 1;
}






int8_t listening_resize_window()
{
    if(!sent_ready())
    {
        return 0;
    }
    const std::size_t expected_size = 4; 
    std::uint8_t* data = new uint8_t[expected_size];    


    boost::system::error_code ec;
    
    std::size_t bytes_read = boost::asio::read(serial, boost::asio::buffer(data, expected_size), boost::asio::transfer_exactly(expected_size), ec);

    if (!ec) {
        if (bytes_read == expected_size) {
            uint16_t width = (static_cast<uint16_t>(data[0]) << 8) | data[1];
            uint16_t height = (static_cast<uint16_t>(data[2]) << 8) | data[3];
            resize_window(width, height);            
        } else {
            std::cerr << "Read error: " << bytes_read << " bytes read" << std::endl;
            return 0;
        }
    } else {
        std::cerr << "Read error " << ec.message() << std::endl;
        return 0;
    }

    delete[] data;
    return 1; 
}





int8_t listening_fill_color()
{
    if(!sent_ready())
    {
        return 0;
    }
    const std::size_t expected_size = 3; 
    uint8_t * data = new uint8_t[expected_size];    


    boost::system::error_code ec;
    std::size_t bytes_read = boost::asio::read(serial, boost::asio::buffer(data, expected_size), boost::asio::transfer_exactly(expected_size), ec);
    if (!ec) {
        if (bytes_read == expected_size) {
            uint8_t red = (static_cast<uint8_t>(data[0]));
            uint8_t green = (static_cast<uint8_t>(data[1]));
            uint8_t blue = (static_cast<uint8_t>(data[2]));
            fill_color(red,green, blue);            
        } else {
            std::cerr << "Read error: " << bytes_read << " bytes read" << std::endl;
            return 0;
        }
    } else {
        std::cerr << "Read error " << ec.message() << std::endl;
        return 0;
    }

    delete[] data;
    return 1;
     
}





int8_t listening_draw_pixel()
{   
    if(!sent_ready())
    {
        return 0;
    }
    const std::size_t expected_size = 7; 
    std::uint8_t * data = new uint8_t[expected_size];     

    boost::system::error_code ec;
    std::size_t bytes_read = boost::asio::read(serial, boost::asio::buffer(data, expected_size), boost::asio::transfer_exactly(expected_size), ec);

    if (!ec) {
        if (bytes_read == expected_size) {
            uint16_t x = (static_cast<uint16_t>(data[0]) << 8) | data[1];
            uint16_t y = (static_cast<uint16_t>(data[2]) << 8) | data[3];
            uint8_t red = (static_cast<uint8_t>(data[4]));
            uint8_t green = (static_cast<uint8_t>(data[5]));
            uint8_t blue = (static_cast<uint8_t>(data[6]));
            draw_pixel(x,y,red,green,blue);
        } else {
            std::cerr << "Read error: " << bytes_read << " bytes read" << std::endl;
            return 0;
        }
    } else {
        std::cerr << "Read error " << ec.message() << std::endl;
        return 0;
    }

    delete[] data; 
    return 1;
}






int8_t listening_draw_image()
{
    if(!sent_ready())
    {
        return 0;
    }
    const std::size_t expected_size = 4+4+4+4; 
    std::uint8_t * data = new uint8_t[expected_size];    
    uint32_t  image_size = 0;


    boost::system::error_code ec;
    std::size_t bytes_read = boost::asio::read(serial, boost::asio::buffer(data, expected_size),  boost::asio::transfer_exactly(expected_size), ec);

    if (!ec) {
        if(!sent_ready())
        {
            return 0;
        }
        uint16_t x = (static_cast<uint16_t>(data[0]) << 8) | data[1];
        uint16_t y =(static_cast<uint16_t>(data[2]) << 8) | data[3];
        uint16_t widht = (static_cast<uint16_t>(data[4]) << 8) | data[5];
        uint16_t height = (static_cast<uint16_t>(data[6]) << 8) | data[7];
        image_size  =(static_cast<uint16_t>(data[8]) << 24) | (data[9] << 16)|(data[10] << 8)| data[11];
        uint16_t * image = new uint16_t[image_size];
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); 
        bytes_read = boost::asio::read(serial, boost::asio::buffer(image, image_size), boost::asio::transfer_exactly(image_size), ec);
        if(bytes_read == image_size)
        {
            draw_image(x, y, widht, height ,image);
        }
        else{
            std::cerr << "Read error: " << bytes_read << " bytes read" << std::endl;
            return 0;
        }
         delete[] image;
    }
     else {
        std::cerr << "Read error " << ec.message() << std::endl;
        return 0;
    }
    delete[] data;  
    return 1;
}





int8_t listening_write_text()
{
 if(!sent_ready())
    {
        return 0;
    }
    const std::size_t expected_size = 4+3+2; 
    uint8_t * data = new uint8_t[expected_size];    
    uint16_t  text_size = 0;

    boost::system::error_code ec;
    std::size_t bytes_read = boost::asio::read(serial, boost::asio::buffer(data, expected_size), boost::asio::transfer_exactly(expected_size), ec);
    if (!ec) {
        if(!sent_ready())
        {
            return 0;
        }
        uint16_t x = (static_cast<uint16_t>(data[0]) << 8) | data[1];
        uint16_t y =(static_cast<uint16_t>(data[2]) << 8) | data[3];
        uint8_t r = data[4];
        uint8_t g = data[5];
        uint8_t b = data[6];
        text_size  =(static_cast<uint16_t>(data[7]) << 8) | data[8];
        uint8_t * text = new uint8_t[text_size];
        bytes_read = boost::asio::read(serial, boost::asio::buffer(text, text_size), boost::asio::transfer_exactly(text_size),  ec);
        if(bytes_read == text_size && !ec)
        {
            write_text(x, y,r,g,b, text);
        }
        else{
            std::cerr << "Read error: " << bytes_read << " bytes read" << std::endl;
            return 0;
        }
         delete[] text;
    } else {
        std::cerr << "Read error " << ec.message() << std::endl;
        return 0;
    }

    delete[] data;
    return 1;
}





int8_t create_window(uint16_t w, uint16_t h)
{
    widht.store(w);
    height.store(h);
    isRender.store(true);
    return 1;
}




int8_t resize_window(uint16_t w, uint16_t h)
{
    widht.store(w);
    height.store(h);
    isResize.store(true);
    return 1;
}



int8_t fill_color(int8_t red, int8_t green,int8_t blue)
{
    isFillScreen.store(true);
    fillColor.store(sf::Color(red, green, blue));
    return 1;
}



int8_t draw_pixel(uint16_t x, uint16_t y , int8_t red, int8_t green, int8_t blue)
{
    sf::Color col{red, green,blue};
    sf::Vertex px{sf::Vector2f{x,y}, col};
    add_pixel(px);
    return 1;
}





int8_t draw_image(uint16_t x, uint16_t y, uint16_t weight, uint16_t height, uint16_t *data) {
    
uint8_t *rgbData = new uint8_t[weight * height * 4];
for (int y = 0; y < height; y++) {
    for (int x = 0; x < weight; x++) {
        uint16_t i = y * weight + x;
        uint16_t pixel = data[i];
        uint8_t r5 = (pixel >> 11) & 0x1F;   
        uint8_t g6 = (pixel >> 5) & 0x3F;    
        uint8_t b5 = pixel & 0x1F;           
        rgbData[i * 4]     = (r5 << 3) | (r5 >> 2);
        rgbData[i * 4 + 1] = (g6 << 2) | (g6 >> 4);
        rgbData[i * 4 + 2] = (b5 << 3) | (b5 >> 2);
        rgbData[i * 4 + 3] = 255;
    }
}


sf::Texture *texture = new sf::Texture();
texture->create(weight, height);
texture->update(rgbData);

sf::Sprite sprite(*texture);
sprite.setPosition(x, y);
images.push_back(texture);
sprites.push_back(sprite);

// sf::Image output_image;
// output_image.create(weight, height, rgbaData);
// if (!output_image.saveToFile("image.png")) {
//     std::cerr << "Failed to save image to PNG file" << std::endl;
//     return 0;
// }

delete[]  rgbData;    

return 1;
}


int8_t write_text(uint16_t x, uint16_t y, uint8_t r,uint8_t g,uint8_t b, uint8_t * data)
{

    
    sf::Text text;
    text.setFont(font);
    text.setString((char*)data);
    text.setCharacterSize(24); 
    text.setFillColor(sf::Color(r,g,b)); 
    text.setPosition(50, 50); 
    strings.push_back(text);
    return 1;


}




int8_t reset_screen()
{
    pixels.erase(pixels.begin(), pixels.end());
    images.erase(images.begin(), images.end());
    sprites.erase(sprites.begin(), sprites.end());
    fill_color(255,255,255);
    return 1;
}



 int8_t close_window()
 {
    pixels.erase(pixels.begin(), pixels.end());
    images.erase(images.begin(), images.end());
    sprites.erase(sprites.begin(), sprites.end());
    fill_color(255,255,255);
    isRender.store(false);
 }


int8_t sent_ready()
{
    boost::system::error_code ec;
    
    uint8_t ready = READY;
    boost::asio::write(serial, boost::asio::buffer(&ready ,sizeof(ready)), ec);
    if (ec) {
        std::cerr << "Error writing to serial port: " << ec.message() << std::endl;
        sent_error();
        return 0;
    } else {
        return 1;
    }

}


int8_t end_conversation()
{
    isActive.store(false);
    isRender.store(false);
    return 1;
}
void sent_error()
{
    boost::asio::write(serial, boost::asio::buffer(&ERROR, sizeof(ERROR)));
}


void sent_succesful()
{
        boost::system::error_code ec;
    
    uint8_t succesful = SUCCESFUL_OPERATION;
    boost::asio::write(serial, boost::asio::buffer(&succesful ,sizeof(succesful)), ec);
    if (ec) {
        std::cerr << "Error writing to serial port: " << ec.message() << std::endl;
        sent_error();
    }
}


RGB rgb565_to_rgb888(uint16_t pixel) {
    RGB rgb;

    rgb.r = ((pixel >> 11) & 0x1F) * 255 / 31;
    rgb.g = ((pixel >> 5) & 0x3F) * 255 / 63;
    rgb.b = (pixel & 0x1F) * 255 / 31;

    return rgb;
}

void add_pixel(sf::Vertex vec)
{
    if(pixels.empty()) pixels.push_back(vec);
    for( int i = 0  ; i <  pixels.size(); i++)
    {
        if(pixels[i].position.x == vec.position.x && pixels[i].position.y == vec.position.y)
        {
            pixels.erase(pixels.begin()+i);
            pixels.push_back(vec);
            break;
        }
        else 
        {
            pixels.push_back(vec);
            break;
        }
    }
}
