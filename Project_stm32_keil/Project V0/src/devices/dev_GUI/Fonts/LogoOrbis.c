#include "Graphics.h"

extern const char L21592[];
//BITMAP NAME CAN BE CHANGED HERE.
volatile const BITMAP_FLASH LogoOrbis = {(TYPE_MEMORY)0,(FLASH_BYTE*)L21592};
const char L21592[] = {
0x00,0x01,0x1B,0x00,0x7C,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x80,0x00,0x00,0x60,0x00,0x00,0x60,
0x00,0x00,0x60,0x00,0x00,0x30,0x00,0x00,0x10,0x80,0x00,0x00,0x60,0x00,0x00,0x60,0x00,0x00,0x60,0x00,0x00,0x30,0x00,0x00,0x10,0x80,0x1E,0x00,0x60,0xFE,0x00,0x60,0x7E,
0x00,0x60,0x03,0x00,0x30,0x01,0x80,0x10,0x80,0x7F,0x80,0x60,0xFF,0x80,0x60,0xFF,0x80,0x60,0x03,0x00,0x30,0x07,0xC0,0x10,0x81,0xC0,0xE0,0x60,0x80,0xC0,0x60,0xC0,0xC0,
0x60,0x03,0x00,0x30,0x0C,0x20,0x10,0x83,0x00,0x30,0x60,0x80,0x60,0x60,0xC0,0x40,0x60,0x03,0x00,0x30,0x18,0x10,0x10,0x86,0x00,0x18,0x60,0x80,0x20,0x60,0xC0,0x60,0x60,
0x03,0x00,0x30,0x18,0x10,0x10,0x84,0x00,0x08,0x60,0x80,0x30,0x60,0xC0,0x60,0x60,0x03,0x00,0x30,0x18,0x00,0x10,0x8C,0x00,0x0C,0x60,0x80,0x30,0x60,0xC0,0x60,0x60,0x03,
0x00,0x30,0x18,0x00,0x10,0x88,0x00,0x04,0x60,0x80,0x30,0x60,0xC0,0x40,0x60,0x03,0x00,0x30,0x18,0x00,0x10,0x98,0x00,0x06,0x60,0x80,0x30,0x60,0xC0,0xC0,0x60,0x03,0x00,
0x30,0x0C,0x00,0x10,0x98,0x00,0x06,0x60,0x80,0x20,0x60,0xFF,0x80,0x60,0x03,0x00,0x30,0x07,0x00,0x10,0x98,0x00,0x06,0x60,0x80,0x60,0x60,0xFF,0xC0,0x60,0x03,0x00,0x30,
0x03,0xC0,0x10,0x98,0x00,0x06,0x60,0x81,0xC0,0x60,0xC0,0x60,0x60,0x03,0x00,0x30,0x00,0x40,0x10,0x98,0x00,0x06,0x60,0x9F,0x80,0x60,0xC0,0x30,0x60,0x03,0x00,0x30,0x00,
0x20,0x10,0x88,0x00,0x04,0x60,0x8C,0x00,0x60,0xC0,0x30,0x60,0x03,0x00,0x30,0x00,0x10,0x10,0x8C,0x00,0x0C,0x60,0x84,0x00,0x60,0xC0,0x10,0x60,0x03,0x00,0x30,0x00,0x10,
0x10,0x84,0x00,0x08,0x60,0x86,0x00,0x60,0xC0,0x10,0x60,0x03,0x00,0x30,0x00,0x10,0x10,0x86,0x00,0x18,0x60,0x83,0x00,0x60,0xC0,0x30,0x60,0x03,0x00,0x30,0x30,0x10,0x10,
0x83,0x00,0x30,0x60,0x81,0x80,0x60,0xC0,0x30,0x60,0x03,0x00,0x30,0x18,0x10,0x10,0x81,0xC0,0xE0,0x60,0x80,0xC0,0x60,0xC0,0x60,0x60,0x03,0x00,0x30,0x18,0x20,0x10,0x80,
0x7F,0x80,0x60,0x80,0x60,0x60,0xFF,0xC0,0x60,0x03,0x00,0x30,0x0F,0xE0,0x10,0x80,0x1E,0x00,0x60,0x80,0x30,0x60,0xFF,0x00,0x60,0x03,0x00,0x30,0x03,0x80,0x10,0x80,0x00,
0x00,0x60,0x00,0x00,0x60,0x00,0x00,0x60,0x00,0x00,0x30,0x00,0x00,0x10,0x80,0x00,0x00,0x60,0x00,0x00,0x60,0x00,0x00,0x60,0x00,0x00,0x30,0x00,0x00,0x10,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,};

void *Graphics_GetLogo(void){
	return((void*)&LogoOrbis);
}
