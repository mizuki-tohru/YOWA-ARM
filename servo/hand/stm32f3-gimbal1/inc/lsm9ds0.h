/**
 ******************************************************************************
 * @file    stm32f3_discovery_LSM9DS0.h
 * @author  MCD Application Team
 * @version V1.1.0
 * @date    20-September-2012
 * @brief   This file contains definitions for stm32f3_discovery_LSM9DS0.c
 *          firmware driver.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F3_LSM9DS0_H
#define __STM32F3_LSM9DS0_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f37x.h"

/* LSM9DS0 Gyro struct */
typedef struct {
	uint8_t GyroOutput_DataRate; /* OUT data rate */
	uint8_t Axes_Enable; /* Axes enable */
	uint8_t INT_G_Enable;
	uint8_t PP_OD;
	uint8_t DRDY_G_Enable;
	uint8_t BlockData_Update; /* Block Data Update */
	uint8_t Endianness; /* Endian Data selection */
	uint8_t GyroFull_Scale; /* Full Scale selection */
	uint8_t GyroSelftest;
	uint8_t FIFO_Enable;
	uint8_t INT1Sel;
} LSM9DS0Gyro_InitTypeDef;

/* LSM9DS0 Gyro High Pass Filter struct */
typedef struct {
	uint8_t HighPassFilter_Enable; /* Internal filter mode */
	uint8_t HighPassFilter_Mode_Selection; /* Internal filter mode */
	uint8_t HighPassFilter_Cutoff;
} LSM9DS0Gyro_FilterConfigTypeDef;

/* LSM9DS0 ACC struct */
typedef struct {
	uint8_t FIFO_Enable;
	uint8_t WTM_Enable;
	uint8_t AccOutput_DataRate; /* OUT data rate */
	uint8_t BlockData_Update; /* Block Data Update */
	uint8_t Axes_Enable; /* Axes enable */
	uint8_t AAF_Enable;
	uint8_t AccFull_Scale; /* Full Scale selection */
	uint8_t AccSelftest;
} LSM9DS0Acc_InitTypeDef;

/* LSM9DS0 Acc High Pass Filter struct */
typedef struct {
	uint8_t HighPassFilter_Enable; /* Internal filter mode */
	uint8_t HighPassFilter_Mode_Selection; /* Internal filter mode */
	uint8_t HighPassFilter_INT1; 
	uint8_t HighPassFilter_INT2; 
	uint8_t HighPassFilter_P1; /* HPF_enabling/disabling for AOI function on interrupt 1 */
	uint8_t HighPassFilter_P2; /* HPF_enabling/disabling for AOI function on interrupt 2 */
} LSM9DS0Acc_FilterConfigTypeDef;

/* LSM9DS0 Mag struct */
typedef struct {
	uint8_t Power_Mode; /* Power-down/Normal Mode */
	uint8_t Temperature_Sensor; /* Temperature sensor enable/disable */
	uint8_t High_Resolution; /* High Resolution enabling/disabling */
	uint8_t MagOutput_DataRate; /* OUT data rate */
	uint8_t MagFull_Scale; /* Full Scale selection */
	uint8_t Working_Mode; /* operating mode */
} LSM9DS0Mag_InitTypeDef;
/**
 * @}
 */

/** @defgroup STM32F3_DISCOVERY_LSM9DS0_Exported_Constants
 * @{
 */
#define LSM9DS0_OK                       ((uint32_t) 0)
#define LSM9DS0_FAIL                     ((uint32_t) 0)

/* Uncomment the following line to use the default LSM9DS0_TIMEOUT_UserCallback() 
 function implemented in stm32f3_discovery_lgd20.c file.
 LSM9DS0_TIMEOUT_UserCallback() function is called whenever a timeout condition
 occure during communication (waiting transmit data register empty flag(TXE)
 or waiting receive data register is not empty flag (RXNE)). */
/* #define USE_DEFAULT_TIMEOUT_CALLBACK */

/* Maximum Timeout values for flags waiting loops. These timeouts are not based
 on accurate values, they just guarantee that the application will not remain
 stuck if the I2C communication is corrupted.
 You may modify these timeout values depending on CPU frequency and application
 conditions (interrupts routines ...). */
#define LSM9DS0_FLAG_TIMEOUT             ((uint32_t)0x1000)
#define LSM9DS0_LONG_TIMEOUT             ((uint32_t)(10 * LSM9DS0_FLAG_TIMEOUT))  
/**
 * @brief  LSM9DS0 I2C Interface pins
 */
#define LSM9DS0_I2C                       I2C2
#define LSM9DS0_I2C_CLK                   RCC_APB1Periph_I2C2

#define LSM9DS0_I2C_SCK_PIN               GPIO_Pin_9                  /* PA.09 */
#define LSM9DS0_I2C_SCK_GPIO_PORT         GPIOA                       /* GPIOA */
#define LSM9DS0_I2C_SCK_GPIO_CLK          RCC_AHBPeriph_GPIOA
#define LSM9DS0_I2C_SCK_SOURCE            GPIO_PinSource9
#define LSM9DS0_I2C_SCK_AF                GPIO_AF_4

