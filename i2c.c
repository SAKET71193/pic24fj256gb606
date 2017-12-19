#include <p24FJ256GB606.h>
#include"usr_i2c.h"


// library functions for i2c - 1
/*********************************************************************
Function Prototype  : void AckI2C1(void)

Include             : i2c.h

Description         : Generates I2C bus Acknowledge condition.

Arguments           : None

Return Value        : None

Remarks             : This function generates an I2C bus Acknowledge 
                      condition.
*********************************************************************/
void AckI2C1(void)
{
    I2C1CONLbits.ACKDT = 0;
    I2C1CONLbits.ACKEN = 1;
}

/******************************************************************************
Function Prototype : void OpenI2C1(unsigned int config1, unsigned int config2)

Include            : i2c.h
 
Description        : Configures the I2C module
 
Arguments          : config1 - This contains the parameter to configure the I2CCON register

					 I2C Enable bit					
					   *	I2C_ON					
					   *	I2C_OFF					
					 I2C Stop in Idle Mode bit					
					   *	I2C_IDLE_STOP					
					   *	I2C_IDLE_CON					
					 SCL Release Control bit					
					   *	I2C_CLK_REL					
					   *	I2C_CLK_HLD					
					 Intelligent Peripheral Management Interface Enable bit					
					   *	I2C_IPMI_EN					
					   *	I2C_IPMI_DIS					
					 10-bit Slave Address bit					
					   *	I2C_10BIT_ADD					
					   *	I2C_7BIT_ADD					
					 Disable Slew Rate Control bit					
					   *	I2C_SLW_DIS					
					   *	I2C_SLW_EN					
					 SMBus Input Level bits					
					   *	I2C_SM_EN					
					   *	I2C_SM_DIS					
					 General Call Enable bit					
					   *	I2C_GCALL_EN					
					   *	I2C_GCALL_DIS					
					 SCL Clock Stretch Enable bit					
					   *	I2C_STR_EN					
					   *	I2C_STR_DIS					
					 Acknowledge Data bit					
					   *	I2C_ACK					
					   *	I2C_NACK					
					 Acknowledge Sequence Enable bit					
					   *	I2C_ACK_EN					
					   *	I2C_ACK_DIS					
					 Receive Enable bit					
					   *	I2C_RCV_EN					
					   *	I2C_RCV_DIS					
					 Stop Condition Enable bit					
					   *	I2C_STOP_EN					
					   *	I2C_STOP_DIS					
					 Repeated Start Condition Enable bit					
					   *	I2C_RESTART_EN					
					   *	I2C_RESTART_DIS					
					 Start Condition Enable bit					
					   *	I2C_START_EN					
					   *	I2C_START_DIS			 
					
					 config2 - computed value for the baud rate generator
	 
Return Value      :  None
					 
Remarks           :  This function configures the I2C Control register and I2C 
                     Baud Rate Generator register
*******************************************************************************/
void OpenI2C1(unsigned int config1,unsigned int config2)
{
    I2C1CONL = config2;
    I2C1CONL = config1;
}

/**********************************************************************************************
Function Prototype : void IdleI2C1(void) 

Include            : i2c.h 

Description        : This function generates Wait condition until I2C bus is Idle. 

Arguments          : None 

Return Value       : None 

Remarks            : This function will be in a wait state until Start Condition Enable bit,
                     Stop Condition Enable bit, Receive Enable bit, Acknowledge Sequence
                     Enable bit of I2C Control register and Transmit Status bit I2C Status
                     register are clear. The IdleI2C function is required since the hardware
                     I2C peripheral does not allow for spooling of bus sequence. The I2C
                     peripheral must be in Idle state before an I2C operation can be initiated
                     or write collision will be generated.
***********************************************************************************************/
void IdleI2C1(void)
{
    /* Wait until I2C Bus is Inactive */
    while(I2C1CONLbits.SEN || I2C1CONLbits.RSEN || I2C1CONLbits.PEN || I2C1CONLbits.RCEN ||
          I2C1CONLbits.ACKEN || I2C1STATbits.TRSTAT);  
}


/*********************************************************************
Function Prototype : void StartI2C1(void)
 
Include            : i2c.h
 
Description        : Generates I2C Bus Start condition.
 
Arguments          : None
 
Return Value       : None
 
Remarks            : This function generates a I2C Bus Start condition.
*********************************************************************/
void StartI2C1(void)
{
     I2C1CONLbits.SEN = 1;   /* initiate Start on SDA and SCL pins */
}

/*********************************************************************
Function Prototype : void CloseI2C1(void)

Include            : i2c.h 

Description        : This function turns off the I2C module 

Arguments          : None 

Return Value       : None 

Remarks            : This function disables the I2C module and clears the
                     Master and Slave Interrupt Enable and Flag bits.
*********************************************************************/
void CloseI2C1(void)
{
    /* clear the I2CEN bit */
    I2C1CONLbits.I2CEN = 0;

    /* clear the SI2C & MI2C Interrupt enable bits */
    IEC1bits.SI2C1IE = 0;
    IEC1bits.MI2C1IE = 0;

    /* clear the SI2C & MI2C Interrupt flag bits */
    IFS1bits.SI2C1IF = 0;
    IFS1bits.MI2C1IF = 0;
}

