#include "dcmotor_controller.h"
#include "dcmotor.h"
#include "encoder.h"
//#include "logg.h"
//#include "logger.h"
//#include "struct_id.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "queue.h"

//#include "uart.h"
//#include "uart_support.h"
//#include "xprintf.h"

static volatile int32_t _target_a = 0;
static volatile int32_t _a_error_prev = 0;
static volatile float _integral_a = 0.f;
//static volatile float _Kp = 0.6f;
//static volatile float _Kp = 1.0f;
static volatile float _Kp = 0.2f;
static volatile float _Kd = 0.f;
//static volatile float _Kd = 0.15f;
//static volatile float _Kd = 0.05f;
static volatile float _Ki = 0.f;
//static volatile float _Ki = 0.2f;
//static volatile float _Ki = 0.05f;
static volatile uint32_t _last_timestamp = 0;


//typedef struct {
//	StructID	id;
//	uint32_t timestamp;
//	int32_t target_a;
//	int32_t target_b;
//	int32_t encoder_a;
//	int32_t encoder_b;
//	int32_t motor_a;
//	int32_t motor_b;
//} DCMotorLog;

//static DCMotorLog	_log;

void dcmotorctrl_init(void) {
	_target_a = 0;
	_a_error_prev = 0;
	_integral_a = 0.f;
	_last_timestamp = 0;
	
//	uart_init(2, 115200);
	
//	memset(&_log, 0, sizeof(_log));
//	_log.id.id = STRUCT_ID_DCMOTOR;
//	_log.id.size = sizeof(_log);
	
	dcmotor_init(200, 80);
	dcmotor_set_a(0);
	dcmotorctrl_stop();
}

void dcmotorctrl_start(void) {
	dcmotor_on_a(true);
}

void dcmotorctrl_stop(void) {
	dcmotor_on_a(false);
}

#if 1

// DCMotor PID control

//void dcmotorctrl_update(void) {
int32_t dcmotorctrl_update(void) {
	float dt = 0.f;
	uint32_t now = xTaskGetTickCount();
  if (_last_timestamp != 0) {
		dt = (float)(now - _last_timestamp) / 1000.f;
	}
	_last_timestamp = now;
	
  int32_t a = encoder_get_a();
  int32_t a_error = _target_a - a;
  int32_t a_error_delta = a_error - _a_error_prev;
  _integral_a += (float)(a_error + _a_error_prev) * 0.5f * dt;
  _a_error_prev = a_error;
	int32_t motor_a = a_error * _Kp + _integral_a * _Ki + a_error_delta * _Kd;
  dcmotor_set_a(motor_a);
  
//    return a_error;
   return _target_a;

//	_log.timestamp = xTaskGetTickCount();
//	_log.target_a = _target_a;
//	_log.target_b = _target_b;
//	_log.encoder_a = a;
//	_log.encoder_b = b;
//	_log.motor_a = motor_a;
//	_log.motor_b = motor_b;
//	logger_append_log((const uint8_t*)&_log, sizeof(_log));

//	uart_write(2, (const uint8_t*)&_log, sizeof(_log));
}

void dcmotorctrl_set_motor_a_deg(float deg) {
//	_target_a = deg / 360.f * 4000.f;
//	_target_a = ((deg / 360.f) * 512.f)/3.0;
	_target_a = (deg / 360.f) * 256.f;
}


#else 

// for DC motor / encoder test

void dcmotorctrl_update(void) {
	static uint32_t t = 0;
	uint32_t now = xTaskGetTickCount();
	if (now - t > 1000) {
	  int32_t a = encoder_get_a();
//		printf("[Control] A:%8ld B:%8ld\n", a, b);
		t = now;
	}
	
//	_log.timestamp = xTaskGetTickCount();
//	_log.target_a = _target_a;
//	_log.target_b = _target_b;
//	_log.encoder_a = encoder_get_a();
//	_log.encoder_b = encoder_get_b();
//	_log.motor_a = 0;
//	_log.motor_b = 0;
//	logger_append_log((const uint8_t*)&_log, sizeof(_log));
}

void dcmotorctrl_set_motor_a_deg(float deg) {
	_target_a = deg;
  dcmotor_set_a(_target_a);
}


#endif

void dcmotorctrl_dump_params(void) {
//	loggf("[Control] Kp: %.3f", _Kp);
//	loggf("[Control] Ki: %.3f", _Ki);
//	loggf("[Control] Kd: %.3f", _Kd);
//	loggf("[Control] _target_a: %d (%.3fdeg)", _target_a, _target_a*360.f/4000.f);
//	loggf("[Control] encoder_a: %d", encoder_get_a());
//	loggf("[Control] _target_b: %d (%.3fdeg)", _target_b, _target_b*360.f/4000.f);
//	loggf("[Control] encoder_b: %d", encoder_get_b());
}

void dcmotorctrl_dump_values(void) {
//	uint32_t timestamp = xTaskGetTickCount();
//	printf("%ld\t%ld\t%ld\t%ld\t%ld\n", timestamp, _target_a, _target_b, encoder_get_a(), encoder_get_b());
}

void dcmotorctrl_set_kp(float kp) {
	_Kp = kp;
}

void dcmotorctrl_set_ki(float ki) {
	_Ki = ki;
}

void dcmotorctrl_set_kd(float kd) {
	_Kd = kd;
}


