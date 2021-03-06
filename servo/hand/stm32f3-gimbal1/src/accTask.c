#include <stdio.h>
#include <math.h>

#include "FreeRTOS.h"
#include "task.h"

#include "lsm9ds0.h"
#include "PWM.h"

extern volatile uint32_t LSM9DS0_Timeout;

#define ABS(x)         (x < 0) ? (-x) : x

#define L3G_Sensitivity_250dps     (float)   114.285f         /*!< gyroscope sensitivity with 250 dps full scale [LSB/dps] */
#define L3G_Sensitivity_500dps     (float)    57.1429f        /*!< gyroscope sensitivity with 500 dps full scale [LSB/dps] */
#define L3G_Sensitivity_2000dps    (float)    14.285f	      /*!< gyroscope sensitivity with 2000 dps full scale [LSB/dps] */
#define PI                         (float)     3.14159265f

#define LSM_Acc_Sensitivity_2g     (float)     1.0f            /*!< accelerometer sensitivity with 2 g full scale [LSB/mg] */
#define LSM_Acc_Sensitivity_4g     (float)     0.5f            /*!< accelerometer sensitivity with 4 g full scale [LSB/mg] */
#define LSM_Acc_Sensitivity_8g     (float)     0.25f           /*!< accelerometer sensitivity with 8 g full scale [LSB/mg] */
#define LSM_Acc_Sensitivity_16g    (float)     0.0834f         /*!< accelerometer sensitivity with 12 g full scale [LSB/mg] */

#define SERVO1(a)	setPWM2_2(1520 + a)
#define SERVO2(a)	setPWM3_1(1520 + a)
#define SERVO3(a)	setPWM12_2(1520 + a)
//#define SERVO1(a)	setPWM2_2(3040 + a)
//#define SERVO2(a)	setPWM3_1(3040 + a)
//#define SERVO3(a)	setPWM12_2(3040 + a)

struct ACCState {
	int waterMark;
	volatile int dataRead;

	// I2C DMA
	//DMA_InitTypeDef i2cRXDMA;

	// result
	volatile uint8_t RawData[10];

	volatile struct ACCStateI2CState {
		uint8_t RegAddrTMP, DeviceAddrTMP, finished;
		uint16_t NumByteToReadTMP;
		uint8_t *pBufferTMP;
	} ACCStateI2CState;
} ACCState;

//void ACC_configureDMA() {
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
//
//	// DMA Configuration for receiver
//	DMA_DeInit(DMA1_Channel7 );
//	ACCState.i2cRXDMA.DMA_BufferSize = 7;
//	ACCState.i2cRXDMA.DMA_MemoryBaseAddr = (uint32_t) & ACCState.RawData[0];
//	//uart_state.uartTXDMA.DMA_MemoryBaseAddr = (uint32_t) value;
//	ACCState.i2cRXDMA.DMA_PeripheralBaseAddr = (uint32_t) & I2C1 ->RXDR;
//	ACCState.i2cRXDMA.DMA_DIR = DMA_DIR_PeripheralSRC;
//	ACCState.i2cRXDMA.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//	ACCState.i2cRXDMA.DMA_MemoryInc = DMA_MemoryInc_Enable;
//	ACCState.i2cRXDMA.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
//	ACCState.i2cRXDMA.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//	ACCState.i2cRXDMA.DMA_Mode = DMA_Mode_Normal;
//	ACCState.i2cRXDMA.DMA_Priority = DMA_Priority_Medium;
//	ACCState.i2cRXDMA.DMA_M2M = DMA_M2M_Disable;
//
//	// Enable DMA Finished Interrupt
//	DMA_ClearITPendingBit(DMA1_IT_TC7);
//	DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);
//	//DMA_Init(DMA1_Channel7, &ACCState.i2cRXDMA);
//
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x9;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//}

