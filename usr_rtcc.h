#include <xc.h> // include processor files - each processor file is guarded.  
#include <GenericTypeDefs.h>

#ifndef usr_rtcc_h
#define usr_rtcc_h


// union/structure for read/write of time into the RTCC device
typedef union
{ 
    struct
    {
        BYTE    rsvd;       // reserved for future use
        BYTE    sec;        // BCD codification for seconds, 00-59
        BYTE    min;        // BCD codification for minutes, 00-59
        BYTE    hour;       // BCD codification for hours, 00-24
    }f;                     // field access
    BYTE        b[4];       // BYTE access
    unsigned short int      w[2];       // 16 bits access
    unsigned long int      l;          // 32 bits access
}rtccTime;

// union/structure for read/write of date into the RTCC device
typedef union
{
    struct
    {
        BYTE    wday;       // BCD codification for day of the week, 00-06
        BYTE    mday;       // BCD codification for day of the month, 01-31
        BYTE    mon;        // BCD codification for month, 01-12
        BYTE    year;       // BCD codification for years, 00-99
    }f;                     // field access
    BYTE        b[4];       // BYTE access
    unsigned short int      w[2];       // 16 bits access
    unsigned long int      l;          // 32 bits access   
}rtccDate;


// union/structure for read/write of time and date from/to the RTCC device
typedef union
{ 
        struct
    {
        BYTE    year;       // BCD codification for year, 00-99
        BYTE    mon;        // BCD codification for month, 01-12
        
        BYTE    mday;       // BCD codification for day of the month, 01-31
        BYTE    wday;       // BCD codification for day of the week, 00-06
        
        
        BYTE    hour;       // BCD codification for hours, 00-24   
        BYTE    min;        // BCD codification for minutes, 00-59
        
        BYTE    sec;        // BCD codification for seconds, 00-59 
        BYTE    rsvd;       // reserved for future use
    }f;                     // field access
    BYTE        b[8];       // BYTE access
    unsigned short int      w[4];       // 16 bits access
    unsigned long int      l[2];       // 32 bits access
}rtccTimeDate;


// valid values of alarm repetition for the RTCC device
typedef enum
{
    RTCC_RPT_HALF_SEC,      // repeat alarm every half second
    RTCC_RPT_SEC,           // repeat alarm every second
    RTCC_RPT_TEN_SEC,       // repeat alarm every ten seconds
    RTCC_RPT_MIN,           // repeat alarm every minute
    RTCC_RPT_TEN_MIN,       // repeat alarm every ten minutes
    RTCC_RPT_HOUR,          // repeat alarm every hour          
    RTCC_RPT_DAY,           // repeat alarm every day
    RTCC_RPT_WEEK,          // repeat alarm every week
    RTCC_RPT_MON,           // repeat alarm every month
    RTCC_RPT_YEAR           // repeat alarm every year (except when configured for Feb 29th.) 
}rtccRepeat;


#define MAX_MIN         	(0x59)/* BCD codification for minutes, 00-59 */
#define MAX_SEC         	(0x59) /* BCD codification for seconds, 00-59 */
#define MAX_WDAY        	(0x6)/* BCD codification for day of the week, 00-06 */
#define MAX_HOUR        	(0x24)/* BCD codification for hours, 00-24 */
#define MAX_MON         	(0x12)/* BCD codification for month, 01-12 */
#define MIN_MON         	(0x1)/* BCD codification for month, 0-1 */
#define MAX_MDAY        	(0x31) /* BCD codification for day of the month, 01-31 */
#define MIN_MDAY        	(0x1)/* BCD codification for day of the month, 0-1 */
#define MAX_YEAR        	(0x99)/* BCD codification for year, 00-99 */

 /*********************************************************************
 * Macro:           mRtccIsWrEn()
 * PreCondition:    None
 * Side Effects:    None
 * Overview:   Returns the value of RCFGCAL.RTCWREN.  
 * Input:           None
 * Output:          Returns the value of RCFGCAL.RTCWREN.         
 * Note:            None
 ********************************************************************/

