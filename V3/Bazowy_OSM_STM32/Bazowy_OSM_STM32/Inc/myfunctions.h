#pragma once

#include <stdbool.h>
#include "ll_spi_ili9341.h"

#define wykres_char 7
// #define SetPixel(Pointer,x,y) Pointer[(x>>5)+(y<<3)]|=(unsigned long)0x80000000>>(x&0x1f)
#define LCD_WIDTH   320U
#define LCD_HEIGHT  240U
#define LCD_LAST_X  318U
#define LCD_LAST_Y  239U

typedef union{
	uint16_t color;
	struct{
		uint16_t blue   :5;
		uint16_t green  :6;
		uint16_t red    :5;
	}scale;
}RGB;

typedef struct{
    int last_temps_insert;
    char os_y_name[wykres_char];
    char os_x_name[4];
    int* source_data;
    int offset;
    int last_temps[220];
}wykres_s;

void Draw_info(int real_temp, int exp_temp, bool window);
void SetPixel(int x, int y, uint16_t color);
void Draw_image(const uint16_t* img);
void wykres_init(wykres_s* wykres, char* name, int* source, int off);
void wykres_draw(wykres_s* wykres);
void wykres_show(wykres_s* wykres);