void ACC_IOInit() {
	/* Configure GPIO PINs to detect Interrupts */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = LSM9DS0_I2C_INT1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LSM9DS0_I2C_INT1_GPIO_PORT, &GPIO_InitStructure);

	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Connect EXTI4 Line to PA0 pin */
	SYSCFG_EXTILineConfig(LSM9DS0_I2C_INT1_EXTI_PORT_SOURCE,
			LSM9DS0_I2C_INT1_EXTI_PIN_SOURCE);

	/* Configure EXTI4 line */
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = LSM9DS0_I2C_INT1_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable Interrupt */
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = LSM9DS0_I2C_INT1_EXTI_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* I2C Interrupts */
	//NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 13;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void I2C1_EV_IRQHandler() {
	/* TXIS Interrupt */
	if (I2C_GetITStatus(LSM9DS0_I2C, I2C_IT_TXIS) == SET) {
		I2C_ITConfig(LSM9DS0_I2C, I2C_IT_TXI, DISABLE);
		I2C_ClearITPendingBit(LSM9DS0_I2C, I2C_IT_TXI);

		I2C_ClearITPendingBit(LSM9DS0_I2C, I2C_IT_TCI);
		I2C_ITConfig(LSM9DS0_I2C, I2C_IT_TCI, ENABLE);

		/* Send Register address */
		I2C_SendData(LSM9DS0_I2C, (uint8_t) ACCState.ACCStateI2CState.RegAddrTMP);

		return;
	}

	/* TCI Interrupt */
	if (I2C_GetITStatus(LSM9DS0_I2C, I2C_IT_TCI) == SET) {

		I2C_ITConfig(LSM9DS0_I2C, I2C_IT_TCI, DISABLE);
		I2C_ClearITPendingBit(LSM9DS0_I2C, I2C_IT_TCI);

		I2C_ClearITPendingBit(LSM9DS0_I2C, I2C_IT_RXI);
		I2C_ITConfig(LSM9DS0_I2C, I2C_IT_RXI, ENABLE);

////		// Enable DMA
//		ACCState.i2cRXDMA.DMA_BufferSize = ACCState.ACCStateI2CState.NumByteToReadTMP;
//		ACCState.i2cRXDMA.DMA_MemoryBaseAddr = (uint32_t) &ACCState.RawData[0];
//
//
//		DMA_ClearITPendingBit(DMA1_IT_TC7);
//		DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);
//
//		DMA_Init(DMA1_Channel7, &ACCState.i2cRXDMA);
//		DMA_Cmd(DMA1_Channel7, ENABLE);
//
//		I2C_DMACmd(LSM9DS0_I2C, I2C_DMAReq_Rx, ENABLE);

		/* Configure slave address, nbytes, reload, end mode and start or stop generation */
		I2C_TransferHandling(LSM9DS0_I2C, ACCState.ACCStateI2CState.DeviceAddrTMP,
				ACCState.ACCStateI2CState.NumByteToReadTMP, I2C_AutoEnd_Mode,
				I2C_Generate_Start_Read);

		return;
	}

	/* RXI Interrupt */
	if (I2C_GetITStatus(LSM9DS0_I2C, I2C_IT_RXI) == SET) {

		I2C_ClearITPendingBit(LSM9DS0_I2C, I2C_IT_RXI);

		/* Read data from RXDR */
		*ACCState.ACCStateI2CState.pBufferTMP = I2C_ReceiveData(LSM9DS0_I2C );
		/* Point to the next location where the byte read will be saved */
		++ACCState.ACCStateI2CState.pBufferTMP;

		/* Decrement the read bytes counter */
		--ACCState.ACCStateI2CState.NumByteToReadTMP;

		if (ACCState.ACCStateI2CState.NumByteToReadTMP == 0) {
			I2C_ITConfig(LSM9DS0_I2C, I2C_IT_RXI, DISABLE);

			I2C_ClearITPendingBit(LSM9DS0_I2C, I2C_IT_STOPF);
			I2C_ITConfig(LSM9DS0_I2C, I2C_IT_STOPF, ENABLE);
		}

		return;
	}

	/* STOP signalized/detected */
	if (I2C_GetITStatus(LSM9DS0_I2C, I2C_IT_STOPF) == SET) {
		I2C_ITConfig(LSM9DS0_I2C, I2C_IT_STOPF, DISABLE);
		I2C_ClearITPendingBit(LSM9DS0_I2C, I2C_IT_STOPF);

		I2C_ClearFlag(LSM9DS0_I2C, I2C_ICR_STOPCF );
		ACCState.ACCStateI2CState.finished = 1;
		return;
	}
}

