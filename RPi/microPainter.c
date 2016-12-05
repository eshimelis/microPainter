// dotStar.c
// Austin Chun
// Nov. 20th, 2016
// E155 Final Project
// Implement SPI protocol with dotStar LED strip
// APA102 datasheet: https://cdn-shop.adafruit.com/datasheets/APA102.pdf


#include <stdio.h>
#include <stdlib.h>
#include "SPI.h"

/////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////

// Arbitrary, can easilly change
#define FPGA_STEPPED 27
#define FPGA_STEP_DIR 22


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
 * @note       Assumes there is a file "image0.bmp" in the directory
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
    // Read 54-byte header (specific to bmp file format)
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f);

    // extract image height and width from header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];
    // Calculate padded row width (since bmp pad to make row multiple of 4 bytes)
    int row_padded = (width*3 + 3) & (~3);
    // Calcualte total size in bytes (3 times for RGB bytes per pixel)
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
    ////////////////////////////////////////////
    // ^^^^^^ USER DEFINED ^^^^^
    ////////////////////////////////////////////

    //////////////////
    // Define Image //
    //////////////////
    RGBandSize image = readBMP();

    int imageHeight = image.height;
    int imageWidth = image.width;
    unsigned char* RGBdata = image.RGB;
    // Starts at bottom left, goes down to up

    int i; // Cause C is stupid

    ////////////////////
    // Initialize SPI //
    ////////////////////
    printf("%s \n", "Initialize SPI");
    pioInit();
    spiInit(6000000, 0); // Initialize the SPI:
                        // 6 MHz clk, default settings
                        // note: I have ran it up to 8 MHz

    // Initialize column
    // note: column can be negative, to indicate before start of frame
    int column = -1;

    // Allows for fast sampling time (like FSM)
    int prevStep = 0;
    int stepped = 0;
    int direction = 0;

    // Update LEDs until reach end of image
    while(column < imageWidth){

        // FPGA Connection
        stepped = digitalRead(FPGA_STEPPED);

        // "Finite-State Machine" (s0: looking for posedge, s1: looking for neg edge)
        if(!prevStep && stepped){

            direction = digitalRead(FPGA_STEP_DIR);

            if(direction) ++column; // (___) WIRE
            else --column;
        }

    	else {
    		prevStep = stepped;
    		continue;
    	}

        prevStep = stepped;

        //printf("Column %d \n", column);

        // Time Stepping (just for tests)
        //   usleep(5000); // Delay 2s
        //   ++column; // increment column

        // Do nothing if out of picture frame
        if(column < 0) {
        continue;
        }

        // Send 32-bit start Frame
        for(i = 0; i < 4; i++){
            spiSendByte(0x00);
        }

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

    // Turn LEDs to black

    // Start Frame
    for(i = 0; i < 4; ++i) spiSendByte(0x00);
    // Black LED data
    for(i = 0; i < 144; ++i){
      spiSendByte(0xFF);
      spiSendByte(0x00);
      spiSendByte(0x00);
      spiSendByte(0x00);
    }
    // End Frame
    for(i = 0; i < 10; ++i) spiSendByte(0xFF);

    // Finished Painting
    printf("%s \n", "Finished painting picture");
    SPI0CSbits.TA = 0;
    free(RGBdata);
    return;
}