#define mRtccIsWrEn()   (RTCCON1Lbits.WRLOCK)


 /*********************************************************************
 * Macro:           mRtccWrOff() 
 * PreCondition:    None
 * Side Effects:    None
 * Overview:   Macro to to clear the RCFGCAL.RTCWREN.
 * Input:           None
 * Output:          Macro to to clear the RCFGCAL.RTCWREN.
 * Note:            None
 ********************************************************************/

#define mRtccWrOff()   (RTCCON1Lbits.WRLOCK = 0)


 /*********************************************************************
 * Macro:           mRtccWrOff() 
 * PreCondition:    None
 * Side Effects:    None
 * Overview:   Macro to to clear the RCFGCAL.RTCWREN.
 * Input:           None
 * Output:          Macro to to clear the RCFGCAL.RTCWREN.
 * Note:            None
 ********************************************************************/

#define mRtccWrOn()   (RTCCON1Lbits.WRLOCK = 1)


 /*********************************************************************
 * Macro:           mRtccIsOn() 
 * PreCondition:    None
 * Side Effects:    None
 * Overview:        Macro to turn the RTCC on. 
 * Input:           None
 * Output:          value of RCFGCAL.RTCEN 
 * Note:            None
 ********************************************************************/
#define mRtccIsOn()   (RTCCON1Lbits.RTCEN)


 /*********************************************************************
 * Macro:           mRtccOff() 
 * PreCondition:    None
 * Side Effects:    None
 * Overview:        Macro to turn the RTCC off.
 * Input:           None
 * Output:          None 
 * Note:            None
 ********************************************************************/

#define mRtccOff()   (RTCCON1Lbits.RTCEN=0)


 /*********************************************************************
 * Macro:           mRtccOn() 
 * PreCondition:    None
 * Side Effects:    None
 * Overview:        Macro helper to turn the RTCC on.
 * Input:           None
 * Output:          None 
 * Note:            Setting RCFGCAL.RTCEN to 1 clears the RTCWREN, RTCSYNC,
 *                  HALFSEC, RTCOE and the ALCFGRPT register.
 *                  TODO: check this is TRUE for PIC24F too.
 ********************************************************************/

#define mRtccOn()   (RTCCON1Lbits.RTCEN=1)

 /*********************************************************************
 * Macro:           mRtccIsSync()
 * PreCondition:    None
 * Side Effects:    None
 * Overview:        Macro read the status of the sync signal.
 * Input:           None
 * Output:          TRUE if sync asserted, FALSE otherwise.
 * Note:            None
 ********************************************************************/

#define mRtccIsSync()   (RTCSTATLbits.SYNC)


 /*********************************************************************
 * Macro:           mRtccWaitSync()
 * PreCondition:    None
 * Side Effects:    None
 * Overview:        Macro to wait until RCFGCAL.RTCSYNC signals read/write is safe.
 * Input:           None
 * Output:          None 
 * Note:            In order to be sure that the write/read op is safe, interrupts 
 *                  should be disabled or kept very short (worst case scenario, sync
 *                  can be asserted for 32 RTCC clocks,i.e. almost 1ms, so it's not 
 *                  advisable to disable the interrupts for such a long period
 *                  of time. Care must be taken under these circumstances).
 ********************************************************************/
#define mRtccWaitSync()   while((BOOL)RTCSTATLbits.SYNC)


 /*********************************************************************
 * Macro:           mRtccIs2ndHalfSecond() 
 * PreCondition:    None
 * Side Effects:    None
 * Overview:        Macro to read the half-second status bit.
 * Input:           None
 * Output:          TRUE if it's the second half period of a second, FALSE otherwise. 
 * Note:            None
 ********************************************************************/

