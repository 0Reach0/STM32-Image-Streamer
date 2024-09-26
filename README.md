# STM32-Image-Streamer
STM32-Image-Streamer is a project that facilitates the transfer of images from an STM32 microcontroller to a computer using UART 



######################################################################
#                          Project Overview                          #
######################################################################

This project includes four main files:

1. **Server**: 
   - This application runs on a PC and waits for incoming signals 
     from the microcontroller. 
   - It uses SFML to display images based on the received data.
   - **Example** : `Server <PATH_TO_PORT>`
      - where <PATH_TO_PORT> is the path to the tty port that the STM32 UART is connected to.

2. **Client**: 
   - This file is designed for integration into your STM32 project 
     using HAL. 
   - Be sure to modify the included HAL file to match your specific 
     microcontroller.

3. **RGBConverter888to565**: 
   - This script converts PNG and JPEG files into the RGB565 format, 
     enabling efficient image representation.
   - **Example**: `RGBConventor <inputfile> <outputfile> <filetype>` 
     - where "filetype" is `p` for PNG and `j` for JPEG.

4. **ArrayMaker**: 
   - This utility transforms RGB565 data into a C array, which is 
     output to a text file. 
   - The resulting array can easily be copied and pasted into your 
     code.
   - **Example**: `ArrayMaker <inputfile> <outputfile>`

######################################################################

