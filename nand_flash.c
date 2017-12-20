#include <msp430x24x.h>
#include<stdarg.h>
#include<stdio.h>
/************************* table_operation_values *******************************************
Read                  00h 30h
Read for Copy Back    00h 35h
Read ID               90h -
Reset                 FFh -
Page Program          80h 10h
Copy-Back Program     85h 10h
Block Erase           60h D0h
Random Data Input(1)  85h -
Random Data Output(1) 05h E0h
Read Status           70h -
***********************************************************************************************/
/* Nand flash commands */
#define CMD_READ_1                      0x00
#define CMD_READ_2                      0x30

#define CMD_READID                      0x90

#define CMD_WRITE_1                     0x80
#define CMD_WRITE_2                     0x10

#define CMD_ERASE_1                     0x60
#define CMD_ERASE_2                     0xD0

#define CMD_STATUS                      0x70
#define CMD_RESET                       0xFF

#define NAND_TIMEOUT 			120000
#define NAND_READY		1
#define NAND_BUSY		0

#define NAND_FLAG_SET		1
#define NAND_FLAG_CLR           0
#define NAND_FLAG_TOUT          2

#define ERR_NAND_HW_TOUT	1 //NAND Flash Reset Command failed
#define RTV_NOERR		0 //NAND Flash Initialisation successful



#define error 1
#define success 0

#define CLE             (1 << 0)                	//activating path when command send to register
#define ALE		(1 << 1)			//activating path when internal_address send to register
#define CE		(1 << 2)			//device in busy state when high
#define WE		(1 << 3)			//write enable for(data,address,command write )
#define RE		(1 << 4)			//serial data output control(read_Data)
#define WP		(1 << 6)			//program protection while power transation
#define RB		(1 << 5)			// ready/busy status (1 - no operation/free)
#define _OUT_PORT1		P1OUT				//input port (IO 0-7
#define _IN_PORT1              P1IN
#define _PORT		P2OUT			 //output port
#define _IN_PORT2  	P2IN
#define _IO_DIR_PORT1	P1DIR
#define _IO_DIR_PORT2	P2DIR
#define INPUT 	0x00
#define OUTPUT  0xff

#define NAND_STAT_RDY 		(1 << 6)		 //Busy/Ready 
#define IO0  		(1 << 0)		 // Pass/Fail

#define PAGESIZE 2112
#define NUMPAGES 65536 //(1024*64)

#define HIGH 1
#define LOW 0

#define NAND_DISABLE_CE() do { _PORT |= CE ;} while(0)
#define NAND_ENABLE_CE() do { _PORT &= ~CE ;} while(0)

void WRITE_NAND(unsigned int);

/************************************************************************************************************/
void Random_Delay(int value)
{int i,j;
for(i=0;i<0x5f;i++)
for(j=0;j< value;j++);
}

/************************************************************************************************************
# command_latch_cycle
 
 function to execution comamnd operation
:PARAMETER - 	Take command 
 
**************************************************************************************************************/
void WRITE_NAND_COMMAND(unsigned char command, int16_t column, int32_t page_addr)
{

_PORT |= CLE; //HIGH	
WRITE_NAND(command);
_PORT &= ~CLE; //low

 if(column != -1) { // I REALLy am  not sure that it is okay to use this like this
    _PORT |= ALE; //ale high
    WRITE_NAND(column & 0xFF);
    WRITE_NAND((column>>8) & 0xFF);
    if (page_addr != -1) {
      WRITE_NAND((page_addr)& 0xFF);
      WRITE_NAND((page_addr>>8)  & 0xFF);
      if (NUMPAGES*PAGESIZE > 0x8000000 /*128<<20 = 128MiB*/)
        WRITE_NAND((page_addr>>16) & 0xFF); //BUGBUG with function argument. signed,unsigned
    }
    _PORT &= ~ALE;
  }
}


void nand_reset ()
{
int i;
  _PORT &= ~CE;
  Random_Delay(10);
  
 _IO_DIR_PORT1 = OUTPUT;
  // default state:
 _PORT &= ~CE;
 _PORT &= ~ALE;
 _PORT |= RE;
 _PORT &= ~WE;
 _PORT &= ~WP;


  _PORT |= CLE;
  WRITE_NAND(CMD_RESET);
  _PORT &= CLE;
  for(i=0; i<0xFFFF; i++);
}

