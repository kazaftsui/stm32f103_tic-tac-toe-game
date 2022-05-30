/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcdtp.h"
#include "xpt2046.h"
#include "bot.h"
#include <stdlib.h>
#include <time.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define box1 gameboard[0][0]
#define box2 gameboard[0][1]
#define box3 gameboard[0][2]
#define box4 gameboard[1][0]
#define box5 gameboard[1][1]
#define box6 gameboard[1][2]
#define box7 gameboard[2][0]
#define box8 gameboard[2][1]
#define box9 gameboard[2][2]
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim3;

SRAM_HandleTypeDef hsram1;

/* USER CODE BEGIN PV */
uint16_t x1=0,x2=0,x3=0,y1=0,y2=0,y3=0;
int game_round = 1,game_status = -1;
int region = 0;
int At_menu = 1;
int mode = -1;  //-1 = not yet selected, 1 = PVP, 2 = PVE
char gameboard[3][3] = { {'1','2','3'}, 
                         {'4','5','6'}, 
                         {'7','8','9'}};
int random = -1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_FSMC_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */
void lock_game_board();												 
int checkGameEnd(int round);
void initialize_game();
void initialize_menu();		
void check_LED(int gameround);
void counter_return(int count_time);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FSMC_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start(&htim3);
	
	LCD_INIT();
	initialize_menu();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		int count_time=__HAL_TIM_GET_COUNTER(&htim3);
		char debug_count[5];
		if (At_menu == 0 && game_status == -1){
				counter_return(count_time);
			if (count_time <= 201) {
				LCD_DrawString(0,0,"TIME'S UP!!!",BLACK);
				lock_game_board();
				//__HAL_TIM_SET_COUNTER(&htim3, 0);
				counter_return(0);
				game_status = 3;
				continue;
			}
		}
		if (mode == 2 && game_round == 1 && At_menu == 0)
		{
			int random_chest = rand() % 9 + 1;
			draw_chest(game_round, random_chest);
			gameboard[(random_chest-1)/3][(random_chest-1)%3] = (game_round%2==1)? 'O': 'X';
			game_round++;
			check_LED(game_round);
			
			continue;
		}
		if (XPT2046_TouchPressed ()) 
	 
		{
			uint16_t x = 0 , y = 0 ;
			if (XPT2046_TouchGetCoordinates ( & x, & y)) 
			{
				y=300-y;
				if (At_menu == 1){
					if (x >= 16 && x <= 224 && y >= 54 && y <= 124 && mode != -1)
					{
						initialize_game();
						continue;
					}
					else if (x >= 16 && x <= 224 && y >= 165 && y <= 205) //PVP
					{
						LCD_DrawString(108,181,"PvP",GREEN);
						LCD_DrawString(108,251,"PvE",BLACK);
						mode = 1;
						continue;
						
					} 
					else if (x >= 16 && x <= 224 && y >= 235 && y <= 275) //PVE
					{
						LCD_DrawString(108,251,"PvE",GREEN);
						LCD_DrawString(108,181,"PvP",BLACK);
						mode = 2;
						continue;
					}
					else
						continue;
				}
				if (x >= 170 && x <=240 && y >=0 && y <= 48)
				{
					initialize_menu();
					continue;
				}
				
				x1=x2=x3=y1=y2=y3=0;
				 if(x<=80)
					 x1=1;
				 else if(x<=160)
					 x2=1;
				 else
					 x3=1;
				 if(y<=105)
					 y1=1;
				 else if(y<=210)
					 y2=1;
				 else
					 y3=1;
				
				if(x1==1 && y1==1)
				{
					region = 1;
				}
				else if(x2==1 && y1==1)
				{
					region = 2;
				}
				else if(x3==1 && y1==1)
				{
					region = 3;
				}
				else if(x1==1 && y2==1)
				{
					region = 4;
				}			
				else if(x2==1 && y2==1)
				{
					region = 5;
				}			
				else if(x3==1 && y2==1)
				{
					region = 6;
				}			
				else if(x1==1 && y3==1)
				{
					region = 7;
				}			
				else if(x2==1 && y3==1)
				{
					region = 8;
				}			
				else if(x3==1 && y3==1)
				{
					region = 9;
				}

				int row = -1, col = -1;
				row=(region-1)/3;
				col=(region-1)%3;
				
				char debugmsg[50];
				int proper_move = gameboard[row][col] - 48;
				if (proper_move >9)
					continue;
				
				draw_chest(game_round, region);
				gameboard[row][col] = (game_round%2==1)? 'O': 'X';

				
				if (gameboard[row][col] == 'X')
				{
					HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0, GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1, GPIO_PIN_SET);
				}
				else
				{
					HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5, GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0, GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1, GPIO_PIN_RESET);
				}
				game_status = checkGameEnd(game_round);
				switch (game_status)
				{
					case -1:
						break;
					case 0:
						LCD_DrawString(0,0,"PLAYERO WIN!!!",BLACK);
						lock_game_board();
						continue;
						break;
					case 1:
						LCD_DrawString(0,0,"PLAYERX WIN!!!",BLACK);
						lock_game_board();
						continue;
						break;
					case 2:
						LCD_DrawString(0,0,"TIE!!!",BLACK);
						lock_game_board();
						continue;
						break;
				}
				
				if (mode == 2)
				{
					game_round++;

					struct Move move = findBestMove(gameboard);
					int best_row = move.row;
					int best_col = move.col;
					
					int region = best_row*3 + best_col + 1;

					draw_chest(game_round, region);
					gameboard[best_row][best_col] = (game_round%2==1)? 'O': 'X';
					
					game_status = checkGameEnd(game_round);
					switch (game_status)
					{
						case -1:
							break;
						case 0:
							LCD_DrawString(0,0,"PLAYERO WIN!!!",BLACK);
							lock_game_board();
							continue;
							break;
						case 1:
							LCD_DrawString(0,0,"PLAYERX WIN!!!",BLACK);
							lock_game_board();
							continue;
							break;
						case 2:
							LCD_DrawString(0,0,"TIE!!!",BLACK);
							lock_game_board();
							continue;
							break;
					}
				}
				game_round++;
				check_LED(game_round);
				__HAL_TIM_SET_COUNTER(&htim3, 49999);
				
			}
   } 
			HAL_Delay(5);
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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_PLLCLK, RCC_MCODIV_1);
}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 7199;
  htim3.Init.CounterMode = TIM_COUNTERMODE_DOWN;
  htim3.Init.Period = 49999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2|GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_5, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_SET);

  /*Configure GPIO pins : PE2 PE0 PE1 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PE3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PE4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PA4 PA5 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PD12 PD13 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PC6 PC7 PC8 PC9 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

}

/* FSMC initialization function */
static void MX_FSMC_Init(void)
{

  /* USER CODE BEGIN FSMC_Init 0 */

  /* USER CODE END FSMC_Init 0 */

  FSMC_NORSRAM_TimingTypeDef Timing = {0};

  /* USER CODE BEGIN FSMC_Init 1 */

  /* USER CODE END FSMC_Init 1 */

  /** Perform the SRAM1 memory initialization sequence
  */
  hsram1.Instance = FSMC_NORSRAM_DEVICE;
  hsram1.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram1.Init */
  hsram1.Init.NSBank = FSMC_NORSRAM_BANK1;
  hsram1.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
  hsram1.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
  hsram1.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
  hsram1.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
  hsram1.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram1.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
  hsram1.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
  hsram1.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
  hsram1.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
  hsram1.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
  hsram1.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram1.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
  /* Timing */
  Timing.AddressSetupTime = 15;
  Timing.AddressHoldTime = 15;
  Timing.DataSetupTime = 255;
  Timing.BusTurnAroundDuration = 15;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FSMC_ACCESS_MODE_A;
  /* ExtTiming */

  if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
  {
    Error_Handler( );
  }

  /** Disconnect NADV
  */

  __HAL_AFIO_FSMCNADV_DISCONNECTED();

  /* USER CODE BEGIN FSMC_Init 2 */

  /* USER CODE END FSMC_Init 2 */
}

