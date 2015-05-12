#include "TFC\TFC.h"

void FTM2_IRQHandler()
{
	
	float czas;
	
	
		/*When a selected edge occurs on the channel input, the current value of the TPM counter
		is captured into the CnV register, at the same time the CHnF bit is set and the channel
		interrupt is generated if enabled by CHnIE = 1 (see the following figure).*/

//---------------------------------OVERFLLOW-------------------------------------
	  if ((TPM_SC_REG(TPM2_BASE_PTR) & TPM_SC_TOF_MASK) != 0U){ //Czy licznik siê przepe³ni³ ?
	    //TU TRZEBA DOKURWIÆ KOD PRZY RESTARCIE
		  Overflow++;
		  TPM_SC_REG(TPM2_BASE_PTR) |= TPM_SC_TOF_MASK;
//		  TPM_CNT_REG(TPM2_BASE_PTR) =  0;//reset
//		  TPM_SC_REG(TPM2_BASE_PTR) |= TPM_SC_TOF_MASK;								//skasuj flagê przerwania

	  }
 //-------------------------------------------------------------------------------------------
	  
//---------------------------------CHANEL 0--------------------------------------
	
	  if ((TPM_CnSC_REG(TPM2_BASE_PTR, 0) & TPM_CnSC_CHF_MASK) != 0U) { //CZY FLAGA KANA£U 0 JEST USTAWIONA
		 /*KOD PRZERWANIE OD PIERWSZEGO CZUJNIKA*/
		  	DstepP++;
		  	if(INT==0)
		  	{
		  		CounterValue=TPM_CNT_REG(TPM2_BASE_PTR);
		  		czas = TPM_CNT_REG(TPM2_BASE_PTR)*0.0031+Overflow*200;
		  		
		  	}
		  	if (INT==1)
		  	{
		  		czas = (CounterValue+TPM_CNT_REG(TPM2_BASE_PTR))*0.0031+Overflow*200; // czas w ms
		  		INT=0;
		  	}

		  	TPM_CNT_REG(TPM2_BASE_PTR) =  0;		//reset counter
		  	TPM_CnSC_REG(TPM2_BASE_PTR, 0) |= TPM_CnSC_CHF_MASK; 	// skasuj flagê kana³u
		  	
		  	INT++;
		  	Overflow=0;
		  	//skalowanie do programu NIE KONIECZNE
		  	speedP = 19.625/czas;	// predkosc w m/s (2*PI*r/4)/czas/*1000*/
		  
	  }
 //------------------------------------------------------------------------------------------

	  
//---------------------------------CHANEL 1-------------------------------------- 
	  if ((TPM_CnSC_REG(TPM2_BASE_PTR, 1) & TPM_CnSC_CHF_MASK) != 0U) {//CZY FLAGA KANA£U 1 JEST USTAWIONA
		  /*KODA PRZERWANIE OD DRUGIEGO CZUJNIKA*/
		  	DstepL++;
		  	if(INT==0)
		  	{
		  		CounterValue=TPM_CNT_REG(TPM2_BASE_PTR);
		  		czas = TPM_CNT_REG(TPM2_BASE_PTR)*0.0031+Overflow*200;
		  	}
		  	if (INT==1)
		  	{
		  		czas = (CounterValue+TPM_CNT_REG(TPM2_BASE_PTR))*0.0031+Overflow*200; // czas w ms
		  		INT=0;
		  	}
		  	TPM_CNT_REG(TPM2_BASE_PTR) =  0;		//reset counter
		  	TPM_CnSC_REG(TPM2_BASE_PTR, 1) |= TPM_CnSC_CHF_MASK; // skasuj flagê przerwania
		  	INT++;
		  	Overflow=0;
		  	//skalowanie do programu NIE KONIECZNE
		  	//speedL = 19.625/czas;	// predkosc w m/s (2*PI*r/4)/czas/*1000*/  (3.14*0.025/(czas))*250 == 19.625/czas
		  
	  }
 //----------------------------------------------------------------------------------
	 
	  
}

