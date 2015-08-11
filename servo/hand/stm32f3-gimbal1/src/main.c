/**
  ******************************************************************************
  * @file    Project/STM32F37x_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    20-September-2012
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "math.h"
#include "stm32f37x.h"
//#include "platform_config.h"
//#include "hw_config.h"
#include "stm32f37x_conf.h"
#include "i2c.h"
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "queue.h"
#include "control.h"

#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "systick.h"

#include "lsm9ds0.h"
#include "accTask.h"

// IO Pin
// PA2  DTIN1  I/O IN
// PA3  DTIN2  I/O IN
// PA4  DTIN3  I/O IN
// PA5  AINT   I/O IN
// PA6  GINT   I/O IN
// PA9  SCL2   AF4
// PA10 SDA2   AF4
// PA11 USB    AF14
// PA12 USB    AF14
// PA13 JTMS   AF0
// PA14 JTCK   AF0
// PA15 TIM12_CH2 AF10   SERVO1
// PB0  JOY_X  ADC_IN8
// PB1  JOY_Y  ADC_IN9
// PB3  TIM2_CH2 AF1     SERVO2
// PB4  TIM3_CH1 AF2     SERVO3
// PB5  U1ENA  I/O OUT
// PB6  U1TX   AF7
// PB7  U1RX   AF7
// PB8  LED0   I/O OUT
// PB9  LED1   I/O OUT

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define MESSAGE1   "STM32F37x CortexM4  " 
#define MESSAGE2   " Device running on  " 
#define MESSAGE3   "   STM32373C-EVAL   " 


#define   ENC_FILTER              6
#define   MAX_COUNT               1200
#define   TIM1_PERIOD      (MAX_COUNT*3)

#define STACK_SIZE_MIN	128	/* usStackDepth	- the stack size DEFINED IN WORDS.*/
int temp1=0;

void vLedBlinkBlue(void *pvParameters);
void vLedBlinkRed(void *pvParameters);

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//USART_InitTypeDef USART_InitStructure;
RCC_ClocksTypeDef RCC_Clocks;

//static __IO uint32_t TimingDelay;
unsigned long count_data=0;
char buf[20];

xSemaphoreHandle xSemaphore = NULL;

/* Functions -----------------------------------------------------------------*/
static void vSensorTask( void *pvParameters );
static void vControlTask( void *pvParameters );

xTaskHandle xCreatedTask;
unsigned long ulMemCheckTaskRunningCount;

uint8_t status_LED1 = 0;
uint8_t status_LED2 = 0;

extern __IO uint8_t Receive_Buffer[64];
extern __IO  uint32_t Receive_length ;
extern __IO  uint32_t length ;
uint8_t Send_Buffer[64];
uint32_t packet_sent=1;
uint32_t packet_receive=1;

/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Private functions ---------------------------------------------------------*/
void IOInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Enable the GPIO_A Clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* Configure the GPIO_A pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,  GPIO_AF_4);/*SCL2*/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_4);/*SDA2*/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_14);/*USB*/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_14);/*USB*/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource13, GPIO_AF_0);/*JTMS*/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource14, GPIO_AF_0);/*JTCK*/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_10);/**/

	/* Enable the GPIO_B Clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  /* Configure the GPIO_B pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_1);/*TIM2_CH2*/
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_2);/*TIM3_CH1*/
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_7);/*U1_TX*/
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_7);/*U1_RX*/
}

void TIMInit()
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 	TIM_OCInitTypeDef  TIM_OCInitStructure;
 	TIM_ICInitTypeDef  TIM_ICInitStructure;
 	/* TIM2 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
 	/* TIM3 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	/* TIM12 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);

	uint16_t PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 1000000) - 1;

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 9999;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	/* Output Compare Timing Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1520;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1520;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable);
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 9999;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	/* Output Compare Timing Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1520;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1520;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Disable);
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 9999;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM12, &TIM_TimeBaseStructure);
	/* Output Compare Timing Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1520;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM12, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM12, TIM_OCPreload_Disable);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1520;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM12, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM12, TIM_OCPreload_Disable);
	TIM_ARRPreloadConfig(TIM12, ENABLE);

	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	TIM_Cmd(TIM12, ENABLE);

}
void vApplicationIdleHook( void )
{
	
	while(1);
}

void vApplicationTickHook( void )
{
	    //GPIOE->BRR = BSRR_VAL;

};
void vApplicationMallocFailedHook( void )
{
	
	while(1);
};


/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	float i;
  /*!< At this stage the microcontrollers clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f37x.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f37x.c file
     */ 
  /* SysTick end of count event each 10ms */
	Set_System();