/* USER CODE BEGIN 4 */
void lock_game_board()
{
		for (int i = 0; i< 3; i++)
		{
			for (int j = 0; j <3; j++)
			{
				gameboard[i][j] = ':';
			}
		}
		HAL_TIM_Base_Stop(&htim3);
}
void initialize_menu()
{
		LCD_Clear(0,0,240,320,BACKGROUND);
		At_menu = 1;
		mode = -1;
	
		//start button boarder
		LCD_DrawLine(16,54,224,54,BLACK);
		LCD_DrawLine(16,124,224,124,BLACK);
		LCD_DrawLine(16,54,16,124,BLACK);
		LCD_DrawLine(224,54,224,124,BLACK);
		LCD_DrawString(100,85,"START",BLACK);
	
		//Draw the PVP button
		LCD_DrawLine(16,165,224,165,BLACK);
		LCD_DrawLine(16,205,224,205,BLACK);
		LCD_DrawLine(16,165,16,205,BLACK);
		LCD_DrawLine(224,165,224,205,BLACK);
		LCD_DrawString(108,181,"PvP",BLACK);
	
		//Draw the PVE button
		LCD_DrawLine(16,235,224,235,BLACK);
		LCD_DrawLine(16,275,224,275,BLACK);
		LCD_DrawLine(16,235,16,275,BLACK);
		LCD_DrawLine(224,235,224,275,BLACK);
		LCD_DrawString(108,251,"PvE",BLACK);
		
		//turn off the light
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
		
		//turn off the 7-segment
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);
}
void initialize_game()
{
	LCD_Clear(0,0,240,320,BACKGROUND);
	
	game_round = 1, game_status = -1;
	At_menu = 0;
	for (int i = 0; i< 3; i++)
	{
		for (int j = 0; j <3; j++)
		{
			gameboard[i][j] = i*3 + j + 1 + 48;
		}
	}
	
	//game boarder
	LCD_DrawLine(85,66,85,265,BLACK);
	LCD_DrawLine(155,66,155,265,BLACK);
	LCD_DrawLine(16,124,224,124,BLACK);
	LCD_DrawLine(16,194,224,194,BLACK);
	
	//back button
	LCD_DrawLine(170,0,170,48,BLACK);
	LCD_DrawLine(170,48,240,48,BLACK);
	LCD_DrawString(189, 20, "Back",BLACK);
	
	__HAL_TIM_SET_COUNTER(&htim3, 49999);
	
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET); //set red on
	
	
	HAL_TIM_Base_Start(&htim3);
}
//return -1 means game not end yet
//return 1 means playerX won
//return 0 means playerO won
//return 2 means draw
int checkGameEnd(const int round)
{
    int game_end = 0;
    if (box1 == box2 && box2 == box3){
        game_end = 1;
    }
    else if (box4 == box5 && box5 == box6){
        game_end = 1;
    }
    else if (box7 == box8 && box8 == box9){
        game_end = 1;
    }
    else if (box1 == box4 && box4 == box7){
        game_end = 1;
    }
    else if (box2 == box5 && box5 == box8){
        game_end = 1;
    }
    else if (box3 == box6 && box6 == box9){
        game_end = 1;
    }
    else if (box1 == box5 && box5 == box9){
        game_end = 1;
    }
    else if (box3 == box5 && box5 == box7){
        game_end = 1;
    }
    else if (round == 9){
        return 2;
    }
    else if (game_end == 0){
        return -1;
    }

    if (round % 2 == 0){
        return 1;
    }
    else{
        return 0;
    }
}
void check_LED(int gameround){
	if (gameround%2==1)
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1, GPIO_PIN_RESET);
	}
}
void counter_return(int count_time){
	if (count_time == 0){
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);
			return;
	}
	count_time=count_time/10000+1;
	switch(count_time){
		case 5:
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET);
			break;
		case 4:
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET);
			break;
		case 3:
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET);
			break;
		case 2:
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET);
		  break;
		case 1:
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);
			break;
		case 0:
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);
			break;
	}
}
/*
void  HAL_GPIO_EXTI_Callback ( uint16_t GPIO_Pin) 
{
	
		if (GPIO_Pin == EXTI4_IRQn) 
 { if (XPT2046_TouchPressed ()) 
	 
  { uint16_t x = 0 , y = 0 ;
   if (XPT2046_TouchGetCoordinates ( & x, & y)) 
    //lcdFillCircle (lcdGetWidth () - x), y, 2 , COLOR_GREENYELLOW); 
	   LCD_DrawString(50,50,"Hello",BLACK);
   } 
  } 
 }
*/
/* USER CODE END 4 */

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