#define LSM9DS0_I2C_SDA_PIN               GPIO_Pin_10                  /* PA.10 */
#define LSM9DS0_I2C_SDA_GPIO_PORT         GPIOA                       /* GPIOA */
#define LSM9DS0_I2C_SDA_GPIO_CLK          RCC_AHBPeriph_GPIOA
#define LSM9DS0_I2C_SDA_SOURCE            GPIO_PinSource10
#define LSM9DS0_I2C_SDA_AF                GPIO_AF_4

#define LSM9DS0_DRDY_PIN                  GPIO_Pin_2                  /* PE.02 */
#define LSM9DS0_DRDY_GPIO_PORT            GPIOE                       /* GPIOE */
#define LSM9DS0_DRDY_GPIO_CLK             RCC_AHBPeriph_GPIOE
#define LSM9DS0_DRDY_EXTI_LINE            EXTI_Line2
#define LSM9DS0_DRDY_EXTI_PORT_SOURCE     EXTI_PortSourceGPIOE
#define LSM9DS0_DRDY_EXTI_PIN_SOURCE      EXTI_PinSource2
#define LSM9DS0_DRDY_EXTI_IRQn            EXTI2_TS_IRQn 

#define LSM9DS0_I2C_INT1_PIN              GPIO_Pin_6                  /* PA.06 */
#define LSM9DS0_I2C_INT1_GPIO_PORT        GPIOA                       /* GPIOA */
#define LSM9DS0_I2C_INT1_GPIO_CLK         RCC_AHBPeriph_GPIOA
#define LSM9DS0_I2C_INT1_EXTI_LINE        EXTI_Line4
#define LSM9DS0_I2C_INT1_EXTI_PORT_SOURCE EXTI_PortSourceGPIOA
#define LSM9DS0_I2C_INT1_EXTI_PIN_SOURCE  EXTI_PinSource6
#define LSM9DS0_I2C_INT1_EXTI_IRQn        EXTI4_IRQn 

#define LSM9DS0_I2C_INT2_PIN              GPIO_Pin_5                  /* PA.05 */
#define LSM9DS0_I2C_INT2_GPIO_PORT        GPIOA                       /* GPIOA */
#define LSM9DS0_I2C_INT2_GPIO_CLK         RCC_AHBPeriph_GPIOA
#define LSM9DS0_I2C_INT2_EXTI_LINE        EXTI_Line5
#define LSM9DS0_I2C_INT2_EXTI_PORT_SOURCE EXTI_PortSourceGPIOA
#define LSM9DS0_I2C_INT2_EXTI_PIN_SOURCE  EXTI_PinSource5ss
#define LSM9DS0_I2C_INT2_EXTI_IRQn        EXTI9_5_IRQn 

/******************************************************************************/
/*************************** START REGISTER MAPPING  **************************/
/******************************************************************************/
/* Acceleration Registers */
#define LSM9DS0_WHOAMI_G                  0x0F  /* Control register 1 acceleration */
#define LSM9DS0_CTRL_REG1_G               0x20  /* Control register 1 acceleration */
#define LSM9DS0_CTRL_REG2_G               0x21  /* Control register 2 acceleration */
#define LSM9DS0_CTRL_REG3_G               0x22  /* Control register 3 acceleration */
#define LSM9DS0_CTRL_REG4_G               0x23  /* Control register 4 acceleration */
#define LSM9DS0_CTRL_REG5_G               0x24  /* Control register 5 acceleration */
#define LSM9DS0_REFERENCE_G               0x25  /* Reference register acceleration */
#define LSM9DS0_STATUS_REG_G              0x27  /* Status register acceleration */
#define LSM9DS0_OUT_X_L_G                 0x28  /* Output Register X acceleration */
#define LSM9DS0_OUT_X_H_G                 0x29  /* Output Register X acceleration */
#define LSM9DS0_OUT_Y_L_G                 0x2A  /* Output Register Y acceleration */
#define LSM9DS0_OUT_Y_H_G                 0x2B  /* Output Register Y acceleration */
#define LSM9DS0_OUT_Z_L_G                 0x2C  /* Output Register Z acceleration */
#define LSM9DS0_OUT_Z_H_G                 0x2D  /* Output Register Z acceleration */ 
#define LSM9DS0_FIFO_CTRL_REG_G           0x2E  /* Fifo control Register acceleration */
#define LSM9DS0_FIFO_SRC_REG_G            0x2F  /* Fifo src Register acceleration */

#define LSM9DS0_INT1_CFG_G                0x30  /* Interrupt 1 configuration Register acceleration */
#define LSM9DS0_INT1_SOURCE_G             0x31  /* Interrupt 1 source Register acceleration */
#define LSM9DS0_INT1_TSH_XH_G             0x32  /* Interrupt 1 Threshold register acceleration */
#define LSM9DS0_INT1_TSH_XL_G             0x33  /* Interrupt 1 Threshold register acceleration */
#define LSM9DS0_INT1_TSH_YH_G             0x34  /* Interrupt 1 Threshold register acceleration */
#define LSM9DS0_INT1_TSH_YL_G             0x35  /* Interrupt 1 Threshold register acceleration */
#define LSM9DS0_INT1_TSH_ZH_G             0x36  /* Interrupt 1 Threshold register acceleration */
#define LSM9DS0_INT1_TSH_ZL_G             0x37  /* Interrupt 1 Threshold register acceleration */
#define LSM9DS0_INT1_DURATION_G           0x38  /* Interrupt 1 DURATION register acceleration */

