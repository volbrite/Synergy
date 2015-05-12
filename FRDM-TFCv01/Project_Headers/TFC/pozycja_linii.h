/*
 * pozycja_linii.h
 *
 *  Created on: May 11, 2015
 *      Author: Maciek
 */

#ifndef POZYCJA_LINII_H_
#define POZYCJA_LINII_H_

#include "..\derivative.h" /* include peripheral declarations */
#include "TFC.h"

uint8_t ABSOfInt(int8_t value);
float xPosToServoPos(int16_t xPosition);
int16_t getXPosition();

#endif /* POZYCJA_LINII_H_ */
