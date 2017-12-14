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



#endif