/*
 * 
 * Arduino Wrapper Function Library for Petit FatFs
 * 
 * Petit FatFs - http://elm-chan.org/fsw/ff/00index_p.html
 * by ChanN
 * 
 * Wrapper Functions by Frank Zhao
 * 
 * mmc.c origially written by ChanN, modified by Frank Zhao
 * 
 */

#ifdef SD_PRINTING

#ifndef PFF_h
#define PFF_h

#include "integer.h"
#include "pff.h"

#include <Arduino.h>

#define max_path_len 32

void PFF_PFF();
int PFF_begin(unsigned char (*)(void), void (*)(unsigned char));
int PFF_open_file(char *);
int PFF_read_file(char *, int, int *);
void PFF_setup_stream(void (*)(void), void (*)(void), char (*)(char), void (*)(void), void (*)(void));
int PFF_stream_file(int, int *);
int PFF_lseek_file(long);
int PFF_open_dir(char *);
int PFF_rewind_dir(void);
int PFF_up_dir(void);
int PFF_read_dir(FILINFO *);
int PFF_open(FILINFO *);
char * PFF_cur_dir();

#endif

#endif // #ifdef SD_PRINTING
