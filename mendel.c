/** \file
	\brief Main file - this is where it all starts, and ends
*/

/** \mainpage Teacup Reprap Firmware
	\section intro_sec Introduction
		Teacup Reprap Firmware (originally named FiveD on Arduino) is a firmware package for numerous reprap electronics sets.

		Please see README for a full introduction and long-winded waffle about this project
	\section install_sec	Installation
		\subsection step1 Step 1: Download
			\code git clone git://github.com/traumflug/Teacup_Firmware \endcode
		\subsection step2 Step 2: configure
			\code cp config.[yourboardhere].h config.h \endcode
			Edit config.h to suit your machone
			Edit Makefile to select the correct chip and programming settings
		\subsection step3 Step 3: Compile
			\code make \endcode
			\code make program \endcode
		\subsection step4 Step 4: Test!
			\code ./func.sh mendel_reset
			./func.sh mendel_talk
			M115
			ctrl+d \endcode
*/

#ifndef SIMULATOR
#include	<avr/io.h>
#include	<avr/interrupt.h>
#endif

#include	"config_wrapper.h"
#include	"fuses.h"

#include	"serial.h"
#include	"dda_queue.h"
#include	"dda.h"
#include	"gcode_parse.h"
#include	"timer.h"
#include	"temp.h"
#include	"sermsg.h"
#include	"watchdog.h"
#include	"debug.h"
#include	"sersendf.h"
#include	"heater.h"
#include	"analog.h"
#include	"pinio.h"
#include	"arduino.h"
#include	"clock.h"
#include	"intercom.h"
#include "simulator.h"

#ifdef SIMINFO
  #include "../simulavr/src/simulavr_info.h"
  SIMINFO_DEVICE("atmega644");
  SIMINFO_CPUFREQUENCY(F_CPU);
  SIMINFO_SERIAL_IN("D0", "-", BAUD);
  SIMINFO_SERIAL_OUT("D1", "-", BAUD);
#endif

#ifdef CANNED_CYCLE
  const char PROGMEM canned_gcode_P[] = CANNED_CYCLE;
#endif

// don't disable SPI if we need to print off the sd card
#ifdef SD_PRINTING
#	define SPI_MASK 0
#else
#	define SPI_MASK MASK(PRSPI)
#endif

