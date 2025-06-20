#include "myfunctions.h"
#include "ll_spi_ili9341.h"

unsigned short int *const LCD_FB = (unsigned short int*) 0xD0000000;
unsigned short int *const LCD_BUF = (unsigned short int*) 0xD0100000;

void Draw_info(int real_temp, int exp_temp, bool window){
    unsigned char data[]= "Temp:  .  Set:  .  Okno";
    if(real_temp > 500) real_temp = 500;
    data[5] = real_temp / 100 + '0';
    data[6] = ((real_temp / 10) - (real_temp / 100 * 10)) + '0';
    data[8] = real_temp % 10 + '0';
    data[14] = exp_temp / 100 + '0';
    data[15] = ((exp_temp / 10) - (exp_temp / 100 * 10)) + '0';
    data[17] = exp_temp % 10 + '0';

    for(int i = 0; i < 23; i++){
    	TFTDisplay_ILI9341_DrawChar(i * 10, 2, data[i]); //nie działa
    }
    if(!window){
        TFTDisplay_ILI9341_DrawLine(190, 9, 230, 9, 0x07E0);
        TFTDisplay_ILI9341_DrawLine(190, 8, 230, 8, 0x07E0);
        TFTDisplay_ILI9341_DrawLine(190, 7, 230, 7, 0x07E0);
    }
}

void SetPixel(int x, int y, uint16_t color){	
	TFTDisplay_ILI9341_DrawPixel(x, y, color);
}

void Draw_image(const uint16_t* img){
    for(int y = 80; y < 320; y++){
        for(int x = 0; x < 240; x++){
            TFTDisplay_ILI9341_DrawPixel(x, y, img[240 * (y -80) + x]);
        }
    }
}

void Draw_image2(const uint16_t* img){
    for(int y = 80; y < 160; y++){
        for(int x = 0; x < 80; x++){
            TFTDisplay_ILI9341_DrawPixel(x, y, img[80 * (y -80) + x]);
        }
    }
}

void Draw_image3(const uint16_t* img, int offset){
	int start = 48 * offset + 80;
    for(int y = start; y < start + 48; y++){
        for(int x = 0; x < 240; x++){
            TFTDisplay_ILI9341_DrawPixel(x, y, img[240 * (y -80) + x]);
        }
    }
}

void wykres_init(wykres_s* wykres, char* name, int* source, int off, uint16_t color){
    for(int i = 0; i < wykres_char; i++){
        wykres->os_y_name[i] = name[i];
        if(sizeof(name) + 1 < i){
            wykres->os_y_name[i] = 0x20;
        }
    }
    wykres->os_x_name[0] = 0x74;
    wykres->os_x_name[1] = 0x69;
    wykres->os_x_name[2] = 0x6d;
    wykres->os_x_name[3] = 0x65;
    wykres->last_temps_insert = 0;
    wykres->source_data = source;
    wykres->offset = off;
    wykres->color = color;
}

void wykres_draw(wykres_s* wykres){
    if(wykres->last_temps_insert <219){
        wykres->last_temps[wykres->last_temps_insert] = *wykres->source_data;
        wykres->last_temps_insert++;
    }else{
        for(int i = 0 ;i < 219;i++){
            wykres->last_temps[i] = wykres->last_temps[i+1];
        }
        wykres->last_temps[219] = *wykres->source_data;
    }
}

void wykres_show(wykres_s* wykres){
//	TFTDisplay_ILI9341_FillScreen(0x001F);
    //data
    for(int i = 0; i< 220; i++){
        // TFTDisplay_ILI9341_DrawPixel(140, 140, TFT_COLOR_ILI9341_WHITE);
        SetPixel(i+19, ((500-wykres->last_temps[i] - (10 * wykres->offset))/5)+20, wykres->color);
        SetPixel(i+19, 120, 0xF410);
        if (i < 100){
            SetPixel(19, i+20, 0xF410);
        }
    }
    //improve after test
    // //value 50
    // for(int i=17; i<22;i++){
    //     SetPixel(ekran,i,20);
    // }
    // if(offset == 20){
    //     textEkran[40*2+1]=0x33;
    // }else{
    //     textEkran[40*2+1]=0x35;
    // }        
    // textEkran[40*2+2]=0x30;
    // //value 20
    // for(int i=17; i<22;i++){
    //     SetPixel(ekran,i,80);
    // }
    // if(offset == 20){
    //     textEkran[40*10+2]=0x30;
    // }else{
    //     textEkran[40*10+1]=0x32;
    //     textEkran[40*10+2]=0x30;
    // }          
    // //value 10
    // for(int i=17; i<22;i++){
    //     SetPixel(ekran,i,100);
    // }
    // if(offset == 20){
    //     textEkran[40*12]=0x2D;
    //     textEkran[40*12+1]=0x31;
    //     textEkran[40*12+2]=0x30;
    // }else{
    //     textEkran[40*12+1]=0x31;
    //     textEkran[40*12+2]=0x30;
    // }  
    
    // //y name
    // for(int i = 0; i < wykres_char; i++){
    //     textEkran[40*(i+4)]=os_y_name[i];
    // }
    // //x name
    // TFTDisplay_ILI9341_DrawChar(100, 100, 0x30 + i);
    // // PrintText(textEkran,os_x_name,4,20,15);

}