#define mRtccIs2ndHalfSecond()   (RTCSTATLbits.HALFSEC)

 /*********************************************************************
 * Macro :          mRtccSetClockOe(enable)
 * PreCondition:    None 
 * Side Effects:    None
 * Overview:        The Macro enables/disables the Output Enable pin of the RTCC. 
 * Input:           enable - the desired status of the Output Enable pin 
 * Output:          None
 * Note:            None
 ********************************************************************/

#define mRtccSetClockOe(enable)   (RTCCON1Lbits.RTCOE=enable)


 /*********************************************************************
 * Macro :          mRtccGetClockOe()
 * PreCondition:    None 
 * Side Effects:    None
 * Overview:        The Macro  returns the enabled/disabled status of the Output
 *                  Enable pin of the RTCC. 
 * Input:           None 
 * Output:          1 if Output Enable is enabled, 0 otherwise.
 * Note:            None
 ********************************************************************/
  
#define mRtccGetClockOe()   (RTCCON1Lbits.RTCOE)


 /*********************************************************************
 * Macro :          mRtccSetClockSource()
 * PreCondition:    None 
 * Side Effects:    None
 * Overview:        The Macro selects the clock source of the RTCC.
 * Input:           0 for SOSC, 1 for LPRC, 2 for 50Hz power-line, 3 for 60Hz power-line.
 * Output:         None.
 * Note:            None
 ********************************************************************/

#define RTCC_CLOCK_SOSC		0x00
#define RTCC_CLOCK_LPRC		0x01
#define RTCC_CLOCK_LINE50Hz	0x02 //PWRLCLK pin
#define RTCC_CLOCK_LINE60Hz	0x03 // system clock
#define mRtccSetClockSource(source)	(RTCCON2Lbits.CLKSEL = source)

 /*********************************************************************
 * Macro :          mRtccSetOutputSource()
 * PreCondition:    None 
 * Side Effects:    None
 * Overview:        The Macro selects the source of the RTCC output pin.
 * Input:           0 for Alarm pulse, 1 for Seconds clock, 2 for RTCC clock, 3 for Power Control.
 * Output:         None.
 * Note:            None
 ********************************************************************/

#define RTCC_OUTPUT_ALARM		0x00
#define RTCC_OUTPUT_SECONDS		0x01
#define RTCC_OUTPUT_RTCCCLK		0x02
#define RTCC_OUTPUT_POWERCONTROL	0x03
#define RTCC_Timestamp 0x04
#define mRtccSetOutputSource(source)	(RTCCON1Lbits.OUTSEL = source)

#define RTCC_POWERCONTROL_OFF (RTCCON1Lbits.PWCEN = 0)
#define RTCC_POWERCONTROL_ON (RTCCON1Lbits.PWCEN = 1)

#define RTCC_POWERCONTROL_ACTIVELOW (RTCCON1Lbits.PWCPOL = 0)
#define RTCC_POWERCONTROL_ACTIVEHIGH (RTCCON1Lbits.PWCPOL = 1)

#define RTCC_POWERCONTROL_STABILITY_PRESCALER(source)  (RTCCON3Lbits.PWCSTAB = source)

#define RTCC_POWERCONTROL_SAMPLE_PRESCALER(source)   (RTCCON3Lbits.PWCSAMP = source)

