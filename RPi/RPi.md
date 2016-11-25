# RPi Image Processing and LED Interface
Micro-P's Final Project, Fall 2016

Image processing (of file in directory), and LED interfacing through the RPi's built-in SPI. (Does not deal with webserver)

# Command line interface
(Assumes there exists some desired image in directory called "image0.png")
>> python png2bmp.py
(Prints stuff)
>> make
>> sudo ./dotStar

# File Summaries
* png2bmp.py
  Python script utilizes Pillow library to load a .png image, scale the image appropriately (144 pixels tall), and convert the image to a .bmp file

* dotStar.c
  Main function for interfacing with the LEDs. Reads the file "image0.bmp", reads the image dimensions from the header bytes, then loads the RGB data into a char[]. Then initializes a SPI interface using the RPi SPI0, and outputs columns when "stepped"

* SPI.h
  Set-up file for GPIO and SPI interfacing. Defines functions like spiInit(), pinMode, etc.

# Hardware Connections
* RPi powered by micro-usb wall charger
* 5V power supply plugged into wall (stripped power cord screwed into terminals)
* LED strip powered by 5V (red and black wires screwed into power supply)
* RPi pins (pinout: https://az835927.vo.msecnd.net/sites/iot/Resources/images/PinMappings/RP2_Pinout.png)
** LED Blue Wire (Ground): Connect to any ground of the RPi (I use header pin (hp) 9)
** LED Green Wire (Data): GPIO 10 (SPI0 MOSI) (hp 19)
** LED Red Wire (Clk): GPIO 11 (SPI0 SCLK) (hp 21)
** LED Black Wire (5V): Unconnected --
** FPGA Stepped Pin: GPIO 27 (hp 13)
** FPGA Step Dir Pin: GPIO 22 (hp 15)

# External Dependencies
* Python and Pillow (Image Processing library)
  Used in png2bmp.py for image manipulation and conversion