/*********************************************************************
Function Prototype : void ConfigIntI2C1(unsigned int config) 

Include            : i2c.h

Description        : This function configures the I2C Interrupt. 

Arguments          : config - I2C interrupt priority and enable/disable 
                     information as defined below
					 
                     Master I2C Interrupt Enable/Disable
                       *	MI2C_INT_ON  
                       *	MI2C_INT_OFF
                     I2C slave Interrupt enable/disable
                       *	SI2C_INT_ON
                       *	SI2C_INT_OFF
                     I2C master Interrupt priority
                       *	MI2C_INT_PRI_7					
					   *	MI2C_INT_PRI_6					
					   *	MI2C_INT_PRI_5					
					   *	MI2C_INT_PRI_4					
					   *	MI2C_INT_PRI_3					
					   *	MI2C_INT_PRI_2					
					   *	MI2C_INT_PRI_1					
					   *	MI2C_INT_PRI_0
					   *	MI2C_SRC_DIS 					
					 I2C slave Interrupt priority					
					   *	SI2C_INT_PRI_7					
					   *	SI2C_INT_PRI_6					
					   *	SI2C_INT_PRI_5					
					   *	SI2C_INT_PRI_4					
					   *	SI2C_INT_PRI_3					
					   *	SI2C_INT_PRI_2					
					   *	SI2C_INT_PRI_1					
					   *	SI2C_INT_PRI_0
					   *	SI2C_SRC_DIS	
					   				 
Return Value       : None 

Remarks            : This function clears the Interrupt Flag bits, 
                     sets the interrupt priorities of master and slave
                     and enables/disables the interrupt.
*********************************************************************/
void ConfigIntI2C1(unsigned int config)
{
     IFS1bits.SI2C1IF = 0;                       /* clear the MI2C & SI2C Interrupts */
     IFS1bits.MI2C1IF = 0;

     IPC4bits.SI2C1IP = (config & 0x0007);        /* set the SI2C priority */
     IPC4bits.MI2C1IP = (config & 0x0070) >> 4;    /* set the MI2C priority */

     IEC1bits.SI2C1IE = (config & 0x0008)>> 3;   /* enable/disable the SI2C Interrupt */
     IEC1bits.MI2C1IE = (config & 0x0080) >> 7;    /* enable/disable the MI2C Interrupt */
}

/************************************************************************
Function Prototype : char DataRdyI2C1(void) 

Include            : i2c.h 

Description        : This function provides status back to user if I2CRCV
                     register contain data. 
                     
Arguments          : None 

Return Value       : This function returns ‘1’ if there is data in I2CRCV register;
                     else return ‘0’ which indicates no data in I2CRCV register. 

Remarks            : This function determines if there is any byte to read from
                     I2CRCV register.
*************************************************************************/
char DataRdyI2C1(void)
{
     return I2C1STATbits.RBF;
}

/**********************************************************************************
Function Prototype : unsigned int getsI2C1_v3(unsigned char *rdptr, 
                                                unsigned int i2c_data_wait)
                                              
Include            : i2c.h
 
Description        : This function reads pre-determined data string length 
                     from the I2C bus.
 
Arguments          : rdptr - Character type pointer to RAM for storage of data 
                     read from I2C device.
                     i2c_data_wait - This is the time-out count for which the 
                     module has  to wait before return.
                     If the timeout count is ‘N’, the actual time out would
                     be about (20*N-1) instruction cycles.
 
Return Value       : Returns the number of bytes received from the I2C bus.
 
Remarks            : This routine reads a predefined data string from the I2C bus.
************************************************************************************/

unsigned int getsI2C1(unsigned char * rdptr, unsigned char length )
{
while ( length-- )            // perform getcI2C1() for 'length' number of bytes
    {
      unsigned int word;
	  word =  *rdptr++ ;       // save byte received
	  
      while ( I2C1CONLbits.RCEN ); // check that receive sequence is over    

      if ( IFS5bits.I2C1BCIF )       // test for bus collision
      {
        return ( -1 );             // return with Bus Collision error 
      }

	  
	if( ((I2C1CONL &0x0F)==0x08) || ((I2C1CONL &0x0F)==0x0B) )	//master mode only
	{	
      if ( length )               // test if 'length' bytes have been read
      {
        I2C1CONLbits.ACKDT = 0;    // set acknowledge bit state for ACK        
        I2C1CONLbits.ACKEN = 1;    // initiate bus acknowledge sequence
        while ( I2C1CONLbits.ACKEN ); // wait until ACK sequence is over 
      } 
	} 
	  
    }
    return ( 0 );                
}


/************************************************************************************************
Function Prototype : char MasterWriteI2C1(unsigned char data_out)

Include            : i2c.h
 
Description        : This function is used to write out a single data byte to the I2C device.
 
Arguments          : data_out - A single data byte to be written to the I2C bus device.
 
Return Value       : This function returns -1 if there was a write collision else it returns a 0.
 
Remarks            : This function writes  a single byte to the I2C bus.
                     This function performs the same function as MasterputcI2C.
**************************************************************************************************/

char MasterWriteI2C1(unsigned char data_out)
{
    I2C1TRN = data_out;

    if(I2C1STATbits.IWCOL)        /* If write collision occurs,return -1 */
        return -1;
    else
    {
        return 0;
    }
}

/***************************************************************************************************
Function Prototype :  unsigned int MastergetsI2C1(unsigned int length,
                          unsigned char *rdptr, unsigned int i2c_data_wait)
                           
Include            :  i2c.h 

Description        :  This function reads predetermined data string length from the I2C bus. 

Arguments          :  length - Number of bytes to read from I2C device.
                      rdptr - Character type pointer to RAM for storage of data read from I2C device
                      i2c_data_wait - This is the timeout count for which the module has
                      to wait before return. If the timeout count is ‘N’, the actual time out would
                      be about (20 * N – 1) instruction cycles. 
                      
Return Value       :  This function returns ‘0’ if all bytes have been sent or number of bytes
                      read from I2C bus if its not able to read the data with in the specified
                      i2c_data_wait time out value 
                     
Remarks            :  This routine reads a predefined data string from the I2C bus.
******************************************************************************************************/

