/*
 * Gyroscope MUP-6050
 */

#ifndef __MPU_6050_H
#define __MPU_6050_H

#include <stdint.h>
#include <libopencm3/stm32/i2c.h>

/*
 * MPU-6050 address.
 */
#define MPU_ADDR_AD0_L    0x68
#define MPU_ADDR_AD0_H    0x69

/* register */
#define MPU_REG_SMPLRT_DIV        0x19
#define MPU_REG_CONFIG            0x1A
#define MPU_REG_GYRO_CONFIG       0x1B
#define MPU_REG_ACCEL_CONFIG      0x1C
#define MPU_REG_FF_THR            0x1D
#define MPU_REG_FF_DUR            0x1E
#define MPU_REG_MOT_THR           0x1f
#define MPU_REG_MOT_DUR           0x20
#define MPU_REG_ZRMOT_THR         0x21
#define MPU_REG_ZRMOT_DUR         0x22
#define MPU_REG_FIFO_EN           0x23

#define MPU_REG_INT_PIN_CFG       0x37
#define MPU_REG_INT_ENABLE        0x38
#define MPU_REG_INT_STATUS        0x3A

#define MPU_REG_ACCEL_XOUT_H      0x3B
#define MPU_REG_ACCEL_XOUT_L      0x3C
#define MPU_REG_ACCEL_YOUT_H      0x3D
#define MPU_REG_ACCEL_YOUT_L      0x3E
#define MPU_REG_ACCEL_ZOUT_H      0x3F
#define MPU_REG_ACCEL_ZOUT_L      0x40
#define MPU_REG_TEMP_OUT_H        0x41
#define MPU_REG_TEMP_OUT_L        0x42
#define MPU_REG_GYRO_XOUT_H       0x43
#define MPU_REG_GYRO_XOUT_L       0x44
#define MPU_REG_GYRO_YOUT_H       0x45
#define MPU_REG_GYRO_YOUT_L       0x46
#define MPU_REG_GYRO_ZOUT_H       0x47
#define MPU_REG_GYRO_ZOUT_L       0x48

#define MPU_REG_MOT_DETECT_STATUS 0x61

#define MPU_REG_SIGNAL_PATH_RESET 0x68
#define MPU_REG_MOT_DETECT_CTRL   0x69
#define MPU_REG_USER_CTRL         0x6A
#define MPU_REG_PWR_MGMT_1        0x6B
#define MPU_REG_PWR_MGMT_2        0x6C
#define MPU_REG_FIFO_COUNTH       0x72
#define MPU_REG_FIFO_COUNTL       0x73
#define MPU_REG_FIFO_R_W          0x74
#define MPU_REG_WHO_AM_I          0x75

/* PWR MGMT1 */
#define MPU_PWR_MGMT1_RESET       (1 << 7)
#define MPU_PWR_MGMT1_SLEEP       (1 << 6)
#define MPU_PWR_MGMT1_TEMP_DIS    (1 << 3)

/*
 * mpu device struct
 */
struct mpu_dev {
	uint32_t i2c;      /* i2c bus */
	uint8_t addr; /* slave address */
};

struct axis {
	int16_t x;
	int16_t y;
	int16_t z;
};



/* */
void mpu_get_gyro(struct mpu_dev *dev, struct axis *ax);
void mpu_get_accel(struct mpu_dev *dev, struct axis *ax);
void mpu_get_temp(struct mpu_dev *dev, uint16_t *temp);

void mpu_dev_init(struct mpu_dev *dev, uint32_t i2c, uint8_t addr);

#endif
