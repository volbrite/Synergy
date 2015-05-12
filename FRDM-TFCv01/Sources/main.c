#include "derivative.h" /* include peripheral declarations */
#include "TFC\TFC.h"

int main(void) {
	Kp = 0.8;
	Kd = 0;
	Ki = 0;
	czas = 5; //ms
	speedMAX = 0.25; //0-1
	PARAMETER1 = 25;
	
	float pozycja = 0;
	int16_t n_pozycja = 0;
	int8_t lineNOTfound=0;
	

	TFC_Init();
	for (;;) {

		if (TFC_PUSH_BUTTON_0_PRESSED) {
			TFC_HBRIDGE_ENABLE;
			//TFC_SetMotorPWM(speedMAX, speedMAX);
			//speedMAX=PARAMETER1/100.0;
//			regulator_TRANS(30);

			for (;;) { //-------------------------JADACA PETLA---------------------------------------
				speedMAX = PARAMETER1 / 100.0;
				TFC_SetMotorPWM(speedMAX, speedMAX);

				//				if ((pozycja = getXPosition()) == LINE_SCAN_WRONG_POS){
				//				lineNOTfound++;
				//				if (lineNOTfound >=5)
				//					break;}
				//				/*
				//				 * do napisania
				//				 */
				//				else
				if(TFC_Ticker[0]>20 && LineScanImageReady==1)
				{
					TFC_Ticker[0]=0;
					LineScanImageReady=0;
					
					n_pozycja = getXPosition();
					pozycja = xPosToServoPos(n_pozycja);
					if (pozycja >=1/*== LINE_SCAN_WRONG_POS*/) {			// stop jak nie ma lini
						lineNOTfound++;										//
						if (lineNOTfound >= 5){
							lineNOTfound=0;
							break;
						}
																		
					}
					
					else
					regulator_ROT(pozycja);

					TFC_SetServo(0, REGULACJA);
					DYFERENCJAL(REGULACJA, speedMAX);		//wprowadzamy polozenie serwa -1 do 1 aktualna predkosc obrotowa 
				}

				//TFC_Delay_mS(czas);

				if (TFC_PUSH_BUTTON_1_PRESSED) {
					TFC_HBRIDGE_DISABLE;
					TFC_SetMotorPWM(0, 0);
					TFC_SetServo(0, 0);
					break;
				}
			} //----------------------------------------------------------------------------------------

		}
		TFC_HBRIDGE_DISABLE;
		TFC_SetMotorPWM(0, 0);
		TFC_SetServo(0, 0);
	}
	return 0;
}

UART1_IRQHandler() {
	GET_NUMBERS(&PARAMETER1);	//interrupt receiver enable
	GREEN_LED_ON_ONLY();
//UART1_RUN();
}

