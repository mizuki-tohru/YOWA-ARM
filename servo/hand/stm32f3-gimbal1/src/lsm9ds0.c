/**
 ******************************************************************************
 * @file    stm32f3_discovery_LSM9DS0.c
 * @author  MCD Application Team
 * @version V1.1.0
 * @date    20-September-2012
 * @brief   This file provides a set of functions needed to manage the LSM9DS0
 *          MEMS accelerometer available on STM32F3-Discovery Kit.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
 *
 * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "LSM9DS0.h"

/** @defgroup STM32F3_DISCOVERY_LSM9DS0_Private_Variables
 * @{
 */__IO uint32_t LSM9DS0_Timeout = LSM9DS0_LONG_TIMEOUT;
/**
 * @}
 */


/**
 * @brief  Set LSM9DS0 Initialization.
 * @param  LSM9DS0_InitStruct: pointer to a LSM9DS0_InitTypeDef structure
 *         that contains the configuration setting for the LSM9DS0.
 * @retval None
 */
void LSM9DS0_GyroInit(LSM9DS0Gyro_InitTypeDef *LSM9DS0_InitStruct) {
	uint8_t tmpreg;

	/* Configure the low level interface ---------------------------------------*/
	LSM9DS0_LowLevel_Init();
	
	LSM9DS0_Read(GYRO_I2C_ADDRESS, LSM9DS0_CTRL_REG1_G, &tmpreg, 1);
	tmpreg |= (uint8_t)(
	LSM9DS0_InitStruct->GyroOutput_DataRate|\
	LSM9DS0_InitStruct->Axes_Enable);
	LSM9DS0_Write(GYRO_I2C_ADDRESS, LSM9DS0_CTRL_REG1_G, &tmpreg);
	
	LSM9DS0_Read(GYRO_I2C_ADDRESS, LSM9DS0_CTRL_REG3_G, &tmpreg, 1);
	tmpreg |= (uint8_t)(
	LSM9DS0_InitStruct->INT_G_Enable|\
	LSM9DS0_InitStruct->PP_OD|\
	LSM9DS0_InitStruct->DRDY_G_Enable);
	LSM9DS0_Write(GYRO_I2C_ADDRESS, LSM9DS0_CTRL_REG3_G, &tmpreg);
	
	LSM9DS0_Read(GYRO_I2C_ADDRESS, LSM9DS0_CTRL_REG4_G, &tmpreg, 1);
	tmpreg |= (uint8_t)(
	LSM9DS0_InitStruct->BlockData_Update|\
	LSM9DS0_InitStruct->Endianness|\
	LSM9DS0_InitStruct->GyroFull_Scale|\
	LSM9DS0_InitStruct->GyroSelftest);
	LSM9DS0_Write(GYRO_I2C_ADDRESS, LSM9DS0_CTRL_REG4_G, &tmpreg);
	
	LSM9DS0_Read(GYRO_I2C_ADDRESS, LSM9DS0_CTRL_REG5_G, &tmpreg, 1);
	tmpreg |= (uint8_t)(
	LSM9DS0_InitStruct->FIFO_Enable|\
	LSM9DS0_InitStruct->INT1Sel);
	LSM9DS0_Write(GYRO_I2C_ADDRESS, LSM9DS0_CTRL_REG5_G, &tmpreg);
}
void LSM9DS0_GyroFilterConfig(
		LSM9DS0Gyro_FilterConfigTypeDef *LSM9DS0_FilterStruct) {
	uint8_t tmpreg;

	/* Read CTRL_REG2 register */
	LSM9DS0_Read(GYRO_I2C_ADDRESS, LSM9DS0_CTRL_REG5_G, &tmpreg, 1);
	tmpreg |= (uint8_t)(
	LSM9DS0_FilterStruct->HighPassFilter_Enable);
	LSM9DS0_Write(GYRO_I2C_ADDRESS, LSM9DS0_CTRL_REG5_G, &tmpreg);

	LSM9DS0_Read(GYRO_I2C_ADDRESS, LSM9DS0_CTRL_REG2_G, &tmpreg, 1);
	tmpreg |= (uint8_t)(
	LSM9DS0_FilterStruct->HighPassFilter_Mode_Selection|\
	LSM9DS0_FilterStruct->HighPassFilter_Cutoff);
	LSM9DS0_Write(GYRO_I2C_ADDRESS, LSM9DS0_CTRL_REG2_G, &tmpreg);
}