unsigned int MastergetsI2C1(unsigned int length, unsigned char * rdptr, unsigned int i2c1_data_wait)
{
    int wait = 0;
    while(length)                    /* Receive the number of bytes specified by length */
    {
        I2C1CONLbits.RCEN = 1;
        while(!DataRdyI2C1())
        {
            if(wait < i2c1_data_wait)
                wait++ ;                 
            else
            return(length);          /* Time out, return number of byte/word to be read */			
        }
        wait = 0;
        *rdptr = I2C1RCV;             /* save byte received */
        rdptr++;
        length--;
        if(length == 0)              /* If last char, generate NACK sequence */
        {
            I2C1CONLbits.ACKDT = 1;
            I2C1CONLbits.ACKEN = 1;
        }
        else                         /* For other chars,generate ACK sequence */
        {
            I2C1CONLbits.ACKDT = 0;
            I2C1CONLbits.ACKEN = 1;
        }
            while(I2C1CONLbits.ACKEN == 1);    /* Wait till ACK/NACK sequence is over */
    }
    return 0;    /* return status that number of bytes specified by length was received */
}


/**************************************************************************************
Function Prototype : char MasterputsI2C1(unsigned char *wrptr) 

Include            : i2c.h 

Description        : This function is used to write out a data string to the I2C bus. 

Arguments          : wrptr - Character type pointer to data objects in RAM. The data
                     objects are written to the I2C device.
                      
Return Value       : This function returns -3 if a write collision occurred.
                     This function returns ‘0’ if the null character was reached 
                     in data string.
	                  
Remarks            : This routine writes a predefined data string from the I2C bus.
**************************************************************************************/

char MasterputsI2C1(unsigned char * wrptr)
{
    while(*wrptr)                           //transmit data until null char
    {
        while(I2C1STATbits.TBF);             //Wait till data is transmitted.
        IdleI2C1();
        wrptr++;
    }
    return 0;           
}


/*********************************************************************
Function Prototype : void StopI2C1(void)
 
Include            : i2c.h
 
Description        : Generates I2C Bus Stop condition.
 
Arguments          : None
 
Return Value       : None
 
Remarks            : This function generates a I2C Bus Stop condition.
*********************************************************************/

void StopI2C1(void)
{
     I2C1CONLbits.PEN = 1;   /* initiate Stop on SDA and SCL pins */
}


/******************************************************************************
Function Prototype : unsigned char MasterReadI2C1(void)
 
Include            : i2c.h
 
Description        : This function is used to read a single byte from I2C bus
 
Arguments          : None
 
Return Value       : The return value is the data byte read from the I2C bus.
 
Remarks            : This function reads in a single byte from the I2C bus.
                     This function performs the same function as MastergetcI2C.
********************************************************************************/

unsigned char MasterReadI2C1(void)
{
    I2C1CONLbits.RCEN = 1;
    while(I2C1CONLbits.RCEN);
    I2C1STATbits.I2COV = 0;
    return(I2C1RCV);
}

/***********************************************************************
Function Prototype : void MasterWaitForIntrI2C1(void)
 
Include            : i2c.h
 
Description        : This routine will wait for Master interrupt request 
                     and then clear interrupt Flag.
                      
Arguments          : None
 
Return Value       : None
 
Remarks            : It wait for Master interrupt request and then 
                     clear interrupt Flag.
************************************************************************/

void MasterWaitForIntrI2C1(void)
{
   while(0 == IFS1bits.MI2C1IF);
   IFS1bits.MI2C1IF = 0;
}

/*********************************************************************************
Function Prototype : void NotAckI2C1(void)
 
Include            : i2c.h
 
Description        : Generates I2C bus Not Acknowledge condition during Master Receive.
 
Arguments          : None
 
Return Value       : None
 
Remarks            : This function generates an I2C bus Not Acknowledge condition.
**********************************************************************************/

void NotAckI2C1(void)
{
    I2C1CONLbits.ACKDT = 1;
    I2C1CONLbits.ACKEN = 1;
}

/*************************************************************************
Function Prototype : void RestartI2C1(void)

Include            : i2c.h
 
Description        : Generates I2C Bus Restart condition.
 
Arguments          : None
 
Return Value       : None
 
Remarks            : This function generates an I2C Bus Restart condition.
**************************************************************************/

void RestartI2C1(void)
{ 
    I2C1CONLbits.RSEN = 1;   /* initiate restart on SDA and SCL pins */
}


// library functions for i2c - 2
/*********************************************************************
Function Prototype  : void AckI2C2(void)

Include             : i2c.h

Description         : Generates I2C bus Acknowledge condition.

Arguments           : None

Return Value        : None

Remarks             : This function generates an I2C bus Acknowledge 
                      condition.
*********************************************************************/
void AckI2C2(void)
{
    I2C2CONLbits.ACKDT = 0;
    I2C2CONLbits.ACKEN = 1;
}

/******************************************************************************
Function Prototype : void OpenI2C2(unsigned int config1, unsigned int config2)

Include            : i2c.h
 
Description        : Configures the I2C module
 
Arguments          : config1 - This contains the parameter to configure the I2CCON register

					 I2C Enable bit					
					   *	I2C_ON					
					   *	I2C_OFF					
					 I2C Stop in Idle Mode bit					
					   *	I2C_IDLE_STOP					
					   *	I2C_IDLE_CON					
					 SCL Release Control bit					
					   *	I2C_CLK_REL					
					   *	I2C_CLK_HLD					
					 Intelligent Peripheral Management Interface Enable bit					
					   *	I2C_IPMI_EN					
					   *	I2C_IPMI_DIS					
					 10-bit Slave Address bit					
					   *	I2C_10BIT_ADD					
					   *	I2C_7BIT_ADD					
					 Disable Slew Rate Control bit					
					   *	I2C_SLW_DIS					
					   *	I2C_SLW_EN					
					 SMBus Input Level bits					
					   *	I2C_SM_EN					
					   *	I2C_SM_DIS					
					 General Call Enable bit					
					   *	I2C_GCALL_EN					
					   *	I2C_GCALL_DIS					
					 SCL Clock Stretch Enable bit					
					   *	I2C_STR_EN					
					   *	I2C_STR_DIS					
					 Acknowledge Data bit					
					   *	I2C_ACK					
					   *	I2C_NACK					
					 Acknowledge Sequence Enable bit					
					   *	I2C_ACK_EN					
					   *	I2C_ACK_DIS					
					 Receive Enable bit					
					   *	I2C_RCV_EN					
					   *	I2C_RCV_DIS					
					 Stop Condition Enable bit					
					   *	I2C_STOP_EN					
					   *	I2C_STOP_DIS					
					 Repeated Start Condition Enable bit					
					   *	I2C_RESTART_EN					
					   *	I2C_RESTART_DIS					
					 Start Condition Enable bit					
					   *	I2C_START_EN					
					   *	I2C_START_DIS			 
					
					 config2 - computed value for the baud rate generator
	 
Return Value      :  None
					 
Remarks           :  This function configures the I2C Control register and I2C 
                     Baud Rate Generator register
*******************************************************************************/
void OpenI2C2(unsigned int config1,unsigned int config2)
{
    I2C2CONL = config2;
    I2C2CONL = config1;
}

