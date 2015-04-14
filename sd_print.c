#include	"gcode_parse.h"
#include	"config.h"

#ifdef SD_PRINTING

void list_sd_card()
{
	serial_writestr_P(PSTR("list_sd_card!"));
}

void init_sd_card()
{
	serial_writestr_P(PSTR("init_sd_card!"));
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


#endif SD_PRINTING
