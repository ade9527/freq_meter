/*
 * This file provide debug mothed.
 * stm32 debug file
 * Redefine printf(...).
 *
 */
#include <stdio.h>
#include <string.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>


// USART1 PA9 PA10
#define DEBUG_USART            USART1
#define DEBUG_USART_GPIO_RCC   RCC_GPIOA
#define DEBUG_USART_RCC        RCC_USART1
#define DEBUG_USART_GPIO_PORT  GPIOA
#define DEBUG_USART_TX         GPIO_USART1_TX
#define DEBUG_USART_RX         GPIO_USART1_RX

/*
 * debug USART init
 */
static int debug_usart_init(void)
{
	/* rrc init */
	rcc_periph_clock_enable(DEBUG_USART_GPIO_RCC);
	rcc_periph_clock_enable(DEBUG_USART_RCC);

	/* gpio init */
	gpio_set_mode(DEBUG_USART_GPIO_PORT, GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, DEBUG_USART_TX);
	gpio_set_mode(DEBUG_USART_GPIO_PORT, GPIO_MODE_INPUT,
		GPIO_CNF_INPUT_FLOAT, DEBUG_USART_RX);

	/* usart init */
	usart_set_baudrate(DEBUG_USART, 115200);
	usart_set_databits(DEBUG_USART, 8);
	usart_set_stopbits(DEBUG_USART, USART_STOPBITS_1);
	usart_set_mode(DEBUG_USART, USART_MODE_TX_RX);
	usart_set_parity(DEBUG_USART, USART_PARITY_NONE);
	usart_set_flow_control(DEBUG_USART, USART_FLOWCONTROL_NONE);

	usart_enable(DEBUG_USART);

	return 0;
}


int debug_print(char *str, int len)
{
	int i = 0;
	
	for(i = 0; i < len; i++) {
		usart_send_blocking(DEBUG_USART, str[i]);
	}

	return len;
}

/*
 * debug init
 */
int debug_init(void)
{
	debug_usart_init();

	return 0;
}


/*
 * Redirect printf()
 */
int _write(int fd, char *ptr, int len)
{
	return debug_print(ptr, len);
}


