/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdbool.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t code[] ={
		0x3F,0x06,0x5B,0x4F,
		0x66,0x6D,0x7D,0x07,
		0x7F,0x67,0x77,0x7C,
		0x39,0x5E,0x79,0x71,
		0x40,0x00
};
int previous_numbers[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int counter_numbers[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};    //Debouncing

void set_segment_direct(uint8_t segs){
	HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, segs & (1<<0) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, segs & (1<<1) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, segs & (1<<2) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, segs & (1<<3) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SEG4_GPIO_Port, SEG4_Pin, segs & (1<<4) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SEG5_GPIO_Port, SEG5_Pin, segs & (1<<5) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SEG6_GPIO_Port, SEG6_Pin, segs & (1<<6) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SEG7_GPIO_Port, SEG7_Pin, segs & (1<<7) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
void display(int number) {
	if(number>=18)return;
	set_segment_direct(code[number]);
}
void set_digit(int8_t which)
{
  HAL_GPIO_WritePin(DIG0_GPIO_Port, DIG0_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DIG1_GPIO_Port, DIG1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DIG2_GPIO_Port, DIG2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(DIG3_GPIO_Port, DIG3_Pin, GPIO_PIN_RESET);
  switch (which)
  {
  case 0:HAL_GPIO_WritePin(DIG0_GPIO_Port, DIG0_Pin, GPIO_PIN_SET);break;
  case 1:HAL_GPIO_WritePin(DIG1_GPIO_Port, DIG1_Pin, GPIO_PIN_SET);break;
  case 2:HAL_GPIO_WritePin(DIG2_GPIO_Port, DIG2_Pin, GPIO_PIN_SET);break;
  case 3:HAL_GPIO_WritePin(DIG3_GPIO_Port, DIG3_Pin, GPIO_PIN_SET);break;
  }
}
void set_keyboard_row(int8_t which)
{
  HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(SEG4_GPIO_Port, SEG4_Pin, GPIO_PIN_RESET);
  switch (which)
  {
  case 0:HAL_GPIO_WritePin(SEG0_GPIO_Port, SEG0_Pin, GPIO_PIN_SET);break;
  case 1:HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, GPIO_PIN_SET);break;
  case 2:HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, GPIO_PIN_SET);break;
  case 3:HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, GPIO_PIN_SET);break;
  case 4:HAL_GPIO_WritePin(SEG4_GPIO_Port, SEG4_Pin, GPIO_PIN_SET);break;
  }
}
int ReadKeyboard(){
	for(int row=0 ;row<5;row++){
		set_keyboard_row(row);
  	int a = HAL_GPIO_ReadPin(KC0_GPIO_Port, KC0_Pin);
  	int b = HAL_GPIO_ReadPin(KC1_GPIO_Port, KC1_Pin);
  	int c = HAL_GPIO_ReadPin(KC2_GPIO_Port, KC2_Pin);
  	int aa = row*3+1;
  	int bb = row*3+2;
  	int cc = row*3+3;

  	if(a != previous_numbers[aa]){
  		counter_numbers[aa]+=1;
  		if(counter_numbers[aa] == 10){
  			previous_numbers[aa] = a;
  				if(a)return aa;
  		}
  	}else counter_numbers[aa]=0;

  	if(b != previous_numbers[bb]){
  		counter_numbers[bb]+=1;
  		if(counter_numbers[bb] == 10){
  			previous_numbers[bb] = b;
  				if(b)return bb;
  		}
  	}else counter_numbers[bb]=0;

  	if(c != previous_numbers[cc]){
  		counter_numbers[cc]+=1;
  		if(counter_numbers[cc] == 10){
  			previous_numbers[cc] = c;
  				if(c)return cc;
  		}
  	}else counter_numbers[cc]=0;
	}
	// 16th button pullup mode
	int temp = HAL_GPIO_ReadPin(SW3_GPIO_Port, SW3_Pin);
	if(temp != previous_numbers[16]){
		counter_numbers[16]+=1;
		if(counter_numbers[16]==10){
			previous_numbers[16] = temp;
			if(!temp)return 16;
		}
	}else counter_numbers[16] = 0;

	return -1;
}

