#ifndef TFC_LINESCANCAMERA_H_
#define TFC_LINESCANCAMERA_H_

#define LINE_SCAN_WRONG_POS 0x0FFF

/*
 * Struktura do wyznaczenia pozycji linii.
 * a,b: pozycje ekstremum pochodnej wskazan kamery
 * a - pozycja minimum
 * b - pozycja maksimum
 * (a+b)/2 - pozycja linii
 * df_a - wartosc minimalna
 * df_b - wartosc maksymalna
 */
typedef struct
{
	uint8_t a;
	int8_t df_a;
	uint8_t b;
	int8_t df_b;
}LinePosDerivative;

typedef struct
{
	uint8_t widthMax;
	uint8_t widthMin;
}LineWidthDataStruct;

#define TAOS_CLK_HIGH  GPIOE_PSOR = (1<<1)  
#define TAOS_CLK_LOW   GPIOE_PCOR = (1<<1)  
#define TAOS_SI_HIGH   GPIOD_PSOR = (1<<7)
#define TAOS_SI_LOW    GPIOD_PCOR =	(1<<7)

#define LED_1 11U
#define LED_2 10U
#define LED_1_PCR PORTB_PCR11
#define LED_2_PCR PORTB_PCR10
#define LED_PDDR  GPIOB_PDDR
#define	LED_PSOR  GPIOB_PSOR 
#define LED_PCOR  GPIOB_PCOR

#define BUTTON 17U
#define BUTTON_PCR PORTC_PCR17
#define BUTTON_PDDR GPIOC_PDDR
#define BUTTON_PSOR GPIOC_PSOR
#define BUTTON_PCOR GPIOC_PCOR
#define BUTTON_PDIR GPIOC_PDIR

extern volatile uint16_t  *LineScanImage0WorkingBuffer;
extern volatile uint16_t  *LineScanImage0;
extern volatile uint16_t  LineScanImage0Buffer[2][128];
extern volatile uint8_t  LineScanWorkingBuffer;
extern volatile uint8_t  LineScanImageReady;

extern volatile int16_t  *LineScanDerivative0WorkingBuffer;
extern volatile int16_t  *LineScanDerivative0;
extern volatile int16_t  LineScanDerivative0Buffer[2][128];

extern volatile LinePosDerivative *LineScanPos0WorkingBuffer;
extern volatile LinePosDerivative *LineScanPos0;
extern volatile LinePosDerivative linePos[2];

extern LineWidthDataStruct lineWidthData;
extern int16_t realLinePointPos;

void TFC_InitLineScanCamera();
void TFC_LineScanImageCamera();

void TFC_SetLineScanExposureTime(uint32_t  TimeIn_uS);

inline int16_t LineScanCentralDerivative(uint16_t fx_0, uint16_t fx_2);

void LinescanCalibration();

void EEPROM_init();

#endif /* TFC_LINESCANCAMERA_H_ */
