/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ll_spi_ili9341.h"
#include "reg2.h"
#include "myfunctions.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
extern bool led_inited;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
PID_s pid;
Inercja_s in3;
Inercja_s in4;
wykres_s entry;
wykres_s out;
float tp = 0.1;
int err = 0;
wykres_s errdr;
//extern const uint16_t jajko[57600];
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* Definitions for LCD_Task */
osThreadId_t LCD_TaskHandle;
const osThreadAttr_t LCD_Task_attributes = {
  .name = "LCD_Task",
  .stack_size = 1664 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for myTask04 */
osThreadId_t myTask04Handle;
const osThreadAttr_t myTask04_attributes = {
  .name = "myTask04",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for myTask05 */
osThreadId_t myTask05Handle;
const osThreadAttr_t myTask05_attributes = {
  .name = "myTask05",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal1,
};
/* Definitions for LCD_Task_cd */
osThreadId_t LCD_Task_cdHandle;
const osThreadAttr_t LCD_Task_cd_attributes = {
  .name = "LCD_Task_cd",
  .stack_size = 1664 * 4,
  .priority = (osPriority_t) osPriorityLow1,
};
/* Definitions for BinarySem01 */
osSemaphoreId_t BinarySem01Handle;
const osSemaphoreAttr_t BinarySem01_attributes = {
  .name = "BinarySem01"
};
/* USER CODE BEGIN PV */
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


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_RNG_Init(void);
static void MX_SPI5_Init(void);
void StartLCDTask(void *argument);
void StartTask04(void *argument);
void StartTask05(void *argument);
void StartLCDTask_cd(void *argument);

/* USER CODE BEGIN PFP */
void move_square_C(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void move_square_C(){
//Aktualizacja po│ozenia na podstawie wektora predkosci
		x+=dx;
		y+=dy;
//Ograniczenie obszaru poruszania kwadratem w obrebie LCD
		if (x <= 1 || x >= 240 - 34)
			dx = -dx;
		if (y <= 1 || y >= 320 - 34)
			dy = -dy;
}
//#define jajko_adres 0x0800F000
//void Flash_Write(void)
//{
//    // 1. Odblokowanie Flash
//    LL_FLASH_Unlock();
//
//    // 2. Kasowanie sektora (np. sektor 11 dla STM32F407 z 512KB Flash)
//    while (LL_FLASH_IsActiveFlag_BSY());
//    LL_FLASH_Erase_Sector(LL_FLASH_SECTOR_11, LL_FLASH_CURRENTPAGE);
//    while (LL_FLASH_IsActiveFlag_BSY());
//
//    // 3. Zapis słowa (32-bit)
//    LL_FLASH_Program(LL_FLASH_PROGRAM_WORD, s, DATA_32);
//    while (LL_FLASH_IsActiveFlag_BSY());
//
//    // 4. Zablokowanie Flash
//    LL_FLASH_Lock();
//}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

	ptr = napis;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* System interrupt init*/
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* PendSV_IRQn interrupt configuration */
  NVIC_SetPriority(PendSV_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));
  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_RNG_Init();
  MX_SPI5_Init();
  /* USER CODE BEGIN 2 */
  LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_4);
  LL_DMA_ClearFlag_TC4(DMA2);
  LL_DMA_ClearFlag_TE4(DMA2);
  LL_SPI_EnableDMAReq_TX(SPI5);
  LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_4);
  LL_DMA_EnableIT_TE(DMA2, LL_DMA_STREAM_4);
  LL_SPI_Enable(SPI5);


  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

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

  /* creation of myTask04 */
  myTask04Handle = osThreadNew(StartTask04, NULL, &myTask04_attributes);

  /* creation of myTask05 */
  myTask05Handle = osThreadNew(StartTask05, NULL, &myTask05_attributes);

  /* creation of LCD_Task_cd */
  LCD_Task_cdHandle = osThreadNew(StartLCDTask_cd, NULL, &LCD_Task_cd_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_5)
  {
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  LL_PWR_EnableOverDriveMode();
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 180, LL_RCC_PLLP_DIV_2);
  LL_RCC_PLL_ConfigDomain_48M(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 180, LL_RCC_PLLQ_DIV_8);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_Init1msTick(180000000);
  LL_SetSystemCoreClock(180000000);
  LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_TWICE);
}