/**********************************************************************************************
Function Prototype : void IdleI2C2(void) 

Include            : i2c.h 

Description        : This function generates Wait condition until I2C bus is Idle. 

Arguments          : None 

Return Value       : None 

Remarks            : This function will be in a wait state until Start Condition Enable bit,
                     Stop Condition Enable bit, Receive Enable bit, Acknowledge Sequence
                     Enable bit of I2C Control register and Transmit Status bit I2C Status
                     register are clear. The IdleI2C function is required since the hardware
                     I2C peripheral does not allow for spooling of bus sequence. The I2C
                     peripheral must be in Idle state before an I2C operation can be initiated
                     or write collision will be generated.
***********************************************************************************************/
void IdleI2C2(void)
{
    /* Wait until I2C Bus is Inactive */
    while(I2C2CONLbits.SEN || I2C2CONLbits.RSEN || I2C2CONLbits.PEN || I2C2CONLbits.RCEN ||
          I2C2CONLbits.ACKEN || I2C2STATbits.TRSTAT);  
}


/*********************************************************************
Function Prototype : void StartI2C2(void)
 
Include            : i2c.h
 
Description        : Generates I2C Bus Start condition.
 
Arguments          : None
 
Return Value       : None
 
Remarks            : This function generates a I2C Bus Start condition.
*********************************************************************/
void StartI2C2(void)
{
     I2C2CONLbits.SEN = 1;   /* initiate Start on SDA and SCL pins */
}

/*********************************************************************
Function Prototype : void CloseI2C1(void)

Include            : i2c.h 

Description        : This function turns off the I2C module 

Arguments          : None 

Return Value       : None 

Remarks            : This function disables the I2C module and clears the
                     Master and Slave Interrupt Enable and Flag bits.
*********************************************************************/
void CloseI2C2(void)
{
    /* clear the I2CEN bit */
    I2C2CONLbits.I2CEN = 0;

    /* clear the SI2C & MI2C Interrupt enable bits */
    IEC3bits.SI2C2IE = 0;
    IEC3bits.MI2C2IE = 0;

    /* clear the SI2C & MI2C Interrupt flag bits */
    IFS3bits.SI2C2IF = 0;
    IFS3bits.MI2C2IF = 0;
}

/*********************************************************************
Function Prototype : void ConfigIntI2C1(unsigned int config) 

Include            : i2c.h

Description        : This function configures the I2C Interrupt. 

Arguments          : config - I2C interrupt priority and enable/disable 
                     information as defined below
					 
                     Master I2C Interrupt Enable/Disable
                       *	MI2C_INT_ON  
                       *	MI2C_INT_OFF
                     I2C slave Interrupt enable/disable
                       *	SI2C_INT_ON
                       *	SI2C_INT_OFF
                     I2C master Interrupt priority
                       *	MI2C_INT_PRI_7					
					   *	MI2C_INT_PRI_6					
					   *	MI2C_INT_PRI_5					
					   *	MI2C_INT_PRI_4					
					   *	MI2C_INT_PRI_3					
					   *	MI2C_INT_PRI_2					
					   *	MI2C_INT_PRI_1					
					   *	MI2C_INT_PRI_0
					   *	MI2C_SRC_DIS 					
					 I2C slave Interrupt priority					
					   *	SI2C_INT_PRI_7					
					   *	SI2C_INT_PRI_6					
					   *	SI2C_INT_PRI_5					
					   *	SI2C_INT_PRI_4					
					   *	SI2C_INT_PRI_3					
					   *	SI2C_INT_PRI_2					
					   *	SI2C_INT_PRI_1					
					   *	SI2C_INT_PRI_0
					   *	SI2C_SRC_DIS	
					   				 
Return Value       : None 

Remarks            : This function clears the Interrupt Flag bits, 
                     sets the interrupt priorities of master and slave
                     and enables/disables the interrupt.
*********************************************************************/
void ConfigIntI2C2(unsigned int config)
{
     IFS3bits.SI2C2IF = 0;                       /* clear the MI2C & SI2C Interrupts */
     IFS3bits.MI2C2IF = 0;

     IPC12bits.SI2C2IP = (config & 0x0007);        /* set the SI2C priority */
     IPC12bits.MI2C2IP = (config & 0x0070) >> 4;    /* set the MI2C priority */

     IEC3bits.SI2C2IE = (config & 0x0008)>> 3;   /* enable/disable the SI2C Interrupt */
     IEC3bits.MI2C2IE = (config & 0x0080) >> 7;    /* enable/disable the MI2C Interrupt */
}

/************************************************************************
Function Prototype : char DataRdyI2C2(void) 

Include            : i2c.h 

Description        : This function provides status back to user if I2CRCV
                     register contain data. 
                     
Arguments          : None 

Return Value       : This function returns ‘1’ if there is data in I2CRCV register;
                     else return ‘0’ which indicates no data in I2CRCV register. 

Remarks            : This function determines if there is any byte to read from
                     I2CRCV register.
*************************************************************************/
char DataRdyI2C2(void)
{
     return I2C2STATbits.RBF;
}

