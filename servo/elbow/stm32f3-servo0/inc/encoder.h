#ifndef ENCODER_H
#define ENCODER_H

#include "stm32f37x.h"
#include "stm32f37x_conf.h"

void encoder_init(void);
void encoder_reset(void);
void encoder_sampling(void);
int32_t encoder_get_a(void);
int32_t encoder_get_b(void);
void encoder_set_a(int32_t val);
void encoder_set_b(int32_t val);
  
#endif // ENCODER_H
