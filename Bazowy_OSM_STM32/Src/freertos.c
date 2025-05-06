/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ll_spi_ili9341.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for LCD_Task */
osThreadId_t LCD_TaskHandle;
const osThreadAttr_t LCD_Task_attributes = {
  .name = "LCD_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for LED1_task */
osThreadId_t LED1_taskHandle;
const osThreadAttr_t LED1_task_attributes = {
  .name = "LED1_task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for LED2_tack */
osThreadId_t LED2_tackHandle;
const osThreadAttr_t LED2_tack_attributes = {
  .name = "LED2_tack",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Reg_task */
osThreadId_t Reg_taskHandle;
const osThreadAttr_t Reg_task_attributes = {
  .name = "Reg_task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for Read_Peripheral */
osThreadId_t Read_PeripheralHandle;
const osThreadAttr_t Read_Peripheral_attributes = {
  .name = "Read_Peripheral",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for BinarySem01 */
osSemaphoreId_t BinarySem01Handle;
const osSemaphoreAttr_t BinarySem01_attributes = {
  .name = "BinarySem01"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
uint8_t DmaSpiCnt=1;

int x = 120, y = 160; // Skladowe x,y polozenia kwadratu
int x_old = 120, y_old = 160;
int dx = 1, dy = 1; // Skladowe wektora predkosci dx, dy

unsigned char *ptr;
unsigned char napis[] = {"Press the blue button..."};
int dl_n = 24;
unsigned int nx = 0;
unsigned int ny = 280;
unsigned int  key;

/* USER CODE END FunctionPrototypes */

void StartLCDTask(void *argument);
void StartTaskLED1(void *argument);
void StartTaskLED2(void *argument);
void Reg_task_init(void *argument);
void StartRead_Peripheral(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of BinarySem01 */
  BinarySem01Handle = osSemaphoreNew(1, 1, &BinarySem01_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of LCD_Task */
  LCD_TaskHandle = osThreadNew(StartLCDTask, NULL, &LCD_Task_attributes);

  /* creation of LED1_task */
  LED1_taskHandle = osThreadNew(StartTaskLED1, NULL, &LED1_task_attributes);

  /* creation of LED2_tack */
  LED2_tackHandle = osThreadNew(StartTaskLED2, NULL, &LED2_tack_attributes);

  /* creation of Reg_task */
  Reg_taskHandle = osThreadNew(Reg_task_init, NULL, &Reg_task_attributes);

  /* creation of Read_Peripheral */
  Read_PeripheralHandle = osThreadNew(StartRead_Peripheral, NULL, &Read_Peripheral_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartLCDTask */
/**
  * @brief  Function implementing the LCD_Task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartLCDTask */
void StartLCDTask(void *argument)
{
  /* USER CODE BEGIN StartLCDTask */
	TFTDisplay_ILI9341_Initialization(240, 320);
	  TFTDisplay_ILI9341_SetRotation(2);
//	  TFTDisplay_ILI9341_FillScreen(TFT_COLOR_ILI9341_GREEN);
//	  osDelay(500);
//	  TFTDisplay_ILI9341_FillScreen(TFT_COLOR_ILI9341_RED);
//	  osDelay(500);
	  TFTDisplay_ILI9341_FillScreen(TFT_COLOR_ILI9341_BLUE);
	  osDelay(100);
	  TFTDisplay_ILI9341_FillRect(5,5,35,35,TFT_COLOR_ILI9341_YELLOW );
	  TFTDisplay_ILI9341_DrawClearRect(40,40, 80, 80, TFT_COLOR_ILI9341_RED);
	  TFTDisplay_ILI9341_DrawCircle(140, 140, 50, TFT_COLOR_ILI9341_GREEN);


	  for (int i=0; i < dl_n; i++){
		  TFTDisplay_ILI9341_DrawChar (nx, ny, *ptr);
		  nx= nx +10;
		  ptr++;

	  }


	  TFTDisplay_ILI9341_DrawLine(0, 0, 240, 240, TFT_COLOR_ILI9341_RED);
	  TFTDisplay_ILI9341_DrawPixel(140, 140, TFT_COLOR_ILI9341_WHITE);


	while(!(GPIOA -> IDR & 0x0001)) //sprawdzanie klawisza pod PA0
		{
		osDelay(2);					//aby nie blokować innych tasków
		}

  /* Infinite loop */
  for(;;)
  {
	  for(int i=0; i< 10; i++)
	  {
		  TFTDisplay_ILI9341_DrawChar(100, 100, 0x30 + i);
		  osDelay(1000);
	  }
//	  	TFTDisplay_ILI9341_FillRect(x, y, x+30, y+30, TFT_COLOR_ILI9341_RED);

//	  	move_square_C();
//	  	TFTDisplay_ILI9341_FillRect(x_old, y_old, x_old+30, y_old+30, TFT_COLOR_ILI9341_BLUE);
//	  	x_old = x;
//	  	y_old = y;

  }
  /* USER CODE END StartLCDTask */
}

/* USER CODE BEGIN Header_StartTaskLED1 */
/**
* @brief Function implementing the LED1_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskLED1 */
void StartTaskLED1(void *argument)
{
  /* USER CODE BEGIN StartTaskLED1 */
  /* Infinite loop */
	for(;;)
	{

	  osSemaphoreAcquire(BinarySem01Handle, osWaitForever);  //synchronizacja tasków - zwolnienie w TaskLED2

	  LL_GPIO_TogglePin(GPIOG, LL_GPIO_PIN_13);


	  osDelay(100);
	}
  /* USER CODE END StartTaskLED1 */
}

/* USER CODE BEGIN Header_StartTaskLED2 */
/**
* @brief Function implementing the LED2_tack thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskLED2 */
void StartTaskLED2(void *argument)
{
  /* USER CODE BEGIN StartTaskLED2 */
  /* Infinite loop */
	for(;;)
	{
	  LL_GPIO_TogglePin(GPIOG, LL_GPIO_PIN_14);

	  osDelay(1000);
	  osSemaphoreRelease(BinarySem01Handle);	//zwolnienie semafora i odblokowanie TaskLED1


	}
  /* USER CODE END StartTaskLED2 */
}

/* USER CODE BEGIN Header_Reg_task_init */
/**
* @brief Function implementing the Reg_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Reg_task_init */
void Reg_task_init(void *argument)
{
  /* USER CODE BEGIN Reg_task_init */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Reg_task_init */
}

/* USER CODE BEGIN Header_StartRead_Peripheral */
/**
* @brief Function implementing the Read_Peripheral thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartRead_Peripheral */
void StartRead_Peripheral(void *argument)
{
  /* USER CODE BEGIN StartRead_Peripheral */
	LL_ADC_REG_StartConversion(ADC1);
  /* Infinite loop */
  for(;;)
  {
	  while (LL_ADC_IsActiveFlag_EOC(ADC1) == 0){
		  osDelay(1);
	  }
	  adc_data_pot1 = LL_ADC_REG_ReadConversionData12(ADC1);
	  while (LL_ADC_IsActiveFlag_EOC(ADC1) == 0){
		  osDelay(1);
	  }
	  adc_data_pot2 = LL_ADC_REG_ReadConversionData12(ADC1);
	  while (LL_ADC_IsActiveFlag_EOC(ADC1) == 0){
		  osDelay(1);
	  }
	  adc_data_pot3 = LL_ADC_REG_ReadConversionData12(ADC1);
	  while (LL_ADC_IsActiveFlag_EOC(ADC1) == 0){
		  osDelay(1);
	  }
	  adc_data_pot3 = LL_ADC_REG_ReadConversionData12(ADC1);

    osDelay(100);
  }
  /* USER CODE END StartRead_Peripheral */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