void DMA1_Channel7_IRQHandler(void) {
	I2C_ClearITPendingBit(LSM9DS0_I2C, I2C_IT_STOPF);
	I2C_ITConfig(LSM9DS0_I2C, I2C_IT_STOPF, ENABLE);

	DMA_Cmd(DMA1_Channel7, DISABLE);
	DMA_ClearITPendingBit(DMA1_IT_TC7);
}

/**
 * @brief  Reads a block of data from the LSM9DS0.
 * @param  DeviceAddr : specifies the slave address to be programmed(ACC_I2C_ADDRESS or MAG_I2C_ADDRESS).
 * @param  RegAddr : specifies the LSM9DS0 internal address register to read from.
 * @param  pBuffer : pointer to the buffer that receives the data read from the LSM303DLH.
 * @param  NumByteToRead : number of bytes to read from the LSM303DLH ( NumByteToRead >1  only for the Mgnetometer readinf).
 * @retval LSM9DS0 register value
 */
uint16_t LSM9DS0_ReadDMA(uint8_t DeviceAddr, uint8_t RegAddr,
		uint8_t* pBuffer, uint16_t NumByteToRead) {

	ACCState.ACCStateI2CState.RegAddrTMP = RegAddr;
	ACCState.ACCStateI2CState.NumByteToReadTMP = NumByteToRead;
	ACCState.ACCStateI2CState.pBufferTMP = pBuffer;
	ACCState.ACCStateI2CState.DeviceAddrTMP = DeviceAddr;

	//ACCState.i2cRXDMA.DMA_MemoryBaseAddr = (uint32_t) & pBuffer[0];

	/* Test on BUSY Flag */
	LSM9DS0_Timeout = LSM9DS0_LONG_TIMEOUT;
	while (I2C_GetFlagStatus(LSM9DS0_I2C, I2C_ISR_BUSY ) != RESET) {
		if ((LSM9DS0_Timeout--) == 0)
			return LSM9DS0_TIMEOUT_UserCallback();
	}

	/* Configure slave address, nbytes, reload, end mode and start or stop generation */
	I2C_ClearITPendingBit(LSM9DS0_I2C, I2C_IT_TXI);
	I2C_ITConfig(LSM9DS0_I2C, I2C_IT_TXI, ENABLE);

	/* Wait until TXIS flag is set */
	if (NumByteToRead > 1)
		ACCState.ACCStateI2CState.RegAddrTMP |= 0x80;

	ACCState.ACCStateI2CState.finished = 0;
	I2C_TransferHandling(LSM9DS0_I2C, DeviceAddr, 1, I2C_SoftEnd_Mode,
			I2C_Generate_Start_Write);

	while (ACCState.ACCStateI2CState.finished == 0) {
		if ((LSM9DS0_Timeout--) == 0)
			return LSM9DS0_TIMEOUT_UserCallback();
	}

	/* If all operations OK */
	return LSM9DS0_OK ;
}

/**
 * @brief  Configure the Mems to compass application.
 * @param  None
 * @retval None
 */
