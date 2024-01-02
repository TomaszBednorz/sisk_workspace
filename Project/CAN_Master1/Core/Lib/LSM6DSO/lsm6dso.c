#include "lsm6dso.h"
#include "lsm6dso_regs.h"
#include "lsm6dso_cfg.h"
#include "lsm6dso_com.h"


#define LSM6DSO_CFG_BUF_LEN              (2U)

#define LSM6DSO_ACC_DATA_LEN              (6U)
#define LSM6DSO_GYRO_DATA_LEN             (6U)
#define LSM6DSO_TEMP_DATA_LEN             (2U)
#define LSM6DSO_STEPS_DATA_LEN            (2U)

#define LSM6DSO_PAGE_SEL_DEFAULT_MASK       (0x01)


static volatile LSM6DSO_Result_t LSM6DSO_Result = {0};

/*!	
 * \brief LSM6DSO initialization
 *
 * \param[in] None
 * 
 * \retval None
 */
void LSM6DSO_Init(void)
{
    uint8_t config_data[LSM6DSO_CFG_BUF_LEN] = {0};

    /* Accelerometer/gyro configuration */
    config_data[0] = (uint8_t)((LSM6DSO_ACC_HIGH_RESOLUTION << LSM6DSO_LPF2_XL_EN_POS) |
                               (LSM6DSO_ACC_FULL_SCALE << LSM6DSO_FS0_XL_POS) |
                               (LSM6DSO_ACC_ODR << LSM6DSO_ODR_XL0_POS));
    config_data[1] = (uint8_t)((LSM6DSO_GYRO_125DPS << LSM6DSO_FS_125_POS) |
                               (LSM6DSO_GYRO_FULL_SCALE << LSM6DSO_FS0_G_POS) |
                               (LSM6DSO_GYRO_ODR << LSM6DSO_ODR_G0_POS));
    LSM6DSO_MultipleWrite(LSM6DSO_REG_CTRL1_XL, 2, config_data);
}

/*!	
 * \brief LSM6DSO results getter
 *
 * \param[in] results Results data
 * 
 * \retval None
 */
void LSM6DSO_GetResults(LSM6DSO_Result_t *results)
{
    results->acceleration.x = LSM6DSO_Result.acceleration.x;
    results->acceleration.y = LSM6DSO_Result.acceleration.y;
    results->acceleration.z = LSM6DSO_Result.acceleration.z;
    results->angular_rate.x = LSM6DSO_Result.angular_rate.x;
    results->angular_rate.y = LSM6DSO_Result.angular_rate.y;
    results->angular_rate.z = LSM6DSO_Result.angular_rate.z;
    results->temperature = LSM6DSO_Result.temperature;
}

/*!	
 * \brief LSM6DSO read acceleration function
 *
 * \param[in] None
 * 
 * \retval None
 */
void LSM6DSO_ReadAcceleration(void)
{
    uint8_t acc_data[LSM6DSO_ACC_DATA_LEN];
    LSM6DSO_MultipleRead(LSM6DSO_REG_OUTX_L_A, LSM6DSO_ACC_DATA_LEN, acc_data);

    LSM6DSO_Result.acceleration.x = (int16_t)((acc_data[1] << 8U) | acc_data[0]);
    LSM6DSO_Result.acceleration.y = (int16_t)((acc_data[3] << 8U) | acc_data[2]);
    LSM6DSO_Result.acceleration.z = (int16_t)((acc_data[5] << 8U) | acc_data[4]);
}

/*!	
 * \brief LSM6DSO read angular rate function
 *
 * \param[in] None
 * 
 * \retval None
 */
void LSM6DSO_ReadAngularRate(void)
{
    uint8_t gyro_data[LSM6DSO_GYRO_DATA_LEN];
    LSM6DSO_MultipleRead(LSM6DSO_REG_OUTX_L_G, LSM6DSO_GYRO_DATA_LEN, gyro_data);

    LSM6DSO_Result.angular_rate.x = (int16_t)((gyro_data[1] << 8U) | gyro_data[0]);
    LSM6DSO_Result.angular_rate.y = (int16_t)((gyro_data[3] << 8U) | gyro_data[2]);
    LSM6DSO_Result.angular_rate.z = (int16_t)((gyro_data[5] << 8U) | gyro_data[4]);
}

/*!	
 * \brief LSM6DSO read temperature
 *
 * \param[in] None
 * 
 * \retval None
 */
void LSM6DSO_ReadTemperature(void)
{
    uint8_t temp_data[LSM6DSO_TEMP_DATA_LEN];
    LSM6DSO_MultipleRead(LSM6DSO_REG_OUT_TEMP_L, LSM6DSO_TEMP_DATA_LEN, temp_data);

    LSM6DSO_Result.temperature = (int16_t)((temp_data[1] << 8U) | temp_data[0]);
}