/**********************************************************************************
Function Prototype : unsigned int getsI2C2(unsigned char *rdptr, 
                                                unsigned int i2c_data_wait)
                                              
Include            : i2c.h
 
Description        : This function reads pre-determined data string length 
                     from the I2C bus.
 
Arguments          : rdptr - Character type pointer to RAM for storage of data 
                     read from I2C device.
                     i2c_data_wait - This is the time-out count for which the 
                     module has  to wait before return.
                     If the timeout count is ‘N’, the actual time out would
                     be about (20*N-1) instruction cycles.
 
Return Value       : Returns the number of bytes received from the I2C bus.
 
Remarks            : This routine reads a predefined data string from the I2C bus.
************************************************************************************/

unsigned int getsI2C2(unsigned char * rdptr, unsigned char length )
{
while ( length-- )            // perform getcI2C1() for 'length' number of bytes
    {
      unsigned int word;
	  word =  *rdptr++;       // save byte received
	  
      while ( I2C2CONLbits.RCEN ); // check that receive sequence is over    

      if ( IFS5bits.I2C2BCIF )       // test for bus collision
      {
        return ( -1 );             // return with Bus Collision error 
      }

	  
	if( ((I2C2CONL &0x0F)==0x08) || ((I2C2CONL &0x0F)==0x0B) )	//master mode only
	{	
      if ( length )               // test if 'length' bytes have been read
      {
        I2C2CONLbits.ACKDT = 0;    // set acknowledge bit state for ACK        
        I2C2CONLbits.ACKEN = 1;    // initiate bus acknowledge sequence
        while ( I2C2CONLbits.ACKEN ); // wait until ACK sequence is over 
      } 
	} 
	  
    }
    return ( 0 );                
}


/************************************************************************************************
Function Prototype : char MasterWriteI2C2(unsigned char data_out)

Include            : i2c.h
 
Description        : This function is used to write out a single data byte to the I2C device.
 
Arguments          : data_out - A single data byte to be written to the I2C bus device.
 
Return Value       : This function returns -1 if there was a write collision else it returns a 0.
 
Remarks            : This function writes  a single byte to the I2C bus.
                     This function performs the same function as MasterputcI2C.
**************************************************************************************************/

char MasterWriteI2C2(unsigned char data_out)
{
    I2C2TRN = data_out;

    if(I2C2STATbits.IWCOL)        /* If write collision occurs,return -1 */
        return -1;
    else
    {
        return 0;
    }
}

/***************************************************************************************************
Function Prototype :  unsigned int MastergetsI2C2(unsigned int length,
                          unsigned char *rdptr, unsigned int i2c_data_wait)
                           
Include            :  i2c.h 

Description        :  This function reads predetermined data string length from the I2C bus. 

Arguments          :  length - Number of bytes to read from I2C device.
                      rdptr - Character type pointer to RAM for storage of data read from I2C device
                      i2c_data_wait - This is the timeout count for which the module has
                      to wait before return. If the timeout count is ‘N’, the actual time out would
                      be about (20 * N – 1) instruction cycles. 
                      
Return Value       :  This function returns ‘0’ if all bytes have been sent or number of bytes
                      read from I2C bus if its not able to read the data with in the specified
                      i2c_data_wait time out value 
                     
Remarks            :  This routine reads a predefined data string from the I2C bus.
******************************************************************************************************/

unsigned int MastergetsI2C2(unsigned int length, unsigned char * rdptr, unsigned int i2c2_data_wait)
{
    int wait = 0;
    while(length)                    /* Receive the number of bytes specified by length */
    {
        I2C2CONLbits.RCEN = 1;
        while(!DataRdyI2C2())
        {
            if(wait < i2c2_data_wait)
                wait++ ;                 
            else
            return(length);          /* Time out, return number of byte/word to be read */			
        }
        wait = 0;
        *rdptr = I2C2RCV;             /* save byte received */
        rdptr++;
        length--;
        if(length == 0)              /* If last char, generate NACK sequence */
        {
            I2C2CONLbits.ACKDT = 1;
            I2C2CONLbits.ACKEN = 1;
        }
        else                         /* For other chars,generate ACK sequence */
        {
            I2C2CONLbits.ACKDT = 0;
            I2C2CONLbits.ACKEN = 1;
        }
            while(I2C2CONLbits.ACKEN == 1);    /* Wait till ACK/NACK sequence is over */
    }
    return 0;    /* return status that number of bytes specified by length was received */
}


/**************************************************************************************
Function Prototype : char MasterputsI2C2(unsigned char *wrptr) 

Include            : i2c.h 

Description        : This function is used to write out a data string to the I2C bus. 

Arguments          : wrptr - Character type pointer to data objects in RAM. The data
                     objects are written to the I2C device.
                      
Return Value       : This function returns -3 if a write collision occurred.
                     This function returns ‘0’ if the null character was reached 
                     in data string.
	                  
Remarks            : This routine writes a predefined data string from the I2C bus.
**************************************************************************************/

char MasterputsI2C2(unsigned char * wrptr)
{
    while(*wrptr)                           //transmit data until null char
    {
        while(I2C2STATbits.TBF);             //Wait till data is transmitted.
        IdleI2C2();
        wrptr++;
    }
    return 0;           
}


/*********************************************************************
Function Prototype : void StopI2C1(void)
 
Include            : i2c.h
 
Description        : Generates I2C Bus Stop condition.
 
Arguments          : None
 
Return Value       : None
 
Remarks            : This function generates a I2C Bus Stop condition.
*********************************************************************/

void StopI2C2(void)
{
     I2C2CONLbits.PEN = 1;   /* initiate Stop on SDA and SCL pins */
}


