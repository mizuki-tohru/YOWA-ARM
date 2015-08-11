/*
 * pwm.h
 *
 *  Created on: 11.06.2013
 *      Author: braun
 */

#ifndef PWM_H_
#define PWM_H_

/**
 * TIM2_Ch1 PD3
 * TIM2_Ch2 PD4
 * TIM2_Ch3 PD7
 * TIM2_Ch4 PD6
 */

#ifdef __cplusplus
extern "C" {
#endif

void PWM_Init(void);
void setPWM2_2(uint32_t);
void setPWM3_1(uint32_t);
void setPWM12_2(uint32_t);


#ifdef __cplusplus
}
#endif



#endif /* PWM_H_ */