void ENKODER_init(void)
{
	Overflow=0;
	DstepP=0;
	speedP=0;

	DstepL=0;
	speedL=0;
	
	INT=0;
	
	
	//Podci¹gniecie zegara pod timer TMP2
    //Clock Setup for the TPM requires a couple steps. 
               //1st,  set the clock mux
               //See Page 124 of f the KL25 Sub-Family Reference Manual, Rev. 3, September 2012
               SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;// We Want MCGPLLCLK/2 (See Page 196 of the KL25 Sub-Family Reference Manual, Rev. 3, September 2012)
               SIM_SOPT2 &= ~(SIM_SOPT2_TPMSRC_MASK);
               SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1); //We want the MCGPLLCLK/2 (See Page 196 of the KL25 Sub-Family Reference Manual, Rev. 3, September 2012)

				//Enable the Clock to the FTM2 Module
				//See Page 207 of f the KL25 Sub-Family Reference Manual, Rev. 3, September 2012
				SIM_SCGC6 |= SIM_SCGC6_TPM2_MASK; 
               //The TPM Module has Clock.  Now set up the peripheral
	
	//Blow away the control registers to ensure that the counter is not running
	     TPM2_SC = 0;
	     TPM2_CONF = 0;
	     TPM2_CNT = TPM_CNT_COUNT(0x00);
	     TPM2_C0SC = 0;
	     TPM2_C1SC = 0;
	    
	//Wlasciwe ustawienia timera
	     
	     TPM2_MOD = TPM_MOD_MOD(0xFFFF);
	     
	     //Setup Channels 0 and 1       
	     TPM2_C0SC = (TPM_CnSC_CHIE_MASK | TPM_CnSC_ELSB_MASK | TPM_CnSC_ELSA_MASK); //,CHF=0,CHIE=1,MSB=0,MSA=0,ELSB=1,ELSA=1,??=0,DMA=0 */
	     TPM2_C1SC = (TPM_CnSC_CHIE_MASK | TPM_CnSC_ELSB_MASK | TPM_CnSC_ELSA_MASK);  // Uruchomine przerwania, tryb capture na dwoch zboczach DMA off
	     
	     TPM2_CNT = TPM_CNT_COUNT(0x00);      /* Reset counter register */
	     
	     
	     
	     // Uruchomienie przerwañ to jest cos z samym rdzeniema ARM, jest jakis modu³ ogarniania przerwañ nie ogarniam co roboie ale powinno dzia³aæ
	    // enable_irq (INT_TPM2-16); 	// w kodzie FRDM-TFC  to jest zdefiniowane
	     // NVIC_ICPR |= 1 << ((INT_TPM2-16)%32);
	     // NVIC_ISER |= 1 << ((INT_TPM2-16)%32);
	     /* NVIC_IPR4: PRI_19=0x80 */
	       NVIC_IPR4 = (uint32_t)((NVIC_IPR4 & (uint32_t)~(uint32_t)(
	                    NVIC_IP_PRI_19(0x7F)
	                   )) | (uint32_t)(
	                    NVIC_IP_PRI_19(0x80)
	                   ));
	       /* NVIC_ISER: SETENA|=0x00080000 */
	       NVIC_ISER |= NVIC_ISER_SETENA(0x00080000);
	      
	     
	     //Uruchomienie modu³u TMP na portach PTA1 i PTA2
	     PORTA_PCR1 = (uint32_t)((PORTA_PCR1 & (uint32_t)~(uint32_t)(
	                    PORT_PCR_ISF_MASK |
	                    PORT_PCR_MUX(0x04)
	                   )) | (uint32_t)(
	                    PORT_PCR_MUX(0x03)
	                   ));
	      /* PORTA_PCR2: ISF=0,MUX=3 */
	      PORTA_PCR2 = (uint32_t)((PORTA_PCR2 & (uint32_t)~(uint32_t)(
	                    PORT_PCR_ISF_MASK |
	                    PORT_PCR_MUX(0x04)
	                   )) | (uint32_t)(
	                    PORT_PCR_MUX(0x03)
	                   ));
	      
	      //ON
	      TPM2_SC = (TPM_SC_TOIE_MASK | TPM_SC_CMOD(0x01) | TPM_SC_PS(0x05));	// przerwania enable|tryb pracy counter| 0x06 prescaler 64 T=~~200ms
	  

}



