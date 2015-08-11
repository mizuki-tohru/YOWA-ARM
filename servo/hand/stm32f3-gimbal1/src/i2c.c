#include "stdio.h"
#include "stm32f37x.h"
#include "i2c.h"

#define I2C                          I2C2
#define I2C_CLK                      RCC_APB1Periph_I2C2

#define I2C_SCL_PIN                  GPIO_Pin_9
#define I2C_SCL_GPIO_PORT            GPIOA
#define I2C_SCL_GPIO_CLK             RCC_AHBPeriph_GPIOA
#define I2C_SCL_SOURCE               GPIO_PinSource9
#define I2C_SCL_AF                   GPIO_AF_4

#define I2C_SDA_PIN                  GPIO_Pin_10
#define I2C_SDA_GPIO_PORT            GPIOA
#define I2C_SDA_GPIO_CLK             RCC_AHBPeriph_GPIOA
#define I2C_SDA_SOURCE               GPIO_PinSource10
#define I2C_SDA_AF                   GPIO_AF_4

#ifndef I2C_SPEED
// #define I2C_SPEED                        100000
 #define I2C_SPEED                        400000
#endif /* I2C_SPEED */

#define FLAG_TIMEOUT         ((uint32_t)0x1000)
#define LONG_TIMEOUT         ((uint32_t)(10 * FLAG_TIMEOUT))

/* SB0 = INT0(in)  PB1 = INT1(in)  SLC = PB2(out)  SDA = PB3(in/out)  */
/* I2C2SCL は PB10 I2C2SDA は PB11 */

/* I2C の初期化 */
void I2Cs_Initialize(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
NVIC_InitTypeDef NVIC_InitStructure;
  I2C_InitTypeDef  I2C_InitStructure;

  RCC_APB1PeriphClockCmd(I2C_CLK, ENABLE);
  RCC_AHBPeriphClockCmd(I2C_SCL_GPIO_CLK | I2C_SDA_GPIO_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);


/* Configure one bit for preemption priority */
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
/* Enable the I2C Interrupt */
NVIC_InitStructure.NVIC_IRQChannel = I2C2_EV_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);

  GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(I2C_SCL_GPIO_PORT, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
  GPIO_Init(I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(I2C_SCL_GPIO_PORT, I2C_SCL_SOURCE, I2C_SCL_AF);
  GPIO_PinAFConfig(I2C_SDA_GPIO_PORT, I2C_SDA_SOURCE, I2C_SDA_AF);  

  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
//  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
//  I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
  I2C_Cmd(I2C, ENABLE);
  I2C_Init(I2C, &I2C_InitStructure);
}

uint16_t I2Cs_read(uint16_t d_address,uint16_t address)
{
  uint16_t r = 0;
  __IO uint32_t  Timeout; 
  
  Timeout = LONG_TIMEOUT;
  while(I2C_GetFlagStatus(I2C, I2C_FLAG_BUSY)) {
    if((Timeout--) == 0) {
      I2C_GenerateSTOP(I2C, ENABLE);
      printf("I2C BUSY TIMEOUT\r\n");
      return 0xffff;
    }
  }
  I2C_GenerateSTART(I2C, ENABLE);
  Timeout = FLAG_TIMEOUT;
  while(!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_MODE_SELECT)) {
    if((Timeout--) == 0) {
      I2C_GenerateSTOP(I2C, ENABLE);
      printf("I2C CHECK EVENT MASTER MODE TIMEOUT\r\n");
      return 0xffff;
    }
  }    
  I2C_Send7bitAddress(I2C,(d_address << 1), I2C_Direction_Transmitter);
  Timeout = FLAG_TIMEOUT;
  while(!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
    if((Timeout--) == 0) {
      I2C_GenerateSTOP(I2C, ENABLE);
      printf("I2C CHECK EVENT TRANS MODE TIMEOUT\r\n");
      return 0xffff;
    }
  } 
  I2C_SendData(I2C, address);  
  Timeout = FLAG_TIMEOUT;
  while(!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
    if((Timeout--) == 0) {
      I2C_GenerateSTOP(I2C, ENABLE);
      printf("I2C CHECK EVENT TRANSMITTED TIMEOUT\r\n");
      return 0xffff;
    }
  }
  I2C_GenerateSTOP(I2C, ENABLE);
  Timeout = LONG_TIMEOUT;
  while(I2C_GetFlagStatus(I2C, I2C_FLAG_BUSY)) {
    if((Timeout--) == 0) {
      I2C_GenerateSTOP(I2C, ENABLE);
      printf("I2C BUSY TIMEOUT\r\n");
      return 0xffff;
    }
  }
  I2C_GenerateSTART(I2C, ENABLE);
  Timeout = FLAG_TIMEOUT;
  while(!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_MODE_SELECT)) {
    if((Timeout--) == 0) {
      I2C_GenerateSTOP(I2C, ENABLE);
      printf("I2C CHECK EVENT MASTER MODE TIMEOUT\r\n");
      return 0xffff;
    }
  }    
  I2C_Send7bitAddress(I2C,(d_address << 1), I2C_Direction_Receiver);  
  Timeout = FLAG_TIMEOUT;
  while(!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
    if((Timeout--) == 0) {
      I2C_GenerateSTOP(I2C, ENABLE);
      printf("I2C CHECK EVENT RECEIVE MODE TIMEOUT\r\n");
      return 0xffff;
    }
  } 
  I2C_AcknowledgeConfig(I2C, DISABLE);   
  I2C_GenerateSTOP(I2C, ENABLE);
  Timeout = FLAG_TIMEOUT;
  while(!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
    if((Timeout--) == 0) {
      I2C_GenerateSTOP(I2C, ENABLE);
      printf("I2C CHECK EVENT RECEIVED TIMEOUT\r\n");
      return 0xffff;
    }
  }
  r = I2C_ReceiveData(I2C);
  return r;
}

