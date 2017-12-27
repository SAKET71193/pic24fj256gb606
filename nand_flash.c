#include <xc.h>
#include <p24FJ256GB606.h>
#include <stdio.h>

#define FOSC    (7370000ULL)
#define FCY     (FOSC/2)
#include <libpic30.h>

#define HIGH 0
#define LOW 1
#define INPUT 1
#define OUTPUT 0
#define TRUE 1
#define FALSE 0


#define NAND_MFR_TOSHIBA	0x98
#define NAND_MFR_SAMSUNG	0xec
#define NAND_MFR_FUJITSU	0x04
#define NAND_MFR_NATIONAL	0x8f
#define NAND_MFR_RENESAS	0x07
#define NAND_MFR_STMICRO	0x20
#define NAND_MFR_HYNIX		0xad
#define NAND_MFR_MICRON		0x2c
#define NAND_MFR_AMD		0x01

#define NAND_CMD_READID		0x90
#define NAND_CMD_RESET		0xff
#define NAND_CMD_STATUS		0x70
#define NAND_CMD_READ0		0
#define NAND_CMD_READ1		1 // For READ0 on Small Page targets.
#define NAND_CMD_READSTART	0x30
#define NAND_CMD_READOOB	0x50 // for READ0 on Small Page targets, to access OOB

#define PAGESIZE 2112
#define NUMPAGES 65536       //(1024*64)

typedef  uint8_t byte;

byte maf_id, dev_id, cellinfo, extid, reserved;
uint32_t writesize, erasesize;
uint16_t oobsize, buswidth;


/**
 * struct nand_manufacturers - NAND Flash Manufacturer ID Structure
 * @name:	Manufacturer name
 * @id:		manufacturer ID code of device.
*/
struct nand_manufacturers {
	int id;
	char *name;
};

/**
 * struct nand_flash_dev - NAND Flash Device ID Structure
 * @name:	Identify the device type
 * @id:		device ID code
 * @pagesize:	Pagesize in bytes. Either 256 or 512 or 0
 *		If the pagesize is 0, then the real pagesize
 *		and the eraseize are determined from the
 *		extended id bytes in the chip
 * @erasesize:	Size of an erase block in the flash device.
 * @chipsize:	Total chipsize in Mega Bytes
 * @options:	Bitfield to store chip relevant options
 */
struct  nand_flash_dev {
	char *name;
	int id;
	unsigned long pagesize;
	unsigned long chipsize;
	unsigned long erasesize;
	unsigned long options;
};


const struct nand_manufacturers nand_manuf_ids[] = {
	{NAND_MFR_TOSHIBA, "Toshiba"},
	{NAND_MFR_SAMSUNG, "Samsung"},
	{NAND_MFR_FUJITSU, "Fujitsu"},
	{NAND_MFR_NATIONAL, "National"},
	{NAND_MFR_RENESAS, "Renesas"},
	{NAND_MFR_STMICRO, "ST Micro"},
	{NAND_MFR_HYNIX, "Hynix"},
	{NAND_MFR_MICRON, "Micron"},
	{NAND_MFR_AMD, "AMD"},
	{0x0, "Unknown"}
};
/*
*	Chip ID list
*
*	Name. ID code, pagesize, chipsize in MegaByte, eraseblock size,
*	options
*
*	Pagesize; 0, 256, 512
*	0	get this information from the extended chip ID
+	256	256 Byte page size
*	512	512 Byte page size
*/
const struct nand_flash_dev nand_flash_ids[] ={"NAND 64MiB 3,3v 8-bit added by SRL", 0x20, 512, 64, 0x4000, 0}; // not sure what to put for erase size. 0x20000? if it is blocks in bytes including spare it would be 16896 0x4200. Lets assume it is without spare
        
#define WE LATDbits.LATD4
#define RE LATDbits.LATD5
#define CLE LATDbits.LATD6
#define ALE LATDbits.LATD7
#define RB LATFbits.LATF0
#define CE LATFbits.LATF1
#define PT LATGbits.LATG6
#define IO_Write LATE
#define IO_Read PORTE
#define IO_Mode TRISE
#define Read_RB PORTFbits.RF0


void printbin (int out) {
  int j;
  for (j=7; j>=0; j--) {
    if (out & (1 << j)) {
      printf("1"); //49= "1"
    }
    else {
      printf("0"); //48= "0"
    }
  }
}


byte nand_read_byte()
{
  // Assumes ALE/CLE low (off). CE high (on)
   IO_Mode = 0xff; // overhead but oh well
   byte ret;
    WE = HIGH; // write off
    RE = LOW; // (active on falling edge)
    ret = IO_Read;
    RE = HIGH;
  return ret;
}