#define LSM9DS0_TEMP_OUT_H_M              0x05  /* Temperature Register magnetic field */
#define LSM9DS0_TEMP_OUT_L_M              0x06  /* Temperature Register magnetic field */
#define LSM9DS0_SR_REG_M                  0x07  /* Status Register magnetic field */
#define LSM9DS0_OUT_X_L_M                 0x08  /* Output Register X magnetic field */
#define LSM9DS0_OUT_X_H_M                 0x09  /* Output Register X magnetic field */
#define LSM9DS0_OUT_Y_L_M                 0x0A  /* Output Register Y magnetic field */
#define LSM9DS0_OUT_Y_H_M                 0x0B  /* Output Register Y magnetic field */
#define LSM9DS0_OUT_Z_L_M                 0x0C  /* Output Register Z magnetic field */ 
#define LSM9DS0_OUT_Z_H_M                 0x0D  /* Output Register Z magnetic field */

#define LSM9DS0_INT1_CFG_M                0x12  /* Interrupt 1 configuration Register acceleration */
#define LSM9DS0_INT1_SOURCE_M             0x13  /* Interrupt 1 source Register acceleration */
#define LSM9DS0_INT1_THS_L_M              0x14  /* Interrupt 1 Threshold register acceleration */
#define LSM9DS0_INT1_THS_H_M              0x15  /* Interrupt 1 Threshold register acceleration */
#define LSM9DS0_OFFSET_X_L_M              0x16  /* Output Register X magnetic field */
#define LSM9DS0_OFFSET_X_H_M              0x17  /* Output Register X magnetic field */
#define LSM9DS0_OFFSET_Y_L_M              0x18  /* Output Register Y magnetic field */
#define LSM9DS0_OFFSET_Y_H_M              0x19  /* Output Register Y magnetic field */
#define LSM9DS0_OFFSET_Z_L_M              0x1A  /* Output Register Z magnetic field */ 
#define LSM9DS0_OFFSET_Z_H_M              0x1B  /* Output Register Z magnetic field */
#define LSM9DS0_REFERENCE_X               0x1C  /* Reference register acceleration */
#define LSM9DS0_REFERENCE_Y               0x1D  /* Reference register acceleration */
#define LSM9DS0_REFERENCE_Z               0x1E  /* Reference register acceleration */
#define LSM9DS0_WHOAMI_XM                 0x0F  /* Control register 1 acceleration */
#define LSM9DS0_CTRL_REG0_XM              0x1F  /* Control register 1 acceleration */
#define LSM9DS0_CTRL_REG1_XM              0x20  /* Control register 1 acceleration */
#define LSM9DS0_CTRL_REG2_XM              0x21  /* Control register 2 acceleration */
#define LSM9DS0_CTRL_REG3_XM              0x22  /* Control register 3 acceleration */
#define LSM9DS0_CTRL_REG4_XM              0x23  /* Control register 4 acceleration */
#define LSM9DS0_CTRL_REG5_XM              0x24  /* Control register 5 acceleration */
#define LSM9DS0_CTRL_REG6_XM              0x25  /* Control register 5 acceleration */
#define LSM9DS0_CTRL_REG7_XM              0x26  /* Control register 5 acceleration */

#define LSM9DS0_STATUS_REG_A              0x27  /* Status register acceleration */
#define LSM9DS0_OUT_X_L_A                 0x28  /* Output Register X magnetic field */
#define LSM9DS0_OUT_X_H_A                 0x29  /* Output Register X magnetic field */
#define LSM9DS0_OUT_Y_L_A                 0x2A  /* Output Register Y magnetic field */
#define LSM9DS0_OUT_Y_H_A                 0x2B  /* Output Register Y magnetic field */
#define LSM9DS0_OUT_Z_L_A                 0x2C  /* Output Register Z magnetic field */ 
#define LSM9DS0_OUT_Z_H_A                 0x2D  /* Output Register Z magnetic field */
#define LSM9DS0_FIFO_CTRL_REG             0x2E  /* Fifo control Register acceleration */
#define LSM9DS0_FIFO_SRC_REG              0x2F  /* Fifo src Register acceleration */

#define LSM9DS0_INT1_CFG                  0x30  /* Interrupt 2 configuration Register acceleration */
#define LSM9DS0_INT1_SOURCE               0x31  /* Interrupt 2 source Register acceleration */
#define LSM9DS0_INT1_THS                  0x32  /* Interrupt 2 Threshold register acceleration */
#define LSM9DS0_INT1_DURATION             0x33  /* Interrupt 2 DURATION register acceleration */

#define LSM9DS0_INT2_CFG                  0x34  /* Interrupt 2 configuration Register acceleration */
#define LSM9DS0_INT2_SOURCE               0x35  /* Interrupt 2 source Register acceleration */
#define LSM9DS0_INT2_THS                  0x36  /* Interrupt 2 Threshold register acceleration */
#define LSM9DS0_INT2_DURATION             0x37  /* Interrupt 2 DURATION register acceleration */

