#ifndef GLOBAL_H
	//header environment variable, is used to detect multiple inclusion
	//of the same header, and can be used in the c file to detect the
	//included library
	#define GLOBAL_H

	/****************************************************************************
	**	ENVROIMENT VARIABILE
	****************************************************************************/

	#define F_CPU 20000000

	//Enable to test reading on the ADC module
	//#define TEST_ADC

	/****************************************************************************
	**	GLOBAL INCLUDE
	**	TIPS: you can put here the library common to all source file
	****************************************************************************/

	//type definition using the bit width and signedness
	#include <stdint.h>
	//define the ISR routune, ISR vector, and the sei() cli() function
	#include <avr/interrupt.h>
	//name all the register and bit
	#include <avr/io.h>
	//hard delay
	#include <util/delay.h>
	//General purpose macros
	#include "at_utils.h"
	//AT Mega specific MACROS
	#include "at_mega_port.h"

	/****************************************************************************
	**	DEFINE
	****************************************************************************/

		///----------------------------------------------------------------------
		///	BUFFERS
		///----------------------------------------------------------------------

		///--------------------------------------------------------------------------
		///	COMMANDS
		///--------------------------------------------------------------------------


	/****************************************************************************
	**	MACRO
	****************************************************************************/

		///----------------------------------------------------------------------
		///
		///----------------------------------------------------------------------

	#define H_BRIDGE_OFF()	\
		SET_BIT( PORTB, PB2 )

	#define H_BRIDGE_ON()	\
		SET_BIT( PORTB, PB2 )

	/****************************************************************************
	**	TYPEDEF
	****************************************************************************/

	//Global flags raised by ISR functions
	typedef struct _Isr_flags Isr_flags;

	/****************************************************************************
	**	STRUCTURE
	****************************************************************************/

	//Global flags raised by ISR functions
	struct _Isr_flags
	{
		//First byte
		U8 led_update	: 1;	//100Hz System Tick
		U8 				: 7;	//unused bits
	};

	/****************************************************************************
	**	PROTOTYPE: INITIALISATION
	****************************************************************************/

	//port configuration and call the peripherals initialization
	extern void global_init( void );

	//
	extern void timer0_init( void );
	//Two PWM channels for H-Bridge A
	extern void timer1_init( void );
	//Two PWM channels for H-Bridge B
	extern void timer2_init( void );
	//UART communication
	extern void usart0_init( void );
	//Init the ADC module
	extern void adc_init( void );

	/****************************************************************************
	**	PROTOTYPE: FUNCTION
	****************************************************************************/

		///----------------------------------------------------------------------
		///	SERVO
		///----------------------------------------------------------------------

	/****************************************************************************
	**	PROTOTYPE: GLOBAL VARIABILE
	****************************************************************************/

		///----------------------------------------------------------------------
		///	STATUS FLAGS
		///----------------------------------------------------------------------

	//Volatile flags used by ISRs
	extern volatile	Isr_flags flags;

#else
	#warning "multiple inclusion of the header file global.h"
#endif
