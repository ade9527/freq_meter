/*
 * This file proivte debug mothed.
 */
#ifndef __DEBUG_H__
#define __DEBUG_H__

#define DEBUG_USART

int debug_init(void);
int debug_print(char *str, int len);

#endif
