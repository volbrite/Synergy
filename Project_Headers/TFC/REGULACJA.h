#include "TFC\TFC.h"

#ifndef REGULACJA_H_
#define REGULACJA_H_

volatile float Kp;
volatile float Kd;
volatile float Ki;
volatile float REGULACJA;


void regulator_ROT(float Line_Position);				//zwraca wartosci kata skretu serwa -100 do 100
void regulator_TRANS(int DROGA);	// zatrzymuje pojazd po przejaechaniu okreslonej drogi w cm	
void DYFERENCJAL(float serwoPos, float AKTspeed ) ;	
float tg(int alfa);


#endif /* __ADC16_H__ */