uint16_t I2Cs_write(uint16_t d_address,uint16_t address,uint16_t data)
{
  __IO uint32_t  Timeout; 
  
  Timeout = LONG_TIMEOUT;
  while(I2C_GetFlagStatus(I2C, I2C_FLAG_BUSY)) {
    if((Timeout--) == 0) {
      I2C_GenerateSTOP(I2C, ENABLE);
      printf("I2C BUSY TIMEOUT\r\n");
      return 0xffff;
    }
  }
  I2C_GenerateSTART(I2C, ENABLE);
  Timeout = FLAG_TIMEOUT;
  while(!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_MODE_SELECT)) {
    if((Timeout--) == 0) {
      I2C_GenerateSTOP(I2C, ENABLE);
      printf("I2C CHECK EVENT MASTER MODE TIMEOUT\r\n");
      return 0xffff;
    }
  }    
  I2C_Send7bitAddress(I2C,(d_address << 1), I2C_Direction_Transmitter);
  Timeout = FLAG_TIMEOUT;
  while(!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
    if((Timeout--) == 0) {
      I2C_GenerateSTOP(I2C, ENABLE);
      printf("I2C CHECK EVENT TRANS MODE TIMEOUT\r\n");
      return 0xffff;
    }
  } 
  I2C_SendData(I2C, address);  
  Timeout = FLAG_TIMEOUT;
  while(!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
    if((Timeout--) == 0) {
      I2C_GenerateSTOP(I2C, ENABLE);
      printf("I2C CHECK EVENT TRANSMITTED TIMEOUT\r\n");
      return 0xffff;
    }
  }
  I2C_SendData(I2C, data);  
  Timeout = FLAG_TIMEOUT;
  while(!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
    if((Timeout--) == 0) {
      I2C_GenerateSTOP(I2C, ENABLE);
      printf("I2C CHECK EVENT TRANSMITTED TIMEOUT\r\n");
      return 0xffff;
    }
  }
  I2C_GenerateSTOP(I2C, ENABLE);
  return 0x0000;
}

// #define NO_BMP085  // これはデバッグ用に、調子の悪いBMP085を使わないようにしているだけ。

