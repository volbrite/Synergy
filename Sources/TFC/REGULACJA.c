#include "TFC\TFC.h"

void regulator_ROT(float Line_Position) {

	float calka = 0;
	float rozniczka;
	float UCHYB;
	static float UCHYB_POPRZEDNI = 0;

	UCHYB = Line_Position;

	calka += UCHYB; //*czas; nie wiem czy ejst sens uzwgledniac czas skoro itak skalujemy do 100 i mamy wspolczyniki k 
	if (calka > 3)
		calka = 3;
	if (calka < -3)
		calka = -3;

	rozniczka = (UCHYB - UCHYB_POPRZEDNI); // /czas;
	UCHYB_POPRZEDNI = UCHYB;

	REGULACJA = Kp * UCHYB; //+ Kd * rozniczka + Ki * calka; // wtedy brak minusa przed mniej jedna operacja

	if (REGULACJA > 1)
		REGULACJA = 1;
	if (REGULACJA < -1)
		REGULACJA = -1;

}

void regulator_TRANS(int DROGA) {  		//regulacja po³o¿enia input w cm

	DROGA = DROGA * 87;

	for (;;) {
//		TFC_SetMotorPWM(0.35, 0.35);
		if ((DstepL >= DROGA) && (DstepP >= DROGA)) {
			TFC_SetMotorPWM(0, 0);
			DstepL = 0;
			DstepP = 0;
			break;
		}
	}

}

void DYFERENCJAL(float serwoPos, float AKTspeed) { //mysle o wproawdzeniu z³otego wspo³nycznika bedzie on odpowiadal za wzm. skretu gdyby serwo nie dawlo rade		
	uint16_t alfa;
	
	alfa = serwoPos * 100.0; //pozycja serwa jest podawana o -1 do 1 skaluje w ten sposób do k¹ta skrêtu (TO TRZEBA JESZCZE DOWIADCZALNIE WYZNACZYC ALFA=ALFAW-ALFAZ nie wiem jak to sie ma do skretu serawa) 
	alfa -= 50;

	if (serwoPos >= 0) {		//skrêt w prawo
		TFC_SetMotorPWM(AKTspeed,
				AKTspeed * ((1 - 0.5 * tg(alfa)) / (1 + 0.5 * tg(alfa))));
	}

	if (serwoPos < 0) {		//skrêt w lewo
		alfa = -alfa;
		TFC_SetMotorPWM(
				AKTspeed * ((1 - 0.5 * tg(alfa)) / (1 + 0.5 * tg(alfa))),
				AKTspeed);
	}

}

float tg(int alfa) {
	float tan;
	if (alfa <= 20) {
		tan = alfa * 0.0175;
	} else if (alfa <= 45) {
		tan = alfa * 0.022;
	} else if (alfa <= 60) {
		tan = alfa * 0.0288;
	}

	else if (alfa <= 100) {
		tan = alfa * 0.33333;
	}
	if (tan > 0.3)
		tan = 0.3;
	return tan;
}
