#include "main.h"
unsigned short int *const LCD_FB = (unsigned short int*) 0xD0000000;
unsigned short int *const LCD_BUF = (unsigned short int*) 0xD0100000;

typedef union{
	unsigned short int color;
	struct{
		uint16_t blue :5;
		uint16_t green :6;
		uint16_t red :5;
	}scale;
}RGB;

void WypelnijEkran(void){
	RGB buf = {0};
	if (!(GPIOA->IDR & 1))	{
		for(unsigned int y = 0; y < LCD_HEIGHT; y++){
			for(unsigned int x = 0; x < LCD_WIDTH; x++){
				buf.scale.red = x / 10;
				LCD_BUF[y*LCD_WIDTH+x]= buf.color;
			}
		}
	}else{
		for(unsigned int y = 0; y < LCD_HEIGHT; y++){
			for(unsigned int x = 0; x < LCD_WIDTH; x++){
				buf.scale.red = (LCD_WIDTH - x) / 10;
				LCD_BUF[y*LCD_WIDTH+x]= buf.color;
			}
		}
	}
}

int main(void) {
	System_Init();
	while (1) {
		WypelnijEkran();
		Clear_And_Reload_Screen();
	}
}


void SysTick_Handler(void) {
	HAL_IncTick();
}

void System_Init() {
	HAL_Init();
	SystemClock_Config();
	BSP_SDRAM_Init();
	BSP_LCD_Init();

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
	GPIOG->MODER |= GPIO_Mode_OUT << (14 * 2);
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= GPIO_Mode_IN;
}


void Clear_And_Reload_Screen() {
	for (int off = 0; off < 320*240; off++) {
		LCD_FB[off]=LCD_BUF[off];
		LCD_BUF[off]=0;
    }
}