void ACC_CompassConfig(void) {
	LSM9DS0Gyro_InitTypeDef GYRO_Init;
	LSM9DS0Gyro_FilterConfigTypeDef GYRO_Filter;
	LSM9DS0Acc_InitTypeDef ACC_Init;
	LSM9DS0Acc_FilterConfigTypeDef ACC_Filter;
	LSM9DS0Mag_InitTypeDef MAG_Init;

	GYRO_Init.GyroOutput_DataRate = LSM9DS0_GODR_95HZ_25HZ;
	GYRO_Init.Axes_Enable         = LSM9DS0_AXES_ENABLE_G;
	GYRO_Init.INT_G_Enable        = LSM9DS0_INTG_MODE_INTERRUPT;
	GYRO_Init.PP_OD               = 0x00;
	GYRO_Init.DRDY_G_Enable       = 0x00;
	GYRO_Init.BlockData_Update    = LSM9DS0_BlockUpdate_Continous;
	GYRO_Init.Endianness          = LSM9DS0_BLE_LSB;
	GYRO_Init.GyroFull_Scale      = LSM9DS0_GFULLSCALE_500;
	GYRO_Init.GyroSelftest        = 0x00;
//	GYRO_Init.FIFO_Enable         = LSM9DS0_FIFO_MODE_ENA;
	GYRO_Init.FIFO_Enable         = LSM9DS0_FIFO_MODE_DIS;
	GYRO_Init.INT1Sel             = 0x0f;
	GYRO_Filter.HighPassFilter_Enable = LSM9DS0_GHPF_MODE_ENA;
//	LSM9DS0_GyroInit(&GYRO_Init);

	GYRO_Filter.HighPassFilter_Enable         = LSM9DS0_GHPF_MODE_ENA;
	GYRO_Filter.HighPassFilter_Mode_Selection = LSM9DS0_GHPM_NORMAL_MODE;
	GYRO_Filter.HighPassFilter_Cutoff         = 0x03; /*0.9Hz*/
//	LSM9DS0_GyroFilterConfig(&GYRO_Filter);

	/* Fill the accelerometer structure */
	ACC_Init.FIFO_Enable         = LSM9DS0_FIFO_MODE_DIS;
	ACC_Init.WTM_Enable          = LSM9DS0_WTM_MODE_ENA;
	ACC_Init.AccOutput_DataRate  = LSM9DS0_AODR_50_HZ;
	ACC_Init.BlockData_Update    = LSM9DS0_BlockUpdate_Continous;
	ACC_Init.Axes_Enable         = LSM9DS0_AXES_ENABLE;
	ACC_Init.AAF_Enable          = LSM9DS0_AHPF_MODE_ENA ;
	ACC_Init.AccFull_Scale       = LSM9DS0_AFULLSCALE_4G;
	ACC_Init.AccSelftest         = 0x00;
	LSM9DS0_AccInit(&ACC_Init);
	
	ACC_Filter.HighPassFilter_Enable         = LSM9DS0_AHPF_MODE_ENA;
	ACC_Filter.HighPassFilter_Mode_Selection = LSM9DS0_HPM_NORMAL_MODE;
	ACC_Filter.HighPassFilter_INT1 = 0x02;
	ACC_Filter.HighPassFilter_INT2 = 0x01;
	ACC_Filter.HighPassFilter_P1   = LSM9DS0_P1_DRDYA_ENA|LSM9DS0_P1_EMPTY_ENA;
	ACC_Filter.HighPassFilter_P2   = LSM9DS0_P2_DRDYM_ENA;
	LSM9DS0_AccFilterConfig(&ACC_Filter);
	
	/* Configure MEMS magnetometer main parameters: temp, working mode, full Scale and Data rate */
	MAG_Init.Power_Mode = 0;
	MAG_Init.Temperature_Sensor = LSM9DS0_TEMPSENSOR_DISABLE;
	MAG_Init.High_Resolution    = LSM9DS0_MHR_ENABLE;
	MAG_Init.MagOutput_DataRate = LSM9DS0_MODR_12_HZ;
	MAG_Init.MagFull_Scale      = LSM9DS0_MFULLSCALE_4G;
	MAG_Init.Working_Mode       = LSM9DS0_CONTINUOS_CONVERSION;
	LSM9DS0_MagInit(&MAG_Init);

	//LSM9DS0_AccClickITConfig();

	// Enable Interrupt for Int1 Watermark Threshold (data ready for readout)
	ACC_IOInit();
	//ACC_configureDMA();
	// TODO: Enable Interrupt for Watermarks and FIFO Mode
}

void EXTI4_IRQHandler() {
	EXTI_ClearITPendingBit(LSM9DS0_I2C_INT1_EXTI_LINE);
}

/**
 * @brief Read LSM9DS0 output register, and calculate the acceleration ACC=(1/SENSITIVITY)* (out_h*256+out_l)/16 (12 bit rappresentation)
 * @param pnData: pointer to float buffer where to store data
 * @retval None
 */
