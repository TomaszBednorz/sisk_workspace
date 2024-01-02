#ifndef __LSM6DSO_REGS_H_
#define __LSM6DSO_REGS_H_


/*
 * Registers addresses
 */
#define LSM6DSO_REG_FUNC_CFG_ACCESS   (0x01)
#define LSM6DSO_REG_WHO_AM_I	      (0x0F)
#define LSM6DSO_REG_PAGE_RW           (0x17)
#define LSM6DSO_REG_PAGE_SEL          (0x02)
#define LSM6DSO_REG_PAGE_ADDRESS      (0x08)
#define LSM6DSO_REG_PAGE_VALUE        (0x09)
#define LSM6DSO_REG_EMB_FUNC_EN_A     (0x04)
#define LSM6DSO_REG_EMB_FUNC_EN_B     (0x05)
#define LSM6DSO_REG_EMB_FUNC_INT1     (0x0A)
#define LSM6DSO_REG_INT1_CTRL         (0x0D)
#define LSM6DSO_REG_MD1_CFG           (0x5E)
#define LSM6DSO_REG_CTRL1_XL          (0x10)
#define LSM6DSO_REG_OUTX_L_A          (0x28)
#define LSM6DSO_REG_OUTX_L_G          (0x22)
#define LSM6DSO_REG_OUT_TEMP_L        (0x20)
#define LSM6DSO_REG_STEP_COUNTER_L    (0x62)

#define LSM6DSO_REG_EMB_FUNC_STATUS   (0x12)
#define LSM6DSO_REG_STATUS_REG        (0x1E)


/*
 * Bit positions in registers
 */
#define LSM6DSO_LPF2_XL_EN_POS        (0x01)
#define LSM6DSO_FS0_XL_POS            (0x02)
#define LSM6DSO_ODR_XL0_POS           (0x04)
#define LSM6DSO_FS_125_POS            (0x01)
#define LSM6DSO_FS0_G_POS             (0x02)
#define LSM6DSO_ODR_G0_POS            (0x04)

/*
 * Registers values
 */
#define LSM6DSO_FUNC_CFG_ACCESS       (0x80)
#define LSM6DSO_PAGE_RW               (0x40)
#define LSM6DSO_PAGE_SEL0             (0x10)
#define LSM6DSO_REG_PEDO_CMD_REG      (0x83)
#define LSM6DSO_FP_REJECTION_EN       (0x04)
#define LSM6DSO_STEP_DET_INIT         (0x08)
#define LSM6DSO_PEDO_ADV_EN           (0x10)
#define LSM6DSO_INT1_STEP_DETECTOR    (0x08)

/*
 * Registers masks
 */
#define LSM6DSO_IS_STEP_DET           (0x08)
#define LSM6DSO_XLDA                  (0x01)
#define LSM6DSO_GDA                   (0x02)
#define LSM6DSO_TDA                   (0x04)

#endif