/******************************************************************************
Function Prototype : unsigned char MasterReadI2C2(void)
 
Include            : i2c.h
 
Description        : This function is used to read a single byte from I2C bus
 
Arguments          : None
 
Return Value       : The return value is the data byte read from the I2C bus.
 
Remarks            : This function reads in a single byte from the I2C bus.
                     This function performs the same function as MastergetcI2C.
********************************************************************************/

unsigned char MasterReadI2C2(void)
{
    I2C2CONLbits.RCEN = 1;
    while(I2C2CONLbits.RCEN);
    I2C2STATbits.I2COV = 0;
    return(I2C2RCV);
}

/***********************************************************************
Function Prototype : void MasterWaitForIntrI2C1(void)
 
Include            : i2c.h
 
Description        : This routine will wait for Master interrupt request 
                     and then clear interrupt Flag.
                      
Arguments          : None
 
Return Value       : None
 
Remarks            : It wait for Master interrupt request and then 
                     clear interrupt Flag.
************************************************************************/

void MasterWaitForIntrI2C2(void)
{
   while(0 == IFS3bits.MI2C2IF);
   IFS3bits.MI2C2IF = 0;
}

/*********************************************************************************
Function Prototype : void NotAckI2C2(void)
 
Include            : i2c.h
 
Description        : Generates I2C bus Not Acknowledge condition during Master Receive.
 
Arguments          : None
 
Return Value       : None
 
Remarks            : This function generates an I2C bus Not Acknowledge condition.
**********************************************************************************/

void NotAckI2C2(void)
{
    I2C2CONLbits.ACKDT = 1;
    I2C2CONLbits.ACKEN = 1;
}

/*************************************************************************
Function Prototype : void RestartI2C2(void)

Include            : i2c.h
 
Description        : Generates I2C Bus Restart condition.
 
Arguments          : None
 
Return Value       : None
 
Remarks            : This function generates an I2C Bus Restart condition.
**************************************************************************/

void RestartI2C2(void)
{ 
    I2C2CONLbits.RSEN = 1;   /* initiate restart on SDA and SCL pins */
}


// end of i2c-  2 library.


// library functions for i2c - 3
/*********************************************************************
Function Prototype  : void AckI2C3(void)

Include             : i2c.h

Description         : Generates I2C bus Acknowledge condition.

Arguments           : None

Return Value        : None

Remarks             : This function generates an I2C bus Acknowledge 
                      condition.
*********************************************************************/
void AckI2C3(void)
{
    I2C3CONLbits.ACKDT = 0;
    I2C3CONLbits.ACKEN = 1;
}

/******************************************************************************
Function Prototype : void OpenI2C3(unsigned int config1, unsigned int config2)

Include            : i2c.h
 
Description        : Configures the I2C module
 
Arguments          : config1 - This contains the parameter to configure the I2CCON register

					 I2C Enable bit					
					   *	I2C_ON					
					   *	I2C_OFF					
					 I2C Stop in Idle Mode bit					
					   *	I2C_IDLE_STOP					
					   *	I2C_IDLE_CON					
					 SCL Release Control bit					
					   *	I2C_CLK_REL					
					   *	I2C_CLK_HLD					
					 Intelligent Peripheral Management Interface Enable bit					
					   *	I2C_IPMI_EN					
					   *	I2C_IPMI_DIS					
					 10-bit Slave Address bit					
					   *	I2C_10BIT_ADD					
					   *	I2C_7BIT_ADD					
					 Disable Slew Rate Control bit					
					   *	I2C_SLW_DIS					
					   *	I2C_SLW_EN					
					 SMBus Input Level bits					
					   *	I2C_SM_EN					
					   *	I2C_SM_DIS					
					 General Call Enable bit					
					   *	I2C_GCALL_EN					
					   *	I2C_GCALL_DIS					
					 SCL Clock Stretch Enable bit					
					   *	I2C_STR_EN					
					   *	I2C_STR_DIS					
					 Acknowledge Data bit					
					   *	I2C_ACK					
					   *	I2C_NACK					
					 Acknowledge Sequence Enable bit					
					   *	I2C_ACK_EN					
					   *	I2C_ACK_DIS					
					 Receive Enable bit					
					   *	I2C_RCV_EN					
					   *	I2C_RCV_DIS					
					 Stop Condition Enable bit					
					   *	I2C_STOP_EN					
					   *	I2C_STOP_DIS					
					 Repeated Start Condition Enable bit					
					   *	I2C_RESTART_EN					
					   *	I2C_RESTART_DIS					
					 Start Condition Enable bit					
					   *	I2C_START_EN					
					   *	I2C_START_DIS			 
					
					 config2 - computed value for the baud rate generator
	 
Return Value      :  None
					 
Remarks           :  This function configures the I2C Control register and I2C 
                     Baud Rate Generator register
*******************************************************************************/
void OpenI2C3(unsigned int config1,unsigned int config2)
{
    I2C3CONL = config2;
    I2C3CONL = config1;
}

/**********************************************************************************************
Function Prototype : void IdleI2C2(void) 

Include            : i2c.h 

Description        : This function generates Wait condition until I2C bus is Idle. 

Arguments          : None 

Return Value       : None 

Remarks            : This function will be in a wait state until Start Condition Enable bit,
                     Stop Condition Enable bit, Receive Enable bit, Acknowledge Sequence
                     Enable bit of I2C Control register and Transmit Status bit I2C Status
                     register are clear. The IdleI2C function is required since the hardware
                     I2C peripheral does not allow for spooling of bus sequence. The I2C
                     peripheral must be in Idle state before an I2C operation can be initiated
                     or write collision will be generated.
***********************************************************************************************/
void IdleI2C3(void)
{
    /* Wait until I2C Bus is Inactive */
    while(I2C3CONLbits.SEN || I2C3CONLbits.RSEN || I2C3CONLbits.PEN || I2C3CONLbits.RCEN ||
          I2C3CONLbits.ACKEN || I2C3STATbits.TRSTAT);  
}


