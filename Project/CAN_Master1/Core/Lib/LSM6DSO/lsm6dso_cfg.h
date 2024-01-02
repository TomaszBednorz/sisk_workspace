#ifndef __LSM6DSO_CFG_H_
#define __LSM6DSO_CFG_H_

#include "spi.h"
#include "gpio.h"
/*
 * LSM6DSO configuration
 */

#define LSM6DSO_ExchangeData(len, tx_data, rx_data)     \
		do { \
		HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET); \
		HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, len, HAL_MAX_DELAY); \
		HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET); \
		}while(0)


#define LSM6DSO_ACC_HIGH_RESOLUTION    (0x00)
#define LSM6DSO_ACC_FULL_SCALE         (0x02)  /* 4g */
#define LSM6DSO_ACC_ODR                (0x04)  /* 104 Hz */
#define LSM6DSO_GYRO_125DPS            (0x00)
#define LSM6DSO_GYRO_FULL_SCALE        (0x01)  /* 500 dps */
#define LSM6DSO_GYRO_ODR               (0x04)  /* 104 Hz */

#endif
