#include"usr_rtcc.h"
#include<xc.h>
#include <p24FJ256GB606.h>

//dharma
/*********************************************************************
 Function:        void RtccInitClock_v2(void)
 
    Overview : For seamless migration, it is recommended to call the above function as "RtccInitClock".

 PreCondition:    None
                  
 Input:           None
 
 Output:          None
 
 Side Effects:    None
 
 Overview:        The function initializes the RTCC device. It starts the RTCC clock,
                  sets the RTCC Off and disables RTCC write. Disables the OE.
                   
 Note:            None
 ********************************************************************/
void RtccInitClock(void)
{
   // enable the Secondary Oscillator
   __builtin_write_OSCCONL(OSCCON | 0x2); //saket check
   
   RTCCON1L = 0x0;
   
   if(mRtccIsOn())
   {
      if(!mRtccIsWrEn())
      {
          RtccWrOn();
      }
      mRtccOff();
   }
   
   mRtccWrOff();
}

/*********************************************************************
  Function:        void RtccWrOn_v2(void)
  
 Overview : For seamless migration, it is recommended to call the above function as "RtccWrOn".
 
  PreCondition:    None
 
  Input:           None
 
  Output:          None
 
  Side Effects:    None
 
  Overview:        Function to set the RCFGCAL.RTCWREN
 
  Note:            The interrupts are disabled in order to have a proper 
                   device initialization
 ********************************************************************/
void RtccWrOn(void)
{
   UINT8  CPU_IPL;
   unsigned register int data;

/* Disable Global Interrupt */
   SET_AND_SAVE_CPU_IPL(CPU_IPL,7);
  //saket check
  
   asm volatile ("MOV #0x55,%0" : "=r"(data));
   asm volatile ("MOV %0,NVMKEY" : : "r"(data)); // Write the 0x55 key
   asm volatile ("MOV #0xAA,%0" : "=r"(data));
   asm volatile ("MOV %0,NVMKEY"  : : "r"(data)); // Write the 0xAA key

   mRtccWrOn();  

/* Enable Global Interrupt */
   RESTORE_CPU_IPL(CPU_IPL);
}

#define done 0
#if done

/**********************************************************************************************
 Function:         void RtccSetCalibration_v2(int drift)
 
   Overview : For seamless migration, it is recommended to call the above function as "RtccSetCalibration". 
 
 PreCondition:     drift has to fit into signed 8 bits representation 
 
 Input:            drift - value to be added/subtracted to perform calibration 
 
 Output:           None
 
 Side Effects:     None
 
 Overview:         The function updates the value that the RTCC uses in the
                   auto-adjust feature, once every minute.
                   The drift value acts as a signed value, [-128*4, +127*4], 0 not having
                   any effect.
                   
 Note:             Writes to the RCFGCAL.CAL[7:0] register should  only occur when the timer
                   is turned off or immediately or after the edge of the seconds pulse
                   (except when SECONDS=00 - due to the possibility of the auto-adjust event).
                   In order to speed-up the process, the API function performs the reading
                   of the HALFSEC field.
                   The function may block for half a second, worst case, when called
                   at the start of the minute.
                   Interrupts can not be disabled for such a long period. However, long 
                   interrupt routines can interfere with the proper functioning of the
                   device.Care must be taken.
 ***********************************************************************************************/
void RtccSetCalibration_v2(int drift)
{
   if(mRtccIsOn())
   {
       UINT16  currSec;
       RCFGCAL1 &= ~RCFGCAL_MASK_RTCPTR;     // make sure you read seconds
       if(((currSec=RTCVAL)&0xff)==00)
       {   // we're at second 00, wait auto-adjust to be performed
           while(!RCFGCAL1bits.HALFSEC); // wait until second half...
       }
   }
   
   // update the CAL value
   RCFGCAL1bits.CAL=drift;
}

#endif
 /*********************************************************************
  Function:        void RtccReadDate_v1(rtccDate* pDt)

  PreCondition:    pDt a valid pointer
 
  Input:           pDt - pointer to a rtccTime union to store the current time
 
  Output:          None
 
  Side Effects:    None
 
  Overview:        The function updates the user supplied union/structure with
                   the current time of the RTCC device.
 
  Note:            The function makes sure that the read value is valid.
                   It avoids waiting for the RTCSYNC to be clear by 
                   performing successive reads.
 ********************************************************************/
