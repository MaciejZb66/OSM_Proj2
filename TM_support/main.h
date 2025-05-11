#include <stdint.h>
#include "stm32f429xx.h"
#include "stm32f4xx_gpio.h"

#define LCD_WIDTH 320U
#define LCD_HEIGHT 240U
#define LCD_LAST_X 318U
#define LCD_LAST_Y 239U

typedef enum
{
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

void WypelnijEkran(void);
void System_Init(void);
HAL_StatusTypeDef HAL_Init(void);
void SystemClock_Config(void);
void Clear_And_Reload_Screen(void);
void BSP_SDRAM_Init(void);
void BSP_LCD_Init(void);
void SysTick_Handler(void);
void HAL_IncTick(void);