/// initialise all I/O - set pins as input or output, turn off unused subsystems, etc
void io_init(void) {
	// disable modules we don't use
	#ifdef PRR
		PRR = MASK(PRTWI) | MASK(PRADC) | SPI_MASK;
	#elif defined PRR0
		PRR0 = MASK(PRTWI) | MASK(PRADC) | SPI_MASK;
		#if defined(PRUSART3)
			// don't use USART2 or USART3- leave USART1 for GEN3 and derivatives
			PRR1 |= MASK(PRUSART3) | MASK(PRUSART2);
		#endif
		#if defined(PRUSART2)
			// don't use USART2 or USART3- leave USART1 for GEN3 and derivatives
			PRR1 |= MASK(PRUSART2);
		#endif
	#endif
	ACSR = MASK(ACD);

	// setup I/O pins

	// X Stepper
	WRITE(X_STEP_PIN, 0);	SET_OUTPUT(X_STEP_PIN);
	WRITE(X_DIR_PIN,  0);	SET_OUTPUT(X_DIR_PIN);
	#ifdef X_MIN_PIN
		SET_INPUT(X_MIN_PIN);
		WRITE(X_MIN_PIN, 0); // pullup resistors off
	#endif
	#ifdef X_MAX_PIN
		SET_INPUT(X_MAX_PIN);
		WRITE(X_MAX_PIN, 0); // pullup resistors off
	#endif

	// Y Stepper
	WRITE(Y_STEP_PIN, 0);	SET_OUTPUT(Y_STEP_PIN);
	WRITE(Y_DIR_PIN,  0);	SET_OUTPUT(Y_DIR_PIN);
	#ifdef Y_MIN_PIN
		SET_INPUT(Y_MIN_PIN);
		WRITE(Y_MIN_PIN, 0); // pullup resistors off
	#endif
	#ifdef Y_MAX_PIN
		SET_INPUT(Y_MAX_PIN);
		WRITE(Y_MAX_PIN, 0); // pullup resistors off
	#endif

	// Z Stepper
	#if defined Z_STEP_PIN && defined Z_DIR_PIN
		WRITE(Z_STEP_PIN, 0);	SET_OUTPUT(Z_STEP_PIN);
		WRITE(Z_DIR_PIN,  0);	SET_OUTPUT(Z_DIR_PIN);
	#endif
	#ifdef Z_MIN_PIN
		SET_INPUT(Z_MIN_PIN);
		WRITE(Z_MIN_PIN, 0); // pullup resistors off
	#endif
	#ifdef Z_MAX_PIN
		SET_INPUT(Z_MAX_PIN);
		WRITE(Z_MAX_PIN, 0); // pullup resistors off
	#endif

	#if defined E_STEP_PIN && defined E_DIR_PIN
		WRITE(E_STEP_PIN, 0);	SET_OUTPUT(E_STEP_PIN);
		WRITE(E_DIR_PIN,  0);	SET_OUTPUT(E_DIR_PIN);
	#endif

	// Common Stepper Enable
	#ifdef STEPPER_ENABLE_PIN
		#ifdef STEPPER_INVERT_ENABLE
			WRITE(STEPPER_ENABLE_PIN, 0);
		#else
			WRITE(STEPPER_ENABLE_PIN, 1);
		#endif
		SET_OUTPUT(STEPPER_ENABLE_PIN);
	#endif

	// X Stepper Enable
	#ifdef X_ENABLE_PIN
		#ifdef X_INVERT_ENABLE
			WRITE(X_ENABLE_PIN, 0);
		#else
			WRITE(X_ENABLE_PIN, 1);
		#endif
		SET_OUTPUT(X_ENABLE_PIN);
	#endif

	// Y Stepper Enable
	#ifdef Y_ENABLE_PIN
		#ifdef Y_INVERT_ENABLE
			WRITE(Y_ENABLE_PIN, 0);
		#else
			WRITE(Y_ENABLE_PIN, 1);
		#endif
		SET_OUTPUT(Y_ENABLE_PIN);
	#endif

	// Z Stepper Enable
	#ifdef Z_ENABLE_PIN
		#ifdef Z_INVERT_ENABLE
			WRITE(Z_ENABLE_PIN, 0);
		#else
			WRITE(Z_ENABLE_PIN, 1);
		#endif
		SET_OUTPUT(Z_ENABLE_PIN);
	#endif

	// E Stepper Enable
	#ifdef E_ENABLE_PIN
		#ifdef E_INVERT_ENABLE
			WRITE(E_ENABLE_PIN, 0);
		#else
			WRITE(E_ENABLE_PIN, 1);
		#endif
		SET_OUTPUT(E_ENABLE_PIN);
	#endif

	#ifdef	STEPPER_ENABLE_PIN
		power_off();
	#endif

	#ifdef	TEMP_MAX6675
		// setup SPI
		WRITE(SCK, 0);				SET_OUTPUT(SCK);
		WRITE(MOSI, 1);				SET_OUTPUT(MOSI);
		WRITE(MISO, 1);				SET_INPUT(MISO);
	#endif

	#ifdef DEBUG_LED_PIN 
		WRITE(DEBUG_LED_PIN, 0);
		SET_OUTPUT(DEBUG_LED_PIN);
	#endif

	#ifdef SD_PRINTING
		init_sd_card(1);
	#endif
}

/// Startup code, run when we come out of reset
void init(void) {
	// set up watchdog
	wd_init();

	// set up serial
	serial_init();

	// set up G-code parsing
	gcode_init();

	// set up inputs and outputs
	io_init();

	// set up timers
	timer_init();

	// read PID settings from EEPROM
	heater_init();

	// set up dda
	dda_init();

	// start up analog read interrupt loop,
	// if any of the temp sensors in your config.h use analog interface
	analog_init();

	// set up temperature inputs
	temp_init();

	// enable interrupts
	sei();

	// reset watchdog
	wd_reset();

	// prepare the power supply
	power_init();

	// say hi to host
	serial_writestr_P(PSTR("start\nok\n"));
}

/// this is where it all starts, and ends
///
/// just run init(), then run an endless loop where we pass characters from the serial RX buffer to gcode_parse_char() and check the clocks
#ifdef SIMULATOR
int main (int argc, char** argv)
{  
  sim_start(argc, argv);
#else
int main (void)
{
#endif
	init();

	// main loop
	for (;;)
	{
		// if queue is full, no point in reading chars- host will just have to wait
    if (queue_full() == 0) {
      if (serial_rxchars() != 0) {
        uint8_t c = serial_popchar();
        gcode_parse_char(c);
      }

      #ifdef CANNED_CYCLE
        /**
          WARNING!

          This code works on a per-character basis.

          Any data received over serial WILL be randomly distributed through
          the canned gcode, and you'll have a big mess!

          The solution is to either store gcode parser state with each source,
          or only parse a line at a time.

          This will take extra ram, and may be out of scope for the Teacup
          project.

          If ever print-from-SD card is implemented, these changes may become
          necessary.
        */
        static uint32_t canned_gcode_pos = 0;

        gcode_parse_char(pgm_read_byte(&(canned_gcode_P[canned_gcode_pos])));

        canned_gcode_pos++;
        if (pgm_read_byte(&(canned_gcode_P[canned_gcode_pos])) == 0)
          canned_gcode_pos = 0;

      #endif /* CANNED_CYCLE */
		}

		clock();
	}
}