/*********************************************************************
Function Prototype : void StartI2C3(void)
 
Include            : i2c.h
 
Description        : Generates I2C Bus Start condition.
 
Arguments          : None
 
Return Value       : None
 
Remarks            : This function generates a I2C Bus Start condition.
*********************************************************************/
void StartI2C3(void)
{
     I2C3CONLbits.SEN = 1;   /* initiate Start on SDA and SCL pins */
}

/*********************************************************************
Function Prototype : void CloseI2C3(void)

Include            : i2c.h 

Description        : This function turns off the I2C module 

Arguments          : None 

Return Value       : None 

Remarks            : This function disables the I2C module and clears the
                     Master and Slave Interrupt Enable and Flag bits.
*********************************************************************/
void CloseI2C3(void)
{
    /* clear the I2CEN bit */
    I2C3CONLbits.I2CEN = 0;

    /* clear the SI2C & MI2C Interrupt enable bits */
    IEC4bits.SI2C3IE = 0;
    IEC4bits.MI2C3IE = 0;

    /* clear the SI2C & MI2C Interrupt flag bits */
    IFS4bits.SI2C3IF = 0;
    IFS4bits.MI2C3IF = 0;
}

/*********************************************************************
Function Prototype : void ConfigIntI2C3(unsigned int config) 

Include            : i2c.h

Description        : This function configures the I2C Interrupt. 

Arguments          : config - I2C interrupt priority and enable/disable 
                     information as defined below
					 
                     Master I2C Interrupt Enable/Disable
                       *	MI2C_INT_ON  
                       *	MI2C_INT_OFF
                     I2C slave Interrupt enable/disable
                       *	SI2C_INT_ON
                       *	SI2C_INT_OFF
                     I2C master Interrupt priority
                       *	MI2C_INT_PRI_7					
					   *	MI2C_INT_PRI_6					
					   *	MI2C_INT_PRI_5					
					   *	MI2C_INT_PRI_4					
					   *	MI2C_INT_PRI_3					
					   *	MI2C_INT_PRI_2					
					   *	MI2C_INT_PRI_1					
					   *	MI2C_INT_PRI_0
					   *	MI2C_SRC_DIS 					
					 I2C slave Interrupt priority					
					   *	SI2C_INT_PRI_7					
					   *	SI2C_INT_PRI_6					
					   *	SI2C_INT_PRI_5					
					   *	SI2C_INT_PRI_4					
					   *	SI2C_INT_PRI_3					
					   *	SI2C_INT_PRI_2					
					   *	SI2C_INT_PRI_1					
					   *	SI2C_INT_PRI_0
					   *	SI2C_SRC_DIS	
					   				 
Return Value       : None 

Remarks            : This function clears the Interrupt Flag bits, 
                     sets the interrupt priorities of master and slave
                     and enables/disables the interrupt.
*********************************************************************/
void ConfigIntI2C3(unsigned int config)
{
     IFS4bits.SI2C3IF = 0;                       /* clear the MI2C & SI2C Interrupts */
     IFS4bits.MI2C3IF = 0;

     IPC17bits.SI2C3IP = (config & 0x0007);        /* set the SI2C priority */
     IPC17bits.MI2C3IP = (config & 0x0070) >> 4;    /* set the MI2C priority */

     IEC4bits.SI2C3IE = (config & 0x0008)>> 3;   /* enable/disable the SI2C Interrupt */
     IEC4bits.MI2C3IE = (config & 0x0080) >> 7;    /* enable/disable the MI2C Interrupt */
}

/************************************************************************
Function Prototype : char DataRdyI2C3(void) 

Include            : i2c.h 

Description        : This function provides status back to user if I2CRCV
                     register contain data. 
                     
Arguments          : None 

Return Value       : This function returns ‘1’ if there is data in I2CRCV register;
                     else return ‘0’ which indicates no data in I2CRCV register. 

Remarks            : This function determines if there is any byte to read from
                     I2CRCV register.
*************************************************************************/
char DataRdyI2C3(void)
{
     return I2C3STATbits.RBF;
}

/**********************************************************************************
Function Prototype : unsigned int getsI2C3(unsigned char *rdptr, 
                                                unsigned int i2c_data_wait)
                                              
Include            : i2c.h
 
Description        : This function reads pre-determined data string length 
                     from the I2C bus.
 
Arguments          : rdptr - Character type pointer to RAM for storage of data 
                     read from I2C device.
                     i2c_data_wait - This is the time-out count for which the 
                     module has  to wait before return.
                     If the timeout count is ‘N’, the actual time out would
                     be about (20*N-1) instruction cycles.
 
Return Value       : Returns the number of bytes received from the I2C bus.
 
Remarks            : This routine reads a predefined data string from the I2C bus.
************************************************************************************/

unsigned int getsI2C3(unsigned char * rdptr, unsigned char length )
{
while ( length-- )            // perform getcI2C1() for 'length' number of bytes
    {
      unsigned int word;
	  word = *rdptr++ ;       // save byte received
	  
      while ( I2C3CONLbits.RCEN ); // check that receive sequence is over    

      if ( IFS6bits.I2C3BCIF )       // test for bus collision
      {
        return ( -1 );             // return with Bus Collision error 
      }

	  
	if( ((I2C3CONL &0x0F)==0x08) || ((I2C3CONL &0x0F)==0x0B) )	//master mode only
	{	
      if ( length )               // test if 'length' bytes have been read
      {
        I2C3CONLbits.ACKDT = 0;    // set acknowledge bit state for ACK        
        I2C3CONLbits.ACKEN = 1;    // initiate bus acknowledge sequence
        while ( I2C3CONLbits.ACKEN ); // wait until ACK sequence is over 
      } 
	} 
	  
    }
    return ( 0 );                
}


