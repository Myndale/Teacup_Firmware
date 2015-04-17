#include	"serial.h"
#include	"sermsg.h"
#include	"gcode_parse.h"
#include	"config.h"
#include	"pff.h"
#include	"petit_fatfs.h"

#ifdef SD_PRINTING

byte rx()
{
  SPDR = 0xFF;
  loop_until_bit_is_set(SPSR, SPIF);
  return SPDR;
}

void tx(byte d)
{
  SPDR = d;
  loop_until_bit_is_set(SPSR, SPIF);
}

void spi_init()
{
  SET_OUTPUT(SCK);
  SET_INPUT(MISO);
  SET_OUTPUT(MOSI);
  SET_OUTPUT(SS);
  
  SPCR = _BV(MSTR) | _BV(SPE);      // Master mode, SPI enable, clock speed MCU_XTAL/4
}

void test_setup()
{
  //Serial.begin(115200);
  serial_writestr_P(PSTR("\nStarting...\n"));
  
  PFF_PFF();
  spi_init();
  PFF_begin(4, rx, tx);
  
  serial_writestr_P(PSTR("All good!\n"));
  
  while (true)
  {
    FILINFO fnfo;
    int err = PFF_read_dir(&fnfo);
    if ((err!=0) || (fnfo.fname[0] == 0))
      break;
    serial_writestr_P(PSTR("Name: "));
    serial_writestr(fnfo.fname);
    if (fnfo.fattrib & AM_DIR)
      serial_writestr_P(PSTR(" is a directory"));
    else
      serial_writestr_P(PSTR(" is a file"));
    serial_writestr_P(PSTR("\n"));
  }
  
  serial_writestr_P(PSTR("\nFinished SD card dump!\n"));
}

void list_sd_card()
{
	serial_writestr_P(PSTR("list_sd_card!"));
}

void init_sd_card()
{
	serial_writestr_P(PSTR("init_sd_card!"));
	test_setup();
}

void select_sd_file()
{
	serial_writestr_P(PSTR("select_sd_file!"));
}

void start_sd_print()
{
	serial_writestr_P(PSTR("start_sd_print!"));
}

void pause_sd_print()
{
	serial_writestr_P(PSTR("pause_sd_print!"));
}

void report_sd_status()
{
	serial_writestr_P(PSTR("report_sd_status!"));
}

void select_and_start_sd_print()
{
	serial_writestr_P(PSTR("select_and_start_sd_print!"));
}

void strobe(int pin)
{
  /*digitalWrite(pin, HIGH);
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  digitalWrite(pin, HIGH);*/
}

#endif // #ifdef SD_PRINTING
