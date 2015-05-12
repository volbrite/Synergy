#include "TFC\TFC.h"

volatile uint16_t  *LineScanImage0WorkingBuffer;
volatile uint16_t  *LineScanImage0;
volatile uint16_t  LineScanImage0Buffer[2][128];

volatile uint8_t  LineScanWorkingBuffer;

volatile int16_t  *LineScanDerivative0WorkingBuffer;
volatile int16_t  *LineScanDerivative0;
volatile int16_t  LineScanDerivative0Buffer[2][128];

volatile uint8_t LineScanImageReady = 0;

volatile LinePosDerivative *LineScanPos0WorkingBuffer;
volatile LinePosDerivative *LineScanPos0;
volatile LinePosDerivative linePos[2];

LineWidthDataStruct lineWidthData;

void TFC_InitLineScanCamera()
{
	SIM_SCGC5 |=     SIM_SCGC5_PORTE_MASK | SIM_SCGC5_PORTD_MASK; //Make sure the clock is enabled for PORTE;
	PORTE_PCR1 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;   //Enable GPIO on on the pin for the CLOCK Signal
	PORTD_PCR7 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;   //Enable GPIO on on the pin for SI signal
	
	PORTD_PCR5 = PORT_PCR_MUX(0); //Make sure AO signal goes to an analog input
	PORTD_PCR6 = PORT_PCR_MUX(0); //Make sure AO signal goes to an analog input
		
	//Make sure the Clock and SI pins are outputs
    GPIOD_PDDR |= (1<<7);
    GPIOE_PDDR |= (1<<1);
            
	TAOS_CLK_LOW;
	TAOS_SI_LOW;

	LineScanWorkingBuffer = 0;
	LineScanImage0WorkingBuffer = &LineScanImage0Buffer[0][0];
	LineScanImage0 = &LineScanImage0Buffer[1][0];
	
	LineScanDerivative0WorkingBuffer = &LineScanDerivative0Buffer[0][0];
	LineScanDerivative0 = &LineScanDerivative0Buffer[1][0];
	
	LineScanPos0WorkingBuffer = &linePos[0];
	LineScanPos0 = &linePos[1];
}


void TFC_SetLineScanExposureTime(uint32_t  TimeIn_uS)
{
		float t;
		
		//Figure out how many Pit ticks we need for for the exposure time
		t = (TimeIn_uS /1000000.0) * (float)(PERIPHERAL_BUS_CLOCK);
		PIT_LDVAL0 = (uint32_t)t;
	
}

inline int16_t LineScanCentralDerivative(uint16_t fx_0, uint16_t fx_2)
{
	return /*(int16_t)*/ (fx_2-fx_0)/2;
}

void LinescanCalibration()
{
	uint32_t i;
	//uint8_t buttonState;
	uint32_t lineWidth = 0;
	//Skopiowanie ustawieñ GPIO diod i przycisku na szildzie
	uint32_t prevSettPCRLed1 = LED_1_PCR;
	uint32_t prevSettPCRLed2 = LED_2_PCR;
	uint32_t prevSettDDRLeds = LED_PDDR;
	uint32_t prevSettPCRButton = BUTTON_PCR;
	uint32_t prevSettDDRButton = BUTTON_PDDR;
	
	
	//Konfiguracja GPIO dla diod I przycisku
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK;
	LED_1_PCR |= PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	LED_2_PCR |= PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	LED_PDDR |= (1<<LED_1) | (1<<LED_2);
	BUTTON_PCR |= PORT_PCR_MUX(1);
	BUTTON_PDDR ^= ~(1<<BUTTON);
	
	if(LINESCAN_CALIBRATION_TYPE & 1)
	{
		/*
		 * Zmierzenie grubosci linii
		* Ustaw pojazd na pod³ozu z lini¹ + zalacz oswietlenie
		*/
		LED_PSOR |= (1<<LED_1);
		
		while(!(BUTTON_PDIR & (1<<BUTTON)));//Oczekiwanie na nacisniecie przycisku 
		LED_PCOR |= (1<<LED_1);
		TFC_Delay_mS(20); //pierwsze drgania styków
		while(BUTTON_PDIR & (1<<BUTTON));
		TFC_Delay_mS(20); //drugie drgania styków
		LED_PSOR |= (1<<LED_1);
		
		for(i=0;i<100;i++)
		{
			while(!(TFC_Ticker[0]>100 && LineScanImageReady==1));
			TFC_Ticker[0] = 0;
			LineScanImageReady=0;
			if(LineScanPos0->a < LineScanPos0->b)	//Linia w zakresie pola widzenia
			{
				lineWidth += LineScanPos0->b - LineScanPos0->a;
			}
			
		}
		lineWidth = lineWidth/100;	
		lineWidthData.widthMax = lineWidth+1;
		lineWidthData.widthMin = lineWidth-1;
		GPIOB_PCOR |= 1<<LED_1;
	}
	
	if(LINESCAN_CALIBRATION_TYPE & 2)
	{
		/*
		 * Zmierzenie szumow
		 * Ustaw pojazd na podlozu BEZ linii
		 * Zapal swiat³o
		 */
		LED_PSOR |= (1<<LED_2);
		while(!(BUTTON_PDIR & (1<<BUTTON)));//Oczekiwanie na nacisniecie przycisku 
		LED_PCOR |= (1<<LED_2);
		TFC_Delay_mS(20); //pierwsze drgania styków
		while(BUTTON_PDIR & (1<<BUTTON));
		TFC_Delay_mS(20); //drugie drgania styków
		LED_PSOR |= (1<<LED_2);
		
		
		
		LED_PCOR |= (1<<LED_2);
	
	}
	//Przywrocenie zmienionych parametrow
	LED_1_PCR = prevSettPCRLed1;
	LED_2_PCR = prevSettPCRLed2;
	LED_PDDR = prevSettDDRLeds;
	BUTTON_PCR =  prevSettPCRButton;
	BUTTON_PDDR = prevSettDDRButton;
}



