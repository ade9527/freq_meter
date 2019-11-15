/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2010 Gareth McMullin <gareth@blacksphere.co.nz>
 * Copyright (C) 2011 Piotr Esden-Tempski <piotr@esden.net>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <stdio.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "debug.h"
#include "platform.h"
#include "mpu_6050.h"


void delay(int ms)
{
	for (int i = 0; i < ms; i++) {
		for(volatile int j = 0; j < 10000; j++){}

	}
}
// led PB12

int main(void)
{
	rcc_clock_setup_in_hse_8mhz_out_72mhz();

	rcc_periph_clock_enable(RCC_GPIOB);

	/* Configure LED GPIO12 */
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL,
		      GPIO12);

	debug_init();
	i2c1_init();

	struct mpu_dev mpu = {0};
	struct axis gyro = {0};
	struct axis accel = {0};
	int16_t temp = 0;

	mpu_dev_init(&mpu, I2C1, MPU_ADDR_AD0_L);

	printf("hello world!\r\n");


	/*set LED toggle */

	gpio_set(GPIOB, GPIO12);
	int s = 0;
	while (1) {
		gpio_toggle(GPIOB, GPIO12);

		mpu_get_temp(&mpu, &temp);
		printf("mpu temp:%d\r\n", temp);

		mpu_get_gyro(&mpu, &gyro);
		printf("gyro x:%d, y:%d, z:%d\r\n", gyro.x, gyro.y, gyro.z);
		mpu_get_accel(&mpu, &accel);
		printf("accel x:%d, y:%d, z:%d\r\n", accel.x, accel.y, accel.z);

		delay(500);
	}
	while(1);
	return 0;
}