void RtccReadDate(rtccDate* pDt)
{
   rtccTimeDate rTD0;

   rTD0.w[2]= DATEL;
   rTD0.w[3]|= DATEH;
    
   pDt->f.mday= rTD0.f.mday;
   pDt->f.mon = rTD0.f.mon;
   pDt->f.wday= rTD0.f.wday;
   pDt->f.year= rTD0.f.year;
}

 /*********************************************************************
  Function:        void RtccReadAlrmDate(rtccDate* pDt)

  PreCondition:    pDt a valid pointer
 
  Input:           pDt - pointer to a rtccDate union to store the alarm Date
 
  Output:          None
 
  Side Effects:    None
 
  Overview:        The function updates the user supplied union/structure with
                   the current alarm Date of the RTCC device. 
 ********************************************************************/

void RtccReadAlrmDate(rtccDate* pDt)
{
   rtccTimeDate rTD0;

   rTD0.w[2]=  ALMDATEL;
   rTD0.w[3]|= ALMDATEH;
    
   pDt->f.mday= rTD0.f.mday;
   pDt->f.mon = rTD0.f.mon;
   pDt->f.wday= rTD0.f.wday;
   pDt->f.year= rTD0.f.year;

}

 /*********************************************************************
  Function:        void RtccReadAlrmTime(rtccTime* pTm)
 
  PreCondition:    pTm a valid pointer
 
  Input:           pTm - pointer to a rtccTime union to store the alarm time
 
  Output:          None
 
  Side Effects:    None
 
  Overview:        The function updates the user supplied union/structure with
                   the current alarm time of the RTCC device.
 ********************************************************************/

void RtccReadAlrmTime(rtccTime* pTm)
{
   rtccTimeDate rTD;

   
   rTD.w[0]=ALMTIMEL;
   rTD.w[1]=ALMTIMEH;  // read the user value

   pTm->f.hour=rTD.f.hour;
   pTm->f.min=rTD.f.min;
   pTm->f.sec=rTD.f.sec;    // update user's data
}

 /*********************************************************************
  Function:        void RtccReadAlrmTimeDate(rtccTimeDate* pTD)
 
  PreCondition:    None 
 
  Input:           pTD - pointer to a rtccTimeDate union to store the alarm time and date
 
  Output:          None
 
  Side Effects:    None
 
  Overview:        The function updates the user supplied union/structure with
                   the current alarm time and date of the RTCC device.
 
  Note:            None
 ********************************************************************/
void RtccReadAlrmTimeDate(rtccTimeDate* pTD)
{
   pTD -> w[0] = ALMTIMEL;  // read the user value
   pTD -> w[1] = ALMTIMEH;
   pTD -> w[2] = ALMDATEL;
   pTD -> w[3] = ALMDATEH;  // read the user value
}

 /*********************************************************************
  Function:        void RtccReadTime(rtccTime *pTm)
 
  PreCondition:    pTm a valid pointer
 
  Input:           pTm - pointer to a rtccTime union to store the current time
  
  Output:          None
  
  Side Effects:    None
  
  Overview:        The function updates the user supplied union/structure with
                   the current time of the RTCC device.                   
  
  Note:            The function makes sure that the read value is valid.
                   It avoids waiting for the RTCSYNC to be clear by 
                   performing successive reads.
 ********************************************************************/
void RtccReadTime(rtccTime* pTm)
{
   rtccTimeDate rTD0;
   rTD0.w[0]=TIMEL;
   rTD0.w[1]=TIMEH;    // read the user value
   
   pTm->f.hour=rTD0.f.hour;
   pTm->f.min=rTD0.f.min;
   pTm->f.sec=rTD0.f.sec;    // update user's data
}

