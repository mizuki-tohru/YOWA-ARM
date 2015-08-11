#ifndef DCMOTOR_CONTROLLER_H
#define DCMOTOR_CONTROLLER_H
#include "stm32f37x.h"
#include "stm32f37x_conf.h"

void dcmotorctrl_init(void);
void dcmotorctrl_start(void);
void dcmotorctrl_stop(void);
//void dcmotorctrl_update(void);
int32_t dcmotorctrl_update(void);
void dcmotorctrl_dump_params(void);
void dcmotorctrl_set_motor_a_deg(float deg);
void dcmotorctrl_set_motor_b_deg(float deg);
void dcmotorctrl_set_kp(float kp);
void dcmotorctrl_set_ki(float ki);
void dcmotorctrl_set_kd(float kd);
void dcmotorctrl_dump_values(void);

#endif // DCMOTOR_CONTROLLER_H