void ACC_CompassReadAcc(float* pfData) {
	int16_t pnRawData[3];
	uint8_t ctrlx[2];
	uint8_t buffer[6];

	uint8_t cDivider;
	uint8_t i = 0;
	float LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_2g;

	/* Read the register content */
//	LSM9DS0_ReadDMA(ACC_I2C_ADDRESS, LSM9DS0_CTRL_REG4_XM, ctrlx, 2);
//	LSM9DS0_ReadDMA(ACC_I2C_ADDRESS, LSM9DS0_OUT_X_L_A, buffer, 6);
//	LSM9DS0_Read(ACC_I2C_ADDRESS, LSM9DS0_CTRL_REG4_XM, ctrlx, 2);
	LSM9DS0_Read(ACC_I2C_ADDRESS, LSM9DS0_OUT_X_L_A, buffer, 6);
	for (i = 0; i < 3; i++) {
			pnRawData[i] = ((int16_t)((uint16_t) buffer[2 * i + 1] << 8) + buffer[2 * i]);
	}


//	if (ctrlx[1] & 0x40)
//		cDivider = 64;
//	else
//		cDivider = 16;

	/* check in the control register4 the data alignment*/
//	if (!(ctrlx[0] & 0x40) || (ctrlx[1] & 0x40)) /* Little Endian Mode or FIFO mode */
//	{
//		for (i = 0; i < 3; i++) {
//			pnRawData[i] = ((int16_t)((uint16_t) buffer[2 * i + 1] << 8)
//					+ buffer[2 * i]) / cDivider;
//		}
//	} else /* Big Endian Mode */
//	{
//		for (i = 0; i < 3; i++)
//			pnRawData[i] = ((int16_t)((uint16_t) buffer[2 * i] << 8)
//					+ buffer[2 * i + 1]) / cDivider;
//	}
	/* Read the register content */
//	LSM9DS0_Read(ACC_I2C_ADDRESS, LSM9DS0_CTRL_REG4_XM, ctrlx, 2);
//
//	LSM_Acc_Sensitivity = 0.25;

//	if (ctrlx[1] & 0x40) {
//		/* FIFO mode */
//		LSM_Acc_Sensitivity = 0.25;
//	} else {
//		/* normal mode */
//		/* switch the sensitivity value set in the CRTL4*/
//		switch (ctrlx[0] & 0x30) {
//		case LSM9DS0_AFULLSCALE_2G :
//			LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_2g;
//			break;
//		case LSM9DS0_AFULLSCALE_4G :
//			LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_4g;
//			break;
//		case LSM9DS0_AFULLSCALE_6G :
//			LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_8g;
//			break;
//		case LSM9DS0_AFULLSCALE_8G :
//			LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_8g;
//			break;
//		case LSM9DS0_AFULLSCALE_16G :
//			LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_16g;
//			break;
//		}
//	}

	/* Obtain the mg value for the three axis */
//	for (i = 0; i < 3; i++) {
//		pfData[i] = (float) pnRawData[i] / LSM_Acc_Sensitivity;
//	}
	for (i = 0; i < 3; i++) {
		pfData[i] = (float) pnRawData[i];
	}

}

/**
 * @brief  calculate the magnetic field Magn.
 * @param  pfData: pointer to the data out
 * @retval None
 */
