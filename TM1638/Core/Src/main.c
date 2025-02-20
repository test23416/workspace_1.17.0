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
#define DATA_SET 0x44
#define DATA_READ 0x42 /////////
#define DISPLAY_ON 0x88
#define DISPLAY_OFF 0x80

uint8_t code[] ={
		0x3F,0x06,0x5B,0x4F,
		0x66,0x6D,0x7D,0x07,
		0x7F,0x67,0x77,0x7C,
		0x39,0x5E,0x79,0x71,
		0x40,0x00
};
int previous_numbers[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int counter_numbers[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};    //Debouncing

typedef struct
{
    GPIO_TypeDef *clk_port;
    GPIO_TypeDef *dio_port;
    GPIO_TypeDef *stb_port;
    uint16_t clk_pin, dio_pin, stb_pin;
} TM1638;

void tm1638_SendData(TM1638 *tm, uint8_t Data)
{
    uint8_t ByteData[8] = {0};
    // convert data to bit array
    for (uint8_t j = 0; j < 8; j++)
    {
        ByteData[j] = (Data & (0x01 << j)) && 1;
    }
    // send bit array
    for (int8_t j = 0; j < 8; j++)
    {
    	HAL_GPIO_WritePin(tm->clk_port, tm->clk_pin, GPIO_PIN_RESET);
        if (ByteData[j] == GPIO_PIN_SET)
        {
        	HAL_GPIO_WritePin(tm->dio_port, tm->dio_pin, GPIO_PIN_SET);
        }
        else
        {
        	HAL_GPIO_WritePin(tm->dio_port, tm->dio_pin, GPIO_PIN_RESET);
        }
        HAL_GPIO_WritePin(tm->clk_port, tm->clk_pin, GPIO_PIN_SET);
    }
}
void tm1638_SendCommand(TM1638 *tm, uint8_t Data){
	HAL_GPIO_WritePin(tm->stb_port, tm->stb_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(tm->stb_port, tm->stb_pin, GPIO_PIN_RESET);
	tm1638_SendData(tm, Data);
	HAL_GPIO_WritePin(tm->stb_port, tm->stb_pin, GPIO_PIN_SET);
}
void tm1638_SendAddress(TM1638 *tm, uint8_t Data, uint8_t address){
	HAL_GPIO_WritePin(tm->stb_port, tm->stb_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(tm->stb_port, tm->stb_pin, GPIO_PIN_RESET);
	tm1638_SendData(tm, address);
	tm1638_SendData(tm, Data);
	HAL_GPIO_WritePin(tm->stb_port, tm->stb_pin, GPIO_PIN_SET);
}

//something else

void tm1638_segInit(TM1638 *tm){
	tm1638_SendCommand(tm, DATA_SET);
	tm1638_SendAddress(tm, 0x00, 0xC8);
	tm1638_SendAddress(tm, 0x00, 0xC9);
	tm1638_SendAddress(tm, 0x00, 0xCA);
	tm1638_SendAddress(tm, 0x00, 0xCB);
	tm1638_SendAddress(tm, 0x00, 0xCC);
	tm1638_SendAddress(tm, 0x00, 0xCD);
	tm1638_SendAddress(tm, 0x00, 0xCE);
	tm1638_SendAddress(tm, 0x00, 0xCF);
	tm1638_SendCommand(tm, 0x80);
}
void tm1638_display(TM1638 *tm, uint8_t *numbers){
	tm1638_SendCommand(tm, DATA_SET);
	tm1638_SendAddress(tm, code[numbers[3]], 0xC0);
	tm1638_SendAddress(tm, 0x00, 0xC1);
	tm1638_SendAddress(tm, code[numbers[2]], 0xC2);
	tm1638_SendAddress(tm, 0x00, 0xC3);
	tm1638_SendAddress(tm, code[numbers[1]], 0xC4);
	tm1638_SendAddress(tm, 0x00, 0xC5);
	tm1638_SendAddress(tm, code[numbers[0]], 0xC6);
	tm1638_SendAddress(tm, 0x00, 0xC7);
	tm1638_SendCommand(tm, 0x8B);
}
//Read key function
int tm1638_ReadKey(TM1638 *tm){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	int a[32] = {0,0,0};
	HAL_GPIO_WritePin(tm->stb_port, tm->stb_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(tm->stb_port, tm->stb_pin, GPIO_PIN_RESET);
	tm1638_SendData(tm, DATA_READ);

	int key = -1;

  GPIO_InitStruct.Pin = tm->dio_pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(tm->dio_port, &GPIO_InitStruct);
	//pause for more than 1 us
  for(int i=0;i<4;++i){
  	HAL_GPIO_WritePin(tm->clk_port, tm->clk_pin, GPIO_PIN_SET);
  	for(int j=0;j<8;++j){
  		HAL_GPIO_WritePin(tm->clk_port, tm->clk_pin, GPIO_PIN_SET);
  		a[i * 8 + j] = HAL_GPIO_ReadPin(tm->dio_port, tm->dio_pin);
  		HAL_GPIO_WritePin(tm->clk_port, tm->clk_pin, GPIO_PIN_RESET);
  	}
  }
	HAL_GPIO_WritePin(tm->clk_port, tm->clk_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(tm->stb_port, tm->stb_pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(tm->stb_port, tm->stb_pin, GPIO_PIN_SET);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(tm->dio_port, &GPIO_InitStruct);

	for(int k=0;k<4;k++){
		if(a[k*8+1]!=previous_numbers[k]){
			counter_numbers[k]++;
			if(counter_numbers[k] == 10){
				previous_numbers[k] = a[k*8+1];
					if(a[k*8+1])key=k;
			}
		}else counter_numbers[k]=0;
	}
	for(int k=0;k<4;k++){
		if(a[k*8+5]!=previous_numbers[k]){
			counter_numbers[k+4]++;
			if(counter_numbers[k+4] == 10){
				previous_numbers[k+4] = a[k*8+5];
					if(a[k*8+5])key=k+4;
			}
		}else counter_numbers[k+4]=0;
	}
	//***************************************
	for(int row=3 ;row<5;row++){
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
  				if(a)key= aa;
  		}
  	}else counter_numbers[aa]=0;

  	if(b != previous_numbers[bb]){
  		counter_numbers[bb]+=1;
  		if(counter_numbers[bb] == 10){
  			previous_numbers[bb] = b;
  				if(b)key = bb;
  		}
  	}else counter_numbers[bb]=0;

  	if(c != previous_numbers[cc]){
  		counter_numbers[cc]+=1;
  		if(counter_numbers[cc] == 10){
  			previous_numbers[cc] = c;
  				if(c)key = cc;
  		}
  	}else counter_numbers[cc]=0;
	}
	// 16th button pullup mode
	int temp = HAL_GPIO_ReadPin(SW3_GPIO_Port, SW3_Pin);
	if(temp != previous_numbers[16]){
		counter_numbers[16]+=1;
		if(counter_numbers[16]==10){
			previous_numbers[16] = temp;
			if(!temp)key= 16;
		}
	}else counter_numbers[16] = 0;

  return key;
}
//**********************************************
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
	for(;count<=3;count++){
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
//**********************************************
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
  TM1638 TM;
  TM.clk_port= CLK_GPIO_Port;
  TM.clk_pin = CLK_Pin;
  TM.stb_port= STB_GPIO_Port;
  TM.stb_pin = STB_Pin;
  TM.dio_port= DIO_GPIO_Port;
  TM.dio_pin = DIO_Pin;

  uint8_t numbers[]={0,17,17,17};

  tm1638_segInit(&TM);

  //int delay =0;
  int period = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  	if(!period){
  		tm1638_display(&TM, numbers);
  	}
  	else{
  		int read = tm1638_ReadKey(&TM);
  		//int read = ReadKeyboard();
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
  	period = 1- period;

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
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, SEG2_Pin|SEG3_Pin|SEG4_Pin|SEG0_Pin
                          |SEG1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, STB_Pin|CLK_Pin|DIO_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : SEG2_Pin SEG3_Pin SEG4_Pin SEG0_Pin
                           SEG1_Pin */
  GPIO_InitStruct.Pin = SEG2_Pin|SEG3_Pin|SEG4_Pin|SEG0_Pin
                          |SEG1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : SW3_Pin */
  GPIO_InitStruct.Pin = SW3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SW3_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : STB_Pin CLK_Pin DIO_Pin */
  GPIO_InitStruct.Pin = STB_Pin|CLK_Pin|DIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

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