/*********************************************************************
  Function:        void RtccReadTimeDate(rtccTimeDate* pTD)
 
  PreCondition:    None
 
  Input:           pTD - pointer to a rtccTimeDate union to store the current 
                        time and date
  Output:          None
  
  Side Effects:    None
  
  Overview:        The function updates the user supplied union/structure with
                   the current time and date of the RTCC device.
 
  Note:            This firmware solution would consist of reading each       
                   register twice and then comparing the two values. If the   
                   two values match, then a rollover did not occur.
 ********************************************************************/
void RtccReadTimeDate(rtccTimeDate* pTD)
{
      pTD->w[0]=TIMEL;
      pTD->w[1]=TIMEH;
      pTD->w[2]=DATEL;
      pTD->w[3]=DATEH;    // read the user value
}

/*********************************************************************
  Function:        void RtccSetAlarmRpt(rtccRepeat rpt, BOOL dsblAlrm)

  PreCondition:    rpt has to be a proper rtccRepeat enumeration value 
 
  Input:           rpt - value of the desired alarm repeat rate 
                   dsblAlrm - if TRUE, the API can temporarily disable
                              the alarm when changing the RPT value 
  Output:          None
  
  Side Effects:    None
  
  Overview:        The function sets the RTCC alarm repeat count.
  
  Note:            If alarm is enabled, changing the repeat count can be safely made
                   only when the sync pulse is unasserted. To avoid waiting for the sync
                   pulse, the user can choose to temporarily disable the alarm and then
                   re-enable it. This means that the user has the knowledge that an alarm
                   event is not imminent.
 ********************************************************************/
void RtccSetAlarmRpt(rtccRepeat rpt, BOOL dsblAlrm)
{
   BOOL    isAlrm=mRtccIsAlrmEnabled();

   if(isAlrm)
   {
       if(dsblAlrm)
       {
           mRtccAlrmDisable();
       }
       else
       {
           mRtccWaitSync();
       }
   }

   // update the repeat rate
    RTCCON1Hbits.AMASK = rpt;   
   
    // restore previous status
   if(isAlrm && dsblAlrm)
   {
       mRtccAlrmEnable();
   }
}

/****************************************************************************************
  Function:        void RtccSetAlarmRptCount(UINT8 rptCnt, BOOL dsblAlrm)
 
  PreCondition:    rptCnt has to be a value less then 255 
  
  Input:           rptCnt - value of the desired alarm repeat rate 
                   dsblAlrm - if TRUE, the API can temporarily disable
                              the alarm when changing the RPT value 
  Output:          None
  
  Side Effects:    None
  
  Overview:        The function sets the RTCC alarm repeat rate.
  
  Note:            rptCnt will be truncated to fit into 8 bit representation.
                   If alarm is enabled, changing the repeat count can be safely made
                   only when the sync pulse is unasserted. To avoid waiting for the sync
                   pulse, the user can choose to temporarily disable the alarm and then
                   re-enable it. This means that the user has the knowledge that an alarm
                   event is not imminent.
 ****************************************************************************************/
void RtccSetAlarmRptCount(UINT8 rptCnt, BOOL dsblAlrm)
{
   BOOL isAlrm= mRtccIsAlrmEnabled();

   if(isAlrm)
   {
       if(dsblAlrm)
       {
           mRtccAlrmDisable();
       }
       else
       {
           mRtccWaitSync();
       }
   }
   
   // update the repeat count value
   RTCCON1Hbits.ALMRPT =rptCnt;

   // restore previous status
   if(isAlrm && dsblAlrm)
   {
       mRtccAlrmEnable();
   }
   
}

/****************************************************************************************
 Function:        void RtccSetChimeEnable(BOOL enable, BOOL dsblAlrm)
 
 PreCondition:    None
 
 Input:           enable - boolean to enable/disable the RTCC chime
                  dsblAlrm - if TRUE, the API can temporarily disable
                             the alarm when changing the Chime status 
 Output:          None
 
 Side Effects:    None
 
 Overview:        The function enables/disables the chime alarm of the RTCC device.
 
 Note:            If alarm is enabled, changing the chime status can be safely made
                  when the sync pulse is unasserted. To avoid waiting for the sync
                  pulse, the user can choose to temporarily disable the alarm and then
                  re-enable it. This means that the user has knowledge that an alarm
                  event is not imminent.                  
 *****************************************************************************************/