void LSM9DS0_GyroRebootCmd(void) {
	uint8_t tmpreg;

	/* Read CTRL_REG5 register */
	LSM9DS0_Read(GYRO_I2C_ADDRESS, LSM9DS0_CTRL_REG5_G, &tmpreg, 1);
	/* Enable or Disable the reboot memory */
	tmpreg |= LSM9DS0_BOOT_REBOOTMEMORY;
	/* Write value to ACC MEMS CTRL_REG5 regsister */
	LSM9DS0_Write(GYRO_I2C_ADDRESS, LSM9DS0_CTRL_REG5_G, &tmpreg);
}
 
 
void LSM9DS0_AccInit(LSM9DS0Acc_InitTypeDef *LSM9DS0_InitStruct) {
	uint8_t tmpreg;

	/* Configure the low level interface ---------------------------------------*/
	LSM9DS0_LowLevel_Init();

	LSM9DS0_Read(ACC_I2C_ADDRESS, LSM9DS0_CTRL_REG0_XM, &tmpreg, 1);
	tmpreg |= (uint8_t)(
	LSM9DS0_InitStruct->FIFO_Enable|\
	LSM9DS0_InitStruct->WTM_Enable);
	LSM9DS0_Write(ACC_I2C_ADDRESS, LSM9DS0_CTRL_REG0_XM, &tmpreg);

	LSM9DS0_Read(ACC_I2C_ADDRESS, LSM9DS0_CTRL_REG1_XM, &tmpreg, 1);
	tmpreg |= (uint8_t)(
	LSM9DS0_InitStruct->AccOutput_DataRate|\
	LSM9DS0_InitStruct->BlockData_Update|\
	LSM9DS0_InitStruct->Axes_Enable); /* Axes enable */
	LSM9DS0_Write(ACC_I2C_ADDRESS, LSM9DS0_CTRL_REG1_XM, &tmpreg);

	LSM9DS0_Read(ACC_I2C_ADDRESS, LSM9DS0_CTRL_REG2_XM, &tmpreg, 1);
	tmpreg |= (uint8_t)(
	LSM9DS0_InitStruct->AAF_Enable|\
	LSM9DS0_InitStruct->AccFull_Scale|\
	LSM9DS0_InitStruct->AccSelftest);
	LSM9DS0_Write(ACC_I2C_ADDRESS, LSM9DS0_CTRL_REG2_XM, &tmpreg);
}

/**
 * @brief  Reboot memory content of LSM9DS0
 * @param  None
 * @retval None
 */
void LSM9DS0_AccRebootCmd(void) {
	uint8_t tmpreg;

	/* Read CTRL_REG5 register */
	LSM9DS0_Read(ACC_I2C_ADDRESS, LSM9DS0_CTRL_REG0_XM, &tmpreg, 1);
	/* Enable or Disable the reboot memory */
	tmpreg |= LSM9DS0_BOOT_REBOOTMEMORY;
	/* Write value to ACC MEMS CTRL_REG5 regsister */
	LSM9DS0_Write(ACC_I2C_ADDRESS, LSM9DS0_CTRL_REG0_XM, &tmpreg);
}

/**
 * @brief  Set High Pass Filter Modality
 * @param  LSM9DS0_FilterStruct: pointer to a LSM9DS0_FilterConfigTypeDef structure
 *         that contains the configuration setting for the LSM9DS0.
 * @retval None
 */
void LSM9DS0_AccFilterConfig(
		LSM9DS0Acc_FilterConfigTypeDef *LSM9DS0_FilterStruct) {
	uint8_t tmpreg;

	/* Read CTRL_REG2 register */
	LSM9DS0_Read(ACC_I2C_ADDRESS, LSM9DS0_CTRL_REG0_XM, &tmpreg, 1);
	tmpreg |= (uint8_t)(
	LSM9DS0_FilterStruct->HighPassFilter_Mode_Selection|\
	LSM9DS0_FilterStruct->HighPassFilter_INT1|\
	LSM9DS0_FilterStruct->HighPassFilter_INT2); 
	LSM9DS0_Write(ACC_I2C_ADDRESS, LSM9DS0_CTRL_REG0_XM, &tmpreg);

	LSM9DS0_Read(ACC_I2C_ADDRESS, LSM9DS0_CTRL_REG3_XM, &tmpreg, 1);
	tmpreg = LSM9DS0_FilterStruct->HighPassFilter_P1;
	LSM9DS0_Write(ACC_I2C_ADDRESS, LSM9DS0_CTRL_REG3_XM, &tmpreg);
	LSM9DS0_Read(ACC_I2C_ADDRESS, LSM9DS0_CTRL_REG4_XM, &tmpreg, 1);
	tmpreg = LSM9DS0_FilterStruct->HighPassFilter_P2;
	LSM9DS0_Write(ACC_I2C_ADDRESS, LSM9DS0_CTRL_REG4_XM, &tmpreg);
}