void nand_write_byte(byte b)
{
  IO_Mode = 0x00; // overhead but oh well
  RE = HIGH; // read off
  WE =  LOW;
  IO_Write = b;
  WE =  HIGH;
}


/* this is close to the nand_command_lp() in u-boot nand_base.c */
void nand_command_lp(byte command, int16_t column, int32_t page_addr)
{
  CLE = HIGH;
  nand_write_byte(command);
  CLE = LOW;

  if (column != -1) { // I REALLy am  not sure that it is okay to use this like this
    ALE = HIGH;
    nand_write_byte( column     & 0xFF);
    nand_write_byte((column>>8) & 0xFF);
    if (page_addr != -1) {
      nand_write_byte(  (page_addr)     & 0xFF);
      nand_write_byte(  (page_addr>>8)  & 0xFF);
      if (NUMPAGES*PAGESIZE > 0x8000000 /*128<<20 = 128MiB*/)
        nand_write_byte((page_addr>>16) & 0xFF); //BUGBUG with function argument. signed,unsigned
    }
    ALE = LOW;
  }
}


void nand_reset ()
{
  CE =  LOW; // chip  on
  __delay_ms(10);
  IO_Mode = 0x00;
  // default state:
//  WP =  LOW; // X
  ALE = LOW;  // addr off
  RE =  HIGH; // read off
  CE =  LOW;  // chip on
  WE =  LOW; // write off

  CLE = HIGH;
  nand_write_byte(NAND_CMD_RESET);
  CLE = LOW; // CLE off

  __delay_ms(500); // tRST = Device Resetting Time (Read/Program/Erase) = max 500us
}


void nand_read_id ()
{
    const struct nand_flash_dev *type = nand_flash_ids;
    const struct nand_manufacturers *maf = nand_manuf_ids;
    
  //io_write_byte(0x00); // not required. was suggested by someone online that looked at some nand chips
  __delay_ms(10);
  CE =  LOW; // chip  on

//  nand_command_lp(NAND_CMD_READID, 0x00, -1);
  nand_command_lp(NAND_CMD_READID, -1, -1);
  ALE = HIGH;
  nand_write_byte(0x00);
  ALE = LOW;

  maf_id = nand_read_byte(); // maker code
  dev_id = nand_read_byte(); // device code

    // UBOOT nand_base.c nand_get_flash_type() suggest repeating:
  /* Try again to make sure, as some systems the bus-hold or other
   * interface concerns can cause random data which looks like a
   * possibly credible NAND flash to appear. If the two results do
   * not match, ignore the device completely.
   */
//  nand_command_lp(NAND_CMD_READID, 0x00, -1);
  nand_command_lp(NAND_CMD_READID, -1, -1);
  ALE = HIGH;
  nand_write_byte(0x00);
  ALE = LOW;
  byte maf_verify = nand_read_byte(); // maker code
  byte dev_verify = nand_read_byte(); // device code
  if (maf_verify != maf_id || dev_verify != dev_id) {
    printf("second ID read did not match "
      "%02x,%02x against %02x,%02x\n", maf_id, dev_id, maf_verify, dev_verify);
    CE =  HIGH; // chip  off
    return;
  }
  cellinfo = nand_read_byte(); // 3rd
  extid    = nand_read_byte(); // 4th
  reserved = nand_read_byte(); // 5th
  printf("Your device ID:   %02X %02X %02X %02X %02X \n", maf_id, dev_id, cellinfo, extid, reserved);
  printf("\ntested...\n");
  printf("K9F2G08X0M:       EC DA 80 15 50\n");
  printf("NAND02GW3B2D:     20 DA 10 95 44\n");
  printf("HY27UF082G2B:     AD DA 10 95 44\n");
  printf("NAND512W3A2S:     20 76\n");
  printf("\nvia docs...\n");
  printf("K9F1G08U0C:       EC F1 00 95 40\n");
  printf("K9F2G08U0M:       EC DA 80 15\n");
  printf("K9F2G16U0M:       EC CA 80 55\n");
  printf("K9F2G08U0C:       EC DA 10 15 44\n");


  for (; type->name != NULL; type++)
    if (maf_id == type->id)
      break;
  
  for (; maf->id != 0; maf++)
    if (maf_id == maf->id)
      break;

  printf("\nExtended details\n");
  printf("%s: %s\n\n", maf->name, type->name);

  printbin(extid); printf(" Extid (4thbyte %02X):\n", extid);

  if (!type->pagesize) {
    /* Calc pagesize */
    writesize = 1024 << (extid & 0x03);
    extid >>= 2;
    /* Calc oobsize */
    oobsize = (8 << (extid & 0x01)) * (writesize >> 9);
    extid >>= 2;
    /* Calc blocksize. Blocksize is multiples of 64KiB */
    //erasesize = (64 * 1024) << (extid & 0x03);
    erasesize = 65536 << (extid & 0x03); // on AVR 64*1024=1 instead of 65536
    extid >>= 2;
    /* Get buswidth information */
    buswidth = (extid & 0x01) ? 16 : 8;
    printf("  Pagesize %lu bytes + OBB %u\n", writesize, oobsize);
    printf("  Blocksize %lu bytes (%uKB)\n", erasesize, erasesize/1024);
    printf("  Buswidth %u bits\n", buswidth);
  }
  else {
    erasesize = type->erasesize;
    writesize = type->pagesize;
    //oobsize = type->writesize / 32;
    buswidth = 8; // only supporting 8 bit atm.  type->options & NAND_BUSWIDTH_16;
	printf("  Pagesize %lu bytes\n", writesize);
	printf("  Blocksize %lu bytes (%uKB)\n", erasesize, erasesize/1024);
	printf("  Buswidth %u bits\n", buswidth);
  }
  CE =  HIGH; // chip  off
}


