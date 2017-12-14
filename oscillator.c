#include "xc.h"
#include <p24FJ256GB606.h>

/************************ initialization oscillator **************************
 * configuration internal oscillator : HFFRQ  - 0101h 16mhz
 * set Center frequency. Oscillator module is running at the calibrated frequency 
 ****************************************************************************/

void Init_osc(void)
{ 

    OSCCONBITS OSCCONbitsCopy; // declare veriable to take backup 
    
    OSCCONbits.COSC = 0b010; // Primary Oscillator (XT, HS, EC
    OSCCONbits.NOSC = 0b100; // Secondary Oscillator (SOSC)
    OSCCONbits.CLKLOCK = 1; //clock and pll section are lock 
    OSCCONbits.IOLOCK = 0; //I/o lock is not active
    //OSCCONbits.CF = 1; //fscn has detect a clock failure //Clock Fail Detect bit
    OSCCONbits.POSCEN = 0; // primary oscillator disable during the  sleep  
    OSCCONbits.SOSCEN = 1; //enable secondary oscillator 

}