/**
 * @brief  Enable or Disable High Pass Filter
 * @param  HighPassFilterState: new state of the High Pass Filter feature.
 *      This parameter can be:
 *         @arg: LSM9DS0_HighPassFilter_DISABLE
 *         @arg: LSM9DS0_HighPassFilter_ENABLE
 * @retval None
 */
void LSM9DS0_AccFilterCmd(uint8_t HighPassFilterState) {
	uint8_t tmpreg;

	/* Read CTRL_REG2 register */
	LSM9DS0_Read(ACC_I2C_ADDRESS, LSM9DS0_CTRL_REG2_XM, &tmpreg, 1);

	tmpreg &= 0xF7;

	tmpreg |= HighPassFilterState;

	/* Write value to ACC MEMS CTRL_REG2 regsister */
	LSM9DS0_Write(ACC_I2C_ADDRESS, LSM9DS0_CTRL_REG2_XM, &tmpreg);
}

/**
 * @brief  Enable or Disable High Pass Filter on CLick
 * @param  HighPassFilterState: new state of the High Pass Filter feature.
 *      This parameter can be:
 *         @arg: LSM9DS0_HighPassFilter_DISABLE
 *         @arg: LSM9DS0_HighPassFilter_ENABLE
 * @retval None
 */
void LSM9DS0_AccFilterClickCmd(uint8_t HighPassFilterClickState) {
	uint8_t tmpreg;

	/* Read CTRL_REG2 register */
	LSM9DS0_Read(ACC_I2C_ADDRESS, LSM9DS0_CTRL_REG2_XM, &tmpreg, 1);

	tmpreg &= 0xFB;

	tmpreg |= HighPassFilterClickState;

	/* Write value to ACC MEMS CTRL_REG2 regsister */
	LSM9DS0_Write(ACC_I2C_ADDRESS, LSM9DS0_CTRL_REG2_XM, &tmpreg);
}

/**
 * @brief Set LSM9DS0 Interrupt1 configuration
 * @param  LSM9DS0_IT: specifies the LSM9DS0 interrupt source to be enabled.
 *           This parameter can be any combination of the following values:
 *         @arg   LSM9DS0_IT1_CLICK
 *         @arg   LSM9DS0_IT1_AOI1
 *         @arg   LSM9DS0_IT1_AOI2
 *         @arg   LSM9DS0_IT1_DRY1
 *         @arg   LSM9DS0_IT1_WTM
 *         @arg   LSM9DS0_IT1_OVERRUN
 * @param  NewState: new state of the selected LSM9DS0 interrupts.
 *          This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void LSM9DS0_AccIT1Config(uint8_t LSM9DS0_IT, FunctionalState NewState) {
	uint8_t tmpval = 0x00;

	/* Read CTRL_REG3 register */
	LSM9DS0_Read(ACC_I2C_ADDRESS, LSM9DS0_CTRL_REG3_XM, &tmpval, 1);

	tmpval &= ~LSM9DS0_IT;

	if (NewState != DISABLE) {
		tmpval |= LSM9DS0_IT;
	} else {
		/* Disable the selected interrupt */
		tmpval = ~LSM9DS0_IT;
	}

	/* Write value to MEMS CTRL_REG3 register */
	LSM9DS0_Write(ACC_I2C_ADDRESS, LSM9DS0_CTRL_REG3_XM, &tmpval);
}

/**
 * @brief Set LSM9DS0 Interrupt2 configuration
 * @param  LSM9DS0_IT: specifies the LSM9DS0 interrupt source to be enabled.
 *           This parameter can be any combination of the following values:
 *         @arg   LSM9DS0_IT2_CLICK2
 *         @arg   LSM9DS0_IT2_INT1
 *         @arg   LSM9DS0_IT2_INT2
 *         @arg   LSM9DS0_IT2_BOOT
 *         @arg   LSM9DS0_IT2_ACT
 *         @arg   LSM9DS0_IT2_HLACTIVE
 * @param  NewState: new state of the selected LSM9DS0 interrupts.
 *          This parameter can be: ENABLE or DISABLE.
 * @retval None
 */
