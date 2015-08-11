/* 重要な全体変数 */
typedef enum {
	STARTING		= 0,	// 起動時
	NORMAL			= 1,	// 定常時
	STANDINGSTILL	= 2,	// 静止時 仰角30度以上
	LAUNCHPREPARE	= 3,	// 打上げ準備完了
	FLIGHT			= 4		// 飛行状態
} OBCSTATUS;

extern OBCSTATUS obcstatus;

#define	PIF				3.14159265f
#define	G_F				9.80665f

#define	LIMIT_A			0.2f	// 静止判定の加速度のズレ上限(G)
#define	LIMIT_W			6.0f	// 静止判定の角速度のズレ上限(dps)
#define	LIMIT_EL		30.0	// 静止判定の仰角の下限(deg)
#define ERRORCOUNT		3		// STATRTING状態でのエラー許容の上限
#define	LIMIT_ACC_Z		0.5f	// FLIGHTへ移行するための加速度下限値(G)
// #define	LIMIT_ACC_Z		0.1f	// テスト用の値、本番は上の値を使うこと(G)
#define	LIMIT_ACC_XY	0.5f	// FLIGHTへ移行する時の加速度で横方向の上限（相対値）
#define	TARGET_EL		85.0f	// 目標仰角(deg)
#define	TARGET_AZ		270.0f	// 目標方位角(deg)

/* 1号機用校正データ*/
#define	ADXL345_PX		(9.500193f)
#define	ADXL345_NX		(-10.419566f)
#define	ADXL345_PY		(10.419566f)
#define	ADXL345_NY		(-9.500193f)
#define	ADXL345_PZ		(9.500193f)
#define	ADXL345_NZ		(-9.80665f)

void initPara0();
void initPara1();
void setRingBuff();
void obcStatusMachine();

void setIntHighSpeed(int32_t d);


// mainルーチンからcontrol部に渡す変数
extern float cal_ax,cal_ay,cal_az;		// 加速度：単位 m/s2
extern float cx,cy,cz;					// 磁北ベクトル：単位 特に無し
extern float wx,wy,wz;					// 角速度：単位 deg/s

// XBee に渡す変数
extern uint32_t tSTATE;
extern uint32_t tStep;
extern float phi,phiX,phiY,phiZ; // デバッグ用
extern float psiX,psiY,psiZ;

void setServo();
/*
void red_led_off();
void red_led_on();
void green_led_off();
void green_led_on();
void ledSTANDINGSTILL();
void ledFLIGHT();
*/

#define	TRIM1 			100
#define	TRIM2 			-100
#define	TRIM3 			50
#define	TRIM4 			-150

#define MI(a)			(int32_t)(a * 1000.0f + 0.5f)

#define	TSTEPSLOW		100		// タスクの周期 (msec)
#define	TSTEPHIGH		20		// タスクの周期 (msec)
// #define	TSTEPHIGH		10		// タスクの周期 (msec)
#define	TIMING1			500		// STATRTING～NORMAL (msec)
#define	TIMING2			5000	// STANDINGSTILLL～LAUNCHPREPARE (msec)
#define	TIMING21		3000	// ゆっくり点滅 (msec)
#define	TIMING22		4000	// 速く点滅 (msec)
#define	TIMING23		5000	// さらに速く点滅 (msec)
#define	TIMING3			200		// 飛行と判断された後、戻り平均化 (msec)
#define	TIMING4			6000	// FLIGHT時間 (msec)

#define	NUM_RING		10		// リングバッファの数

// 位置 速度関係の変数。
extern float px,py,pz;
extern float vx,vy,vz;
extern float ax,ay,az;
extern float q0,q1,q2,q3; 
extern float targetQ0,targetQ1,targetQ2,targetQ3;
extern float initQ0,initQ1,initQ2,initQ3;

// サーボ用
extern int servo1,servo2,servo3,servo4;
extern float finX,finY,finZ;
extern float psiIntegralX,psiIntegralY,psiIntegralZ;

void getStaticAttitude();
void getTargetAttitude();
void getInitAttitude();
void calculateIntegration();
void controlCalculate();