extern void __attribute__ ((section (".libperi"))) RtccWrOn(void);
extern void __attribute__ ((section (".libperi"))) RtccInitClock(void) ;
extern void __attribute__ ((section (".libperi"))) RtccSetCalibration(int drift) ;
extern void __attribute__ ((section (".libperi"))) RtccReadDate(rtccDate* pDt) ;
extern void __attribute__ ((section (".libperi"))) RtccReadAlrmDate(rtccDate* pDt); 
extern void __attribute__ ((section (".libperi"))) RtccReadAlrmTime(rtccTime* pTm); 
extern void __attribute__ ((section (".libperi"))) RtccReadAlrmTimeDate(rtccTimeDate* pTD) ;
extern void __attribute__ ((section (".libperi"))) RtccReadTime(rtccTime* pTm) ; 
extern void __attribute__ ((section (".libperi"))) RtccReadTimeDate(rtccTimeDate* pTD) ;
extern void __attribute__ ((section (".libperi"))) RtccSetAlarmRpt(rtccRepeat rpt, BOOL dsblAlrm) ; 
extern void __attribute__ ((section (".libperi"))) RtccSetAlarmRptCount(unsigned char rptCnt, BOOL dsblAlrm) ; 
extern void __attribute__ ((section (".libperi"))) RtccSetChimeEnable(BOOL enable, BOOL dsblAlrm) ; 
extern BOOL __attribute__ ((section (".libperi"))) RtccWriteAlrmDate(const rtccDate* pDt) ; 
extern BOOL __attribute__ ((section (".libperi"))) RtccWriteAlrmTime(const rtccTime* pTm) ;
extern BOOL __attribute__ ((section (".libperi"))) RtccWriteAlrmTimeDate(const rtccTimeDate* pTD) ; 
extern BOOL __attribute__ ((section (".libperi"))) RtccWriteDate(const rtccDate* pDt , BOOL di); 
extern BOOL __attribute__ ((section (".libperi"))) RtccWriteTime(const rtccTime* pTm , BOOL di) ;
extern BOOL __attribute__ ((section (".libperi"))) RtccWriteTimeDate(const rtccTimeDate* pTD , BOOL di) ;
extern void __attribute__ ((section (".libperi"))) RtccInitPowerControl (unsigned int config); 


/*********************************************************************
 * Macro:           mRtccIsAlrmEnabled() 
 * PreCondition:    None
 * Side Effects:    None
 * Overview:        Inline helper.
 * Input:           None
 * Output:          value of ALCFGRPT.ALRMEN
 * Note:            None
 ********************************************************************/
#define mRtccIsAlrmEnabled()   (RTCCON1Hbits.ALRMEN)

 /*********************************************************************
 * Macro:           mRtccAlrmEnable() 
 * PreCondition:    None
 * Side Effects:    None
 * Overview:        Macro to set the ALCFGRPT.ALRMEN.
 * Input:           None
 * Output:          None 
 * Note:            None
 ********************************************************************/
#define mRtccAlrmEnable()   (RTCCON1Hbits.ALRMEN =1)

 /*********************************************************************
 * Macro:           mRtccAlrmDisable()
 * PreCondition:    None
 * Side Effects:    None
 * Overview:        Macro to clear the ALCFGRPT.ALRMEN.
 * Input:           None
 * Output:          None 
 * Note:            None
 ********************************************************************/
#define mRtccAlrmDisable()   (RTCCON1Hbits.ALRMEN =0)


 /*********************************************************************
 * Macro:           mRtccSetAlrmPtr(mask)
 * PreCondition:    None
 * Side Effects:    None
 * Overview:        Macro to set the Alarm Value Register Window Pointer bits.
 * Input:           None
 * Output:          None
 * Note:            It sets the pointer to the slot beyond the ALRMDAY
 *                  macro mRtccClearAlrmPtr() must be called prior to this macro
********************************************************************/
#define mRtccSetAlrmPtr(mask)   (ALMRPT |= mask)

 /*********************************************************************
 * Macro:           mRtccClearAlrmPtr() 
 * PreCondition:    None
 * Side Effects:    None
 * Overview:        Macro to clear the Alarm Value Register Window Pointer bits.
 * Input:           None
 * Output:          None
 * Note:            It sets the pointer to the slot beyond the ALRMDAY
 ********************************************************************/