void LSM9DS0_AccIT2Config(uint8_t LSM9DS0_IT, FunctionalState NewState) {
	uint8_t tmpval = 0x00;

	/* Read CTRL_REG3 register */
	LSM9DS0_Read(ACC_I2C_ADDRESS, LSM9DS0_CTRL_REG6_XM, &tmpval, 1);

	tmpval &= ~LSM9DS0_IT;

	if (NewState != DISABLE) {
		tmpval |= LSM9DS0_IT;
	} else {
		/* Disable the selected interrupt */
		tmpval = ~LSM9DS0_IT;
	}

	/* Write value to MEMS CTRL_REG3 register */
	LSM9DS0_Write(ACC_I2C_ADDRESS, LSM9DS0_CTRL_REG6_XM, &tmpval);
}

/**
 * @brief  INT1 interrupt config
 * @param  ITCombination: Or or And combination
 *         ITAxes: axes to be enabled
 *         NewState: Enable or Disable
 * @retval None
 */
void LSM9DS0_AccINT1InterruptConfig(uint8_t ITCombination, uint8_t ITAxes,
		FunctionalState NewState) {
	uint8_t tmpval = ITCombination;

	/* Read INT1_CFR register */
	LSM9DS0_Read(ACC_I2C_ADDRESS, LSM9DS0_INT1_CFG, &tmpval, 1);

	if (NewState != DISABLE) {
		tmpval |= ITAxes;
	} else {
		/* Disable the selected interrupt */
		tmpval = (~ITAxes) | ITCombination;
	}

	/* Write value to MEMS INT1_CFR register */
	LSM9DS0_Write(ACC_I2C_ADDRESS, LSM9DS0_INT1_CFG, &tmpval);
}

/**
 * @brief  INT1 interrupt config
 * @param  ITCombination: Or or And combination
 *         ITAxes: axes to be enabled
 *         NewState: Enable or Disable
 * @retval None
 */
void LSM9DS0_AccINT2InterruptConfig(uint8_t ITCombination, uint8_t ITAxes,
		FunctionalState NewState) {
	uint8_t tmpval = ITCombination;

	/* Read INT2_CFR register */
	LSM9DS0_Read(ACC_I2C_ADDRESS, LSM9DS0_INT2_CFG, &tmpval, 1);

	if (NewState != DISABLE) {
		tmpval |= ITAxes;
	} else {
		/* Disable the selected interrupt */
		tmpval = (~ITAxes) | ITCombination;
	}

	/* Write value to MEMS INT2_CFR register */
	LSM9DS0_Write(ACC_I2C_ADDRESS, LSM9DS0_INT2_CFG, &tmpval);
}

/**
 * @brief  INT1 interrupt config
 * @param  ITCombination: Or or And combination
 *         ITAxes: axes to be enabled
 *         NewState: Enable or Disable
 * @retval None
 */
void LSM9DS0_AccClickITConfig(uint8_t ITClick, FunctionalState NewState) {
	uint8_t tmpval;

	/* Read CLICK_CFR register */
	LSM9DS0_Read(ACC_I2C_ADDRESS, LSM9DS0_CLICK_CFG_A, &tmpval, 1);

	if (NewState != DISABLE) {
		tmpval |= ITClick;
	} else {
		/* Disable the selected interrupt */
		tmpval = ~ITClick;
	}

	/* Write value to MEMS CLICK_CFR register */
	LSM9DS0_Write(ACC_I2C_ADDRESS, LSM9DS0_CLICK_CFG_A, &tmpval);
}

/**
 * @brief  Get status for Acc LSM9DS0 data
 * @param  None
 * @retval Data status in a LSM9DS0 Data register
 */
uint8_t LSM9DS0_AccGetDataStatus(void) {
	uint8_t tmpreg;

	/* Read Mag STATUS register */
	LSM9DS0_Read(ACC_I2C_ADDRESS, LSM9DS0_STATUS_REG_A, &tmpreg, 1);

	return tmpreg;
}

