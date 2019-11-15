/*
 * Gyroscope MPU-6050 driver
 *
 */


#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>

#include "mpu_6050.h"

#include <stdio.h>
extern void delay(int ms);

/*
 * Read the regiter value.
 * @param
 *     dev: MPU device, record slave address and I2C bus.
 *     reg:  register address.
 *     value: store the read value of the register.
 *     len: read len.
 * @return 0 write success. value < 0, write failed.
 */

void mpu_read_reg(struct mpu_dev *dev, uint8_t reg, uint8_t *value, int len)
{
//	i2c_transfer7(dev->i2c, dev->addr, &reg, 1, value, len);
//	i2c_write7_v1(dev->i2c, dev->addr, &reg, 1);
	while ((I2C_SR2(dev->i2c) & I2C_SR2_BUSY)) {
	}

	i2c_send_start(dev->i2c);

	/* Wait for master mode selected */
	while (!((I2C_SR1(dev->i2c) & I2C_SR1_SB)
		& (I2C_SR2(dev->i2c) & (I2C_SR2_MSL | I2C_SR2_BUSY))));

	i2c_send_7bit_address(dev->i2c, dev->addr, I2C_WRITE);

	/* Waiting for address is transferred. */
	while (!(I2C_SR1(dev->i2c) & I2C_SR1_ADDR));

	/* Clearing ADDR condition sequence. */
	(void)I2C_SR2(dev->i2c);

	i2c_send_data(dev->i2c, reg);
	while (!(I2C_SR1(dev->i2c) & (I2C_SR1_BTF)));

	i2c_send_start(dev->i2c);
	i2c_enable_ack(dev->i2c);

	/* Wait for master mode selected */
	while (!((I2C_SR1(dev->i2c) & I2C_SR1_SB)
		& (I2C_SR2(dev->i2c) & (I2C_SR2_MSL | I2C_SR2_BUSY))));

	i2c_send_7bit_address(dev->i2c, dev->addr, I2C_READ);

	/* Waiting for address is transferred. */
	while (!(I2C_SR1(dev->i2c) & I2C_SR1_ADDR));
	/* Clearing ADDR condition sequence. */
	(void)I2C_SR2(dev->i2c);

	for (int i = 0; i < len; ++i) {
		if (i == len - 1) {
			i2c_disable_ack(dev->i2c);
			i2c_nack_next(dev->i2c);
		}
		while (!(I2C_SR1(dev->i2c) & I2C_SR1_RxNE));
		value[i] = i2c_get_data(dev->i2c);
	}
	i2c_send_stop(dev->i2c);

}

/*
 * write the value to the regiter.
 * @param
 *     dev: MPU device, record slave address and I2C bus.
 *     reg:  register address.
 *     value: write the value of the register.
 * @return 0 write success. value < 0, write failed.
 */
void mpu_write_reg(struct mpu_dev *dev, uint8_t reg, uint8_t value)
{
	uint8_t data[2];
	
	data[0] = reg;
	data[1] = value;

	i2c_transfer7(dev->i2c, dev->addr, data, 2, NULL, 0);

}

void mpu_get_gyro(struct mpu_dev *dev, struct axis *ax)
{
	uint8_t data[6] = {0};

	mpu_read_reg(dev, MPU_REG_GYRO_XOUT_H, data, 6);
	
/*	for (int i = 0; i < 1; i++)
		mpu_read_reg(dev, MPU_REG_GYRO_XOUT_H + i,
			&data[i], 1);
*/
	ax->x = data[0] << 8 | data[1];
	ax->y = data[2] << 8 | data[3];
	ax->z = data[4] << 8 | data[5];
}

void mpu_get_accel(struct mpu_dev *dev, struct axis *ax)
{
	uint8_t data[6] = {0};

	mpu_read_reg(dev, MPU_REG_ACCEL_XOUT_H, data, 6);
/*	for (int i = 0; i < 6; i++)
		mpu_read_reg(dev, MPU_REG_ACCEL_XOUT_H + i,
			&data[i], 1);
*/
	ax->x = data[0] << 8 | data[1];
	ax->y = data[2] << 8 | data[3];
	ax->z = data[4] << 8 | data[5];
}


void mpu_get_temp(struct mpu_dev *dev, uint16_t *temp)
{
	uint8_t data[2] = {0};

	mpu_read_reg(dev, MPU_REG_TEMP_OUT_H, data, 2);

	*temp = data[0] << 8 | data[1];
}


/*
 * MPU-6050 init.
 * please init I2C bus first.
 */

int mpu_6050_init(void)
{

}


void mpu_dev_init(struct mpu_dev *dev, uint32_t i2c, uint8_t addr)
{
	uint8_t data;

	dev->i2c = i2c;
	dev->addr = addr;

	printf("init mpu\r\n");
	/**/
	delay(20);
	data = 0 | MPU_PWR_MGMT1_RESET;
	mpu_write_reg(dev, MPU_REG_PWR_MGMT_1, data);

	printf("Write reset\r\n");
	delay(100);

	data = 0;
//	mpu_read_reg(dev, MPU_REG_PWR_MGMT_1, &data, 1);
	printf("read pwr mgmt1:0x%x\r\n", data);

	mpu_write_reg(dev, MPU_REG_PWR_MGMT_1, 0x01);
/*	mpu_write_reg(dev, MPU_REG_USER_CTRL, 0x45);
	printf("Write user\r\n");
	mpu_write_reg(dev, MPU_REG_SMPLRT_DIV, 0x00);
	printf("Write div\r\n");
	mpu_write_reg(dev, MPU_REG_CONFIG, 0x03);
	printf("Write config\r\n");
	mpu_write_reg(dev, MPU_REG_GYRO_CONFIG, 0x08);
	printf("Write gyro\r\n");
	mpu_write_reg(dev, MPU_REG_ACCEL_CONFIG, 0x08);
	printf("Write accel\r\n");
	mpu_write_reg(dev, MPU_REG_FIFO_EN, 0x78);
	printf("Write fifo\r\n");
*/
	

}