char maf_id, dev_id, cellinfo, extid, reserved;
uint32_t writesize, erasesize;
uint16_t oobsize, buswidth;

/************************************************************************************************************
# address_latch_cycle
 
 function to write address operation nand_flash
 paramete
**************************************************************************************************************/
void WRITE_NAND_ADDRESS(unsigned long _address,unsigned char nm_cycle)
{
	int shift_addr=0;
	 _PORT &= ~CLE; //LOW 
	 _PORT |= ALE; //HIGH	
	
	while(nm_cycle--)
	{
	 _PORT &= ~WE;  // LOW
	 _OUT_PORT1 = _address >> shift_addr;
	shift_addr += 8;
	 _PORT |= WE; //HIGH
	} 
	
	_PORT &= ~ALE; //low	
}

/************************************************************************************************************
# input_data_latch_cycle
 
-- function to write data in nand_flash
 parameter : data_ip - data to write 
**************************************************************************************************************/
void WRITE_NAND(unsigned int data)
{

_IO_DIR_PORT1 = OUTPUT;
_PORT &= ~RE;//low read off
_PORT &= ~ALE;//LOW
_PORT &= ~WE; //LOW
_OUT_PORT1 = data;
_PORT |= WE;//HIGH

}

/************************************************************************************************************
# read_data_latch_cycle
 
-- function to read data from nand_flash

 parameter : returns data read from nand flash
**************************************************************************************************************/
unsigned char READ_NAND(void)
{
unsigned char data_op;
_IO_DIR_PORT1 = INPUT;
Random_Delay(10);
_PORT |= WE;//HIGH
Random_Delay(10);
_PORT &= ~RE; //LO

data_op = _IN_PORT1;
Random_Delay(10);
_PORT |= RE;//HIGH
Random_Delay(10);
return data_op;
}

/*********************************************************************//**
 * Ready/Busy check, no timeout, basically, R/B bit should once to bail out from this routine
 * @param[in]   None
 * @return      NAND_READY
 *              NAND_BUSY
 **********************************************************************/
static int WaitReady (void)
{
        long int i = NAND_TIMEOUT;

        for (i = NAND_TIMEOUT; i; i--)
        {
                if (_IN_PORT2 & RB)           // NAND is ready if R/B signal goes high
                {
                        return NAND_READY;
                }
        }

        return NAND_BUSY;                               //Time Out
}


/*********************************************************************//**
 * @brief               Check the current status of the Nand Flash Memory
 * @param[in]   flag: The bit that is to be checked. (IO7 - write protection, IO6 - Busy/Ready, IO0 - Pass/Fail)
 
 * @return              NAND_FLAG_SET  - NAND flag is set
 *                      NAND_FLAG_CLR  - NAND flag is cleared
 *                      NAND_FLAG_TOUT - Timeout expired, NAND busy 
 **********************************************************************/
static int ChkStatus (unsigned char flag)
{
        /* Read status */
        //WRITE_NAND_COMMAND(CMD_STATUS);

        if( WaitReady() == NAND_BUSY)                                           // Wait until NAND ready
        {
                return NAND_FLAG_TOUT ;
        }

        return (_IN_PORT1 & flag) ? NAND_FLAG_SET : NAND_FLAG_CLR;
}


/************************************************************************************************************
# NAND_ERASE_BLOCK
 
-- ERASE THE ADDRESS BLOCK 
 
**************************************************************************************************************/
int NAND_Erase_Block(unsigned long address)
{

//WRITE_NAND_COMMAND(CMD_ERASE_1);

WRITE_NAND_ADDRESS(address,2);


//WRITE_NAND_COMMAND(CMD_ERASE_2);
Random_Delay(10);

//WRITE_NAND_COMMAND(CMD_ERASE_2);


	while((P2IN >> 5 & 0X01) != 0X01);  //WHILE RB IS NOT HIGH WAITING FOR COMPLATION
	
	if((_OUT_PORT1 & 0x01)== 0x01)
		return error;
	else
		return success;
}

