/*
 * pwm.c
 *
 *  Created on: 11.06.2013
 *      Author: braun
 */
#include "stdint.h"
//#include "stm32f30x.h"
//#include "stm32f30x_rcc.h"
//#include "stm32f30x_gpio.h"
//#include "stm32f30x_tim.h"
#include "stm32f37x.h"
#include "stm32f37x_rcc.h"
#include "stm32f37x_gpio.h"
#include "stm32f37x_tim.h"

#include "pwm.h"

/**
 * TIM12_Ch2 PA15
 * TIM2_Ch2 PB3
 * TIM3_Ch1 PB4
 */
void PWM_IO_Init() {
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOA, GPIOB Clocks enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_10 );
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_1 );
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_2 );
}

void PWM_Init() {
	PWM_IO_Init();

	/* TIM1 Configuration ---------------------------------------------------
	 Generate 3 combined PWM signals:
	 TIM1 input clock (TIM1CLK) is set to APB2 clock (PCLK2)
	 => TIM1CLK = PCLK2 = SystemCoreClock
	 TIM1CLK = SystemCoreClock, Prescaler = 0, TIM1 counter clock = SystemCoreClock
	 SystemCoreClock is set to 72 MHz for STM32F30x devices

	 The objective is to generate 3 combined PWM signal at 8.78 KHz (in center aligned mode):
	 - TIM1_Period = (SystemCoreClock / (8.78*2)) - 1
	 The channel 1  duty cycle is set to 50%
	 The channel 2  duty cycle is set to 37.5%
	 The channel 3  duty cycle is set to 25%
	 The Timer pulse is calculated as follows:
	 - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100

	 The channel 5  is used in PWM2 mode with duty cycle set to 6.22%

	 The 3 resulting signals are made of an AND logical combination of two reference PWMs:
	 - Channel 1 and Channel 5
	 - Channel 2 and Channel 5
	 - Channel 3 and Channel 5

	 Note:
	 SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f30x.c file.
	 Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
	 function to update SystemCoreClock variable value. Otherwise, any configuration
	 based on this variable will be incorrect.
	 ----------------------------------------------------------------------- */
	/* Compute the value to be set in ARR register to generate signal frequency at 50 Hz */
//	uint32_t TimerPeriod = (SystemCoreClock / 50) - 1;
//	uint32_t TimerPeriod = 9999;
//	uint32_t TimerPeriod = 29999;
	uint32_t TimerPeriod = 14999;
	/* Compute CCR1 value to generate a duty cycle at 50% for channel 1 */
//	uint32_t Channel1Pulse = (uint32_t)(
//			((uint32_t) 5 * (TimerPeriod - 1)) / 10);
	uint32_t Channel1Pulse = 1520;
//	uint16_t PrescalerValue = 0;
//	uint16_t PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 1000000) - 1;
	uint16_t PrescalerValue = (uint16_t) ((SystemCoreClock ) / 1000000) - 1;

	/* TIM2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);

	/* Time Base configuration */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM12, &TIM_TimeBaseStructure);

	/* Channel 1, 2,3 and 4 Configuration in PWM mode */
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC2Init(TIM2, &TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
	TIM_OC2Init(TIM12, &TIM_OCInitStructure);

	/* Enable preload */
//	TIM_ARRPreloadConfig(TIM2, ENABLE);
//	TIM_OC2PreloadConfig(TIM2, ENABLE);
	TIM_OC2PreloadConfig(TIM2, DISABLE);
//	TIM_ARRPreloadConfig(TIM3, ENABLE);
//	TIM_OC1PreloadConfig(TIM3, ENABLE);
	TIM_OC1PreloadConfig(TIM3, DISABLE);
//	TIM_ARRPreloadConfig(TIM12, ENABLE);
//	TIM_OC2PreloadConfig(TIM12, ENABLE);
	TIM_OC2PreloadConfig(TIM12, DISABLE);

	/* TIM2 counter enable */
	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	TIM_Cmd(TIM12, ENABLE);

	/* TIM2 Main Output Enable */
	TIM_CtrlPWMOutputs(TIM2, ENABLE);
	TIM_CtrlPWMOutputs(TIM3, ENABLE);
	TIM_CtrlPWMOutputs(TIM12, ENABLE);
}

void setPWM2_2(uint32_t i)
{
  TIM_SetCompare1(TIM2,i);
}

void setPWM3_1(uint32_t i)
{
  TIM_SetCompare1(TIM3,i);
}

void setPWM12_2(uint32_t i)
{
  TIM_SetCompare1(TIM12,i);
}