/**
 * @brief  Set LSM9DS0 Mag Initialization.
 * @param  LSM9DS0_InitStruct: pointer to a LSM9DS0_MagInitTypeDef structure
 *         that contains the configuration setting for the LSM9DS0.
 * @retval None
 */
void LSM9DS0_MagInit(LSM9DS0Mag_InitTypeDef *LSM9DS0_InitStruct) {
	uint8_t cra_regm = 0x00, crb_regm = 0x00, mr_regm = 0x00;

	/* Configure the low level interface ---------------------------------------*/
	LSM9DS0_LowLevel_Init();

	/* Configure MEMS: temp and Data rate */
	cra_regm |= (uint8_t)(
			LSM9DS0_InitStruct->Temperature_Sensor
					| LSM9DS0_InitStruct->MagOutput_DataRate);

	/* Configure MEMS: full Scale */
	crb_regm |= (uint8_t)(LSM9DS0_InitStruct->MagFull_Scale);

	/* Configure MEMS: working mode */
	mr_regm |= (uint8_t)(LSM9DS0_InitStruct->Working_Mode);

	/* Write value to Mag MEMS CRA_REG regsister */
//	LSM9DS0_Write(MAG_I2C_ADDRESS, LSM9DS0_CRA_REG_M, &cra_regm);

	/* Write value to Mag MEMS CRB_REG regsister */
//	LSM9DS0_Write(MAG_I2C_ADDRESS, LSM9DS0_CRB_REG_M, &crb_regm);

	/* Write value to Mag MEMS MR_REG regsister */
//	LSM9DS0_Write(MAG_I2C_ADDRESS, LSM9DS0_MR_REG_M, &mr_regm);
}

/**
 * @brief  Get status for Mag LSM9DS0 data
 * @param  None
 * @retval Data status in a LSM9DS0 Data register
 */
uint8_t LSM9DS0_MagGetDataStatus(void) {
	uint8_t tmpreg;

	/* Read Mag STATUS register */
	LSM9DS0_Read(MAG_I2C_ADDRESS, LSM9DS0_SR_REG_M, &tmpreg, 1);

	return tmpreg;
}

/**
 * @brief  Writes one byte to the LSM9DS0.
 * @param  DeviceAddr : specifies the slave address to be programmed.
 * @param  RegAddr : specifies the LSM9DS0 register to be written.
 * @param  pBuffer : pointer to the buffer  containing the data to be written to the LSM303DLH.
 * @retval LSM9DS0 Status
 */
uint16_t LSM9DS0_Write(uint8_t DeviceAddr, uint8_t RegAddr, uint8_t* pBuffer) {
	/* Test on BUSY Flag */
	LSM9DS0_Timeout = LSM9DS0_LONG_TIMEOUT;
	while (I2C_GetFlagStatus(LSM9DS0_I2C, I2C_ISR_BUSY ) != RESET) {
		if ((LSM9DS0_Timeout--) == 0)
			return LSM9DS0_TIMEOUT_UserCallback();
	}

	/* Configure slave address, nbytes, reload, end mode and start or stop generation */
	I2C_TransferHandling(LSM9DS0_I2C, DeviceAddr, 1, I2C_Reload_Mode,
			I2C_Generate_Start_Write);

	/* Wait until TXIS flag is set */
	LSM9DS0_Timeout = LSM9DS0_LONG_TIMEOUT;
	while (I2C_GetFlagStatus(LSM9DS0_I2C, I2C_ISR_TXIS ) == RESET) {
		if ((LSM9DS0_Timeout--) == 0)
			return LSM9DS0_TIMEOUT_UserCallback();
	}

	/* Send Register address */
	I2C_SendData(LSM9DS0_I2C, (uint8_t) RegAddr);

	/* Wait until TCR flag is set */
	LSM9DS0_Timeout = LSM9DS0_LONG_TIMEOUT;
	while (I2C_GetFlagStatus(LSM9DS0_I2C, I2C_ISR_TCR ) == RESET) {
		if ((LSM9DS0_Timeout--) == 0)
			return LSM9DS0_TIMEOUT_UserCallback();
	}

	/* Configure slave address, nbytes, reload, end mode and start or stop generation */
	I2C_TransferHandling(LSM9DS0_I2C, DeviceAddr, 1, I2C_AutoEnd_Mode,
			I2C_No_StartStop);

	/* Wait until TXIS flag is set */
	LSM9DS0_Timeout = LSM9DS0_LONG_TIMEOUT;
	while (I2C_GetFlagStatus(LSM9DS0_I2C, I2C_ISR_TXIS ) == RESET) {
		if ((LSM9DS0_Timeout--) == 0)
			return LSM9DS0_TIMEOUT_UserCallback();
	}

	/* Write data to TXDR */
	I2C_SendData(LSM9DS0_I2C, *pBuffer);

	/* Wait until STOPF flag is set */
	LSM9DS0_Timeout = LSM9DS0_LONG_TIMEOUT;
	while (I2C_GetFlagStatus(LSM9DS0_I2C, I2C_ISR_STOPF ) == RESET) {
		if ((LSM9DS0_Timeout--) == 0)
			return LSM9DS0_TIMEOUT_UserCallback();
	}

	/* Clear STOPF flag */
	I2C_ClearFlag(LSM9DS0_I2C, I2C_ICR_STOPCF );

	return LSM9DS0_OK ;
}