/************************************************************************************************************
#Program page(write data to the page)

parameter : 	col_address_1 - address of colomn1 first cycle
		col_address_2 - address of column2 second cycle
		row_address_1 - address of row 1 third cycle 
		row_address_2 - address of row 2 fourth cycle
		data 	      - data to write nand_flash
	return-  success - 0
		 error   - 1
************************************************************************************************************/
int NAND_WRITE_PAGE(unsigned long address,unsigned char data)
{

//WRITE_NAND_COMMAND(CMD_WRITE_1);

WRITE_NAND_ADDRESS(address,4);

WRITE_NAND(data);

//WRITE_NAND_COMMAND(CMD_WRITE_2);

Random_Delay(10);
//WRITE_NAND_COMMAND(CMD_WRITE_1);

while((P1IN & 0X01) != 0X00);

	if((_OUT_PORT1 & 0x01)== 0x01)
		return error;
	else
		return success;
}

/*************************************************************************************************************
#read page

parameter : 	col_address_1 - address of colomn1 first cycle
		col_address_2 - address of column2 second cycle
		row_address_1 - address of row 1 third cycle 
		row_address_2 - address of row 2 fourth cycle
	return-
		temp_data_read  - data to write nand_flash
*************************************************************************************************************/
unsigned char NAND_READ_PAGE(unsigned long address)
{
unsigned char temp_data_read;
//WRITE_NAND_COMMAND(CMD_READ_1);
WRITE_NAND_ADDRESS(address,4);


//WRITE_NAND_COMMAND(CMD_READ_2);

temp_data_read = READ_NAND();


return temp_data_read;
}
/*------------------------------------------------------------------------------*/
/* \fn    _NandReadID                                                      */
/* \brief Read Nand ID                                                          */
/*------------------------------------------------------------------------------*/
unsigned char _NandReadID(void)
{
        unsigned long uChipID, i=0;
        unsigned char bManufacturerID, bDeviceID , cellinfo, extid, reserved;;
        unsigned char bManufacturerID_verify, bDeviceID_verify;

	for(i=0; i<0xff; i++);
        /* Enable chipset */
        NAND_ENABLE_CE();

   	 WRITE_NAND_COMMAND(CMD_READID,-1,-1);
     	 _PORT |= ALE;
	WRITE_NAND(0X00);
	_PORT &= ~ALE;

	    /* Read answer */
  	 bManufacturerID = READ_NAND();
   	 bDeviceID = READ_NAND();

	
	WRITE_NAND_COMMAND(CMD_READID, -1, -1);
  	_PORT |= ALE;
  	Random_Delay(10);
  	WRITE_NAND(0x00);
  	Random_Delay(10);
  	_PORT &= ~ALE;
  	Random_Delay(10);
  	 
	bManufacturerID_verify = READ_NAND();
   	 bDeviceID_verify = READ_NAND();
  
	if (bManufacturerID_verify != bManufacturerID || bDeviceID_verify != bDeviceID) {
    printf("second ID read did not match %02x,%02x against %02x,%02x\n", bManufacturerID, bDeviceID, bManufacturerID_verify, bDeviceID_verify);
   	_PORT |= CE; 
    return;
  }
	
	cellinfo = READ_NAND(); // 3rd
 	 extid    = READ_NAND(); // 4th
  	reserved = READ_NAND(); // 5th
	printf("Your device ID:  %02X %02X %02X %02X %02X \n", bManufacturerID, bDeviceID, cellinfo, extid, reserved);

	NAND_DISABLE_CE();
        return RTV_NOERR;                                                               // NAND is ready
}




/*************************************************************************************************************/
/*
CLE : LOW //WHEN HIGH IT LATCH COMMAND ON RISING AGE OF WE 
ALE : LOW //WHEN HIGH IT LATCH ADDRESS ON RISING AGE OF WE 
CE  : DEVICE SELECTION CONTROL

RE  : THE SERIAL OUTPUT CONTROL 
WE  : INPUT CONTROL WRITE ON IO PORT RISING AGE OF WE PULSE
WP  : PROTECTED DURING POWER TRANSATION
RB  : READY_BUSY STATUS

*/

void init_pins(void)
{
P1DIR = 0XFF;
P1OUT=0X00;
P2DIR = 0X1F;
_PORT &= ~CE;
_PORT &= ~CLE;
_PORT &= ~ALE;
_PORT |= RE;
_PORT &= ~WE;
_PORT |= RB;
Random_Delay(10);

}


int nand_chk()
{

int i;
unsigned char temp;
// Stop watchdog timer to prevent time out reset
WDTCTL = WDTPW + WDTHOLD;
init_uart();
init_pins();
printf("hey \n");

temp =  _NandReadID();
printf("%d",temp);

}


