#ifndef QUATERNION_H
#define QUATERNION_H

#include <math.h>

#define PI_F		3.14159265358979f
#define RAD_TO_DEG(x)			((x)/PI_F*180.f)
#define DEG_TO_RAD(x)			((x)/180.f*PI_F)


typedef struct {
    float q0;
    float q1;
    float q2;
    float q3;
} Quaternion;

void quaternion_init(Quaternion *q);
void quaternion_to_yaw_pitch_roll(Quaternion *q, float *ypr);

#endif
