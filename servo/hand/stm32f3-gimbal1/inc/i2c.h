#ifndef __i2c_h__
#define __i2c_h__

#ifdef __cplusplus
extern "C" {
#endif

void I2Cs_Initialize(void);		/* I2Cs の初期化 */
uint16_t I2Cs_read(uint16_t d_address,uint16_t address);
	// エラーだと 0xffff
uint16_t I2Cs_write(uint16_t d_address,uint16_t address,uint16_t data);
	// エラーだと 0xffff、正常だとエラーだと 0x0000
ErrorStatus I2C_CheckEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT);

#define ADXL345_ADDRESS 0x53
#define L3G4200D_ADDRESS 0x69
#define HMC5883L_ADDRESS 0x1e
#define BMP085_ADDRESS 0x77



#define ITG3200_ADDRESS 0x68
#define HMC6352_ADDRESS 0x21
#define HMC6352_TIMER   200

#define  I2C_EVENT_MASTER_MODE_SELECT                      ((uint32_t)0x00030001)  /* BUSY, MSL and SB flag */
/* --EV6 */
#define  I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED        ((uint32_t)0x00070082)  /* BUSY, MSL, ADDR, TXE and TRA flags */
#define  I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED           ((uint32_t)0x00030002)  /* BUSY, MSL and ADDR flags */
/* --EV9 */
#define  I2C_EVENT_MASTER_MODE_ADDRESS10                   ((uint32_t)0x00030008)  /* BUSY, MSL and ADD10 flags */
/* Master RECEIVER mode -----------------------------*/ 
/* --EV7 */
#define  I2C_EVENT_MASTER_BYTE_RECEIVED                    ((uint32_t)0x00030040)  /* BUSY, MSL and RXNE flags */

/* Master TRANSMITTER mode --------------------------*/
/* --EV8 */
#define I2C_EVENT_MASTER_BYTE_TRANSMITTING                 ((uint32_t)0x00070080) /* TRA, BUSY, MSL, TXE flags */
/* --EV8_2 */
#define  I2C_EVENT_MASTER_BYTE_TRANSMITTED                 ((uint32_t)0x00070084)  /* TRA, BUSY, MSL, TXE and BTF flags */

#define I2C_IT_BUF                      ((uint16_t)0x0400)
#define I2C_IT_EVT                      ((uint16_t)0x0200)
#define I2C_IT_ERR                      ((uint16_t)0x0100)

#ifdef __cplusplus
}
#endif
#endif	/* __i2c_h__ */
