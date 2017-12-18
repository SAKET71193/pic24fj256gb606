#include "xc.h"
#include <p24FJ256GB606.h>
#include "oscillator.h"


/************************ initialization oscillator **************************
 * configuration internal oscillator : HFFRQ  - 0101h 16mhz
 * set Center frequency. Oscillator module is running at the calibrated frequency 
 ****************************************************************************/

int Init_osc(void)
{
    unsigned int pllCounter; 
    OSCCONBITS OSCCONbitsCopy; // declare veriable to take backup 
    OSCCONbitsCopy =  OSCCONbits;
    
    // Slow output clock down to 4Mhz
    CLKDIVbits.CPDIV = 3;
    // Enable the PLL - Note: Fuse bits don't do this
    CLKDIVbits.PLLEN = 1;
    // Wait for the PLL to stabalise
    for (pllCounter = 0; pllCounter < 600; pllCounter++);

    // Setup the uC to use the internal FRCPLL mode
    Nosc_Set(1);
    OscSwitchEnable(1);
    
    // Setup the uC to use the external crystal with the PLL
    //OSCCONbitsCopy.NOSC = 3;
    //OSCCONbitsCopy.OSWEN = 1;    
    
    // Switch over to the new clock setup
    __builtin_write_OSCCONH( BITS2BYTEH( OSCCONbitsCopy ) );
    __builtin_write_OSCCONL( BITS2BYTEL( OSCCONbitsCopy ) );
    // Wait for this transfer to take place
    while(OSCCONbits.LOCK == 0);
    
    if(user_COSC() != user_NOSC());
    return FALSE;
    
    //Setup the DIV bits for the FRC, this values means the config_word needs to be: PLLDIV_DIV2
    CLKDIVbits.RCDIV0 = 1; //FRCPLL mode 

    //// Setup the PLL divider for the correct clock frequency // 00 : 32MHz; 01 :16MHz; 10 : 8MHz; 11 : 4MHz
    CLKDIVbits.CPDIV = CLOCK_FREQ; 
    
    //Check that the PLL is enabled again and locked properly to the new setup
    CLKDIVbits.PLLEN = 1;
    
    OSCTUN = 0x9000;        //Enable active clock tuning from USB host reference
    
    #define try 0
    #if try 
        OSCCONbits.COSC = 0b010; // Primary Oscillator (XT, HS, EC
        OSCCONbits.NOSC = 0b100; // Secondary Oscillator (SOSC)
        OSCCONbits.CLKLOCK = 1; //clock and pll section are lock 
        OSCCONbits.IOLOCK = 0; //I/o lock is not active
        //OSCCONbits.CF = 1; //fscn has detect a clock failure //Clock Fail Detect bit
        OSCCONbits.POSCEN = 0; // primary oscillator disable during the  sleep  
        OSCCONbits.SOSCEN = 1; //enable secondary oscillator 
    #endif
        return TRUE;
}


int oscillator_switch(BYTE new_NOSC)
{
    
    OSCCONBITS OSCCONbitsCopy; // declare veriable to take backup 
    OSCCONbitsCopy =  OSCCONbits;

    // Setup the uC to use the internal FRCPLL mode
    // Setup the uC to use the internal FRCPLL mode
    Nosc_Set(new_NOSC);
    OscSwitchEnable(1);
    
    // Setup the uC to use the external crystal with the PLL
    //OSCCONbitsCopy.NOSC = 3;
    //OSCCONbitsCopy.OSWEN = 1;    
    
    // Switch over to the new clock setup
    __builtin_write_OSCCONH( BITS2BYTEH( OSCCONbitsCopy ) );
    __builtin_write_OSCCONL( BITS2BYTEL( OSCCONbitsCopy ) );
    // Wait for this transfer to take place
    while(OSCCONbits.LOCK == 0);
    
    if(user_COSC() != user_NOSC());
    return FALSE;
    
    return TRUE;
}
