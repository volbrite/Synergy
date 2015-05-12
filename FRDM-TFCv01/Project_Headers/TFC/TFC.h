/*
 * TFC.h
 *
 *  Created on: Apr 13, 2012
 *      Author: emh203
 */

#ifndef TFC_H_
#define TFC_H_

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "Derivative.h"
#include "TFC\TFC_Config.h"
#include "TFC\TFC_Types.h"
#include "TFC\TFC_arm_cm0.h"
#include "TFC\TFC_ARM_SysTick.h"
#include "TFC\TFC_BoardSupport.h"
#include "TFC\TFC_CrystalClock.h"
#include "TFC\TFC_Servo.h"
#include "TFC\TFC_Motor.h"
#include "TFC\TFC_ADC.h"
#include "TFC\TFC_LineScanCamera.h"
#include "TFC\TFC_Queue.h"
#include "TFC\TFC_UART.h"
#include "TFC\TFC_Terminal.h"
#include "TFC\ENKODER.h"
#include "TFC\REGULACJA.h"
#include "TFC\UART1_Init.h"
#include "TFC\pozycja_linii.h"

int PARAMETER1;
int PARAMETER2;
int PARAMETER3;
int PARAMETER4;

float speedMAX; // 0-1 
int czas; //czas pracy programu MS



void TFC_Task();
void TFC_Init();

#endif /* TFC_H_ */