#define LSM9DS0_CLICK_CFG_A               0x38  /* Click configuration Register acceleration */
#define LSM9DS0_CLICK_SOURCE_A            0x39  /* Click 2 source Register acceleration */
#define LSM9DS0_CLICK_THS_A               0x3A  /* Click 2 Threshold register acceleration */

#define LSM9DS0_TIME_LIMIT_A              0x3B  /* Time Limit Register acceleration */
#define LSM9DS0_TIME_LATENCY_A            0x3C  /* Time Latency Register acceleration */
#define LSM9DS0_TIME_WINDOW_A             0x3D  /* Time window register acceleration */

/******************************************************************************/
/**************************** END REGISTER MAPPING  ***************************/
/******************************************************************************/

#define GYRO_I2C_ADDRESS                     0xD4
#define MAG_I2C_ADDRESS                      0x3C
#define ACC_I2C_ADDRESS                      0x3C

#define LSM9DS0_GODR_95HZ_12HZ           ((uint8_t)0x00)
#define LSM9DS0_GODR_95HZ_25HZ           ((uint8_t)0x10)
#define LSM9DS0_GODR_190HZ_12HZ          ((uint8_t)0x40)
#define LSM9DS0_GODR_190HZ_25HZ          ((uint8_t)0x50)
#define LSM9DS0_GODR_190HZ_50HZ          ((uint8_t)0x60)
#define LSM9DS0_GODR_190HZ_70HZ          ((uint8_t)0x70)
#define LSM9DS0_GODR_380HZ_20HZ          ((uint8_t)0x80)
#define LSM9DS0_GODR_380HZ_25HZ          ((uint8_t)0x90)
#define LSM9DS0_GODR_380HZ_50HZ          ((uint8_t)0xA0)
#define LSM9DS0_GODR_380HZ_100HZ         ((uint8_t)0xB0)
#define LSM9DS0_GODR_760HZ_30HZ          ((uint8_t)0xC0)
#define LSM9DS0_GODR_760HZ_35HZ          ((uint8_t)0xD0)
#define LSM9DS0_GODR_760HZ_50HZ          ((uint8_t)0xE0)
#define LSM9DS0_GODR_760HZ_100HZ         ((uint8_t)0xF0)

#define LSM9DS0_GHPM_NORMAL_MODE_RES     ((uint8_t)0x00)
#define LSM9DS0_GHPM_REF_SIGNAL          ((uint8_t)0x10)
#define LSM9DS0_GHPM_NORMAL_MODE         ((uint8_t)0x20)
#define LSM9DS0_GHPM_AUTORESET_INT       ((uint8_t)0x30)

#define LSM9DS0_INTG_MODE_INTERRUPT      ((uint8_t)0x80)
#define LSM9DS0_INTG_BOOTSTATUS          ((uint8_t)0x40)
#define LSM9DS0_INTG_ACT_INTERRUPT       ((uint8_t)0x20)
#define LSM9DS0_PP_OD_ENA                ((uint8_t)0x10)
#define LSM9DS0_DRDY_G_ENA               ((uint8_t)0x08)
#define LSM9DS0_DRDY_G_WTM               ((uint8_t)0x04)
#define LSM9DS0_DRDY_G_FIFOOR            ((uint8_t)0x02)
#define LSM9DS0_DRDY_G_FIFOENP           ((uint8_t)0x01)

#define LSM9DS0_GFULLSCALE_245           ((uint8_t)0x00)  /*245dps   */
#define LSM9DS0_GFULLSCALE_500           ((uint8_t)0x10)  /*500dps   */
#define LSM9DS0_GFULLSCALE_2000          ((uint8_t)0x20)  /*2000dps  */
#define LSM9DS0_GFULLSCALE_20000         ((uint8_t)0x30)  /*20000dps */

/** @defgroup Acc_Boot_Mode_selection 
 * @{
 */
/*AGG GYRO*/
#define LSM9DS0_BOOT_NORMALMODE              ((uint8_t)0x00)
#define LSM9DS0_BOOT_REBOOTMEMORY            ((uint8_t)0x80)

/*AGG GYRO*/
#define LSM9DS0_FIFO_MODE_ENA                ((uint8_t)0x40)
#define LSM9DS0_FIFO_MODE_DIS                ((uint8_t)0x00)

/*ACC*/
#define LSM9DS0_WTM_MODE_ENA                 ((uint8_t)0x20)
#define LSM9DS0_WTM_MODE_DIS                 ((uint8_t)0x00)

/*ACC*/
#define LSM9DS0_AHPF_MODE_ENA                 ((uint8_t)0x04)
#define LSM9DS0_AHPF_MODE_DIS                 ((uint8_t)0x00)
/*GYRO*/
#define LSM9DS0_GHPF_MODE_ENA                 ((uint8_t)0x10)
#define LSM9DS0_GHPF_MODE_DIS                 ((uint8_t)0x00)

#define LSM9DS0_HPFINT1_MODE_ENA             ((uint8_t)0x02)
#define LSM9DS0_HPFINT1_MODE_DIS             ((uint8_t)0x00)
#define LSM9DS0_HPFINT2_MODE_ENA             ((uint8_t)0x01)
#define LSM9DS0_HPFINT2_MODE_DIS             ((uint8_t)0x00)


