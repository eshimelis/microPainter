// dotStar.c
// Austin Chun
// Nov. 20th, 2016
// E155 Final Projecy
// Implement SPI protocol with dotStar LED strip
// APA102 datasheet: https://cdn-shop.adafruit.com/datasheets/APA102.pdf

     
// #include <sys/mman.h>
// #include <fcntl.h>
// #include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "SPI.h"

/////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////

#define FPGA_STEPPED 27 // YELLOW
#define FPGA_STEP_DIR 22 // BLUE


// Define RGB data type
typedef struct
{
    unsigned char R;
    unsigned char G;
    unsigned char B;
} colorData;

// Simple struct for passing data between functions
typedef struct {
    unsigned char* RGB;
    int width;
    int height;
} RGBandSize;


/**
 * @brief      Reads RGB data from a bmp image file
 *
 * @return     struct with RGB data, and image dimensions
 */
RGBandSize readBMP(void)
{
    // Loop variables
    int i, j, k;
    // Load bmp file
    FILE* f = fopen("image0.bmp", "rb");
    // Error catching
    if(f == NULL){
        printf("ERROR: Null file");
    }
    // Read 54-byte header
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f);

    // extract image height and width from header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];
    // Calculate padded row width (since bmp pad to make row multiple of 4 bytes)
    int row_padded = (width*3 + 3) & (~3);
    // Calcualte total size in bytes
    int size = 3 * width * height;

    // Single row to read off one row at a time
    unsigned char* singlerow = (unsigned char*) malloc(row_padded * sizeof(unsigned char));
    // Total data storage
    unsigned char* data = (unsigned char*) malloc(size * sizeof(unsigned char));
    // Temp char for swapping
    unsigned char temp;

    // Loop through each row of image
    for(i = 0; i < height; ++i){
        // Read a whole row (including padded bytes)
        fread(singlerow, sizeof(unsigned char), row_padded, f);

        // Convert from BGR to RGB
        for(j = 0; j < 3*width; j +=3){
            temp = singlerow[j];
            singlerow[j] = singlerow[j+2];
            singlerow[j+2] = temp;
        }
        // Copy over RGB bytes of singlerow to total data storage
        for(k = 0; k < 3*width; ++k){
            data[(i * 3* width) + k] = singlerow[k];
        }
    }
    
    // Clean-up
    free(singlerow);

    // Return as struct to maintain dimensions
    RGBandSize result = {data, width, height};
    return result;
}

void main(void) {
    printf("%s \n", "Start Lights");

    // Brightness level, 0-255, DO NO SET ABOVE 
    // 0 = full brightness
    // Intentionally 0, for manual brightness adjustment
    // 1 = minimum brightness (off)
    // 255 = one less than full brightness
    unsigned short brightness = 60; 

    //////////////////
    // Define Image //
    //////////////////
    RGBandSize image = readBMP();

    int imageHeight = image.height;
    int imageWidth = image.width;
    unsigned char* RGBdata = image.RGB;
    // Starts at bottom left, goes down to up

    int i;


    ////////////////////
    // Initialize SPI //
    ////////////////////
    printf("%s \n", "Initialize SPI");
    pioInit();
    spiInit(6000000, 0); // Initialize the SPI:
                        // 100 kHz clk, default settings

    // Initialize column
    // note: column can be negative, to indicate before start of frame
    int column = -1;

    // Allows for fast sampling time (like FSM)
    int justStepped = 0;
    int stepped = 0;

    // Update LEDs until reach end of image
    while(column < imageWidth){


  // FPGA Connection
      stepped = digitalRead(FPGA_STEPPED);

      if(justStepped || !stepped){
        // Only update LED's when step is made
        if(!stepped){ // YELLOW WIRE
          justStepped = 0;
        }
        // usleep(1000); // Wait 1ms before next sample
        continue;
      }

      justStepped = 1;
      // Thus it has registered a step, and shall proceed

      // Update column position
      if(digitalRead(FPGA_STEP_DIR)) ++column; // BLUE WIRE
      else                           --column; 


  // Time Stepping
      // usleep(10000); // Delay 2s
      // ++column; // increment column

      // Do nothing if out of picture frame
      if(column < 0) {
        continue; 
      }
      // Update LEDs 
      printf("%s %d\n", "Column: ",column);
      
      // Send 32-bit start Frame
      for(i = 0; i < 4; i++){
        spiSendByte(0x00); 
      }
      // printf("%s \n", "Sent start bit");
      int row;
      // Send LED data
      // printf("%s %d\n", "Send LED Data: ",column);
      // Loop through rows, from bottom to top
      for(row = 0; row < imageHeight; ++row){
        // 3-bit start frame, and 5-bit brightness (fixed at FULL b/c hardware problems)
        spiSendByte(0xFF); 

        // printf("%d,%d: %x,%x,%x\n",row, column, RGBdata[3*(row*imageWidth + column) + 0], RGBdata[3*(row*imageWidth + column) + 1], RGBdata[3*(row*imageWidth + column) + 2]);
        // Adjust color data for brightness
        if(brightness){ // Won't adjust if desired brightness is 0 ie FULL
          // scale by the brightness, and take 8 MSB for scaled color
          spiSendByte( (RGBdata[3*(row*imageWidth + column) + 2] * brightness) >> 8); // Blue data
          spiSendByte( (RGBdata[3*(row*imageWidth + column)+ 1] * brightness) >> 8); // Green aata
          spiSendByte( (RGBdata[3*(row*imageWidth + column) + 0] * brightness) >> 8); // Red data
        }
        // No need to adjust color
        else{
          spiSendByte(RGBdata[3*(row*imageWidth + column) + 2]);
          spiSendByte(RGBdata[3*(row*imageWidth + column) + 1]);
          spiSendByte(RGBdata[3*(row*imageWidth + column) + 0]);
        }
        // printf("%s %d \n", "Sent led data", row);
      }
      // Send 72-bit end Frame
      // printf("%s %d\n", "Send End: ",column);
      for(i = 0; i < 9; ++i) {
        spiSendByte(0xFF); 
      }  

    }

    // Finished Painting
    printf("%s \n", "Finished painting picture");
    SPI0CSbits.TA = 0;
    free(RGBdata);
    return;
}

