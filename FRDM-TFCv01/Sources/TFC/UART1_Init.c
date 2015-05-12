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
#define TRANSMISION_CONDITION (UART1_S1 & UART_S1_RDRF_MASK)

void UART1_Init() {
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK | // W³¹czenie sygna³u taktuj¹cego port A
			SIM_SCGC5_PORTB_MASK |      // W³¹czenie sygna³u taktuj¹cego port B
			SIM_SCGC5_PORTC_MASK |      // W³¹czenie sygna³u taktuj¹cego port C
			SIM_SCGC5_PORTD_MASK |      // W³¹czenie sygna³u taktuj¹cego port D
			SIM_SCGC5_PORTE_MASK;        // W³¹czenie sygna³u taktuj¹cego port E

	SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;

	PORTB_PCR(RED_LED) = PORT_PCR_MUX(1);    // Funkcja alternatywna: 1 (GPIO)
	PORTB_PCR(GREEN_LED) = PORT_PCR_MUX(1);    // Funkcja alternatywna: 1 (GPIO)
	PORTD_PCR(BLUE_LED) = PORT_PCR_MUX(1);    // Funkcja alternatywna: 1 (GPIO)
	
	
	PORTE_PCR0 = PORT_PCR_ISF_MASK|PORT_PCR_MUX(3);
	PORTE_PCR1 = PORT_PCR_ISF_MASK|PORT_PCR_MUX(3);
//	PORTC_PCR3 = PORT_PCR_MUX(3);
//		PORTC_PCR4 = PORT_PCR_MUX(3);
	GPIOB_PDDR |= (1 << RED_LED);
	GPIOB_PDDR |= (1 << GREEN_LED);
	GPIOD_PDDR |= (1 << BLUE_LED);

	GPIOB_PCOR |= (1 << RED_LED);
	GPIOB_PSOR |= (1 << GREEN_LED);
	GPIOD_PSOR |= (1 << BLUE_LED);

	UART1_C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);
	
	
//	NVIC_CPR0=(1<<13);
//	NVIC_SER0=(1<<13);
	
	/* NVIC_IPR3: PRI_13=0x80 */
	NVIC_IPR3 = (uint32_t)((NVIC_IPR3 & (uint32_t)~(uint32_t)(
			NVIC_IP_PRI_13(0x7F)
	)) | (uint32_t)(
			NVIC_IP_PRI_13(0x80)
	));
	/* NVIC_ISER: SETENA|=0x2000 */
	NVIC_ISER |= NVIC_ISER_SETENA(0x2000);
	UART1_BDH = UART_BDH_SBR(0x00);

	UART1_BDL = UART_BDL_SBR(0x4E);
	UART1_C2 = UART_C2_RIE_MASK;
	UART1_C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK);
	  enable_irq(13);
}

void UART1_RUN() {
	PARAMETER3=9;
	char data;

	for (;;) {

		if (TRANSMISION_CONDITION)

		{
			data = UART1_D;    //READING INPUT
			//MENU MODE ALL LED ON----------------------------------------------------
			if (data == 'L') {
				PRINTF_WORDS(
						"\r\nMENU:\r\n"
								"PRESS 1-If YOU WANT TO RUN TEST MODE(BLUE LED ON)\r\n"
								"PRESS 2-If YOU WANT TO RUN SWITCH PARAMETERS MODE(GREEN LED ON)\r\n"
								"PRESS 3-If YOU WANT TO RUN RACE MODE(NO LEDs ON)\r\n");
				ALL_LED_ON();

			}

			if (data == '1') {
				BLUE_LED_ON_ONLY();//TEST MODE MACIEK KUBASZCZYK
			}

			//------------------------------SWITCH PARAMETERS MODE----------------

			if (data == '2') {
				GREEN_LED_ON_ONLY();
				GET_NUMBERS(&PARAMETER1);
				PLOT_NUMBERS(PARAMETER1);
				BLINK_LEDS(PARAMETER1);
			}

			//------------------------RACE MODE--------------
			if (data == '3') {
				ALL_LED_OFF();
				//GET_NUMBERS(&PARAMETER3);
				PLOT_NUMBERS(PARAMETER3);
			}

		}

	}

}
void RED_LED_ON_ONLY() {
	RED_LED_ON;
	BLUE_LED_OFF;
	GREEN_LED_OFF;
}
void BLUE_LED_ON_ONLY() {
	BLUE_LED_ON;
	RED_LED_OFF;
	GREEN_LED_OFF;
}
void GREEN_LED_ON_ONLY() {
	GREEN_LED_ON;
	BLUE_LED_OFF;
	RED_LED_OFF;
}
void ALL_LED_ON() {
	RED_LED_ON;
	BLUE_LED_ON;
	GREEN_LED_ON;
}
void ALL_LED_OFF() {
	RED_LED_OFF;
	BLUE_LED_OFF;
	GREEN_LED_OFF;
}
void PRINTF_WORDS(char *s)

