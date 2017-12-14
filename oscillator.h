#include <xc.h> // include processor files - each processor file is guarded.  
#include"GenericTypeDefs.h"

#ifndef usr_osc_h
#define usr_osc_h

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




#endif