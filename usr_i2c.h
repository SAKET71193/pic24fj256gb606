#ifndef usr_i2c
#define usr_i2c

#define I2C1RCV_VALUE               0x0000
#define I2C1TRN_VALUE               0x00FF
#define I2C1BRG_VALUE               0x0000
#define I2C1CON_VALUE               0x0000
#define I2C1STAT_VALUE              0x0000
#define I2C1ADD_VALUE               0x0000

#define I2C2RCV_VALUE               0x0000
#define I2C2TRN_VALUE               0x00FF
#define I2C2BRG_VALUE               0x0000
#define I2C2CON_VALUE               0x0000
#define I2C2STAT_VALUE              0x0000
#define I2C2ADD_VALUE               0x0000

#define I2C3RCV_VALUE               0x0000
#define I2C3TRN_VALUE               0x00FF
#define I2C3BRG_VALUE               0x0000
#define I2C3STAT_VALUE              0x0000
#define I2C3ADD_VALUE               0x0000
#define I2C3CON_VALUE               0x0000

#define I2C_ON           			0x8000 /*I2C module enabled */
#define I2C_OFF          			0x0000 /*I2C module disabled */
#define I2C_ON_OFF_MASK  			(~I2C_ON)

#define I2C_IDLE_STOP    			0x2000 /*stop I2C module in Idle mode */
#define I2C_IDLE_CON     			0x0000 /*continue I2C module in Idle mode */
#define I2C_IDLE_MASK    			(~I2C_IDLE_STOP)

#define I2C_CLK_REL      			0x1000 /*release clock */
#define I2C_CLK_HLD      			0x0000 /*hold clock  */
#define I2C_CLK_MASK     			(~I2C_CLK_REL)

#define I2C_IPMI_EN      			0x0800 /*IPMI mode enabled */
#define I2C_IPMI_DIS     			0x0000 /*IPMI mode not enabled */
#define I2C_IPMI_EN_DIS_MASK    	(~I2C_IPMI_EN)

#define I2C_10BIT_ADD    			0x0400 /*I2CADD is 10-bit address */
#define I2C_7BIT_ADD     			0x0000 /*I2CADD is 7-bit address */
#define I2C_10BIT_7BIT_MASK    		(~I2C_10BIT_ADD)

#define I2C_SLW_DIS       			0x0200 /*Disable Slew Rate Control for 100KHz */
#define I2C_SLW_EN        			0x0000 /*Enable Slew Rate Control for 400KHz */
#define I2C_SLW_EN_DIS_MASK     	(~I2C_SLW_DIS)

#define I2C_SM_EN        			0x0100 /*Enable SM bus specification */
#define I2C_SM_DIS       			0x0000 /*Disable SM bus specification */
#define I2C_SM_EN_DIS_MASK      	(~I2C_SM_EN)

#define I2C_GCALL_EN     			0x0080 /*Enable Interrupt when General call address is received. */
#define I2C_GCALL_DIS    			0x0000 /*Disable General call address. */
#define I2C_GCALL_EN_DIS_MASK   	(~I2C_GCALL_EN)

#define I2C_STR_EN       			0x0040 /*Enable clock stretching */
#define I2C_STR_DIS      			0x0000 /*disable clock stretching */
#define I2C_STR_EN_DIS_MASK     	(~I2C_STR_EN)

#define I2C_NACK         			0x0020 /*Transmit 0 to send ACK as acknowledge */
#define I2C_ACK         			0x0000 /*Transmit 1 to send NACK as acknowledge*/
#define I2C_ACK_MASK     			(~I2C_NACK)

#define I2C_ACK_EN       			0x0010 /*Initiate Acknowledge sequence */
#define I2C_ACK_DIS      			0x0000 /*Acknowledge condition Idle */
#define I2C_TX_ACK_MASK  			(~I2C_ACK_EN)

#define I2C_RCV_EN       			0x0008 /*Enable receive mode */
#define I2C_RCV_DIS      			0x0000 /*Receive sequence not in progress */
#define I2C_RCV_EN_DIS_MASK         (~I2C_RCV_EN)

#define I2C_STOP_EN      			0x0004 /*Initiate Stop sequence */
#define I2C_STOP_DIS    			0x0000 /*Stop condition Idle */
#define I2C_STOP_EN_DIS_MASK    	(~I2C_STOP_EN)

//end i2cxcon reg



#define I2C_RESTART_EN   			0x0002 /*Initiate Restart sequence */
#define I2C_RESTART_DIS  			0x0000 /*Start condition Idle */
#define I2C_RESTART_MASK 			(~I2C_RESTART_EN)

#define I2C_START_EN     			0x0001 /*Initiate Start sequence */
#define I2C_START_DIS    			0x0000 /*Start condition Idle */
#define I2C_START_MASK   			(~I2C_START_EN)