/** @defgroup Acc_OutPut_DataRate_Selection 
 * @{
 */
#define LSM9DS0_AODR_3_HZ                ((uint8_t)0x10)  /*!< Output Data Rate = 1 Hz */
#define LSM9DS0_AODR_12_HZ               ((uint8_t)0x20)  /*!< Output Data Rate = 10 Hz */
#define LSM9DS0_AODR_25_HZ               ((uint8_t)0x30)  /*!< Output Data Rate = 25 Hz */
#define LSM9DS0_AODR_50_HZ               ((uint8_t)0x40)  /*!< Output Data Rate = 50 Hz */
#define LSM9DS0_AODR_100_HZ              ((uint8_t)0x50)  /*!< Output Data Rate = 100 Hz */
#define LSM9DS0_AODR_200_HZ              ((uint8_t)0x60)  /*!< Output Data Rate = 200 Hz */
#define LSM9DS0_AODR_400_HZ              ((uint8_t)0x70)  /*!< Output Data Rate = 400 Hz */
#define LSM9DS0_AODR_800_HZ              ((uint8_t)0x90)  /*!< Output Data Rate = 1620 Hz only in Low Power Mode */
#define LSM9DS0_AODR_1600_HZ             ((uint8_t)0xA0)  /*!< Output Data Rate = 1344 Hz in Normal mode and 5376 Hz in Low Power Mode */

/**
 * @}
 */
 
/** @defgroup Acc_Block_Data_Update 
 * @{
 */
#define LSM9DS0_BlockUpdate_Continous   ((uint8_t)0x00) /*!< Continuos Update */
#define LSM9DS0_BlockUpdate_Single      ((uint8_t)0x08) /*!< Single Update: output registers not updated until MSB and LSB reading */
/**
 * @}
 */

/** @defgroup Acc_Axes_Selection 
 * @{
 */
#define LSM9DS0_X_ENABLE                ((uint8_t)0x01)
#define LSM9DS0_Y_ENABLE                ((uint8_t)0x02)
#define LSM9DS0_Z_ENABLE                ((uint8_t)0x04)

#define LSM9DS0_AXES_ENABLE             ((uint8_t)0x07)
#define LSM9DS0_AXES_DISABLE            ((uint8_t)0x00)
#define LSM9DS0_AXES_ENABLE_G           ((uint8_t)0x0F)
/**
 * @}
 */
#define LSM9DS0_AAFILTER_773_HZ         ((uint8_t)0x00)
#define LSM9DS0_AAFILTER_194_HZ         ((uint8_t)0x40)
#define LSM9DS0_AAFILTER_362_HZ         ((uint8_t)0x80)
#define LSM9DS0_AAFILTER_50_HZ          ((uint8_t)0xC0)

/** @defgroup Acc_Full_Scale_Selection 
 * @{
 */
#define LSM9DS0_AFULLSCALE_2G           ((uint8_t)0x00)  /* */
#define LSM9DS0_AFULLSCALE_4G           ((uint8_t)0x08)  /* */
#define LSM9DS0_AFULLSCALE_6G           ((uint8_t)0x10)  /* */
#define LSM9DS0_AFULLSCALE_8G           ((uint8_t)0x18)  /* */
#define LSM9DS0_AFULLSCALE_16G          ((uint8_t)0x20)  /* */

/*ACC GYRO*/
#define LSM9DS0_SELFTEST_NORMAL         ((uint8_t)0x00)
#define LSM9DS0_SELFTEST_POS            ((uint8_t)0x02)
#define LSM9DS0_SELFTEST_NEG            ((uint8_t)0x04)
#define LSM9DS0_SELFTEST_NO             ((uint8_t)0x06)


#define LSM9DS0_P1_BOOT_ENA             ((uint8_t)0x80)
#define LSM9DS0_P1_TAP_ENA              ((uint8_t)0x40)
#define LSM9DS0_P1_INT1_ENA             ((uint8_t)0x20)
#define LSM9DS0_P1_INT2_ENA             ((uint8_t)0x10)
#define LSM9DS0_P1_INTM_ENA             ((uint8_t)0x08)
#define LSM9DS0_P1_DRDYA_ENA            ((uint8_t)0x04)
#define LSM9DS0_P1_DRDYM_ENA            ((uint8_t)0x02)
#define LSM9DS0_P1_EMPTY_ENA            ((uint8_t)0x01)

#define LSM9DS0_P2_TAP_ENA              ((uint8_t)0x80)
#define LSM9DS0_P2_INT1_ENA             ((uint8_t)0x40)
#define LSM9DS0_P2_INT2_ENA             ((uint8_t)0x20)
#define LSM9DS0_P2_INTM_ENA             ((uint8_t)0x10)
#define LSM9DS0_P2_DRDYA_ENA            ((uint8_t)0x08)
#define LSM9DS0_P2_DRDYM_ENA            ((uint8_t)0x04)
#define LSM9DS0_P2_PVERRUN_ENA          ((uint8_t)0x02)
#define LSM9DS0_P2_WTM_ENA              ((uint8_t)0x01)

