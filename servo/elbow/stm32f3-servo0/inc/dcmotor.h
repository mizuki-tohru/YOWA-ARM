#ifndef DCMOTOR_H
#define DCMOTOR_H

#include "stm32f37x.h"
#include "stm32f37x_conf.h"
#include <stdbool.h>

void dcmotor_init(int16_t period, int16_t max_value);
void dcmotor_on_a(bool on);
void dcmotor_on_b(bool on);
void dcmotor_set_a(int16_t value);
void dcmotor_set_b(int16_t value);

#endif // DCMOTOR_H
