// SPI.h
// Austin Chun
// Nov. 19th, 2016
// E155 Final Project
// Setup the SPI and GPIO interface
// Include Guards

#ifndef _INCLUDE_SPI_
#define _INCLUDE_SPI_
    
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

/////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////

// GPIO FSEL Types
#define INPUT  0
#define OUTPUT 1
#define ALT0   4
#define ALT1   5
#define ALT2   6
#define ALT3   7
#define ALT4   3
#define ALT5   2

#define GPFSEL   ((volatile unsigned int *) (gpio + 0))
#define GPSET    ((volatile unsigned int *) (gpio + 7))
#define GPCLR    ((volatile unsigned int *) (gpio + 10))
#define GPLEV    ((volatile unsigned int *) (gpio + 13))


typedef struct
{
    unsigned CS         :2;
    unsigned CPHA       :1;
    unsigned CPOL       :1;
    unsigned CLEAR      :2;
    unsigned CSPOL      :1;
    unsigned TA         :1;
    unsigned DMAEN      :1;
    unsigned INTD       :1;
    unsigned INTR       :1;
    unsigned ADCS       :1;
    unsigned REN        :1;
    unsigned LEN        :1;
    unsigned LMONO      :1;
    unsigned TE_EN      :1;
    unsigned DONE       :1;
    unsigned RXD        :1;
    unsigned TXD        :1;
    unsigned RXR        :1;
    unsigned RXF        :1;
    unsigned CSPOL0     :1;
    unsigned CSPOL1     :1;
    unsigned CSPOL2     :1;
    unsigned DMA_LEN    :1;
    unsigned LEN_LONG   :1;
    unsigned            :6;
}spi0csbits;

#define SPI0CSbits (* (volatile spi0csbits*) (spi0 + 0))
#define SPI0CS     (* (volatile unsigned int *) (spi0 + 0))
#define SPI0FIFO   (* (volatile unsigned int *) (spi0 + 1))
#define SPI0CLK    (* (volatile unsigned int *) (spi0 + 2))
#define SPI0DLEN (* (volatile unsigned int *) (spi0 + 3))

// Physical addresses
#define BCM2836_PERI_BASE    0x3F000000
#define GPIO_BASE            (BCM2836_PERI_BASE + 0x200000) // 0x3F2000000
#define BLOCK_SIZE           (4*1024)
// #define TIMER_BASE           0x3F003000
#define SPI0_BASE            0x3F204000

// Pointers that will be memory mapped when pioInit() is called
volatile unsigned int *gpio; //pointer to base of gpio
volatile unsigned int *spi0; //pointer to base of gpio

/**
 * @brief      setup access to GPIO and system_timer, navigating physcial/virtual memory mapping
 */
void pioInit() {
    int  mem_fd;
    void *reg_map;
    void *spi0_map;

    // /dev/mem is a psuedo-driver for accessing memory in the Linux filesystem
    if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
          printf("can't open /dev/mem \n");
          exit(-1);
    }

    reg_map = mmap(
      NULL,             //Address at which to start local mapping (null means don't-care)
      BLOCK_SIZE,       //Size of mapped memory block
      PROT_READ|PROT_WRITE,// Enable both reading and writing to the mapped memory
      MAP_SHARED,       // This program does not have exclusive access to this memory
      mem_fd,           // Map to /dev/mem
      GPIO_BASE);       // Offset to GPIO peripheral
    spi0_map = mmap(
      NULL,             
      BLOCK_SIZE,       
      PROT_READ|PROT_WRITE,
      MAP_SHARED,      
      mem_fd,           
      SPI0_BASE);       // Offset to SPI0 peripheral

    if (reg_map == MAP_FAILED) {
      printf("gpio mmap error %d\n", (int)reg_map);
      close(mem_fd);
      exit(-1);
    }
    if (spi0_map == MAP_FAILED) {
      printf("spi0 mmap error %d\n", (int)spi0_map);
      close(mem_fd);
      exit(-1);
    }

    gpio = (volatile unsigned *)reg_map;
    spi0 = (volatile unsigned *)spi0_map;
}


/////////////////////////////////////////////////////
//////////////// Helper Methods /////////////////////
/////////////////////////////////////////////////////

/**
 * @brief      sets the mode of a GPIO pin (defined above as FSEL types)
 *
 * @param[in]  pin   The pin number
 * @param[in]  fn    Input, Output, Alt1 etc.
 */
void pinMode(int pin, int fn){
    int reg = pin/10; // determines which register, 0-5
    int offset = (pin%10)*3; // determines offset, 0-9 
    GPFSEL[reg] &= ~((0b111&fn)<< offset); // clear bits
    GPFSEL[reg] |=  ((0b111&fn)<<offset);  // set bits
}

/**
 * @brief      Writes a 0 or 1 to a GPIO pin
 *
 * @param[in]  pin    The pin number
 * @param[in]  value  The value (0 or 1)
 */
void digitalWrite(int pin, int value){
    int reg = pin/32; // determines i for GPSET[i] and GPCLR[i]
    int offset = pin%32; // determines offset in GPSET[i]/GPCLR[i]
    if (value)  GPSET[reg] |= (0x1 << offset); // write HIGH
    else         GPCLR[reg] |= (0x1 << offset); // write LOW
}

/**
 * @brief      Reads the value of a GPIO pin
 *
 * @param[in]  pin   The pin number
 *
 * @return     the value of the pin
 */
int digitalRead(int pin){
    int reg = pin/32; 
    int offset = pin%32;
    return (GPLEV[reg]>>offset) & 0x1; // read corrsponding bit
}

/**
 * @brief      Initialize SPI
 *
 * @param[in]  freq      The frequency
 * @param[in]  settings  The settings {CPhase,CPolarity}
 *                       0: {0,0}, 1:{0,1}, 2:{1,0}, 3:{1,1}
 */
void spiInit(int freq, int settings) {
    //set GPIO 8 (CE), 9 (MISO), 10 (MOSI), 11 (SCLK) alt fxn 0 (SPI0)
    pinMode(8, ALT0); 
    pinMode(9, ALT0); 
    pinMode(10, ALT0); 
    pinMode(11, ALT0); 
    SPI0CLK = 250000000/freq; // Set SPI clock divider to desired freq
    SPI0CS = settings;
    SPI0CSbits.TA = 1; // Turn SPI on
}

/**
 * @brief      send data from SPI
 *
 * @param[in]  send  a single byte
 */
char spiSendByte(unsigned char send){
    SPI0FIFO = send;          // Send data to slave
    while (!SPI0CSbits.DONE); // Wait until SPI complete
    return SPI0FIFO;
}

#endif