/** @defgroup Mag_Temperature_Sensor
 * @{
 */
#define LSM9DS0_TEMPSENSOR_ENABLE       ((uint8_t) 0x80)   /*!< Temp sensor Enable */
#define LSM9DS0_TEMPSENSOR_DISABLE      ((uint8_t) 0x00)   /*!< Temp sensor Disable */
/**                           
 * @}
 */
/** @defgroup Mag_High_Resolution
 * @{
 */
#define LSM9DS0_MHR_ENABLE              ((uint8_t)0x60)
#define LSM9DS0_MHR_DISABLE             ((uint8_t)0x00)
/**
 * @}
 */
/** @defgroup Mag_Data_Rate 
 * @{
 */
#define LSM9DS0_MODR_3_HZ                ((uint8_t) 0x00)  /*!< Output Data Rate = 3 Hz */
#define LSM9DS0_MODR_6_HZ                ((uint8_t) 0x04)  /*!< Output Data Rate = 7.5 Hz */
#define LSM9DS0_MODR_12_HZ               ((uint8_t) 0x08)  /*!< Output Data Rate = 15 Hz */
#define LSM9DS0_MODR_25_HZ               ((uint8_t) 0x0C)  /*!< Output Data Rate = 30 Hz */
#define LSM9DS0_MODR_50_HZ               ((uint8_t) 0x10)  /*!< Output Data Rate = 75 Hz */
#define LSM9DS0_MODR_100_HZ              ((uint8_t) 0x18)  /*!< Output Data Rate = 220 Hz */
/**
 * @}
 */
#define LSM9DS0_INTREQ2_LATCH           ((uint8_t) 0x02)
#define LSM9DS0_INTREQ1_LATCH           ((uint8_t) 0x01)

#define LSM9DS0_MFULLSCALE_2G           ((uint8_t) 0x00)  /* */
#define LSM9DS0_MFULLSCALE_4G           ((uint8_t) 0x20)  /* */
#define LSM9DS0_MFULLSCALE_8G           ((uint8_t) 0x40)  /* */
#define LSM9DS0_MFULLSCALE_12G          ((uint8_t) 0x60)  /* */



/** @defgroup Acc_High_Pass_Filter_Mode 
 * @{
 */
#define LSM9DS0_HPM_NORMAL_MODE_RES     ((uint8_t)0x00)
#define LSM9DS0_HPM_REF_SIGNAL          ((uint8_t)0x40)
#define LSM9DS0_HPM_NORMAL_MODE         ((uint8_t)0x80)
#define LSM9DS0_HPM_AUTORESET_INT       ((uint8_t)0xC0)
/**
 * @}
 */
/** @defgroup Acc_High_Pass_Filter_status 
 * @{
 */
#define LSM9DS0_HIGHPASSFILTER_DISABLE  ((uint8_t)0x00)
#define LSM9DS0_HIGHPASSFILTER_ENABLE   ((uint8_t)0x20)
/**
 * @}
 */

/** @defgroup Mag_Power_Mode_selection 
 * @{
 */
#define LSM9DS0_MAG_NORMAL_MODE         ((uint8_t)0x00)
#define LSM9DS0_MAG_LOWPOWER_MODE       ((uint8_t)0x04)
/**
 * @}
 */


/** @defgroup Acc_High_Resolution
 * @{
 */
#define LSM9DS0_HR_ENABLE               ((uint8_t)0x08)
#define LSM9DS0_HR_DISABLE              ((uint8_t)0x00)
/**
 * @}
 */


/*ACC GYRO*/
#define LSM9DS0_BLE_LSB            ((uint8_t)0x00) /*!< Little Endian: data LSB @ lower address */
#define LSM9DS0_BLE_MSB	           ((uint8_t)0x40) /*!< Big Endian: data MSB @ lower address */



/** @defgroup Acc_High_Pass_CUT OFF_Frequency 
 * @{
 */
#define LSM9DS0_HPFCF_8                 ((uint8_t)0x00)
#define LSM9DS0_HPFCF_16                ((uint8_t)0x10)
#define LSM9DS0_HPFCF_32                ((uint8_t)0x20)
#define LSM9DS0_HPFCF_64                ((uint8_t)0x30)
/**
 * @}
 */


/** @defgroup Acc_High_Pass_Filter_Click_status 
 * @{
 */
#define LSM9DS0_HPF_CLICK_DISABLE       ((uint8_t)0x00)
#define LSM9DS0_HPF_CLICK_ENABLE	       ((uint8_t)0x04)
/**
 * @}
 */

/** @defgroup Acc_High_Pass_Filter_AOI1_status 
 * @{
 */
#define LSM9DS0_HPF_AOI1_DISABLE        ((uint8_t)0x00)
#define LSM9DS0_HPF_AOI1_ENABLE	       ((uint8_t)0x01)
/**
 * @}
 */

/** @defgroup Acc_High_Pass_Filter_AOI2_status 
 * @{
 */
#define LSM9DS0_HPF_AOI2_DISABLE        ((uint8_t)0x00)
#define LSM9DS0_HPF_AOI2_ENABLE	   ((uint8_t)0x02)
/**
 * @}
 */