/**
 * @brief  Reads a block of data from the LSM9DS0.
 * @param  DeviceAddr : specifies the slave address to be programmed(ACC_I2C_ADDRESS or MAG_I2C_ADDRESS).
 * @param  RegAddr : specifies the LSM9DS0 internal address register to read from.
 * @param  pBuffer : pointer to the buffer that receives the data read from the LSM303DLH.
 * @param  NumByteToRead : number of bytes to read from the LSM303DLH ( NumByteToRead >1  only for the Mgnetometer readinf).
 * @retval LSM9DS0 register value
 */
uint16_t LSM9DS0_Read(uint8_t DeviceAddr, uint8_t RegAddr, uint8_t* pBuffer,
		uint16_t NumByteToRead) {
	/* Test on BUSY Flag */
	LSM9DS0_Timeout = LSM9DS0_LONG_TIMEOUT;

	while (I2C_GetFlagStatus(LSM9DS0_I2C, I2C_ISR_BUSY ) != RESET) {
		if ((LSM9DS0_Timeout--) == 0)
			return LSM9DS0_TIMEOUT_UserCallback();
	}

	/* Configure slave address, nbytes, reload, end mode and start or stop generation */
	I2C_TransferHandling(LSM9DS0_I2C, DeviceAddr, 1, I2C_SoftEnd_Mode,
			I2C_Generate_Start_Write);

	/* Wait until TXIS flag is set */
	LSM9DS0_Timeout = LSM9DS0_LONG_TIMEOUT;
	while (I2C_GetFlagStatus(LSM9DS0_I2C, I2C_ISR_TXIS ) == RESET) {
		if ((LSM9DS0_Timeout--) == 0)
			return LSM9DS0_TIMEOUT_UserCallback();
	}

	if (NumByteToRead > 1)
		RegAddr |= 0x80;

	/* Send Register address */
	I2C_SendData(LSM9DS0_I2C, (uint8_t) RegAddr);

	/* Wait until TC flag is set */
	LSM9DS0_Timeout = LSM9DS0_LONG_TIMEOUT;
	while (I2C_GetFlagStatus(LSM9DS0_I2C, I2C_ISR_TC ) == RESET) {
		if ((LSM9DS0_Timeout--) == 0)
			return LSM9DS0_TIMEOUT_UserCallback();
	}

	/* Configure slave address, nbytes, reload, end mode and start or stop generation */
	I2C_TransferHandling(LSM9DS0_I2C, DeviceAddr, NumByteToRead,
			I2C_AutoEnd_Mode, I2C_Generate_Start_Read);

	/* Wait until all data are received */
	while (NumByteToRead) {
		/* Wait until RXNE flag is set */
		LSM9DS0_Timeout = LSM9DS0_LONG_TIMEOUT;
		while (I2C_GetFlagStatus(LSM9DS0_I2C, I2C_ISR_RXNE ) == RESET) {
			if ((LSM9DS0_Timeout--) == 0)
				return LSM9DS0_TIMEOUT_UserCallback();
		}

		/* Read data from RXDR */
		*pBuffer = I2C_ReceiveData(LSM9DS0_I2C );
		/* Point to the next location where the byte read will be saved */
		pBuffer++;

		/* Decrement the read bytes counter */
		NumByteToRead--;
	}

	/* Wait until STOPF flag is set */
	LSM9DS0_Timeout = LSM9DS0_LONG_TIMEOUT;
	while (I2C_GetFlagStatus(LSM9DS0_I2C, I2C_ISR_STOPF ) == RESET) {
		if ((LSM9DS0_Timeout--) == 0)
			return LSM9DS0_TIMEOUT_UserCallback();
	}

	/* Clear STOPF flag */
	I2C_ClearFlag(LSM9DS0_I2C, I2C_ICR_STOPCF );

	/* If all operations OK */
	return LSM9DS0_OK ;
}
/**
 * @brief  Initializes the low level interface used to drive the LSM9DS0
 * @param  None
 * @retval None
 */