void rezero(uint8_t *numbers){
	numbers[0]=0;
	numbers[1]=17;
	numbers[2]=17;
	numbers[3]=17;
}
void num_to_array(int num, uint8_t *numbers){
	if(!num)rezero(numbers);
	int count = 0;
	while(num){
		numbers[count] = num %10;
		num = num /10;
		count++;
	}
	for(count;count<=3;count++){
		numbers[count]=17;
	}
}
int power(int n){
	int res = 1;
	while(n){
		res*=10;
		n--;
	}
	return res;
}
int array_to_num(uint8_t *numbers){
	int i = 0;
	int result = 0;
	while(numbers[i]!=17 && i<4){
		result += power(i)*numbers[i];
		i++;
	}
	return result;
}
int operand_1 = 0;
int operand_2 = 0;
int oper = 0;
int result = 0;
bool is_start = true;
bool can_calculate = false;

void init_all(){
	operand_1 = 0;
	operand_2 = 0;
	oper=0;
	result = 0;
	is_start = true;
	can_calculate = false;
}
void display_4seg(int period, int delay, uint8_t *numbers){
	set_digit(-1);
	display(numbers[period]);
	set_digit(period);
	HAL_Delay(delay);
}
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
  /* USER CODE BEGIN 2 */
  int delay =0;
  int period = 0;

  uint8_t numbers[]={0,17,17,17};

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  	if(period < 4){
  		display_4seg(period, delay, numbers);
  	}
  	else{
  	set_digit(-1);
  	int read = ReadKeyboard();
  	if(read != -1){

  		if((1<=read && read<=9 )|| read ==11){
				if(numbers[3]!=17){HAL_Delay(499);rezero(numbers);init_all();}
				else{
					for(int i = 2;i>=0;i--){
						numbers[i+1]=numbers[i];
					}
					numbers[0] = (read == 11) ? 0 : read;
					if(is_start){
						numbers[1]=17;
						if(read!=11)is_start = false;
					}
				}
  		}

  		else if(read == 10){
  			rezero(numbers);
  			init_all();
  		}

  		else if(read == 12 && can_calculate){
  			operand_2 = array_to_num(numbers);
  			switch(oper){
  				case 13 :result = operand_1 + operand_2;break;
  				case 14 :result = operand_1 - operand_2;break;
  				case 15 :result = operand_1 * operand_2;break;
  				case 16 :result = operand_1 / operand_2;break;
  			}
  			if(0<result && result<10000){
  				num_to_array(result, numbers);
  			}
  			else if(result <0 && result >=-999){
  				num_to_array(-result, numbers);
  				if(result<-99)numbers[3]=16;
  				else if(result<-9)numbers[2]=16;
  				else numbers[1]=16;
  			}
  			else{rezero(numbers);}
  				init_all();
  		}

  		else if(13<=read && read<=16){
  			oper = read;
  			operand_1 = array_to_num(numbers);
  			can_calculate = true;
  			rezero(numbers);
  			is_start = true;
  		}

  	}
  	}
  	++period;
  	if(period == 5)period = 0;

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

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, SEG2_Pin|SEG3_Pin|SEG4_Pin|SEG5_Pin
                          |SEG6_Pin|SEG7_Pin|SEG0_Pin|SEG1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, DIG0_Pin|DIG1_Pin|DIG2_Pin|DIG3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : SEG2_Pin SEG3_Pin SEG4_Pin SEG5_Pin
                           SEG6_Pin SEG7_Pin SEG0_Pin SEG1_Pin */
  GPIO_InitStruct.Pin = SEG2_Pin|SEG3_Pin|SEG4_Pin|SEG5_Pin
                          |SEG6_Pin|SEG7_Pin|SEG0_Pin|SEG1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : SW3_Pin */
  GPIO_InitStruct.Pin = SW3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SW3_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : DIG0_Pin DIG1_Pin DIG2_Pin DIG3_Pin */
  GPIO_InitStruct.Pin = DIG0_Pin|DIG1_Pin|DIG2_Pin|DIG3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : KC0_Pin KC1_Pin */
  GPIO_InitStruct.Pin = KC0_Pin|KC1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : KC2_Pin */
  GPIO_InitStruct.Pin = KC2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(KC2_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