//    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
	
	RCC_GetClocksFreq(&RCC_Clocks);
	SystemCoreClockUpdate();
	SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK );
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);

	NVIC_SetPriority(SysTick_IRQn, 0x0);	
 	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
 	
//	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);

//	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
//	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	/* Configure HCLK clock as SysTick clock source. */
	/* Set SysTickCounter for _delay_ms(); */
//	SysTickInit(100);
//	SysTickInit(1000UL);
//	NVIC_SetPriority(SysTick_IRQn, 0x0);	

	Set_USBClock();
	USB_Interrupts_Config();
	USB_Init();
	
	IOInit();
//	TIMInit();

	void LSM9DS0_LowLevel_Init();
	
//	i = 2.0/3.0;
//	CDC_Send_DATA ("OK ",3);

  /* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
#if 0
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	STM_EVAL_COMInit(COM1, &USART_InitStructure);
#endif
//  printf("\n\r %s", MESSAGE1);
//  printf(" %s", MESSAGE2);
//  printf(" %s\n\r", MESSAGE3);
  
	GPIO_SetBits(GPIOB,GPIO_Pin_9);
      
  /* Add your application code here
     */
//	xSemaphore = xSemaphoreCreateMutex(); 
//	xTaskCreate( vSensorTask, "SENSOR", 1024, NULL,tskIDLE_PRIORITY,NULL);
//	xTaskCreate( vControlTask, "CONTROL", 1024, NULL,tskIDLE_PRIORITY+1,NULL);
	xTaskCreate(accTask, (signed char* )"accTask", 1024*2, NULL, 3, NULL);
	xTaskCreate( vLedBlinkBlue, (const signed char*)"Led Blink Task Blue", 
		STACK_SIZE_MIN, NULL, 1, NULL );
	xTaskCreate( vLedBlinkRed, (const signed char*)"Led Blink Task Red", 
		STACK_SIZE_MIN, NULL, 2, NULL );
	vTaskStartScheduler();
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);

  /* Infinite loop */
  while (1)
  {
#if 0
//		Delay(100);
		if(status_LED2 == 0){
			GPIO_ResetBits(GPIOB,GPIO_Pin_9);
			status_LED2 = 1;
		}else{
			GPIO_SetBits(GPIOB,GPIO_Pin_9);
			status_LED2 = 0;
		}
//		GPIO_SetBits(GPIOB,GPIO_Pin_9);
//		GPIO_ResetBits(GPIOB,GPIO_Pin_8);
#endif
	}
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(EVAL_COM1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	/* This function will get called if a task overflows its stack.   If the
	parameters are corrupt then inspect pxCurrentTCB to find which was the
	offending task. */

	( void ) pxTask;
	( void ) pcTaskName;

	for( ;; );
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */
static void vSensorTask( void *pvParameters )
{
	portTickType xLastWakeTime;

	GPIO_SetBits(GPIOB,GPIO_Pin_8);
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);

	xLastWakeTime = xTaskGetTickCount ();
	for( ;; ){
		vTaskDelayUntil(&xLastWakeTime, 1000/portTICK_RATE_MS);
		if(status_LED1 == 0){
			GPIO_ResetBits(GPIOB,GPIO_Pin_9);
			status_LED1 = 1;
		}else{
			GPIO_SetBits(GPIOB,GPIO_Pin_9);
			status_LED1 = 0;
		}
	}
}
static void vControlTask( void *pvParameters )
{
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);
	for( ;; ){
		Delay(100);
	}
}
void vLedBlinkBlue(void *pvParameters)
{
	for(;;)
	{
	/* Set PE14 and PE15 */
	GPIOB->BSRR = GPIO_Pin_8;  
//    GPIOE->BSRR |= 0xC000;
		    		vTaskDelay( 500 / portTICK_RATE_MS );
//    GPIOE->BRR |= 0xC000;
	GPIOB->BRR = GPIO_Pin_8;  

		temp1++;
    /* Reset PE14 and PE15 */
    		vTaskDelay( 500 / portTICK_RATE_MS );
	}
}

void vLedBlinkRed(void *pvParameters)
{
	for(;;)
	{
	/* Set PE14 and PE15 */
	GPIOB->BSRR = GPIO_Pin_9;  
//    GPIOE->BSRR |= 0x3000;
		    		vTaskDelay( 500 / portTICK_RATE_MS );
//    GPIOE->BRR |= 0x3000;
	GPIOB->BRR = GPIO_Pin_9;  
    /* Reset PE14 and PE15 */
   		vTaskDelay( 500 / portTICK_RATE_MS );
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
