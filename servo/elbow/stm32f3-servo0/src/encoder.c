#include "encoder.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "queue.h"

// based on http://www.micromouseonline.com/2013/02/16/quadrature-encoders-with-the-stm32f4/

/*
* definitions for the quadrature encoder pins
*/

// A Channels
// JP-6 3
#define ENCLA_PIN               GPIO_Pin_8
#define ENCLA_GPIO_PORT         GPIOA
#define ENCLA_GPIO_CLK          RCC_AHBPeriph_GPIOA
#define ENCLA_SOURCE            GPIO_PinSource8
#define ENCLA_AF                GPIO_AF_2

// JP-6 4
#define ENCLB_PIN               GPIO_Pin_9
#define ENCLB_GPIO_PORT         GPIOA
#define ENCLB_GPIO_CLK          RCC_AHBPeriph_GPIOA
#define ENCLB_SOURCE            GPIO_PinSource9
#define ENCLB_AF                GPIO_AF_2

// determine the timers to use
#define ENCL_TIMER              TIM2
#define ENCL_TIMER_CLK          RCC_APB1Periph_TIM2
//#define ENCR_TIMER              TIM4
//#define ENCR_TIMER_CLK          RCC_APB1Periph_TIM4

static volatile int32_t _leftTotal;
static volatile int32_t _rightTotal;
static volatile int16_t _leftEncoder;
static volatile int16_t _rightEncoder;


/*
 * Configure two timers as quadrature encoder counters. 
 * Details of which timers should be used are
 * in the project hardware header file.
 * Most timers can be used if channels 1 and 2 are available on pins.
 * The timers are mostly 16 bit. Timers can be set to 32 bit but they are
 * not very convenient for IO pins so the counters are simply set to to
 * 16 bit counting regardless.
 * A mouse needs 32 bits of positional data and, since it also needs the
 * current speed, distance is not maintained by the encoder code but will
 * be looked after by the motion control code.
 * The counters are set to X4 mode. The only alternative is X2 counting.
 */

void encoder_init(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  // turn on the clocks for each of the ports needed
  RCC_AHBPeriphClockCmd (ENCLA_GPIO_CLK, ENABLE);
//RCC_AHBPeriphClockCmd (ENCLB_GPIO_CLK, ENABLE);

  // now configure the pins themselves
  // they are all going to be inputs with pullups
  GPIO_StructInit (&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = ENCLA_PIN;
  GPIO_Init (ENCLA_GPIO_PORT, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = ENCLB_PIN;
  GPIO_Init (ENCLB_GPIO_PORT, &GPIO_InitStructure);

  // Connect the pins to their Alternate Functions
  GPIO_PinAFConfig (ENCLA_GPIO_PORT, ENCLA_SOURCE, ENCLA_AF);
  GPIO_PinAFConfig (ENCLB_GPIO_PORT, ENCLB_SOURCE, ENCLB_AF);

  // Timer peripheral clock enable
  RCC_APB1PeriphClockCmd (ENCL_TIMER_CLK, ENABLE);

  // set them up as encoder inputs
  // set both inputs to rising polarity to let it use both edges
  TIM_EncoderInterfaceConfig (ENCL_TIMER, TIM_EncoderMode_TI12, 
                              TIM_ICPolarity_Rising, 
                              TIM_ICPolarity_Rising);
  TIM_SetAutoreload (ENCL_TIMER, 0xffff);

  // turn on the timer/counters
  TIM_Cmd (ENCL_TIMER, ENABLE);
  encoder_reset();
}

void encoder_reset(void) {
	taskENTER_CRITICAL();
  _leftTotal = 0;
  _leftEncoder = 0;
  TIM_SetCounter(ENCL_TIMER, 0);
  encoder_sampling();
	taskEXIT_CRITICAL();
}

void encoder_sampling(void) {
#if 0
  int16_t oldLeftEncoder = _leftEncoder;
  _leftEncoder = TIM_GetCounter(ENCL_TIMER);
  _leftTotal += _leftEncoder - oldLeftEncoder;
#else
//_leftTotal  = (int16_t)TIM_GetCounter(ENCL_TIMER);
  _leftTotal = TIM_GetCounter(TIM2);
	if((_leftTotal & 0xffff) > 0x7fff){
		_leftTotal = (0xffff - (_leftTotal & 0xffff));
	}else{
		_leftTotal = -(_leftTotal);
	}
#endif
}

int32_t encoder_get_a(void) {
  return _leftTotal;
}


void encoder_set_a(int32_t val) {
  _leftTotal = val;
}


