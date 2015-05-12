/*
 * UART1_Init.h
 *
 *  Created on: Apr 20, 2015
 *      Author: Mateusz Wasielewski
 */

#ifndef UART1_INIT_H_
#define UART1_INIT_H_

#include "TFC\TFC.h"

#define RED_LED 18
#define GREEN_LED 19
#define BLUE_LED 1
#define BLUE_LED_ON (GPIOD_PCOR =(1 << BLUE_LED))
#define GREEN_LED_ON (GPIOB_PCOR =(1 << GREEN_LED))
#define RED_LED_ON (GPIOB_PCOR =(1 << RED_LED))
#define BLUE_LED_OFF (GPIOD_PSOR =(1 << BLUE_LED))
#define GREEN_LED_OFF (GPIOB_PSOR =(1 << GREEN_LED))
#define RED_LED_OFF (GPIOB_PSOR =(1 << RED_LED))

void UART1_Init();
void RED_LED_ON_ONLY();
void BLUE_LED_ON_ONLY();
void GREEN_LED_ON_ONLY();
void ALL_LED_ON();
void ALL_LED_OFF();
void UART1_RUN();
void GET_NUMBERS(int*);
void PRINTF_WORDS(char*);
void BLINK_LEDS(int);
void PLOT_NUMBERS(int);
#endif /* UART1_INIT_H_ */