void nand_status ()
{
  IO_Mode = 0x00;
  CE =  LOW; // chip  o
  __delay_ms(10);

  CLE = HIGH;
  nand_write_byte(NAND_CMD_STATUS);
  CLE = LOW;

  IO_Mode = 0xff;
  int input[5];
  input[0] = nand_read_byte();

  printf("0x%02x\n", input[0]);
  printbin(input[0]);

  CE =  HIGH; // chip  off
}


void nand_status_verbose ()
{
  printbin(NAND_CMD_STATUS);
  IO_Mode = 0x00;
  CE =  LOW; // chip  o
  __delay_ms(10);

  CLE = HIGH;
  nand_write_byte(NAND_CMD_STATUS);
  CLE = LOW;

  IO_Mode = 0xff;
  int input[5];
  input[0] = nand_read_byte();

  printf("0x%02x\n",input[0]);
  printbin(input[0]);

  CE =  HIGH; // chip  off
}


// DETAILS FOR K9F1G08U0C
//(2K+64)Byte 2048+64 2112  page size
//(128K+4K)Byte 1024*128+(1024*4) 135168  block size
// 1 page   is 2112   bytes (2048+64)
// 1 block  is 64     pages... hence
// 1 block  is 135168 bytes
// Total blocks = 1024
// Total pages  = 65536 (1024*64) <
// Total bytes  = 138412032 1Mbits
// addressing:
// "row" = page
// "col" = byte in row
// Address size for cols is 12bits (4096) enough for 2112
// Address size for rows is 16bits (65536) enough for all pages.
// read operations are page basis
void   K9F1G08U0C_write_addr (uint32_t page, uint16_t col)
{
  nand_write_byte(col & 0xFF);
  nand_write_byte(col>>8 & 0xF );
  nand_write_byte(page & 0xFF);
  nand_write_byte(page>>8 & 0xFF);
}

void nand_read_page(uint32_t page, uint16_t col, uint16_t size)
{
    uint16_t i=0;
    int thresh=150; // will wait 150*10, aka 1.5 milisecond
    
    CE =  LOW; // chip  on
  __delay_ms(1);

  nand_command_lp(NAND_CMD_READ0, col, page);
  nand_command_lp(NAND_CMD_READSTART, -1, -1);

  
  while(( Read_RB) && thresh--) __delay_ms(1); // loop until RB=LOW, ready
  __delay_ms(25); // tR = Data Transfer from Cell to Register tR = 25us max

  byte input[PAGESIZE];
  for (i =0; i<size; i++) {
    //if (i % 50 == 0) p("\n"); p(".");
    input[i] = nand_read_byte(); //delayMicroseconds(1);
#ifdef INVERTDATA
    if (col < 2048)
      input[i] = 255 - input[i];
#endif
  }

#ifdef INTERACTIVE
  //p("\nDone reading %u bytes from page %lu \n", size, page);
  for (uint16_t i=0; i<size; i++) {
    //if (i % 16 == 0)
      //Serial.println();
    //p("%02x ", input[i]);
    if ((input[i] >= 32 && input[i] <= 126) || input[i] == 9/*\t*/|| input[i] == 10/*\n*/ || input[i] == 13/*\n*/ ) // only check printable text
      printf("%c", input[i]);
    //else
    //  p("  ");
  }
  printf("\n");
#else
#ifdef PRINTBLOCKIDENT
  printf("BLOCK %025lu\n", page); // prints a marking of exactly 32 bytes (including \n)
#endif
  
  for (i=0; i<size; i++) {
    printf("%d",input[i]);
  }
#endif
  CE =  HIGH; // chip  off
}

void nand_read_page1(uint32_t page, uint16_t col)
{
  nand_read_page(page, col, PAGESIZE);
}
void nand_read_page0(uint32_t page)
{
  nand_read_page(page, 0, PAGESIZE);
}