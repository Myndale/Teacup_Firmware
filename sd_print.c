#include	"config.h"
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

void list_sd_card()
{
	serial_writestr_P(PSTR("Files: {"));
	PFF_rewind_dir();
	while (true)
	{
		FILINFO fnfo;
		int err = PFF_read_dir(&fnfo);
		if ((err != 0) || (fnfo.fname[0] == 0))
			break;
		if (!(fnfo.fattrib & AM_DIR))
		{
			serial_writestr(fnfo.fname);
			serial_writestr_P(PSTR(","));
		}
	}
	serial_writestr_P(PSTR("}"));	
}

void init_sd_card(uint8_t suppress_output)
{
	spi_init();
	PFF_begin(4, rx, tx);
	if (!suppress_output)
		serial_writestr_P(PSTR("sd card initialized"));
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

#endif // #ifdef SD_PRINTING