volatile int16_t irq_status,i2c_sensor,i2c_sensorN;
volatile uint16_t flgNext;

const uint16_t tableD[23] = {
		ADXL345_ADDRESS,ADXL345_ADDRESS,ADXL345_ADDRESS,
		ADXL345_ADDRESS,ADXL345_ADDRESS,ADXL345_ADDRESS,
		L3G4200D_ADDRESS,L3G4200D_ADDRESS,L3G4200D_ADDRESS,
		L3G4200D_ADDRESS,L3G4200D_ADDRESS,L3G4200D_ADDRESS,
		L3G4200D_ADDRESS,
		HMC5883L_ADDRESS,HMC5883L_ADDRESS,HMC5883L_ADDRESS,
		HMC5883L_ADDRESS,HMC5883L_ADDRESS,HMC5883L_ADDRESS,
		BMP085_ADDRESS,BMP085_ADDRESS,BMP085_ADDRESS,
		0};
const uint16_t tableA[23] = {
		0x32,0x33,0x34,
		0x35,0x36,0x37,
		0x28,0x29,0x2a,
		0x2b,0x2c,0x2d,
		0x26,
		0x03,0x04,0x05,
		0x06,0x07,0x08,
		0xf6,0xf7,0xf8,
		0};
#ifndef NO_BMP085
const uint16_t tableWD[2] = {
		BMP085_ADDRESS,
		0};
const uint16_t tableWA[2] = {
		0xf4,
		0};
volatile uint8_t writeData;
#endif
volatile uint8_t i2c_buf[26];

uint16_t I2Cs_read_withIRQ(int flgBMP085)
{
  __IO uint32_t  Timeout; 
  Timeout = LONG_TIMEOUT;
  while(I2C_GetFlagStatus(I2C, I2C_FLAG_BUSY)) {
    if((Timeout--) == 0) {
      I2C_GenerateSTOP(I2C, ENABLE);
      printf("I2C BUSY TIMEOUT\r\n");
      return 0xffff;
    }
  }
  irq_status = 0;
  i2c_sensor = 0;
#ifndef NO_BMP085
  if (flgBMP085) {
    i2c_sensorN = 22;
    writeData = 0x2e; // 温度読み設定
  } else {
    i2c_sensorN = 21;
    writeData = 0x34; // 圧力読み設定
  }
#else
  i2c_sensorN = 19;
#endif
  I2C_GenerateSTART(I2C, ENABLE);
  I2C_ITConfig(I2C,I2C_IT_BUF, ENABLE);
  I2C_ITConfig(I2C,I2C_IT_EVT, ENABLE);
// I2C_ITConfig(I2C,I2C_IT_ERR, ENABLE);
  return 0;
}

void I2C2_EV_IRQHandler(void)
{
	switch (irq_status) {
		case 0:
			if (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_MODE_SELECT)) {
				return;
			}
			I2C_Send7bitAddress(I2C,(tableD[i2c_sensor] << 1), I2C_Direction_Transmitter);
			irq_status++;
			break;
		case 1:
			if (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
				return;
			}
			I2C_SendData(I2C, tableA[i2c_sensor]);  
			irq_status++;
			break;
		case 2:
			if (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
				return;
			}
			I2C_GenerateSTOP(I2C, ENABLE);
			I2C_GenerateSTART(I2C, ENABLE);
			irq_status++;
			break;
		case 3:
			if (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_MODE_SELECT)) {
				return;
			}
			I2C_Send7bitAddress(I2C,(tableD[i2c_sensor] << 1), I2C_Direction_Receiver);  
			irq_status++;
			break;
		case 4:
			if (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
				return;
			}
			I2C_AcknowledgeConfig(I2C, DISABLE);   
			I2C_GenerateSTOP(I2C, ENABLE);
			irq_status++;
			break;
		case 5:
			if (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
				return;
			}
			i2c_buf[i2c_sensor] = I2C_ReceiveData(I2C);
            i2c_sensorN--;
			if ((i2c_sensorN) && (tableD[i2c_sensor+1])) {
				i2c_sensor++;
				irq_status = 0;
				I2C_GenerateSTART(I2C, ENABLE);
			} else {
#ifndef NO_BMP085
				i2c_sensor = 0;
				irq_status = 6;
				I2C_GenerateSTART(I2C, ENABLE);
#else
				I2C_ITConfig(I2C,I2C_IT_BUF, DISABLE);
				I2C_ITConfig(I2C,I2C_IT_EVT, DISABLE);
#endif
			}
			break;
