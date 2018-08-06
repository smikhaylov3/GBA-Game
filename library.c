#include "library.h"
#include "text.h"
#include <stdlib.h>
//unsigned short *videoBuffer = (unsigned short*) 0x6000000;
int scale = 0;
//DMA implementation of draw image.
void drawImage3(int r, int x, int width, int height, const
u16* image) {
	for (int i = 0; i < height; i++) {
		DMA[3].src = &image[OFFSET(i, 0, width)];
		DMA[3].dst = videoBuffer + OFFSET(r + i, x, WIDTH);
		DMA[3].cnt = (width) | DMA_ON;
	}
}

void waitForVblank(){
	while(*SCANLINECOUNTER > 160)
	{}
	while(*SCANLINECOUNTER < 160)
	{}
}
//DMA method to set screen to a color. Unused in current game.
void fillScreen3(volatile unsigned short color)

{
	volatile unsigned int colorcolor = color<<16 | color;
	DMA[3].src = &colorcolor;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = 19200 | DMA_SOURCE_FIXED | DMA_32 | DMA_ON;
}
//Copies iamge through dma. Unused in current implementation.
void copyPicture4(const unsigned short *buffer)
{

	DMA[3].src = buffer;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = 19200 | DMA_16 | DMA_ON;
}

int random_between(int min, int max) {
    return rand() % (max - min + 1) + min;
}
//Draw colored rectangle through dma.
void drawRect3(int r, int c, int width, int height, u16 color) {
	for (int x = 0; x < height; x++) {
		DMA[3].src = &color;
		DMA[3].dst = &videoBuffer[OFFSET(r + x, c, 240)];
		DMA[3].cnt = (width) | DMA_SOURCE_FIXED | DMA_ON;
	}
}
//Writes string at specified coordinates by writing individual characters.
void drawString(int x, int y, char *string, u8 index) {
    while (*string) {
        drawChar(x, y, *string++, index);
        y = y + 6;
    }
}
//Access pixel array for individual character and draws it to screen.
void drawChar(int x, int y, char str, u8 ind) {
    for (int a = 0; a < 8; a++)
        for (int b = 0; b < 6; b++)
            if (fontdata_6x8[a * 6 + b + 48 * str]){
            	setPixel(x + a, y + b, ind);
        }
            
}
//Set pixel on screen to specified color.
void setPixel(int row, int column, u8 index) {
    int pixel = row * 240 + column;
    int shortNum = pixel;
    if (column & 1)
        videoBuffer[shortNum] = (videoBuffer[shortNum] & 0x00FF) | (index << 8);
    else
        videoBuffer[shortNum] = (videoBuffer[shortNum] & 0xFF00) | (index);
}