#define mRtccClearAlrmPtr()   (ALMRPT &=~ ALCFGRPT_MASK_ALRMPTR)


 /*********************************************************************
 * Macro :          mRtccGetChimeEnable()
 * PreCondition:    None
 * Side Effects:    None
 * Overview:        The Macro  returns the chime alarm of the RTCC device.
 * Input:           None
 * Output:          current status of the alarm chime 
 * Note:            None
 ********************************************************************/
#define mRtccGetChimeEnable()   (RTCCON1Hbits.CHIME)


 /*********************************************************************
 * Macro :          mRtccSetInt(enable)
 * PreCondition:    None
 * Side Effects:    None
 * Overview:        This Macro  enables/disables the RTCC event interrupts.
 * Input:           enable - enable/disable the interrupt
 * Output:          None
 * Note:            None.
 ********************************************************************/
#define mRtccSetInt(enable)   (IEC3bits.RTCIE = enable)

 /*********************************************************************
 * Macro :          mRtccSetIntPriority(pri)
 * PreCondition:    None
 * Side Effects:    None
 * Overview:        This Macro  sets the RTCC event interrupt priority.
 * Input:           pri - the interrupt priority
 * Output:          None 
 * Note:            None.
 ********************************************************************/
#define mRtccSetIntPriority(pri)   (IPC15bits.RTCIP = pri)

/*******************************************************************
Macro       : mRtcc_Clear_Intr_Status_Bit

Include     : Rtcc.h 

Description : Macro to Clear RTCC Interrupt Status bit 

Arguments   : None 

Remarks     : None 
*******************************************************************/
#define mRtcc_Clear_Intr_Status_Bit     (IFS3bits.RTCIF = 0)


 /*********************************************************************
 * Macro:           mRtccGetAlarmRpt()
 * PreCondition:    None 
 * Side Effects:    None
 * Overview:        The macro returns the current RTCC alarm repeat rate.
 * Input:           None 
 * Output:          The value of the current alarm repeat rate. 
 * Note:            None
 ********************************************************************/
#define mRtccGetAlarmRpt()   (RTCCON1Hbits.AMASK)

 /*********************************************************************
 * Macro:           mRtccGetAlarmRptCount()
 * PreCondition:    None 
 * Side Effects:    None
 * Overview:        The macro reads the RTCC alarm repeat counter.
 * Input:           None 
 * Output:          the current alarm repeat count 
 * Note:            None
 ********************************************************************/
#define mRtccGetAlarmRptCount()   ((unsigned char)ALMRPTbits.ARPT)


// RTCC definitions
typedef enum
{
    RCFGCAL_MASK_RTCEN      =   0x8000, 
    RCFGCAL_MASK_FRZ        =   0x4000,
    RCFGCAL_MASK_RTCWREN    =   0x2000,
    RCFGCAL_MASK_RTCSYNC    =   0x1000,
    RCFGCAL_MASK_HALFSEC    =   0x0800,
    RCFGCAL_MASK_RTCOE      =   0x0400,
    RCFGCAL_MASK_RTCPTR     =   0x0300,
    RCFGCAL_MASK_CAL        =   0x00ff
}RCFGCAL_MASK;

//Alarm definitions
typedef enum
{
    ALCFGRPT_MASK_ALRMEN    =   0x8000,
    ALCFGRPT_MASK_CHIME     =   0x4000,
    ALCFGRPT_MASK_AMASK     =   0x3c00,
    ALCFGRPT_MASK_ALRMPTR   =   0x0300,
    ALCFGRPT_MASK_ARPT      =   0x00ff
}ALCFGRPT_MASK;

// accessing the RTCC/Alarm Value Register Window pointer bits
typedef enum
{
    RTCCPTR_MASK_SECMIN     =   0x0000,
    RTCCPTR_MASK_HRSWEEK    =   0x0100,
    RTCCPTR_MASK_DAYMON     =   0x0200,
    RTCCPTR_MASK_YEAR       =   0x0300   
}RTCCPTR_MASK;


#endif