void RtccSetChimeEnable(BOOL enable, BOOL dsblAlrm)
{
   BOOL    isAlrm=mRtccIsAlrmEnabled();

   if(isAlrm)
   {
       if(dsblAlrm)
       {
           mRtccAlrmDisable();
       }
       else
       {
           mRtccWaitSync();
       }
   }

   // update the chime status
   RTCCON1Hbits.CHIME=enable;

   // restore previous status
   if(isAlrm && dsblAlrm)
   {
       mRtccAlrmEnable();
   }
}

/*********************************************************************
 Function:        void RtccWriteAlrmDate(const rtccDate* pDt)
 
 PreCondition:    pDt a valid rtccDate pointer having proper values:
                      *	wday: BCD codification, 00-06 
                      *	mday: BCD codification, 01-31
                      *	mon: BCD codification, 01-12
 
 Input:           pDt - pointer to a constant rtccDate union
 
 Output:          None
 
 Side Effects:    None
 
 Overview:        The function sets the alarm date in the RTCC device.
 
 Note:            The write is successful only if Wr Enable is set.
                  The function will enable the write itself, if needed.
                  Also, the Alarm will be temporarily disabled in order
                  to safely perform the update of the ALRMTIME register.
                  However, the device status will be restored. 
                  Note that the alarm date does not contain a year field.
 ********************************************************************/
BOOL RtccWriteAlrmDate(const rtccDate* pDt)
{
   WORD_VAL tempDayWDay ;
   WORD_VAL tempMonth ;

   BOOL    wasWrEn;
   BOOL    wasAlrm;

   if((MAX_MON < pDt->f.mon) || (MIN_MON > pDt->f.mon) || (MAX_MDAY < pDt->f.mday) || (MIN_MDAY > pDt->f.mday) || 
      (MAX_YEAR < pDt->f.year))
   {
       return FALSE;
   }

   tempMonth.byte.LB = pDt->f.mon;
   tempMonth.byte.HB = ALMDATEH;

   if(!(wasWrEn=mRtccIsWrEn()))
   {
       RtccWrOn();            // have to allow the WRTEN in order to write the new value
   }
   if((wasAlrm=mRtccIsAlrmEnabled()))
   {
       mRtccAlrmDisable();     // avoid spurious alarm interrupts
   }

   tempDayWDay.byte.LB =  pDt->f.mday;
   tempDayWDay.byte.HB = pDt->f.wday;


   ALMDATEL = tempDayWDay.Val;
   ALMDATEH = tempMonth.Val;

   if(wasAlrm)
   {
       mRtccAlrmEnable();
   }
   if(!wasWrEn)
   {
       mRtccWrOff();
   }

   return(TRUE);
}

/*********************************************************************
  Function:        void RtccWriteAlrmTime(const rtccTime* pTm)
 
  PreCondition:    pTm a valid rtccTime pointer having proper values:
                       *	sec:  BCD codification, 00-59 
                       *	min:  BCD codification, 00-59
                       *	hour: BCD codification, 00-24
 
  Input:           pTm - pointer to a constant rtccTime union
 
  Output:          None
 
  Side Effects:    None
 
  Overview:        The function sets the current time in the RTCC device.
 
  Note:            The write is successful only if Wr Enable is set.
                   The function will enable the write itself, if needed.
                   Also, the Alarm will be temporarily disabled in order
                   to safely perform the update of the ALRMTIME register.
                   However, the device status will be restored.
 ********************************************************************/
BOOL RtccWriteAlrmTime(const rtccTime* pTm)
{
   WORD_VAL tempSec ;
   WORD_VAL tempHourMin ;

   BOOL    wasWrEn;
   BOOL    wasAlrm;

   if((MAX_MIN < pTm->f.min)|| (MAX_SEC < pTm->f.sec) || (MAX_HOUR < pTm->f.hour))
   {
       return FALSE;
   }

   tempSec.byte.LB = ALMTIMEL;
   tempSec.byte.HB =pTm->f.sec;        // update the desired fields

   if(!(wasWrEn=mRtccIsWrEn()))
   {
       RtccWrOn();            // have to allow the WRTEN in order to write the new value
   }
   if((wasAlrm=mRtccIsAlrmEnabled()))
   {
       mRtccAlrmDisable();     // avoid spurious alarm interrupts
   }

   tempHourMin.byte.LB = pTm->f.min;
   tempHourMin.byte.HB = pTm->f.hour;

   ALMTIMEH = tempHourMin.Val;
   ALMTIMEL = tempSec.Val;

   if(wasAlrm)
   {
       mRtccAlrmEnable();
   }
   if(!wasWrEn)
   {
       mRtccWrOff();
   }

   return(TRUE);
}