/**
  * @brief RNG Initialization Function
  * @param None
  * @retval None
  */
static void MX_RNG_Init(void)
{

  /* USER CODE BEGIN RNG_Init 0 */

  /* USER CODE END RNG_Init 0 */

  /* Peripheral clock enable */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_RNG);

  /* USER CODE BEGIN RNG_Init 1 */

  /* USER CODE END RNG_Init 1 */
  LL_RNG_Enable(RNG);
  /* USER CODE BEGIN RNG_Init 2 */

  /* USER CODE END RNG_Init 2 */

}

/**
  * @brief SPI5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI5_Init(void)
{

  /* USER CODE BEGIN SPI5_Init 0 */

  /* USER CODE END SPI5_Init 0 */

  LL_SPI_InitTypeDef SPI_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI5);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
  /**SPI5 GPIO Configuration
  PF7   ------> SPI5_SCK
  PF8   ------> SPI5_MISO
  PF9   ------> SPI5_MOSI
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_7|LL_GPIO_PIN_8|LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
  LL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /* SPI5 DMA Init */

  /* SPI5_TX Init */
  LL_DMA_SetChannelSelection(DMA2, LL_DMA_STREAM_4, LL_DMA_CHANNEL_2);

  LL_DMA_SetDataTransferDirection(DMA2, LL_DMA_STREAM_4, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

  LL_DMA_SetStreamPriorityLevel(DMA2, LL_DMA_STREAM_4, LL_DMA_PRIORITY_LOW);

  LL_DMA_SetMode(DMA2, LL_DMA_STREAM_4, LL_DMA_MODE_CIRCULAR);

  LL_DMA_SetPeriphIncMode(DMA2, LL_DMA_STREAM_4, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA2, LL_DMA_STREAM_4, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA2, LL_DMA_STREAM_4, LL_DMA_PDATAALIGN_BYTE);

  LL_DMA_SetMemorySize(DMA2, LL_DMA_STREAM_4, LL_DMA_MDATAALIGN_BYTE);

  LL_DMA_DisableFifoMode(DMA2, LL_DMA_STREAM_4);

  /* USER CODE BEGIN SPI5_Init 1 */

  /* USER CODE END SPI5_Init 1 */
  /* SPI5 parameter configuration*/
  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
  SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
  SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
  SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
  SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV2;
  SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
  SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.CRCPoly = 10;
  LL_SPI_Init(SPI5, &SPI_InitStruct);
  LL_SPI_SetStandard(SPI5, LL_SPI_PROTOCOL_MOTOROLA);
  /* USER CODE BEGIN SPI5_Init 2 */

  /* USER CODE END SPI5_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* Init with LL driver */
  /* DMA controller clock enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2);

  /* DMA interrupt init */
  /* DMA2_Stream4_IRQn interrupt configuration */
  NVIC_SetPriority(DMA2_Stream4_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5, 0));
  NVIC_EnableIRQ(DMA2_Stream4_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOH);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOG);

  /**/
  LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_2);

  /**/
  LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_12|LL_GPIO_PIN_13);

  /**/
  LL_GPIO_ResetOutputPin(GPIOG, LL_GPIO_PIN_13|LL_GPIO_PIN_14);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_13|LL_GPIO_PIN_11|LL_GPIO_PIN_12;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_12|LL_GPIO_PIN_13;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_13|LL_GPIO_PIN_14;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOG, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void DMA1_Stream4_TransferComplete(void)
{
  LL_DMA_ClearFlag_TC4(DMA2);
  DmaSpiCnt--;

  if(DmaSpiCnt == 0)
  {
    LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_4);
    DmaSpiCnt=1;
    CS_DESELECT();
  }
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartLCDTask */
/**
  * @brief  Function implementing the LCD_Task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartLCDTask */
void StartLCDTask(void *argument)
{
  /* USER CODE BEGIN 5 */
	TFTDisplay_ILI9341_Initialization(240, 320);
    TFTDisplay_ILI9341_SetRotation(2);
    led_inited = true;


	  TFTDisplay_ILI9341_DrawLine(0, 20, 240, 20, TFT_COLOR_ILI9341_RED);


	while(!(GPIOA -> IDR & 0x0001)) //sprawdzanie klawisza pod PA0
	{
		osDelay(2);					//aby nie blokować innych tasków
	}

	/* Infinite loop */
	for(;;)
	{
//		for(int i=0; i< 10; i++)
//		  {
//			if(i < 3){
//				Draw_image3(jajko1, 0);
//				osDelay(100);
//			}else if(i<6){
//				Draw_image3(jajko2, 1);
//				osDelay(100);
//			}else{
////				Draw_image3(jajko3, 2);
//				osDelay(100);
//			}
//			  Draw_image2(test);


//			  Draw_image3(jajko3, 2);
//			  Draw_image3(jajko4, 3);
//			  Draw_image3(jajko5, 4);

			  // TFTDisplay_ILI9341_DrawChar(100, 100, 0x30 + i);
//			   TFTDisplay_ILI9341_DrawChar(100, 120, 0x31); //według fizyki czarnej dziury tu to działa
//			  int real_temp = (int)output;
//			  int exp_temp = (int)expected;
//			  bool window = false;
//			  Draw_info((int)in4.output * 10, (int)pid.expected * 10, true);
//		TFTDisplay_ILI9341_FillRect(2, 71, 230, 140, 0x001F);
//		osDelay(100);
//		TFTDisplay_ILI9341_DrawLine(2, 71, 230, 140, 0x001F);

		osDelay(100);
//		  }

	}
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartTask04 */
/**
* @brief Function implementing the myTask04 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask04 */
void StartTask04(void *argument)
{
  /* USER CODE BEGIN StartTask04 */

  /* Infinite loop */
  for(;;)
  {
	    if(!(GPIOC -> IDR & 0x0800)){
	    	if(pid.expected < 50){
	    		pid.expected += 0.1;
	    		pid.expected_i = (int)(10 * pid.expected);
	    	}
	        TFTDisplay_ILI9341_DrawChar(100, 150, 0x31);
	        osDelay(40);
	    }
	    if(!(GPIOC -> IDR & 0x1000)){
	    	if(pid.expected >0){
	    		pid.expected -= 0.1;
	    		pid.expected_i = (int)(10 * pid.expected);
	    	}

	        TFTDisplay_ILI9341_DrawChar(100, 150, 0x32);
	        osDelay(40);
	    }
	    if(!(GPIOC -> IDR & 0x2000)){
	    	TFTDisplay_ILI9341_DrawChar(100, 150, 0x30);
//	    	TFTDisplay_ILI9341_DrawChar(100, 120, 0x31); //według fizyki czarnej dziury tu to działa
	    }
//	    if (lcdprop.pFont != NULL && lcdprop.pFont->table != NULL) { //chat idea
//	        TFTDisplay_ILI9341_DrawChar(100, 120, 0x31);
//	    }
//		TFTDisplay_ILI9341_DrawChar(100, 120, 0x31); //według fizyki czarnej dziury tu to nie działa

	    osDelay(10);



  }
  /* USER CODE END StartTask04 */
}

/* USER CODE BEGIN Header_StartTask05 */
/**
* @brief Function implementing the myTask05 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask05 */
void StartTask05(void *argument)
{
  /* USER CODE BEGIN StartTask05 */
	   // PID_s_init(&pid, 10, 3, 0.9, tp);
	    PID_s_init(&pid, 6.0, 5.6, 0.8, tp);
	    Inercja_s_init(&in3, 1, tp, 5);
	    Inercja_s_init(&in4, 2, tp, 10);
	    pid.expected = 18;
	    in3.last_input = 17.5;
	    in4.last_input = 17.5;
	    in4.output = 17.5;
	  /* Infinite loop */
	  for(;;)
	  {
//		printf("%d.\t", i);
		pid.input = in4.output;
		Reg_s_step(&pid);
		//output = pid.output;
		in3.input = pid.output;
		Inercja_s_step(&in3);
		in4.input = in3.output;
		Inercja_s_step(&in4);
		Draw_info((int)(in4.output * 10), (int)(pid.expected * 10), false);
		err = abs(pid.expected_i - in4.output_i);
		wykres_draw(&entry);
		wykres_draw(&out);
		wykres_draw(&errdr);
//	    Draw_info(104, 152, false);

		osDelay(50);

	  }
  /* USER CODE END StartTask05 */
}

/* USER CODE BEGIN Header_StartLCDTask_cd */
/**
* @brief Function implementing the LCD_Task_cd thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLCDTask_cd */
void StartLCDTask_cd(void *argument)
{
  /* USER CODE BEGIN StartLCDTask_cd */
	wykres_init(&entry, "in", &pid.expected_i, 0, 0xF800);
	wykres_init(&out, "out", &in4.output_i, 0, 0x001F);
	wykres_init(&errdr, "err", &err, 0, 0x07E0);
  /* Infinite loop */
  for(;;)
  {
		if(in4.output_i < 83){
			TFTDisplay_ILI9341_FillRect(100, 280, 140, 317, TFT_COLOR_ILI9341_CLAY);
			TFTDisplay_ILI9341_FillRect(80, 280, 160, 300, TFT_COLOR_ILI9341_CLAY);
		}else if(in4.output_i < 166){
			TFTDisplay_ILI9341_FillRect(100, 280, 140, 317, TFT_COLOR_ILI9341_CLAY);
			TFTDisplay_ILI9341_FillRect(80, 280, 160, 300, TFT_COLOR_ILI9341_CLAY);
			TFTDisplay_ILI9341_FillCircle(120, 280, 6, TFT_COLOR_ILI9341_GREEN);
		}else if (in4.output_i < 250){
			TFTDisplay_ILI9341_FillRect(100, 280, 140, 317, TFT_COLOR_ILI9341_GREEN);
			TFTDisplay_ILI9341_FillRect(100, 280, 140, 317, TFT_COLOR_ILI9341_CLAY);
			TFTDisplay_ILI9341_FillRect(115, 240, 125, 280, TFT_COLOR_ILI9341_CLAY);
		}else if(in4.output_i < 333){
			TFTDisplay_ILI9341_FillRect(100, 280, 140, 317, TFT_COLOR_ILI9341_CLAY);
			TFTDisplay_ILI9341_FillRect(80, 280, 160, 300, TFT_COLOR_ILI9341_CLAY);
			TFTDisplay_ILI9341_FillRect(115, 230, 125, 280, TFT_COLOR_ILI9341_GREEN);
			TFTDisplay_ILI9341_FillCircle(120, 215, 15, TFT_COLOR_ILI9341_GREENYELLOW);
		}else if(in4.output_i < 416){
			TFTDisplay_ILI9341_FillRect(100, 280, 140, 317, TFT_COLOR_ILI9341_CLAY);
			TFTDisplay_ILI9341_FillRect(80, 280, 160, 300, TFT_COLOR_ILI9341_CLAY);
			TFTDisplay_ILI9341_FillRect(117, 215, 123, 279, TFT_COLOR_ILI9341_GREEN);
			TFTDisplay_ILI9341_FillCircle(120, 200, 30, TFT_COLOR_ILI9341_GREENYELLOW);
		}else{
			TFTDisplay_ILI9341_FillRect(100, 280, 140, 317, TFT_COLOR_ILI9341_CLAY);
			TFTDisplay_ILI9341_FillRect(80, 280, 160, 300, TFT_COLOR_ILI9341_CLAY);
		}
	TFTDisplay_ILI9341_FillRect(20, 21, 238, 119, 0xFFFF);
	wykres_show(&entry);
	wykres_show(&out);
	wykres_show(&errdr);
//	TFTDisplay_ILI9341_FillRect(100, 260, 140, 300, TFT_COLOR_ILI9341_PURPLE);

    osDelay(30);
  }
  /* USER CODE END StartLCDTask_cd */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