void ACC_CompassReadMag(float* pfData) {
	static uint8_t buffer[6] = { 0 };
	uint8_t CTRLB = 0;
	uint16_t Magn_Sensitivity_XY = 0, Magn_Sensitivity_Z = 0;
	uint8_t i = 0;
	// todo remove, since this is static config
//	LSM9DS0_Read(MAG_I2C_ADDRESS, LSM9DS0_CRB_REG_M, &CTRLB, 1);

	LSM9DS0_Read(MAG_I2C_ADDRESS, LSM9DS0_OUT_X_H_M, buffer, 1);
	LSM9DS0_Read(MAG_I2C_ADDRESS, LSM9DS0_OUT_X_L_M, buffer + 1, 1);
	LSM9DS0_Read(MAG_I2C_ADDRESS, LSM9DS0_OUT_Y_H_M, buffer + 2, 1);
	LSM9DS0_Read(MAG_I2C_ADDRESS, LSM9DS0_OUT_Y_L_M, buffer + 3, 1);
	LSM9DS0_Read(MAG_I2C_ADDRESS, LSM9DS0_OUT_Z_H_M, buffer + 4, 1);
	LSM9DS0_Read(MAG_I2C_ADDRESS, LSM9DS0_OUT_Z_L_M, buffer + 5, 1);

//	LSM9DS0_ReadDMA(MAG_I2C_ADDRESS, LSM9DS0_OUT_X_H_M, buffer, 6);

	// now we have to resort the entries of buffer
	uint8_t tmp[2];
	tmp[0] = buffer[4];
	tmp[1] = buffer[5];

	buffer[4] = buffer[2];
	buffer[5] = buffer[3];

	buffer[2] = tmp[0];
	buffer[3] = tmp[1];

	/* Switch the sensitivity set in the CRTLB*/
#if 0
	switch (CTRLB & 0xE0) {
	case LSM9DS0_FS_1_3_GA :
		Magn_Sensitivity_XY = LSM9DS0_M_SENSITIVITY_XY_1_3Ga;
		Magn_Sensitivity_Z = LSM9DS0_M_SENSITIVITY_Z_1_3Ga;
		break;
	case LSM9DS0_FS_1_9_GA :
		Magn_Sensitivity_XY = LSM9DS0_M_SENSITIVITY_XY_1_9Ga;
		Magn_Sensitivity_Z = LSM9DS0_M_SENSITIVITY_Z_1_9Ga;
		break;
	case LSM9DS0_FS_2_5_GA :
		Magn_Sensitivity_XY = LSM9DS0_M_SENSITIVITY_XY_2_5Ga;
		Magn_Sensitivity_Z = LSM9DS0_M_SENSITIVITY_Z_2_5Ga;
		break;
	case LSM9DS0_FS_4_0_GA :
		Magn_Sensitivity_XY = LSM9DS0_M_SENSITIVITY_XY_4Ga;
		Magn_Sensitivity_Z = LSM9DS0_M_SENSITIVITY_Z_4Ga;
		break;
	case LSM9DS0_FS_4_7_GA :
		Magn_Sensitivity_XY = LSM9DS0_M_SENSITIVITY_XY_4_7Ga;
		Magn_Sensitivity_Z = LSM9DS0_M_SENSITIVITY_Z_4_7Ga;
		break;
	case LSM9DS0_FS_5_6_GA :
		Magn_Sensitivity_XY = LSM9DS0_M_SENSITIVITY_XY_5_6Ga;
		Magn_Sensitivity_Z = LSM9DS0_M_SENSITIVITY_Z_5_6Ga;
		break;
	case LSM9DS0_FS_8_1_GA :
		Magn_Sensitivity_XY = LSM9DS0_M_SENSITIVITY_XY_8_1Ga;
		Magn_Sensitivity_Z = LSM9DS0_M_SENSITIVITY_Z_8_1Ga;
		break;
	}
#endif
	Magn_Sensitivity_XY = LSM9DS0_M_SENSITIVITY_XY_1_3Ga;
	Magn_Sensitivity_Z = LSM9DS0_M_SENSITIVITY_Z_1_3Ga;

	for (i = 0; i < 2; i++) {
		pfData[i] = (float) ((int16_t)(
				((uint16_t) buffer[2 * i] << 8) + buffer[2 * i + 1]) * 1000)
				/ Magn_Sensitivity_XY;
	}
	pfData[2] = (float) ((int16_t)(((uint16_t) buffer[4] << 8) + buffer[5])
			* 1000) / Magn_Sensitivity_Z;
}

/**
 * @brief  Basic management of the timeout situation.
 * @param  None.
 * @retval None.
 */
uint32_t LSM9DS0_TIMEOUT_UserCallback(void) {
	printf("LSM9DS0_TIMEOUT\n");
	return 0;
}

/**
 * Internal variables
 */
float MagBuffer[3] = { 0.0f }, AccBuffer[3] = { 0.0f }, Buffer[3] = { 0.0f };
float fNormAcc, fSinRoll, fCosRoll, fSinPitch, fCosPitch = 0.0f, fSinYaw,
		fCosYaw, RollAng = 0.0f, PitchAng = 0.0f;
float fTiltedX, fTiltedY = 0.0f, HeadingValue = 0.0f;
float roll = 0.0f, pitch = 0.0f, yaw = 0.0f;

