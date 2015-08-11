#include "quaternion.h"

void quaternion_init(Quaternion *q) {
    q->q0 = 1.f;
    q->q1 = 0.f;
    q->q2 = 0.f;
    q->q3 = 0.f;
}

#define SQ(x)		((x)*(x))

void quaternion_to_yaw_pitch_roll(Quaternion *q, float *ypr) {
/*
#if 0
    float gx = 2.f * (q->q1*q->q3 - q->q0*q->q2);
    float gy = 2.f * (q->q0*q->q1 + q->q2*q->q3);
    float gz = q->q0*q->q0 - q->q1*q->q1 - q->q2*q->q2 + q->q3*q->q3;
    ypr[0] = atan2f(2.f * q->q1 * q->q2 - 2.f * q->q0 * q->q3, 2.f * q->q0*q->q0 + 2.f * q->q1 * q->q1 - 1.f) * 180.f / PI_F;
    ypr[1] = atanf(gx / sqrtf(gy*gy + gz*gz)) * 180.f / PI_F;
    ypr[2] = atanf(gy / sqrtf(gx*gx + gz*gz)) * 180.f / PI_F;
#else
    // algorythm by http://www.x-io.co.uk/res/doc/quaternions.pdf
    
    float q1q3q0q2 = (2.f*q->q1*q->q3 + 2.f*q->q0*q->q2) * (2.f*q->q1*q->q3 + 2.f*q->q0*q->q2);
    float yaw = atan2f(2.f*(q->q2*q->q3 - q->q0*q->q1), 2.f*q->q0*q->q0 - 1.f + 2.f*q->q3*q->q3);
    float pitch = -atanf(2.f*(q->q1*q->q3 + q->q0*q->q2) / sqrtf(1.f - q1q3q0q2));
    float roll = atan2f(2.f*(q->q1*q->q2 - q->q0*q->q3), 2.f*q->q0*q->q0 - 1.f + 2.f*q->q1*q->q1);
    
    ypr[0] = RAD_TO_DEG(yaw);
    ypr[1] = RAD_TO_DEG(pitch);
    ypr[2] = RAD_TO_DEG(roll);
#endif
*/
		// Inagawa, 2014-04-21 ジャイロセンサと地磁気センサの統合
		float yaw =   atan2f(2.f*q->q1*q->q2 - 2.f*q->q0*q->q3, 2.f*SQ(q->q0) + 2.f*SQ(q->q1) - 1.f);
		float pitch = -asinf(2.f*q->q0*q->q2 + 2.f*q->q1*q->q3);
		float roll =  atan2f(2.f*q->q2*q->q3 - 2.f*q->q0*q->q1, 2.f*SQ(q->q0) + 2.f*SQ(q->q3) - 1.f);
    ypr[0] = RAD_TO_DEG(yaw);
    ypr[1] = RAD_TO_DEG(pitch);
    ypr[2] = RAD_TO_DEG(roll);
}
