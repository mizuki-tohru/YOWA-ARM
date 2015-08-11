#include "task_sequence.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "queue.h"

//#include "task_sensor.h"
//#include "logger.h"
#include "encoder.h"
#include "dcmotor_controller.h"
//#include "struct_id.h"
//#include "logg.h"

#include "quaternion.h"
#include "armangle.h"

#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"

char encbuf[10];

//------------------------------------------------------------------------
//	long値実数から16進ASCIIへ。ltoa()を使わないのは、出力を大文字にしたい
//だけの理由から
//------------------------------------------------------------------------
int ltohex(unsigned long l,unsigned char * buf,unsigned char i)
{
	int j,k;
	unsigned long x;
	for(j = 0;j < i;j++) {
		x = l;
		for (k = 0;k < (i - j - 1);k++) x = (x >> 4);
		buf[j] = "0123456789ABCDEF"[(x & 0x0f)];
	}
	buf[j] = 0;
	return j;
}


static volatile float _rate = 1.f;
static volatile float _max_angle = 10.f;
//static volatile float _max_angle = 0.5f;
static volatile float _duration = 10.f;
//static volatile float _duration = 1.f;


static void motion_sequence(uint32_t start_time) {
  uint32_t now = xTaskGetTickCount();
	float t = (now - start_time) / 1000.f / _duration;
	t = fmodf(t, 1.f);
	
	float a1 = 90.f - sinf(t * PI_F) * _max_angle;
//  float a2 = t*360.f*_rate;
//	a1 = DEG_TO_RAD(a1);
//	a2 = DEG_TO_RAD(a2);
//  float y_rotate = atan2f(sinf(a1),cosf(a1)*cosf(a2))-PI_F/2.f;
//  float x_rotate = M_PI/2.f-atan2f(sinf(a1),cosf(a1)*sinf(a2));
//  float a_arm_deg = RAD_TO_DEG(get_a_arm_agnle(x_rotate, y_rotate));
//	dcmotorctrl_set_motor_a_deg(a_arm_deg);
	dcmotorctrl_set_motor_a_deg(a1);
}

void sequence_set_rate(float rate) {
	_rate = rate;
}

void sequence_set_max_angle_deg(float deg) {
	_max_angle = deg;
}

void sequence_set_duration(float duration) {
	_duration = duration;
}

void sequence_dump(void) {
	printf("[Sequence] rate=%.2f\n", _rate);
	printf("[Sequence] max_angle=%.2f\n", _max_angle);
	printf("[Sequence] duration=%.2f\n", _duration);
}

void vSequenceControl(void *param) {
	encoder_init();
	dcmotorctrl_init();
	int32_t dend;

//	logg("[Sequence] Sequence Initialized.");

	dcmotorctrl_start();

	for (;;) {
		int n = 0;
//		logg("[Sequence] start.");
	  portTickType xLastWakeTime = xTaskGetTickCount();
	  uint32_t start_time = xLastWakeTime;
		uint32_t end_time = start_time + _duration * 1000;
		for (;;) {
			encoder_sampling();
			
		//	ltohex(encoder_get_a(),encbuf,8);

			motion_sequence(start_time);
			dend = dcmotorctrl_update();
			
			if(dend >= 0){
				ltohex(dend,encbuf,8);
			}else{
				encbuf[0] = '-';
				ltohex(-dend,encbuf+1,7);
			}
			encbuf[8] = '\n';
			encbuf[9] = 0;
			CDC_Send_DATA (encbuf,9);
			
			n++;
			if (n % 10 == 0) {
				dcmotorctrl_dump_values();
			}
			vTaskDelayUntil(&xLastWakeTime, 10/portTICK_RATE_MS);
			if (xLastWakeTime >= end_time) break;
		}

//		logg("[Sequence] end. 10sec waiting...");

		dcmotorctrl_set_motor_a_deg(0.f);
		
		n = 0;
		for (;;) {
			encoder_sampling();
			dend = dcmotorctrl_update();
			
			if(dend >= 0){
				ltohex(dend,encbuf,8);
			}else{
				encbuf[0] = '-';
				ltohex(-dend,encbuf+1,7);
			}
			encbuf[8] = ' ';
			encbuf[9] = 0;
			CDC_Send_DATA (encbuf,9);
			
		//	n++;
		//	if (n > 100*10) break;
			vTaskDelayUntil(&xLastWakeTime, 10/portTICK_RATE_MS);
		}
	}
}