#include "main.h"

#define   ENC_FILTER              6
#define   MAX_COUNT               1200
#define   TIM1_PERIOD      (MAX_COUNT*3)

static __IO uint32_t TimingDelay;
unsigned long count_data=0;
char buf[20];

void IOInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Enable the GPIO_A Clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* Configure the GPIO_A pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_7|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Enable the GPIO_B Clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  /* Configure the GPIO_B pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void TIMInit()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 	TIM_OCInitTypeDef  TIM_OCInitStructure;
 	TIM_ICInitTypeDef  TIM_ICInitStructure;
 	/* TIM1 clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	/* TIM14 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);

	/* GPIOA Configuration: TIM14 CH1 (PA4) */
	/* GPIOA Configuration: TIM1  CH1 (PA8) */
	/* GPIOA Configuration: TIM1  CH2 (PA9) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	/* Connect TIM Channels to AF1 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_4);/*TIM14*/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_2);


	uint16_t PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 1000000) - 1;

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 999;
//	TIM_TimeBaseStructure.TIM_Period = 9999;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);
	/* Output Compare Timing Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM14, &TIM_OCInitStructure);
	TIM_ARRPreloadConfig(TIM14, ENABLE);
	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Disable);
	
	/* TIM3 enable counter */
	TIM_Cmd(TIM14, ENABLE);
	TIM_CtrlPWMOutputs(TIM14, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Prescaler = 0;  // No prescaling 
	TIM_TimeBaseStructure.TIM_Period = TIM1_PERIOD-1;  
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	TIM_EncoderInterfaceConfig(TIM1,TIM_EncoderMode_TI12,
		TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = ENC_FILTER;//ICx_FILTER;
	TIM_ICInit(TIM1, &TIM_ICInitStructure);
	TIM1->CNT = 0;

	TIM_Cmd(TIM1, ENABLE);

}
void setPWM14(uint32_t i)
{
  TIM_SetCompare1(TIM14,i);
}
//------------------------------------------------------------------------
//	long値実数から16進ASCIIへ。ltoa()を使わないのは、出力を大文字にしたい
//だけの理由から
//------------------------------------------------------------------------
int ltohex(unsigned long l,unsigned char * buf,unsigned char i)
{
	int j,k;
	unsigned long x;
	for(j = 0;j < i;j++) {
		x = l;
		for (k = 0;k < (i - j - 1);k++) x = (x >> 4);
		buf[j] = "0123456789ABCDEF"[(x & 0x0f)];
	}
	buf[j] = 0;
	return j;
}

int main(void)
{
	int i;
	
	NVIC_SetPriority(SysTick_IRQn, 0x0);
	
	IOInit();

	SystemCoreClockUpdate();
 	SysTick_Config(SystemCoreClock/1000);

	conio_init(UART_DEFAULT_NUM,UART_BAUDLATE);
	cputs("Hello World\n");

	GPIOA->BRR  = GPIO_Pin_7;
	
	TIMInit();
	
	GPIOA->BSRR  = GPIO_Pin_7;

	i=0;
	while(1){
		if(i>900){
			setPWM14(1800-i);
		}else{
			setPWM14(i);
 		}
 		/* maintain LED3 status for 200ms */
		GPIOA->BRR  = GPIO_Pin_15;
		GPIOB->BSRR = GPIO_Pin_3;
//		GPIOA->BRR  = GPIO_Pin_6;
		Delay(500);

		count_data = TIM_GetCounter(TIM1);
	//	ltohex(count_data,(unsigned char)buf,8);
		buf[0] = (count_data >> 24)&0xff;
		if(buf[0] == 0)buf[0] = ' ';
		buf[1] = (count_data >> 16)&0xff;
		if(buf[1] == 0)buf[1] = ' ';
		buf[2] = (count_data >> 8) &0xff;
		if(buf[2] == 0)buf[2] = ' ';
		buf[3] = count_data        &0xff;
		if(buf[3] == 0)buf[3] = ' ';
		buf[4] = '\n';
		buf[5] = 0;
		cputs(buf);
		
	//	cputs("Hello World\n");
	//	xprintf("Hello World\n");
		GPIOA->BSRR  = GPIO_Pin_15;
		GPIOB->BRR = GPIO_Pin_3;
//		GPIOA->BSRR  = GPIO_Pin_6;
		Delay(500);
//		i+=100;
		i+=50;
//		if(i>8000)i=0;
		if(i>1800)i=0;
	}
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 1 ms.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
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
  {}
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
