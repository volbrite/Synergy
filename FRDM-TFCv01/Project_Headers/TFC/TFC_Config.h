
#ifndef TFC_CONFIG_H_
#define TFC_CONFIG_H_

#define RELEASE					"1.0"

#define PCB_REV					A

//A TFC Ticker is a variable that will increment every 1mS in the Systick interrupt routine
//you can use it to for general purpose timing, scheduling events, etc.  The TFC_Ticker variable
//is just an array of 32-bit integers, use the Macro below to set how many tickers you need

#define NUM_TFC_TICKERS			4

//This is what the exposure time for the Linescan will be set to out of bootup
#define TFC_DEFAULT_LINESCAN_EXPOSURE_TIME_uS	5000 //previous : 50000
#define TAOS_CLK 5		//previous 50
//#define TERMINAL_FORMAT %d //prevoius: "%X"
#define LINE_WIDTH_MIN 10//in pixels. Dobrac doswiadczalnie przed zawodami i kazdym testem
#define LINESCAN_CALIBRATION_TYPE 1 //0 - none, 1 - tylko szerokosc linii, 2 - tylko szum, 3 - both
#define LINESCAN_TRESHOLD_MAIN 8
#define LINESCAN_TRESHOLD_EDGE 5

#define TFC_MOTOR_SWITCHING_FREQUENCY	((float)(4000.0))


#define TERMINAL_USE_SDA_SERIAL
#define SDA_SERIAL_BAUD		115200	
#define SDA_SERIAL_OUTGOING_QUEUE_SIZE	2048
#define SDA_SERIAL_INCOMING_QUEUE_SIZE	128

#endif /* TFC_CONFIG_H_ */
