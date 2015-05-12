/*
 * pozycja_linii.c
 *
 *  Created on: May 11, 2015
 *      Author: Maciek
 */

#include "TFC\TFC.h"

uint8_t ABSOfInt(int8_t value)
{
	int8_t mask;
	if(value>0)
		return value;
	mask = value >> 7;
	return ((value + mask) ^ mask);
}


float xPosToServoPos(int16_t xPosition)
{
	xPosition -=64.0;
	return xPosition / (64.0 + (lineWidthData.widthMax) / 2.0);
}

int16_t getXPosition()
{
	int16_t xPos;
	//Polozenie linii wg algorytmu "Zczytywanie polozenia linii" (31.03.2015)
	if(LineScanPos0->a < LineScanPos0->b)	//Linia w zakresie pola widzenia
	{
		//Progowanie pochodnych. Nie dodane do rysunku poprzedniego algorytmu 11.05.2015
		if((ABSOfInt(LineScanPos0->df_a) < LINESCAN_TRESHOLD_MAIN) ||  (LineScanPos0->df_b < LINESCAN_TRESHOLD_MAIN))
			xPos = LINE_SCAN_WRONG_POS;
		else
		{
			if((LineScanPos0->b - LineScanPos0->a) < lineWidthData.widthMin && (LineScanPos0->b - LineScanPos0->a) > lineWidthData.widthMax)
			{
				xPos = LINE_SCAN_WRONG_POS;
				//TERMINAL_PRINTF("%d;",LINE_SCAN_WRONG_POS);
			}
			else
			{
				xPos = (LineScanPos0->a+LineScanPos0->b)/2;
				//TERMINAL_PRINTF("%d;",xPos);
			}
		}
	}
	else //Linia na krawêdzi p.w.
	{
		//Progowanie pochodnych. Nie dodane do rysunku poprzedniego algorytmu 11.05.2015
		if((ABSOfInt(LineScanPos0->df_a) < LINESCAN_TRESHOLD_EDGE) ||  (LineScanPos0->df_b < LINESCAN_TRESHOLD_EDGE))
			xPos = LINE_SCAN_WRONG_POS;
		else
		{
			if(ABSOfInt(LineScanPos0->df_a) > LineScanPos0->df_b)// Linia na prawej krawêdzi
			{
				xPos = LineScanPos0->a;
				xPos +=lineWidthData.widthMin/2;
				//TERMINAL_PRINTF("%d;",xPos);
				//TERMINAL_PRINTF("%d;",0);
			}
			else //Linia na lewej krawêdzi
			{
				xPos = LineScanPos0->b;
				xPos -=lineWidthData.widthMin/2;
				//TERMINAL_PRINTF("%d;",xPos);
				//TERMINAL_PRINTF("%d;",127);			
			}
		}
	}
	return xPos;
	//TERMINAL_PRINTF("Szerokosc linii: %d, %d", lineWidthData.widthMin, lineWidthData.widthMax);
	//TERMINAL_PRINTF("\r\n");
}
