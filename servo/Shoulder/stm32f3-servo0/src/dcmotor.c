#include "dcmotor.h"

static int16_t _max_value = 0;
static int16_t _center_value = 0;

void dcmotor_init(int16_t period, int16_t max_value) {
  _max_value = max_value;
	_center_value = period / 2;
  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	// PB14 for motordrive
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	/* TIM12 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);

	/* Connect TIM Channels to AF1 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14,GPIO_AF_9);

	/* TIM14 CLK = 5MHz */
	uint16_t PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 5000000) - 1;
	/* Time base configuration */
	/* COUNTER = 200 */
	/*5MHz / 200 = 25KHz */
	TIM_TimeBaseStructure.TIM_Period = period - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM12, &TIM_TimeBaseStructure);
	
	/* Output Compare Timing Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OC1Init(TIM14, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM12, TIM_OCPreload_Enable);
//	TIM_OC1PreloadConfig(TIM12, TIM_OCPreload_Disable);
	TIM_ARRPreloadConfig(TIM12, ENABLE);
	
	/* TIM14 enable counter */
	TIM_Cmd(TIM12, ENABLE);
}


void dcmotor_on_a(bool on) {
	if (on) {
		GPIO_SetBits(GPIOB,GPIO_Pin_15);
	} else {
		GPIO_ResetBits(GPIOB,GPIO_Pin_15);
	}
}

#define DEADZONE		2

int16_t valconv(int16_t value) {
	if (value == 0) {

	} else if (value > 0) {
		value += DEADZONE;
	} else if (value < 0) {
		value -= DEADZONE;
	}

  if (value < -_max_value) value = -_max_value;
  if (value >  _max_value) value =  _max_value;
	return value;
}

void dcmotor_set_a(int16_t value) {
	value = valconv(value);
	TIM_SetCompare1(TIM12, value + _center_value);
}


