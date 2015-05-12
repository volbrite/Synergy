#include "TFC\TFC.h"

#ifndef ENKODER_H_
#define ENKODER_H_

void ENKODER_init();

//void FTM2_IRQHandler();

//-te zmiene finalnie wprzerwaniu tylko
volatile int Overflow;
volatile int CounterValue;

volatile int INT;
//-------

volatile unsigned int DstepP;
volatile float speedP;

volatile unsigned int DstepL;
volatile float speedL;



#endif /* __ADC16_H__ */