#ifndef NO_BMP085
		case 6:
			if (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_MODE_SELECT)) {
				return;
			}
			I2C_Send7bitAddress(I2C,(tableWD[i2c_sensor] << 1), I2C_Direction_Transmitter);
			irq_status++;
			break;
		case 7:
			if (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
				return;
			}
			I2C_SendData(I2C, tableWA[i2c_sensor]);
			irq_status++;
			break;
		case 8:
			if (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
				return;
			}
			I2C_SendData(I2C, writeData);  
			irq_status++;
			break;
		case 9:
			if (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
				return;
			}
			I2C_GenerateSTOP(I2C, ENABLE);
			I2C_ITConfig(I2C,I2C_IT_BUF, DISABLE);
			I2C_ITConfig(I2C,I2C_IT_EVT, DISABLE);
			break;
#endif
	}
}












uint16_t i2c2_ISR0(uint16_t d_address,uint16_t address)
{
	uint16_t r;
	switch (irq_status) {
		case 0:
			if (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_MODE_SELECT)) {
				return 0x0100;
			}
			I2C_Send7bitAddress(I2C,(d_address << 1), I2C_Direction_Transmitter);
			irq_status++;
			break;
		case 1:
			if (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
				return 0x0100;
			}
			I2C_SendData(I2C, address);  
			irq_status++;
			break;
		case 2:
			if (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
				return 0x0100;
			}
			I2C_GenerateSTOP(I2C, ENABLE);
			irq_status++;
			break;
		case 3:
			if (I2C_GetFlagStatus(I2C, I2C_FLAG_BUSY)) {
				return 0x0100;
			}
			I2C_GenerateSTART(I2C, ENABLE);
			irq_status++;
			break;
		case 4:
			if (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_MODE_SELECT)) {
				return 0x0100;
			}
			I2C_Send7bitAddress(I2C,(d_address << 1), I2C_Direction_Receiver);  
			irq_status++;
			break;
		case 5:
			if (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
				return 0x0100;
			}
			I2C_AcknowledgeConfig(I2C, DISABLE);   
			I2C_GenerateSTOP(I2C, ENABLE);
			irq_status++;
			break;
		case 6:
			if (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
				return 0x0100;
			}
			r = I2C_ReceiveData(I2C);
			if (flgNext) {
				irq_status = 0;
				I2C_GenerateSTART(I2C, ENABLE);
			} else {
				I2C_ITConfig(I2C,I2C_IT_BUF, DISABLE);
				I2C_ITConfig(I2C,I2C_IT_EVT, DISABLE);
			}
			return (0x00ff & r);
			break;
	}
	return 0x0100;
}
ErrorStatus I2C_CheckEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
  uint32_t lastevent = 0;
  uint32_t flag1 = 0, flag2 = 0;
  ErrorStatus status = ERROR;

  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_I2C_EVENT(I2C_EVENT));

  /* Read the I2Cx status register */
  flag1 = I2Cx->SR1;
  flag2 = I2Cx->SR2;
  flag2 = flag2 << 16;

  /* Get the last event value from I2C status register */
  lastevent = (flag1 | flag2) & FLAG_MASK;

  /* Check whether the last event contains the I2C_EVENT */
  if ((lastevent & I2C_EVENT) == I2C_EVENT)
  {
    /* SUCCESS: last event is equal to I2C_EVENT */
    status = SUCCESS;
  }
  else
  {
    /* ERROR: last event is different from I2C_EVENT */
    status = ERROR;
  }
  /* Return status */
  return status;
}