/*********************************************************************
  Function:        BOOL RtccWriteAlrmTimeDate(const rtccTimeDate* pTD)
 
  PreCondition:    rtccTimeDate structure fields have to have proper values:
                       *	 sec:  BCD codification, 00-59 
                        *	min:  BCD codification, 00-59
                        *	hour: BCD codification, 00-24
                        *	wday: BCD codification, 00-06 
                        *	mday: BCD codification, 01-31
                        *	mon: BCD codification, 01-12
 
  Input:           pTD - pointer to a constant rtccTimeDate union
 
  Output:          TRUE '1' : If all the values are within range
                   FALSE '0' : If any value is out of above mentioned range.
 
  Side Effects:    None
 
  Overview:        The function sets the current alarm time and date in the RTCC device.
 
  Note:            Note that the alarm time does not contain a year field.
 ********************************************************************/
BOOL RtccWriteAlrmTimeDate(const rtccTimeDate* pTD)
{
    BOOL    wasWrEn;
    BOOL    wasAlrm;

   if((MAX_MIN < pTD->b[7]) || (MAX_SEC < pTD->b[6]) || (MAX_WDAY < pTD->b[5]) || (MAX_HOUR < pTD->b[4]) || 
      (MAX_MON < pTD->b[3]) || (MIN_MON > pTD->b[3]) || (MAX_MDAY < pTD->b[2]) || (MIN_MDAY > pTD->b[2]) )
   {
       return FALSE;
   }

   if(!(wasWrEn=mRtccIsWrEn()))
   {
       RtccWrOn();            // have to allow the WRTEN in order to write the new value
   }
   if((wasAlrm=mRtccIsAlrmEnabled()))
   {
       mRtccAlrmDisable();     // avoid spurious alarm interrupts
   }

   ALMTIMEL =pTD->w[0];  
   ALMTIMEH =pTD->w[1];
   ALMDATEL =pTD->w[2];
   ALMTIMEH =pTD->w[3];

   if(wasAlrm)
   {
       mRtccAlrmEnable();
   }
   if(!wasWrEn)
   {
       mRtccWrOff();
   }

   return(TRUE);
}

/***********************************************************************************
  Function:        BOOL RtccWriteDate(const rtccDate* pDt, BOOL di)
 
  PreCondition:    pDt is a valid rtccDate pointer having proper values:
                        *	wday: BCD codification, 00-06 
                        *	mday: BCD codification, 01-31
                        *	mon: BCD codification, 01-12
                        *	year: BCD codification, 00-99
 
  Input:           pDt - pointer to a constant rtccDate union
                   di - if interrupts need to be disabled
 
  Output:          TRUE '1' : If all the values are within range
                   FALSE '0' : If any value is out of above mentioned range.
 
  Side Effects:    None
 
  Overview:        The function updates the user supplied union/structure with
                   the current time and date of the RTCC device.
 
  Note:            The write is successful only if Wr Enable is set.
                   The function will enable the write itself, if needed.
                   Also, the Alarm will be temporarily disabled and the
                   device will be stopped (On set to 0) in order
                   to safely perform the update of the RTC time register.
                   However, the device status will be restored.
                   Usually the disabling of the interrupts is desired, if the 
                   user has to have more precise control over the actual moment 
                   of the time setting.
 *************************************************************************************/