/** @defgroup Acc_LSM9DS0_Interrupt1_Configuration_definition
 * @{
 */
#define LSM9DS0_IT1_CLICK               ((uint8_t)0x80)
#define LSM9DS0_IT1_AOI1                ((uint8_t)0x40)
#define LSM9DS0_IT1_AOI2                ((uint8_t)0x20)
#define LSM9DS0_IT1_DRY1                ((uint8_t)0x10)
#define LSM9DS0_IT1_DRY2                ((uint8_t)0x08)
#define LSM9DS0_IT1_WTM                 ((uint8_t)0x04)
#define LSM9DS0_IT1_OVERRUN             ((uint8_t)0x02)
/**
 * @}
 */

/** @defgroup Acc_LSM9DS0_Interrupt2_Configuration_definition
 * @{
 */
#define LSM9DS0_IT2_CLICK               ((uint8_t)0x80)
#define LSM9DS0_IT2_INT1                ((uint8_t)0x40)
#define LSM9DS0_IT2_INT2                ((uint8_t)0x20)
#define LSM9DS0_IT2_BOOT                ((uint8_t)0x10)
#define LSM9DS0_IT2_ACT                 ((uint8_t)0x08)
#define LSM9DS0_IT2_HLACTIVE            ((uint8_t)0x02)
/**
 * @}
 */

/** @defgroup Acc_INT_Combination_Status 
 * @{
 */
#define LSM9DS0_OR_COMBINATION          ((uint8_t)0x00)  /*!< OR combination of enabled IRQs */
#define LSM9DS0_AND_COMBINATION	       ((uint8_t)0x80)  /*!< AND combination of enabled IRQs */
#define LSM9DS0_MOV_RECOGNITION         ((uint8_t)0x40)  /*!< 6D movement recognition */
#define LSM9DS0_POS_RECOGNITION	       ((uint8_t)0xC0)  /*!< 6D position recognition */
/**
 * @}
 */

/** @defgroup Acc_INT_Axes 
 * @{
 */
#define LSM9DS0_Z_HIGH                  ((uint8_t)0x20)  /*!< Z High enabled IRQs */
#define LSM9DS0_Z_LOW	                 ((uint8_t)0x10)  /*!< Z low enabled IRQs */
#define LSM9DS0_Y_HIGH                  ((uint8_t)0x08)  /*!< Y High enabled IRQs */
#define LSM9DS0_Y_LOW	                 ((uint8_t)0x04)  /*!< Y low enabled IRQs */
#define LSM9DS0_X_HIGH                  ((uint8_t)0x02)  /*!< X High enabled IRQs */
#define LSM9DS0_X_LOW	                 ((uint8_t)0x01)  /*!< X low enabled IRQs */
/**
 * @}
 */

/** @defgroup Acc_INT_Click 
 * @{
 */
#define LSM9DS0_Z_DOUBLE_CLICK          ((uint8_t)0x20)  /*!< Z double click IRQs */
#define LSM9DS0_Z_SINGLE_CLICK	         ((uint8_t)0x10)  /*!< Z single click IRQs */
#define LSM9DS0_Y_DOUBLE_CLICK          ((uint8_t)0x08)  /*!< Y double click IRQs */
#define LSM9DS0_Y_SINGLE_CLICK	         ((uint8_t)0x04)  /*!< Y single click IRQs */
#define LSM9DS0_X_DOUBLE_CLICK          ((uint8_t)0x02)  /*!< X double click IRQs */
#define LSM9DS0_X_SINGLE_CLICK	         ((uint8_t)0x01)  /*!< X single click IRQs */
/**
 * @}
 */

/** @defgroup Acc_INT1_Interrupt_status 
 * @{
 */
#define LSM9DS0_INT1INTERRUPT_DISABLE   ((uint8_t)0x00)
#define LSM9DS0_INT1INTERRUPT_ENABLE	   ((uint8_t)0x80)
/**
 * @}
 */

/** @defgroup Acc_INT1_Interrupt_ActiveEdge 
 * @{
 */
#define LSM9DS0_INT1INTERRUPT_LOW_EDGE  ((uint8_t)0x20)
#define LSM9DS0_INT1INTERRUPT_HIGH_EDGE ((uint8_t)0x00)
/**
 * @}
 */

/** @defgroup Mag_Working_Mode
 * @{
 */
#define LSM9DS0_CONTINUOS_CONVERSION      ((uint8_t) 0x00)   /*!< Continuous-Conversion Mode */
#define LSM9DS0_SINGLE_CONVERSION         ((uint8_t) 0x01)   /*!< Single-Conversion Mode */
#define LSM9DS0_SLEEP                     ((uint8_t) 0x02)   /*!< Sleep Mode */                       
/**
 * @}
 */

/**
 * @defgroup Magnetometer_Sensitivity              
 * @{
 */
