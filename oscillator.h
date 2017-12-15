#include <xc.h> // include processor files - each processor file is guarded.  
#include"GenericTypeDefs.h"

#ifndef usr_osc_h
#define usr_osc_h


/****************************************************************************/
/*               Useful Macros                                              */
#define BITS2WORD(sfrBitfield)   ( *((unsigned int*) &sfrBitfield) )
// Convert a bitfield to a word (unsigned int).
#define BITS2BYTEL(sfrBitfield)   ( ((unsigned char*) &sfrBitfield)[0] )
// Return the low byte (as a unsigned char) of a bitfield.
#define BITS2BYTEH(sfrBitfield)   ( ((unsigned char*) &sfrBitfield)[1] )
// Return the high byte (as a unsigned char) of a bitfield.


/****************************************************************************/
/*               User Configurable Definitions                              */
// Processor clock frequency selection
#define   CLOCK_FREQ  0		// Use 32MHz clock - default for demo board
//#define CLOCK_FREQ  1     //16000000ULL Use 16MHz clock
//#define CLOCK_FREQ  2     //8000000UL -  Use 8MHz clock
//#define CLOCK_FREQ  3     //4000000ULL -Use 4MHz clock

/*********************************************************************
 * Macro:           Cosc_Set() 
 * PreCondition:    None
 * Side Effects:    None
 * Overview:        Current Oscillator Selection bits. 
 * Input:           source - the desired status of the Output Enable pin
 * Output:          None
 * Note:            None
 ********************************************************************/

#define Cosc_Set(source)  (OSCCONbits.COSC =  source)


/*********************************************************************
 * Macro:           Cosc_Set() 
 * PreCondition:    None
 * Side Effects:    None
 * Overview:        Current Oscillator Selection bits. 
 * Input:           None
 * Output:          Give current oscillator setting
 * Note:            None
 ********************************************************************/

#define user_COSC()  (OSCCONbits.COSC)


/*********************************************************************
 * Macro:           Nosc_Set() 
 * PreCondition:    None
 * Side Effects:    None
 * Overview:        New Oscillator Selection bits. 
 * Input:           source - the desired status of the Output Enable pin
 * Output:          None 
 * Note:            None
 ********************************************************************/
#define Nosc_Set(source)  (OSCCONbits.NOSC =  source)


/*********************************************************************
 * Macro:           Nosc() 
 * PreCondition:    None
 * Side Effects:    None
 * Overview:        New Oscillator Selection bits. 
 * Input:           None
 * Output:          give new oscillator bit  
 * Note:            None
 ********************************************************************/
#define user_NOSC()  (OSCCONbits.NOSC)


/*********************************************************************
 * Macro:           IsClockLock() 
 * PreCondition:    None
 * Side Effects:    None
 * Overview:        Get Clock Selection Lock Enable bit 
 * Input:           None
 * Output:          value OSCCONbits.CLKLOCK 
 * Note:            None
 ********************************************************************/
#define IsClockLock()  (OSCCONbits.CLKLOCK)

/*********************************************************************
 * Macro:           IsClockLock() 
 * PreCondition:    None
 * Side Effects:    None
 * Overview:        Clock Selection Lock Enable bit 
 * Input:           enable bit to set or clear clock lock bit
 * Output:          None
 * Note:            None
 ********************************************************************/
#define ClockLockSet(enable)  (OSCCONbits.CLKLOCK = enable)

/*********************************************************************
 * Macro:           ClockFailDetect() 
 * PreCondition:    None
 * Side Effects:    None
 * Overview:        fscn has detect a clock failure // Clock Fail Detect bit : if 1 set
 * Input:           enable bit to set or clear clock lock bit
 * Output:          value OSCCONbits.CF  bit
 * Note:            None
 ********************************************************************/
#define ClockFailDetect(enable)  (OSCCONbits.CF)

/*********************************************************************
 * Macro:           IsIoLock() 
 * PreCondition:    None
 * Side Effects:    None
 * Overview:        I/O Lock bit
 * Input:           None
 * Output:          Get I/O lock bit.
 * Note:            None
 ********************************************************************/
#define IsIoLock()  (OSCCONbits.IOLOCK)


/*********************************************************************
 * Macro:           IoLockSet() 
 * PreCondition:    None
 * Side Effects:    None
 * Overview:        Set I/o lock bit.
 * Input:           None
 * Output:          None.
 * Note:            None
 ********************************************************************/
#define IoLockSet(enable)  (OSCCONbits.IOLOCK = enable)

/*********************************************************************
 * Macro:           SecOscSet() 
 * PreCondition:    None
 * Side Effects:    None
 * Overview:        Enable / Disable secondary oscillator 
 * Input:           0/1 : to set 1
 * Output:          None.
 * Note:            None
 ********************************************************************/
#define SecOscSet(enable)  (OSCCONbits.SOSCEN = enable)

/*********************************************************************
 * Macro:           PllLockStatus() 
 * PreCondition:    None
 * Side Effects:    None
 * Overview:        PLL lock status bit 
                    1 = PLL module is in lock or PLL module start-up timer is satisfied
                    0 = PLL module is out of lock, PLL start-up timer is running or PLL is disabled 
 * Input:           None
 * Output:          Value Of OSCCONbits.lock
 * Note:            None
 ********************************************************************/
#define PllLockStatus(enable)  (OSCCONbits.LOCK)


/*********************************************************************
 * Macro:           PriOscSleep() 
 * PreCondition:    None
 * Side Effects:    None
 * Overview:        Primary Oscillator Sleep Enable bit 
                    1 = Primary Oscillator continues to operate during Sleep mode
                    0 = Primary Oscillator is disabled during Sleep mode
 * Input:           None
 * Output:          Value Of OSCCONbits.lock
 * Note:            None
 ********************************************************************/
#define PriOscSleep(enable)  (OSCCONbits.POSCEN = enable)



/*********************************************************************
 * Macro:           OscSwitchEnable() 
 * PreCondition:    None
 * Side Effects:    None
 * Overview:        OSWEN: Oscillator Switch Enable bit
                    1 = Initiates an oscillator switch to a clock source specified by the NOSC<2:0> bits
                    0 = Oscillator switch is complete
 * Input:           enable Switch oscillator option
 * Output:          Value Of OSCCONbits.lock
 * Note:            None
 ********************************************************************/
#define OscSwitchEnable(enable)  (OSCCONbits.OSWEN = enable)

/*********************************************************************
 * Macro:           IsOscSwitchComplete() 
 * PreCondition:    None
 * Side Effects:    None
 * Overview:        OSWEN: Oscillator Switch Enable bit
                    1 = Initiates an oscillator switch to a clock source specified by the NOSC<2:0> bits
                    0 = Oscillator switch is complete
 * Input:           enable Switch oscillator option
 * Output:          Value Of OSCCONbits.lock
 * Note:            None
 ********************************************************************/
#define IsOscSwitchComplete()  (OSCCONbits.OSWEN)



        
#endif