/************************************************************************************************
Function Prototype : char MasterWriteI2C3(unsigned char data_out)

Include            : i2c.h
 
Description        : This function is used to write out a single data byte to the I2C device.
 
Arguments          : data_out - A single data byte to be written to the I2C bus device.
 
Return Value       : This function returns -1 if there was a write collision else it returns a 0.
 
Remarks            : This function writes  a single byte to the I2C bus.
                     This function performs the same function as MasterputcI2C.
**************************************************************************************************/

char MasterWriteI2C3(unsigned char data_out)
{
    I2C3TRN = data_out;

    if(I2C3STATbits.IWCOL)        /* If write collision occurs,return -1 */
        return -1;
    else
    {
        return 0;
    }
}

/***************************************************************************************************
Function Prototype :  unsigned int MastergetsI2C2(unsigned int length,
                          unsigned char *rdptr, unsigned int i2c_data_wait)
                           
Include            :  i2c.h 

Description        :  This function reads predetermined data string length from the I2C bus. 

Arguments          :  length - Number of bytes to read from I2C device.
                      rdptr - Character type pointer to RAM for storage of data read from I2C device
                      i2c_data_wait - This is the timeout count for which the module has
                      to wait before return. If the timeout count is ‘N’, the actual time out would
                      be about (20 * N – 1) instruction cycles. 
                      
Return Value       :  This function returns ‘0’ if all bytes have been sent or number of bytes
                      read from I2C bus if its not able to read the data with in the specified
                      i2c_data_wait time out value 
                     
Remarks            :  This routine reads a predefined data string from the I2C bus.
******************************************************************************************************/

unsigned int MastergetsI2C3(unsigned int length, unsigned char * rdptr, unsigned int i2c3_data_wait)
{
    int wait = 0;
    while(length)                    /* Receive the number of bytes specified by length */
    {
        I2C3CONLbits.RCEN = 1;
        while(!DataRdyI2C3())
        {
            if(wait < i2c3_data_wait)
                wait++ ;                 
            else
            return(length);          /* Time out, return number of byte/word to be read */			
        }
        wait = 0;
        *rdptr = I2C3RCV;             /* save byte received */
        rdptr++;
        length--;
        if(length == 0)              /* If last char, generate NACK sequence */
        {
            I2C3CONLbits.ACKDT = 1;
            I2C3CONLbits.ACKEN = 1;
        }
        else                         /* For other chars,generate ACK sequence */
        {
            I2C3CONLbits.ACKDT = 0;
            I2C3CONLbits.ACKEN = 1;
        }
            while(I2C3CONLbits.ACKEN == 1);    /* Wait till ACK/NACK sequence is over */
    }
    return 0;    /* return status that number of bytes specified by length was received */
}


/**************************************************************************************
Function Prototype : char MasterputsI2C3(unsigned char *wrptr) 

Include            : i2c.h 

Description        : This function is used to write out a data string to the I2C bus. 

Arguments          : wrptr - Character type pointer to data objects in RAM. The data
                     objects are written to the I2C device.
                      
Return Value       : This function returns -3 if a write collision occurred.
                     This function returns ‘0’ if the null character was reached 
                     in data string.
	                  
Remarks            : This routine writes a predefined data string from the I2C bus.
**************************************************************************************/

char MasterputsI2C3(unsigned char * wrptr)
{
    while(*wrptr)                           //transmit data until null char
    {
        while(I2C3STATbits.TBF);             //Wait till data is transmitted.
        IdleI2C3();
        wrptr++;
    }
    return 0;           
}


/*********************************************************************
Function Prototype : void StopI2C1(void)
 
Include            : i2c.h
 
Description        : Generates I2C Bus Stop condition.
 
Arguments          : None
 
Return Value       : None
 
Remarks            : This function generates a I2C Bus Stop condition.
*********************************************************************/

void StopI2C3(void)
{
     I2C3CONLbits.PEN = 1;   /* initiate Stop on SDA and SCL pins */
}


/******************************************************************************
Function Prototype : unsigned char MasterReadI2C3(void)
 
Include            : i2c.h
 
Description        : This function is used to read a single byte from I2C bus
 
Arguments          : None
 
Return Value       : The return value is the data byte read from the I2C bus.
 
Remarks            : This function reads in a single byte from the I2C bus.
                     This function performs the same function as MastergetcI2C.
********************************************************************************/

unsigned char MasterReadI2C3(void)
{
    I2C3CONLbits.RCEN = 1;
    while(I2C3CONLbits.RCEN);
    I2C3STATbits.I2COV = 0;
    return(I2C3RCV);
}

/***********************************************************************
Function Prototype : void MasterWaitForIntrI2C3(void)
 
Include            : i2c.h
 
Description        : This routine will wait for Master interrupt request 
                     and then clear interrupt Flag.
                      
Arguments          : None
 
Return Value       : None
 
Remarks            : It wait for Master interrupt request and then 
                     clear interrupt Flag.
************************************************************************/

void MasterWaitForIntrI2C3(void)
{
   while(0 == IFS4bits.MI2C3IF);
   IFS4bits.MI2C3IF = 0;
}

/*********************************************************************************
Function Prototype : void NotAckI2C3(void)
 
Include            : i2c.h
 
Description        : Generates I2C bus Not Acknowledge condition during Master Receive.
 
Arguments          : None
 
Return Value       : None
 
Remarks            : This function generates an I2C bus Not Acknowledge condition.
**********************************************************************************/

void NotAckI2C3(void)
{
    I2C3CONLbits.ACKDT = 1;
    I2C3CONLbits.ACKEN = 1;
}

/*************************************************************************
Function Prototype : void RestartI2C3(void)

Include            : i2c.h
 
Description        : Generates I2C Bus Restart condition.
 
Arguments          : None
 
Return Value       : None
 
Remarks            : This function generates an I2C Bus Restart condition.
**************************************************************************/

void RestartI2C3(void)
{ 
    I2C3CONLbits.RSEN = 1;   /* initiate restart on SDA and SCL pins */
}


// end of i2c-  2 library.