#define SI2C_INT_PRI_0      		0x0000 /*Slave I2C Interrupt Priority 0*/
#define SI2C_INT_PRI_1      		0x0001 /*Slave I2C Interrupt Priority 1*/
#define SI2C_INT_PRI_2     			0x0002 /*Slave I2C Interrupt Priority 2*/
#define SI2C_INT_PRI_3      		0x0003 /*Slave I2C Interrupt Priority 3*/
#define SI2C_INT_PRI_4      		0x0004 /*Slave I2C Interrupt Priority 4*/
#define SI2C_INT_PRI_5      		0x0005 /*Slave I2C Interrupt Priority 5*/
#define SI2C_INT_PRI_6      		0x0006 /*Slave I2C Interrupt Priority 6*/
#define SI2C_INT_PRI_7      		0x0007 /*Slave I2C Interrupt Priority 7*/
#define SI2C_SRC_DIS        		SI2C_INT_PRI_0 /*Slave I2C Source is disabled*/
#define SI2C_INT_PRI_MASK  			(~SI2C_INT_PRI_7)

#define SI2C_INT_ON     			0x0008 /*Slave I2C Interrupt enable*/
#define SI2C_INT_OFF    			0x0000 /*Slave I2C Interrupt disable*/
#define SI2C_INT_MASK   			(~SI2C_INT_ON)

#define MI2C_INT_PRI_0  			0x0000 /*Master I2C Interrupt Priority 0*/
#define MI2C_INT_PRI_1  			0x0010 /*Master I2C Interrupt Priority 1*/
#define MI2C_INT_PRI_2  			0x0020 /*Master I2C Interrupt Priority 2*/
#define MI2C_INT_PRI_3  			0x0030 /*Master I2C Interrupt Priority 3*/
#define MI2C_INT_PRI_4  			0x0040 /*Master I2C Interrupt Priority 4*/
#define MI2C_INT_PRI_5  			0x0050 /*Master I2C Interrupt Priority 5*/
#define MI2C_INT_PRI_6  			0x0060 /*Master I2C Interrupt Priority 6*/
#define MI2C_INT_PRI_7  			0x0070 /*Master I2C Interrupt Priority 7*/
#define MI2C_SRC_DIS    			MI2C_INT_PRI_0 /*Master I2C Source is disabled*/
#define MI2C_INT_PRI_MASK  			(~MI2C_INT_PRI_7)

// Master I2C Interrupt Enable/Disable
#define MI2C_INT_ON     			0x0080 /*Master I2C Interrupt enabled*/
#define MI2C_INT_OFF    			0x0000 /*Master I2C Interrupt disabled*/
#define MI2C_INT_MASK   			(~MI2C_INT_ON)

// I2CxCONH: I2Cx CONTROL REGISTER HIGH


//STOP CONDITION INTERRUPT BIT -- IN slave mode only
#define I2C_INT_PCIE_EN   		0x0040 /*Enables interrupt on detection of Stop condition*/
#define I2C_INT_PCIE_DIS  		0x0000 /*Stop detection interrupts are disabled*/
#define I2C_INT_PCIE_MASK 			(~I2C_INT_PCIE_EN)

//START CONDITION INTERRUPT BIT -- in slave mode only
#define I2C_INT_SCIE_EN   		0x0020 /*Enables interrupt on detection of Start or Restart conditions*/
#define I2C_INT_SCIE_DIS  		0x0000 /*Start detection interrupts are disabled*/
#define I2C_INT_SCIE_MASK 			(~I2C_INT_SCIE_EN)


//BOEN: Buffer Overwrite Enable bit -- in slave mode only
#define I2C_BOEN_EN   		0x0010 /*2CxRCV is updated and an ACK is generated for a received address/data byte, ignoring the state
                                   of the I2COV bit only if RBF bit = 0*/
#define I2C_BOEN_DIS  		0x0000 /*I2CxRCV is only updated when I2COV is clear*/
#define I2C_BOEN_MASK 			(~I2C_BOEN_EN)

//SDAHT: SDAx Hold Time Selection bit 
#define I2C_SDAHT_SET   	0x0008 /* Minimum of 300 ns hold time on SDAx after the falling edge of SCLx */
#define I2C_SDAHT_CLR  		0x0000 /*Minimum of 100 ns hold time on SDAx after the falling edge of SCLx*/

//SBCDE Slave Mode Bus Collision Detect Enable bit (I 2 C Slave mode only)
#define I2C_SBCDE_SET       0x0004 /*Enables slave bus collision interrupts*/
#define I2C_SBCDE_CLR       0x0000 /*Slave bus collision interrupts are disabled*/

