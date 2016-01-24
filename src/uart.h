#ifndef UART_H
#define UART_H

#include <avr/io.h>
#include <stdio.h>

void uart_putchar(char c, FILE *stream);
char uart_getchar(FILE *stream);

void uart_init();

#endif