#define LSM9DS0_M_SENSITIVITY_XY_1_3Ga     1100  /*!< magnetometer X Y axes sensitivity for 1.3 Ga full scale [LSB/Ga] */
#define LSM9DS0_M_SENSITIVITY_XY_1_9Ga     855   /*!< magnetometer X Y axes sensitivity for 1.9 Ga full scale [LSB/Ga] */
#define LSM9DS0_M_SENSITIVITY_XY_2_5Ga     670   /*!< magnetometer X Y axes sensitivity for 2.5 Ga full scale [LSB/Ga] */
#define LSM9DS0_M_SENSITIVITY_XY_4Ga       450   /*!< magnetometer X Y axes sensitivity for 4 Ga full scale [LSB/Ga] */
#define LSM9DS0_M_SENSITIVITY_XY_4_7Ga     400   /*!< magnetometer X Y axes sensitivity for 4.7 Ga full scale [LSB/Ga] */
#define LSM9DS0_M_SENSITIVITY_XY_5_6Ga     330   /*!< magnetometer X Y axes sensitivity for 5.6 Ga full scale [LSB/Ga] */
#define LSM9DS0_M_SENSITIVITY_XY_8_1Ga     230   /*!< magnetometer X Y axes sensitivity for 8.1 Ga full scale [LSB/Ga] */
#define LSM9DS0_M_SENSITIVITY_Z_1_3Ga      980   /*!< magnetometer Z axis sensitivity for 1.3 Ga full scale [LSB/Ga] */
#define LSM9DS0_M_SENSITIVITY_Z_1_9Ga      760   /*!< magnetometer Z axis sensitivity for 1.9 Ga full scale [LSB/Ga] */
#define LSM9DS0_M_SENSITIVITY_Z_2_5Ga      600   /*!< magnetometer Z axis sensitivity for 2.5 Ga full scale [LSB/Ga] */
#define LSM9DS0_M_SENSITIVITY_Z_4Ga        400   /*!< magnetometer Z axis sensitivity for 4 Ga full scale [LSB/Ga] */
#define LSM9DS0_M_SENSITIVITY_Z_4_7Ga      355   /*!< magnetometer Z axis sensitivity for 4.7 Ga full scale [LSB/Ga] */
#define LSM9DS0_M_SENSITIVITY_Z_5_6Ga      295   /*!< magnetometer Z axis sensitivity for 5.6 Ga full scale [LSB/Ga] */
#define LSM9DS0_M_SENSITIVITY_Z_8_1Ga      205   /*!< magnetometer Z axis sensitivity for 8.1 Ga full scale [LSB/Ga] */
/**
 * @}
 */

/** @defgroup STM32F3_DISCOVERY_LSM9DS0_Exported_Functions
 * @{
 */
/* Gyro functions */
void LSM9DS0_GyroInit(LSM9DS0Gyro_InitTypeDef *);
void LSM9DS0_GyroFilterConfig(LSM9DS0Gyro_FilterConfigTypeDef *);
void LSM9DS0_GyroRebootCmd(void);

/* Acc functions */
void LSM9DS0_AccInit(LSM9DS0Acc_InitTypeDef *LSM9DS0_InitStruct);
void LSM9DS0_AccRebootCmd(void);
void LSM9DS0_AccFilterConfig(
		LSM9DS0Acc_FilterConfigTypeDef *LSM9DS0_FilterStruct);
void LSM9DS0_AccFilterCmd(uint8_t HighPassFilterState);
void LSM9DS0_AccFilterClickCmd(uint8_t HighPassFilterClickState);
void LSM9DS0_AccIT1Config(uint8_t LSM9DS0_IT, FunctionalState NewState);
void LSM9DS0_AccIT2Config(uint8_t LSM9DS0_IT, FunctionalState NewState);
void LSM9DS0_AccINT1InterruptConfig(uint8_t ITCombination, uint8_t ITAxes,
		FunctionalState NewState);
void LSM9DS0_AccINT2InterruptConfig(uint8_t ITCombination, uint8_t ITAxes,
		FunctionalState NewState);
void LSM9DS0_AccClickITConfig(uint8_t ITClick, FunctionalState NewState);
uint8_t LSM9DS0_AccGetDataStatus(void);

/* Mag functions */
void LSM9DS0_MagInit(LSM9DS0Mag_InitTypeDef *LSM9DS0_InitStruct);
uint8_t LSM9DS0_MagGetDataStatus(void);

/* read write funtions */
uint16_t LSM9DS0_Write(uint8_t DeviceAddr, uint8_t RegAddr,
		uint8_t* pBuffer);
uint16_t LSM9DS0_Read(uint8_t DeviceAddr, uint8_t RegAddr, uint8_t* pBuffer,
		uint16_t NumByteToRead);
		
static void LSM9DS0_LowLevel_Init(void);

/* USER Callbacks: This is function for which prototype only is declared in
 MEMS accelerometre driver and that should be implemented into user applicaiton. */
/* LSM9DS0_TIMEOUT_UserCallback() function is called whenever a timeout condition 
 occure during communication (waiting transmit data register empty flag(TXE)
 or waiting receive data register is not empty flag (RXNE)).
 You can use the default timeout callback implementation by uncommenting the
 define USE_DEFAULT_TIMEOUT_CALLBACK in stm32f3_discovery_LSM9DS0.h file.
 Typically the user implementation of this callback should reset MEMS peripheral
 and re-initialize communication or in worst case reset all the application. */
uint32_t LSM9DS0_TIMEOUT_UserCallback(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F3_DISCOVERY_LSM9DS0_H */
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