//AHEN: Address Hold Enable bit (I 2 C Slave mode only)
#define I2C_AHEN_EN       0x0002 /*ollowing the 8th falling edge of SCLx for a matching received address byte; SCLREL bit
                                    (I2CxCONL<12>) will be cleared and SCLx will be held low*/
#define I2C_AHEN_DIS       0x0000 /*Slave bus collision interrupts are disabled*/

//DHEN: Data Hold Enable bit (I 2 C Slave mode only)
#define I2C_DHEN_EN        0x0001 /*Following the 8th falling edge of SCLx for a received data byte; slave hardware clears the SCLREL
                                    bit (I2CxCONL<12>) and SCLx is held low*/
#define I2C_DHEN_DIS       0x0000 /*Data holding is disabled*/

//end I2CxCONH reg....


//saket check _switch statement
/*
// I2CxSTAT: I2Cx STATUS REGISTER

//ACKSTAT: Acknowledge Status bit (updated in all Master and Slave modes)
#define I2C_ACK_BIT(I2C_NUM)     int flag; \
                                 switch(I2C_NUM){case 1: flag = I2C1STATbits.ACKSTAT; break;case 2: flag = I2C2STATbits.ACKSTAT; break;case 3: flag = I2C3STATbits.ACKSTAT; break;}

//TRSTAT: Transmit Status bit (when operating as I 2 C master; applicable to master transmit operation)
#define I2C_TRANS_BIT(I2C_NUM)   int flag; \
                                 switch(I2C_NUM){case 1: flag = I2C1STATbits.TRSTAT; break;case 2: flag = I2C2STATbits.TRSTAT; break;case 3: flag = I2C3STATbits.TRSTAT; break;}

//ACKTIM: Acknowledge Time Status bit (valid in I 2 C Slave mode only)
#define I2C_TIME_ACK_BIT(I2C_NUM)   int flag; \
                                 switch(I2C_NUM){case 1: flag = I2C1STATbits.ACKTIM; break;case 2: flag = I2C2STATbits.ACKTIM; break;case 3: flag = I2C3STATbits.ACKTIM; break;}
  */

/*******************************************************************
Macro       : I2C1_ACK_Status

Include     : i2c.h 

Description : Macro to return I2C Acknowledge Status

Arguments   : None

Remarks     : None
*******************************************************************/
#define I2C1_ACK_Status		I2C1STATbits.ACKSTAT

/*******************************************************************
Macro       : I2C1_TRANS_Status

Include     : i2c.h 

Description : Macro to return I2C Transmit  bit  Status

Arguments   : None

Remarks     : None
*******************************************************************/
#define I2C1_TRANS_Status		I2C1STATbits.TRSTAT


/*******************************************************************
Macro       : I2C1_TIME_ACK_Status

Include     : i2c.h 

Description : Macro to return I2C Time Acknowledge bit  Status

Arguments   : None

Remarks     : None
*******************************************************************/
#define I2C1_TIME_ACK_Status		I2C1STATbits.ACKTIM

/*******************************************************************
Macro       : I2C1_TIME_ACK_Status

Include     : i2c.h 

Description : Macro to return I2C Time BUS CollisIon bit  Status
 *              BCL: Bus Collision Detect bit (Master/Slave mode; cleared when I 2 C module is disabled, I2CEN = 0)
                1 = A bus collision has been detected during a master or slave transmit operation
                0 = No bus collision has been detected

Arguments   : None

Remarks     : None
*******************************************************************/
#define I2C1_BCL_Status		I2C1STATbits.BCL

/*******************************************************************
Macro       : I2C1_GC_Status

Include     : i2c.h 

Description : Macro to return I2C GC  bit  Status
 *             GCSTAT: General Call Status bit (cleared after Stop detection)
                    1 = General call address was received
                    0 = General call address was not received
Arguments   : None

Remarks     : None
*******************************************************************/
#define I2C1_GC_Status		I2C1STATbits.GCSTAT

/*******************************************************************
Macro       : I2C1_ADD10_Status

Include     : i2c.h 

Description : Macro to return I2C add10  bit  Status
 *             ADD10: 10-Bit Address Status bit (cleared after Stop detection)
                    1 = 10-bit address was matched
                    0 = 10-bit address was not matched
Arguments   : None

Remarks     : None
*******************************************************************/
#define I2C1_ADD10_Status		I2C1STATbits.ADD10

/*******************************************************************
Macro       : I2C1_IWCOL_Status

Include     : i2c.h 

Description : Macro to return I2C Write collosion  bit  Status
 *             IWCOL: I2Cx Write Collision Detect bit
                1 = An attempt to write to the I2CxTRN register failed because the I 2 C module is busy; must be cleared
                in software
                0 = No collision
Arguments   : None

Remarks     : None
*******************************************************************/
#define I2C1_IWCOL_Status		I2C1STATbits.IWCOL





#endif