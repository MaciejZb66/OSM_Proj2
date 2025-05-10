#include "myfunctions.h"

void Draw_info(int real_temp, int exp_temp, bool window){
    unsigned char data[]= "Temp:  .  Set:  .  Okno: ";
    if(real_temp > 500) real_temp = 500;
    data[5] = real_temp / 100 + '0';
    data[6] = ((real_temp / 10) - (real_temp / 100 * 10)) + '0';
    data[8] = real_temp % 10 + '0';
    data[14] = exp_temp / 100 + '0';
    data[15] = ((exp_temp / 10) - (exp_temp / 100 * 10)) + '0';
    data[17] = exp_temp % 10 + '0';
    if(window){
        data[24] = '1';
    }else{
        data[24] = '0';
    }
    for(int i = 0; i < 25; i++){
    	TFTDisplay_ILI9341_DrawChar(i * 6 + 5, 5, data[i]);
    }

}