BOOL RtccWriteDate(const rtccDate* pDt , BOOL di)
{
   WORD_VAL tempDayWDay ;
   WORD_VAL tempYearMonth ;
 
   UINT8  CPU_IPL;

   BOOL        wasWrEn;
   BOOL        wasOn;
   BOOL        wasAlrm=FALSE;

   if((MAX_MON < pDt->f.mon) || (MIN_MON > pDt->f.mon) || (MAX_MDAY < pDt->f.mday) || (MIN_MDAY > pDt->f.mday) || 
      (MAX_YEAR < pDt->f.year))
   {
       return(FALSE);
   }

   tempDayWDay.byte.HB = pDt->f.mday;
   tempDayWDay.byte.LB = pDt->f.wday;
   
   tempYearMonth.byte.HB = pDt->f.year;
   tempYearMonth.byte.LB = pDt->f.mon;

   if(di)
   {
      /* Disable Global Interrupt */
      SET_AND_SAVE_CPU_IPL(CPU_IPL,7);
   }
   
   if(!(wasWrEn= mRtccIsWrEn()))
   {
       RtccWrOn();            // have to allow the WRTEN in order to write the new value
   }
   if((wasOn= mRtccIsOn()))
   {
       wasAlrm= mRtccIsAlrmEnabled();
       mRtccOff();         // turn module off before updating the time
   }


   DATEL =  tempDayWDay.Val;
   DATEH = tempYearMonth.Val;

   if(wasOn)
   {
       mRtccOn();
       if(wasAlrm)
       {
           mRtccAlrmEnable();
       }
   
       if(wasWrEn)
       {
           RtccWrOn();
       }
   }
   else
   {
       if(!wasWrEn)
       {
           mRtccWrOff();
       }
   }
   
   if(di)
   {
      /* Enable Global Interrupt */
      RESTORE_CPU_IPL(CPU_IPL);
   }
   

   return(TRUE);
}

/***********************************************************************************
  Function:        BOOL RtccWriteTime(const rtccTime* pTm, BOOL di)
 
  PreCondition:    pTm pointing to a valid rtccTime structure having proper values:
                        *	sec:  BCD codification, 00-59 
                        *	min:  BCD codification, 00-59
                       *	hour: BCD codification, 00-24
                       
  Input:           pTm - pointer to a constant rtccTime union
                   di - if interrupts need to be disabled
                   
  Output:          TRUE '1' : If all the values are within range
                   FALSE '0' : If any value is out of above mentioned range.
                   
  Side Effects:    None
  
  Overview:        The function updates the user supplied union/structure with
                   the current time and date of the RTCC device.
                   
  Note:            The write is successful only if Wr Enable is set.
                   The function will enable the write itself, if needed.
                   Also, the Alarm will be temporarily disabled and the
                   device will be stopped (On set to 0) in order
                   to safely perform the update of the RTC time register.
                   However, the device status will be restored.
                   Usually the disabling of the interrupts is desired, if the 
                   user has to have more precise control over the actual moment
                   of the time setting.
 ***********************************************************************************/
BOOL RtccWriteTime(const rtccTime* pTm , BOOL di)
{
   WORD_VAL tempHourMin ;
   WORD_VAL tempSec;
   UINT8  CPU_IPL;

   BOOL        wasWrEn;
   BOOL        wasOn;
   BOOL        wasAlrm=FALSE;

   if((MAX_MIN < pTm->f.min )|| (MAX_SEC < pTm->f.sec) || (MAX_HOUR < pTm->f.hour))
   {
       return(FALSE);
   }

   tempSec.byte.LB = TIMEL;        // update the desired fields
   tempSec.byte.HB = pTm->f.sec;        // update the desired fields
   
   if(di)
   {
      /* Disable Global Interrupt */
      SET_AND_SAVE_CPU_IPL(CPU_IPL,7);
   }
   
   if(!(wasWrEn= mRtccIsWrEn()))
   {
       RtccWrOn();            // have to allow the WRTEN in order to write the new value
   }
   if((wasOn= mRtccIsOn()))
   {
       wasAlrm= mRtccIsAlrmEnabled();
       mRtccOff();         // turn module off before updating the time
   }

   tempHourMin.byte.LB = pTm->f.min;
   tempHourMin.byte.HB = pTm->f.hour;

   TIMEH = tempHourMin.Val;
   TIMEL = tempSec.Val;

   if(wasOn)
   {
       mRtccOn();
       if(wasAlrm)
       {
           mRtccAlrmEnable();
       }
   
       if(wasWrEn)
       {
           RtccWrOn();
       }
   }
   else
   {
       if(!wasWrEn)
       {
           mRtccWrOff();
       }
   }
   
   if(di)
   {
      /* Enable Global Interrupt */
      RESTORE_CPU_IPL(CPU_IPL);
   }
   

   return(TRUE);
}

