#include "armangle.h"
#include <math.h>

#define MOTOR_ARMLENGTH			29.f
#define A_RODLENGTH      180.5f  // rodend centor to center length
#define B_RODLENGTH      260.5f  // rodend centor to center length

#define MotorRodendOffset 		(4.f + 3.f)
#define EngineRodendOffset		(4.f + 3.f)

#define A_MOTOR_JOINT_POS    {-79.f-MotorRodendOffset, -54.f, 82.f}   // {x,y,z}
#define B_MOTOR_JOINT_POS    {79.f+MotorRodendOffset, -54.f, 162.f}   // {x,y,z}

#define EngineJointR 				((82.f+31.f+EngineRodendOffset)/1.414213562373f)
#define A_ENGINE_JOINT_POS    {-EngineJointR, -EngineJointR, -103.5+5}   // {x,y,z}
#define B_ENGINE_JOINT_POS    {EngineJointR, -EngineJointR, -103.5+5}   // {x,y,z}


static float get_arm_angle(float x0, float y0, float z0, float rf, float re) {
    /* Thanks to mzavatsky
     http://forums.trossenrobotics.com/tutorials/introduction-129/delta-robot-kinematics-3276/
     */
    // z = a + b*y
    float a = (x0*x0 + y0*y0 + z0*z0 + rf*rf - re*re)/(2*z0);
    float b = -y0/z0;
    /* discriminant */
    float d = -(a)*(a) + rf * (b*b*rf + rf);
    
    float yj = (- a*b - sqrtf(d))/(b*b + 1.f); //choosing outer point
    float zj = a + b*yj;
    
    // get the angle!
    return atan2f(-zj, -yj);
}

static void rotate_xy(float *pos, float x_rotate, float y_rotate) {
    float x = pos[0] * cosf(y_rotate) + pos[2] * sinf(y_rotate);
    float y = pos[1];
    float z = -pos[0] * sinf(y_rotate) + pos[2] * cosf(y_rotate);
    pos[0] = x;
    pos[1] = y * cosf(x_rotate) - z * sinf(x_rotate);
    pos[2] = y * sinf(x_rotate) + z * cosf(x_rotate);
}

float get_a_arm_agnle(float x_rotate, float y_rotate) {
	float pos[] = A_ENGINE_JOINT_POS;
	float from[] = A_MOTOR_JOINT_POS;
	rotate_xy(pos, x_rotate, y_rotate);
	return get_arm_angle(pos[0]-from[0], pos[1]-from[1], pos[2]-from[2], MOTOR_ARMLENGTH, A_RODLENGTH);
}

float get_b_arm_agnle(float x_rotate, float y_rotate) {
	float pos[] = B_ENGINE_JOINT_POS;
	float from[] = B_MOTOR_JOINT_POS;
	rotate_xy(pos, x_rotate, y_rotate);
	return get_arm_angle(pos[0]-from[0], pos[1]-from[1], pos[2]-from[2], MOTOR_ARMLENGTH, B_RODLENGTH);
}
/*
static void test(float x_rotate_deg, float y_rotate_deg) {
    float x_rotate = x_rotate_deg / 180.f * M_PI;
    float y_rotate = y_rotate_deg / 180.f * M_PI;
    float a_arm_deg = get_a_arm_agnle(x_rotate, y_rotate) / M_PI * 180.f;
    float b_arm_deg = get_b_arm_agnle(x_rotate, y_rotate) / M_PI * 180.f;
    printf("Rotate[%.4f,%.4f] => ArmRoate[%.4f,%.4f]\n", x_rotate_deg, y_rotate_deg, a_arm_deg, b_arm_deg);
}

void single_points_test(void) {
    test(0.f, 0.f); 		// ECHO: 0.025297,ECHO: 0.0176743
    test(10.f, 0.f); 		// ECHO: 28.1749, ECHO: 27.8264
    test(0.f, 10.f); 		// ECHO: 29.7162, ECHO: -32.9468
    test(10.f, 10.f); 	// ECHO: 70.0032, ECHO: -4.04348
    test(-10.f, 0.f); 	// ECHO: -30.9481, ECHO: -31.8757
    test(0.f, -10.f); 	// ECHO: -32.415, ECHO: 28.8749
    test(-10.f, -10.f); // ECHO: -74.6601, ECHO: -3.42973
    test(7.f, 3.f); 		// ECHO: 29.1818, ECHO: 10.441
    test(2.145f, -5.f); // ECHO: -8.96907, ECHO: 20.8206
}
*/