{
	while (*s != '\0') {

		while (!(UART1_S1 & UART_S1_TDRE_MASK))

		{
		}
		UART1_D = *s;
		s++;

	}
}
void GET_NUMBERS(int *s) {
	PRINTF_WORDS("WPROWADZ WARTOSC PARAMETRU:");
	for (;;) {
		if ((UART1_S1 & UART_S1_RDRF_MASK)) {
			*s = UART1_D;
	PRINTF_WORDS("\r\n");
			break;
		}
	}

}
void PLOT_NUMBERS(int s) {
	if (s<10)
	{
		while (!(UART1_S1 & UART_S1_TDRE_MASK))

						{
						}
		UART1_D=s+48;
	}
	if (s>9 && s<100)
	{
		char dziesietna=s/10;
		char jednostkowa=s-10*dziesietna;
		while (!(UART1_S1 & UART_S1_TDRE_MASK))

								{
								}
				UART1_D=dziesietna+48;
		while (!(UART1_S1 & UART_S1_TDRE_MASK))

												{
												}
								UART1_D=jednostkowa+48;
	}
	if (s>99 && s<1000)
		{
			char setna=s/100;
			char dziesietna=(s-100*setna)/10;
			char jednostkowa=s-10*dziesietna-100*setna;
			while (!(UART1_S1 & UART_S1_TDRE_MASK))

												{
												}
								UART1_D=setna+48;
			while (!(UART1_S1 & UART_S1_TDRE_MASK))

									{
									}
					UART1_D=dziesietna+48;
			while (!(UART1_S1 & UART_S1_TDRE_MASK))

													{
													}
									UART1_D=jednostkowa+48;
		}
	if (s>999 && s<10000)
			{
				char tysieczna=s/1000;
				char setna=(s-1000*tysieczna)/100;
				char dziesietna=(s-100*setna-1000*tysieczna)/10;
				char jednostkowa=s-10*dziesietna-100*setna-1000*tysieczna;
				while (!(UART1_S1 & UART_S1_TDRE_MASK))

																	{
																	}
													UART1_D=tysieczna+48;
				while (!(UART1_S1 & UART_S1_TDRE_MASK))

													{
													}
									UART1_D=setna+48;
				while (!(UART1_S1 & UART_S1_TDRE_MASK))

										{
										}
						UART1_D=dziesietna+48;
				while (!(UART1_S1 & UART_S1_TDRE_MASK))

														{
														}
										UART1_D=jednostkowa+48;
			}
	}
void BLINK_LEDS(int NUMBER) {
	ALL_LED_OFF();
	int i;
for(i=0;i<NUMBER;i++)
{
	TFC_Delay_mS(500);
	BLUE_LED_ON_ONLY();
	TFC_Delay_mS(500);
	ALL_LED_OFF();

}
}