/*********************************************************************
  Function:        BOOL RtccWriteTimeDate(const rtccTimeDate* pTD , BOOL di)
 
  PreCondition:    rtccTimeDate structure fields have to have proper values:
                        *	sec:  BCD codification, 00-59 
                        *	min:  BCD codification, 00-59
                        *	hour: BCD codification, 00-24
                        *	wday: BCD codification, 00-06 
                        *	mday: BCD codification, 01-31
                        *	mon: BCD codification, 01-12
                        *	year: BCD codification, 00-99
 
  Input:           pTD - pointer to a rtccTimeDate union to store the current 
                         time and date 
                       di - if interrupts need to be disabled
                   
  Output:          TRUE '1' : If all the values are within range
                   FALSE '0' : If any value is out of above mentioned range.
 
  Side Effects:    None
 
  Overview:        The function updates the user supplied union/structure with
                   the current time and date of the RTCC device.
 
  Note:            The write is successful only if Wr Enable is set.
                   The function will enable the write itself, if needed.
                   Also, the Alarm will be temporarily disabled and the
                   device will be stopped (On set to 0) in order
                   to safely perform the update of the RTC time register.
                   However, the device status will be restored.
                   Usually the disabling of the interrupts is desired, 
                   if the user has to have more precise control over the 
                   actual moment of the time setting.
 ********************************************************************/
BOOL RtccWriteTimeDate(const rtccTimeDate* pTD , BOOL di)
{
   BOOL        wasWrEn;
   BOOL        wasOn;
   BOOL        wasAlrm=FALSE;
   UINT8  CPU_IPL;

   if((MAX_MIN < pTD->b[7]) || (MAX_SEC < pTD->b[6]) || (MAX_WDAY < pTD->b[5]) || (MAX_HOUR < pTD->b[4]) || 
      (MAX_MON < pTD->b[3]) || (MIN_MON > pTD->b[3]) || (MAX_MDAY < pTD->b[2]) || (MIN_MDAY > pTD->b[2]) || 
      (MAX_YEAR < pTD->b[0]))
   {
       return(FALSE);
   }

   if(di)
   {
      /* Disable Global Interrupt */
      SET_AND_SAVE_CPU_IPL(CPU_IPL,7);
   }
   
   if(!(wasWrEn= mRtccIsWrEn()))
   {
       RtccWrOn();            // have to allow the WRTEN in order to write the new value
   }
   if((wasOn= mRtccIsOn()))
   {
       wasAlrm= mRtccIsAlrmEnabled();
       mRtccOff();         // turn module off before updating the time
   }

//        mRtccSetRtcPtr(3);

   TIMEL    = pTD->w[0];
   TIMEH    = pTD->w[1];
   ALMTIMEL = pTD->w[2];
   ALMTIMEH = pTD->w[3];   // update the RTCTIME value

   if(wasOn)
   {
       mRtccOn();
       if(wasAlrm)
       {
           mRtccAlrmEnable();
       }
   
       if(wasWrEn)
       {
           RtccWrOn();
       }
   }
   else
   {
       if(!wasWrEn)
       {
           mRtccWrOff();
       }
   }
   
   if(di)
   {
      /* Enable Global Interrupt */
      RESTORE_CPU_IPL(CPU_IPL);
   }
   

   return(TRUE);
}

 /*********************************************************************
 Function:        void RtccInitPowerControl(void)

 PreCondition:    None
                  
 Input:           config - Contents of the RTCCPWC control register.
 
 Output:          None
 
 Side Effects:    None
 
 Overview:        The function initializes the RTCC power control features.
                   
 Note:            None
 ********************************************************************/
void RtccInitPowerControl (unsigned int config)
{
   RTCCON1Lbits.PWCEN = config;
}
