//*********************************************************************  
//*                         Final Project
//*                       BONUS 2 (X-Y axis)                           
//*                       McMaster University                         
//*                      2DP4 Microcontrollers                                                   
//*                  Zishu Wu   wuz78    400089778               
//*********************************************************************
//*********************************************************************
//*                           Description                              
//* Plot X and Y axis on the same graph.
//* Use Matlab "xy axis to performance this function"                                                            
//*********************************************************************
//*********************************************************************
//*                            References                             
//* 2DP4Lecture_DoyleFazliani_W8 Slide22,28,52
//* 2DP4Lecture_DoyleFazliani_W9 Slide5,7,15
//* HCS12 instruction set reference
//* The HCS19 9S12 An Introduction to Software and Hardware Interface 
//*********************************************************************
//*********************************************************************
/*Include*/
  #include <hidef.h>      /* common defines and macros */
  #include "derivative.h" /* derivative-specific definitions */
  #include "SCI.h"
/*Prototypes*/
  void setClk(void);
  void delay1ms(unsigned int multiple);
  void OutCRLF(void);


         /* DECLARE GLOBAL VARIABLES BELOW*/
 ///////////////////////////////////////////////////// 
unsigned int x;
unsigned int deg; 
char char_1;
char char_2;
unsigned int y; 
 /////////////////////////////////////////////////////

void main(void) { 
 
  // Initialize uC environment//
  setClk();         //set bus speed 6MHz
  SCI_Init(14400);
  
  ////////////////////ADC configuration//////////////////////////
  ATDCTL1 = 0x4F;		// set for 12-bit resolution
	ATDCTL3 = 0x90;		// right justified, one sample per sequence
	ATDCTL4 = 0x02;		// prescaler = 2; ATD clock = 6.25MHz / (2 * (2 + 1)) == 1.04MHz
	ATDCTL5 = 0x35;		// continuous conversion on channel 0
  
 

  for(;;) {
   x = ATDDR0;
   y = ATDDR1;

   char_1 = x & 0x00FF;
   char_2 = x >> 8;
   SCI_OutChar(char_1);
   SCI_OutChar(char_2);

   //SCI_OutString("X is");SCI_OutUDec(x);
   //SCI_OutString("   Y is");SCI_OutUDec(y); OutCRLF();
   
   char_1 = y & 0x00FF;
   char_2 = y >> 8;
   SCI_OutChar(char_1);
   SCI_OutChar(char_2);
   delay1ms(50);

        }  /*loop end*/
  } /* main end */ 


 

/////////////////////FUNCTION ///////////////////////
/////////////////Delay Function///////////////////////
void delay1ms(unsigned int multiple){
  unsigned int i; //loop control variable
  TSCR1 = 0x90;   //enable timer and fast timer flag clear
  TSCR2 = 0x00;   //Disable timer interrupt, set prescaler=1
  TIOS |= 0x01;   //Enable OC0 (not necessary)
  TC0 = TCNT + 6000; 
  for(i=0;i<multiple;i++) {
    TFLG2 = 0x80; //clear the TOF flag
    while (!(TFLG1_C0F));
    TC0 += 6000;
  }
  TIOS &= -0x01; //Disable OC0 (not necessary) 
}


////////////////Set Clock////////////////////
#define VCOFRQ 0x00        //VCOFRQ[1:0]  32MHz <= VCOCLK <= 48MHz
#define SYNDIV 0x05        //SYNDIV[5:0]  
#define REFFRQ 0x00        //REFFRQ[1:0]  2MHz < fREF <= 6MHz
#define REFDIV 0x80        //REFDIV[3:0]  Ref Divide is 1
void setClk(void){
  CPMUPROT = 0x26;               //Protection of clock configuration is disabled
								 // NOTE: On some Esduinos you may need to use CPMUPROT=0. Try both and see which value allows you to change the registers below in your debugger!
  
  CPMUCLKS = 0x80;               //PLLSEL=1. Select Bus Clock Source:  PLL clock or Oscillator.
  CPMUOSC = 0x00;                //OSCE=1. Select Clock Reference for PLLclk as:fOSC (8 MHz).
  
  CPMUREFDIV = REFFRQ+REFDIV;    //Set fREF divider and selects reference clock frequency Range. fREF= 4 MHz.  
  
  CPMUSYNR=VCOFRQ + SYNDIV;      //Set Syn divide and selects VCO frequency range. fVCO = 48 MHz.
  
  CPMUPOSTDIV=0x00;              //Set Post Divider (0x00= 0000 0000).   
  
  while (CPMUFLG_LOCK == 0) {}  //Wait for PLL to achieve desired tolerance of target frequency. NOTE: For use when the source clock is PLL. comment out when using external oscillator as source clock 
 
  CPMUPROT = 1;                  //Protection for clock configuration is reenabled 
}


////////////////OutCRLF////////////////////
void OutCRLF(void){
  SCI_OutChar(CR);
  SCI_OutChar(LF);
}