static void LSM9DS0_LowLevel_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;

	/* Enable the I2C periph */
	RCC_APB1PeriphClockCmd(LSM9DS0_I2C_CLK, ENABLE);

	/* Enable SCK and SDA GPIO clocks */
	RCC_AHBPeriphClockCmd(
			LSM9DS0_I2C_SCK_GPIO_CLK | LSM9DS0_I2C_SDA_GPIO_CLK, ENABLE);

	/* Enable INT1 GPIO clock */
	RCC_AHBPeriphClockCmd(LSM9DS0_I2C_INT1_GPIO_CLK, ENABLE);

	/* Enable INT2 GPIO clock */
	RCC_AHBPeriphClockCmd(LSM9DS0_I2C_INT2_GPIO_CLK, ENABLE);

	/* Enable DRDY clock */
//	RCC_AHBPeriphClockCmd(LSM9DS0_DRDY_GPIO_CLK, ENABLE);

	GPIO_PinAFConfig(LSM9DS0_I2C_SCK_GPIO_PORT, LSM9DS0_I2C_SCK_SOURCE,
			LSM9DS0_I2C_SCK_AF);
	GPIO_PinAFConfig(LSM9DS0_I2C_SDA_GPIO_PORT, LSM9DS0_I2C_SDA_SOURCE,
			LSM9DS0_I2C_SDA_AF);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/* I2C SCK pin configuration */
	GPIO_InitStructure.GPIO_Pin = LSM9DS0_I2C_SCK_PIN;
	GPIO_Init(LSM9DS0_I2C_SCK_GPIO_PORT, &GPIO_InitStructure);

	/* I2C SDA pin configuration */
	GPIO_InitStructure.GPIO_Pin = LSM9DS0_I2C_SDA_PIN;
	GPIO_Init(LSM9DS0_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);

	/* Mems DRDY */
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/* Mems DRDY pin configuration */
//	GPIO_InitStructure.GPIO_Pin = LSM9DS0_DRDY_PIN;
//	GPIO_Init(LSM9DS0_DRDY_GPIO_PORT, &GPIO_InitStructure);

	/* Connect EXTI Line to Mems DRDY Pin */
//	SYSCFG_EXTILineConfig(LSM9DS0_DRDY_EXTI_PORT_SOURCE,
//			LSM9DS0_DRDY_EXTI_PIN_SOURCE);

//	EXTI_InitStructure.EXTI_Line = LSM9DS0_DRDY_EXTI_LINE;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;

//	EXTI_Init(&EXTI_InitStructure);

	/* I2C configuration -------------------------------------------------------*/
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
	I2C_InitStructure.I2C_DigitalFilter = 0x00;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_Timing = 0x00902025;

	/* Apply LSM9DS0_I2C configuration after enabling it */
	I2C_Init(LSM9DS0_I2C, &I2C_InitStructure);

	/* LSM9DS0_I2C Peripheral Enable */
	I2C_Cmd(LSM9DS0_I2C, ENABLE);

	/* Configure GPIO PINs to detect Interrupts */
	GPIO_InitStructure.GPIO_Pin = LSM9DS0_I2C_INT1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LSM9DS0_I2C_INT1_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LSM9DS0_I2C_INT2_PIN;
	GPIO_Init(LSM9DS0_I2C_INT2_GPIO_PORT, &GPIO_InitStructure);
}

#ifdef USE_DEFAULT_TIMEOUT_CALLBACK
/**
 * @brief  Basic management of the timeout situation.
 * @param  None.
 * @retval None.
 */
uint32_t LSM9DS0_TIMEOUT_UserCallback(void)
{
	/* Block communication and all processes */
	while (1)
	{
	}
}
#endif /* USE_DEFAULT_TIMEOUT_CALLBACK */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