void accTask(void *pvParameters) {
	char buf[100];
	int duty1;

	duty1 = 0;

	ACC_CompassConfig();
	PWM_Init();

#if 0
	vTaskDelay(10000);
	LSM9DS0_Read(MAG_I2C_ADDRESS, LSM9DS0_WHOAMI_XM, buf, 1);
	if(buf[0] = 0x49){
		CDC_Send_DATA ("LSM9DS0 MAG OK\n",15);
	}else{
		CDC_Send_DATA ("LSM9DS0 MAG NG\n",15);
	}
	vTaskDelay(1000);
	LSM9DS0_Read(GYRO_I2C_ADDRESS, LSM9DS0_WHOAMI_G, buf, 1);
	if(buf[0] = 0xD4){
		CDC_Send_DATA ("LSM9DS0 GYRO OK\n",16);
	}else{
		CDC_Send_DATA ("LSM9DS0 GYRO NG\n",16);
	}
#endif
	SERVO1(duty1);
	SERVO2(duty1);
	SERVO3(duty1);

	while (1) {
		switch(duty1){
			case 0:
				duty1 = 500;
				break;
			case 500:
				duty1 = -500;
				break;
			case -500:
				duty1 = 0;
				break;
			default:
				duty1 = 0;
				break;
		}
		SERVO1(duty1);
		SERVO2(duty1);
		SERVO3(duty1);
		vTaskDelay(1000);
		/* Read Compass data */
		ACC_CompassReadMag(MagBuffer);
		ACC_CompassReadAcc(AccBuffer);
		for (int i = 0; i < 3; i++)	AccBuffer[i] /= 100.0f;

		fNormAcc = sqrtf(
				(AccBuffer[0] * AccBuffer[0]) + (AccBuffer[1] * AccBuffer[1])
						+ (AccBuffer[2] * AccBuffer[2]));

		fCosRoll = -AccBuffer[1] / fNormAcc;
		fSinRoll = sqrtf(1.0 - (fCosRoll * fCosRoll));

		fCosPitch = AccBuffer[0] / fNormAcc;
		fSinPitch = sqrtf(1.0 - (fCosPitch * fCosPitch));

		fCosYaw = AccBuffer[2] / fNormAcc;
		fSinYaw = sqrtf(1.0 - (fCosYaw * fCosYaw));

		roll = atan2f(fSinRoll, fCosRoll) * 180 / PI;
		pitch = atan2f(fSinPitch, fCosPitch) * 180 / PI;
		yaw = atan2f(fSinYaw, fCosYaw) * 180 / PI;

		if (fSinRoll > 0) {
			if (fCosRoll > 0) {
				RollAng = acosf(fCosRoll) * 180 / PI;
			} else {
				RollAng = acosf(fCosRoll) * 180 / PI + 180;
			}
		} else {
			if (fCosRoll > 0) {
				RollAng = acosf(fCosRoll) * 180 / PI + 360;
			} else {
				RollAng = acosf(fCosRoll) * 180 / PI + 180;
			}
		}

		if (fSinPitch > 0) {
			if (fCosPitch > 0) {
				PitchAng = acosf(fCosPitch) * 180 / PI;
			} else {
				PitchAng = acosf(fCosPitch) * 180 / PI + 180;
			}
		} else {
			if (fCosPitch > 0) {
				PitchAng = acosf(fCosPitch) * 180 / PI + 360;
			} else {
				PitchAng = acosf(fCosPitch) * 180 / PI + 180;
			}
		}

		if (RollAng >= 360) {
			RollAng = RollAng - 360;
		}

		if (PitchAng >= 360) {
			PitchAng = PitchAng - 360;
		}

		fTiltedX = MagBuffer[0] * fCosPitch + MagBuffer[2] * fSinPitch;
		fTiltedY = MagBuffer[0] * fSinRoll * fSinPitch + MagBuffer[1] * fCosRoll
				- MagBuffer[1] * fSinRoll * fCosPitch;

		HeadingValue = (float) ((atan2f((float) fTiltedY, (float) fTiltedX))
				* 180) / PI;

		if (HeadingValue < 0) {
			HeadingValue = HeadingValue + 360;
		}

	//	printf("ACC: Roll: %f, Pitch: %f, Yaw: %f\n", roll, pitch, yaw);
	//	sprintf(buf,"ACC: Roll: %f, Pitch: %f, Yaw: %f\n", roll, pitch, yaw);
		sprintf(buf,"ACC: Roll: %d, Pitch: %d, Yaw: %d\n", roll*1000, pitch*1000, yaw*1000);
        CDC_Send_DATA ((unsigned char*)buf,strlen(buf));
//      CDC_Send_DATA ("Hoge",4);
	}
}
