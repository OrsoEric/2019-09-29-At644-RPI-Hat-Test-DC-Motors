/****************************************************************************
**	INCLUDE
****************************************************************************/

#include "global.h"

/****************************************************************************
** GLOBAL INITIALISATION
****************************************************************************/

void global_init( void )
{
	///----------------------------------------------------------------------
	///	PORT I/O SETTING:
	///----------------------------------------------------------------------
	///	'I' = Input (HI-Z)
	///	'R' = Input (Rpu)
	///	'L' = Output (low)
	///	'H' = Output (Hi)
	///	the bit are in the order: LSB, ... , MSB
	///	TIPS: unused pin should be configured as 'R' to avoid power losses
	///	by spurious transition on the disconnected pin
	///	TIPS: I/O is the primary function of any pin, if you engage a peripheral
	///	that use a pin, that configuration will override the pin's I/O configuration
	///	TIPS: if you want to use oc0,1a,1b,2 waveform pins, you must
	///	configure the related I/O pin as output
	///----------------------------------------------------------------------

	//PA0			: SHIELD-A0
	//PA1			: SHIELD-A1
	//PA2			: SHIELD-A2
	//PA3			: SHIELD-A3
	//PA4			: SHIELD-A4
	//PA5			: SHIELD-A5
	//PA6			: ENC-A Z Channel
	//PA7			: ENC-B Z Channel
	PORT_A_CONFIG('R','R','R','R','R','R','Z','Z');

	//PB0			: ENC-B A Channel
	//PB1			: ENC-B B Channel
	//PB2			: H-Bridge Power
	//PB3			: SHIELD OC0A
	//PB4			: SHIELD OC0B
	//PB5			:
	//PB6			: LED0#
	//PB7			: LED1#
	PORT_B_CONFIG('Z','Z','L','R','R','R','H','H');

	//PC0			: SRV0 Servomotor |  SHIELD-D2
	//PC1			: SRV1 Servomotor |  SHIELD-D4
	//PC2			: SRV2 Servomotor |  SHIELD-D7
	//PC3			: SRV3 Servomotor |  SHIELD-D8
	//PC4			: SHIELD-D12
	//PC5			: SHIELD-D13
	//PC6			: ENC-A A Channel
	//PC7			: ENC-B B Channel
	PORT_C_CONFIG('L','L','L','L','R','R','Z','Z');

	//PD0			: RPI_TXO AT_RXI
	//PD1			: RPI_RXI AT_TXO
	//PD2			: ARD_TXO AT_RXI
	//PD3			: ARD_RXI AT_TXO
	//PD4, OC1B		: H-Bridge A -
	//PD5, OC1A		: H-Bridge A +
	//PD6, OC2B		: H-Bridge B -
	//PD7, OC2A		: H-Bridge B +
	PORT_D_CONFIG('R','H','R','R','L','L','L','L');

	///----------------------------------------------------------------------
	///	DEVICES INIT
	///----------------------------------------------------------------------

	//Init the serial port 1. Rx INT active
	//usart0_init();
	//init Timer0. Handle user timing flags
	timer0_init();
	//Two PWM channels for H-Bridge A
	timer1_init();
	//Two PWM channels for H-Bridge B
	timer2_init();
	//Init the ADC module
	//adc_init();

	///----------------------------------------------------------------------
	///	ENABLE ALL INTERRUPT:
	///	TIPS: sei() should be called after all the pheriperals have been configured,
	///	so that ISR will not mess up other initialization.
	/// TIPS: the "all interrupt enable flag" is automatically shut down at the
	///	start of any ISR and engaged again at it's bottom to avoid slow nested ISR call,
	///	if you need nested ISR call then call sei() at the beginning of the ISR
	///	TIPS: the function to disable all the interrupt is "cli()"
	///----------------------------------------------------------------------

	sei();

	return;
}	//end function: global_init

/****************************************************************************
** PHERIPERALS INITIALISATION
****************************************************************************/

void usart0_init( void )
{
	///----------------------------------------------------------------------
	//	VARS
	///----------------------------------------------------------------------

	uint8_t ucsr0a_temp = 0x00;
	uint8_t ucsr0b_temp = 0x00;
	uint8_t ucsr0c_temp = 0x00;
	
	///----------------------------------------------------------------------
	//	REGISTER CONFIGURATION
	///----------------------------------------------------------------------

	//Double the USART0 speed
	//SET_BIT( ucsr0a_temp, U2X0 );

	//Multi-processor mode
	//SET_BIT( ucsr0a_temp, MPCM0 );

	//Rx interrupt enable
	SET_BIT( ucsr0b_temp, RXCIE0 );

	//Tx interrupt enable
	//SET_BIT( ucsr0b_temp, TXCIE0 );

	//Tx buffer empty interrupt enable
	//SET_BIT( ucsr0b_temp, UDRIE0 );

	//Enable receiver
	SET_BIT( ucsr0b_temp, RXEN0 );

	//Enable Transmitter
	SET_BIT( ucsr0b_temp, TXEN0 );

	//Word size
	//	2	1	0	|	Mode
	//-------------------------------
	//	0	0	0	|	5 bits
	//	0	0	1	|	6 bits
	//	0	1	0	|	7 bits
	//	0	1	1	|	8 bits
	//	1	0	0	|	Reserved
	//	1	0	1	|	Reserved
	//	1	1	0	|	Reserved
	//	1	1	1	|	9 bits
	//SET_BIT( ucsr0b_temp, UCSZ02 );
	SET_BIT( ucsr0c_temp, UCSZ01 );
	SET_BIT( ucsr0c_temp, UCSZ00 );

	//Operation mode
	//	1	0	|	Mode
	//-------------------------------
	//	0	0	|	Asynchronous USART
	//	0	1	|	Synchronous USART
	//	1	0	|	Reserved
	//	1	1	|	Master SPI
	//SET_BIT( ucsr0c_temp, UMSEL01 );
	//SET_BIT( ucsr0c_temp, UMSEL00 );

	//Parity Check
	//	1	0	|	Mode
	//-------------------------------
	//	0	0	|	Disabled
	//	0	1	|	Reserved
	//	1	0	|	Even Parity
	//	1	1	|	Odd Parity
	//SET_BIT( ucsr0c_temp, UPM01 );
	//SET_BIT( ucsr0c_temp, UPM00 );

	//Two stop bits enabled
	//SET_BIT( ucsr0c_temp, USBS0 );

	//XCK0 clock polarity (synchronous mode only)
	//SET_BIT( ucsr0c_temp, UCPOL0 );

	///----------------------------------------------------------------------
	//	REGISTER WRITE BACK
	///----------------------------------------------------------------------

	UCSR0A = ucsr0a_temp;
	UCSR0B = ucsr0b_temp;
	UCSR0C = ucsr0c_temp;
	
	UBRR0H = 0;
	//UBRR0L = 8;
	UBRR0L = 21;

	return;
}	//end function: usart0_initialisation


/****************************************************************************
**	TIMER0 INITIALISATION
*****************************************************************************
**	This timer generate a ~100uS time base
**	Normal mode
**	Fclk = 20e6
**	F = Fclk / N / (OCR+1)
**	OCR = Fclk * T / N - 1
**	N = 64, OCR =  30 -> F = 10080.6 [Hz]
**	N = 1024, OCR = 194 -> 100.16 [Hz]
****************************************************************************/

void timer0_init( void )
{
	///----------------------------------------------------------------------
	///	VARS
	///----------------------------------------------------------------------

	//temp control register variable
	uint8_t tccr0a_temp = 0x00;
	uint8_t tccr0b_temp = 0x00;
	uint8_t timsk0_temp = 0x00;

	///----------------------------------------------------------------------
	///	CONTROL REGISTER SETTINGS
	///----------------------------------------------------------------------

	//	1	0	|	OC0A
	//-------------------------------
	//	0	0	|	Disconnected
	//	0	1	|	Toggle on CM
	//	1	0	|	Clear on CM
	//	1	1	|	Set on CM

	//SET_BIT( tccr0a_temp, COM0A0 );
	//SET_BIT( tccr0a_temp, COM0A1 );

	//	1	0	|	OC0B
	//-------------------------------
	//	0	0	|	Disconnected
	//	0	1	|	Toggle on CM
	//	1	0	|	Clear on CM
	//	1	1	|	Set on CM

	//SET_BIT( tccr0a_temp, COM0B0 );
	//SET_BIT( tccr0a_temp, COM0B1 );

	//Compare Out Mode
	//	2	1	0	|	Mode
	//-------------------------------
	//	0	0	0	|	Normal (TOP = 0xff)
	//	0	0	1	|	PWM Phase Correct (TOP = 0xff)
	//	0	1	0	|	CTC (TOP = OCRA)
	//	0	1	1	|	Fast PWM (TOP = 0xff)
	//	1	0	0	|	Reserved
	//	1	0	1	|	PWM Phase Correct (TOP = OCRA)
	//	1	1	0	|	Reserved
	//	1	1	1	|	Fast PWM (TOP = OCRA)
		
	//SET_BIT( tccr0a_temp, WGM00 );
	SET_BIT( tccr0a_temp, WGM01 );
	//SET_BIT( tccr0b_temp, WGM02 );

	///Clock Source
	///	CS02	|	CS01	|	CS00	|| 
	///-------------------------------------------
	///	0		|	0		|	0		|| no clock
	///	0		|	0		|	1		|| clk / 1
	///	0		|	1		|	0		|| clk / 8
	///	0		|	1		|	1		|| clk / 64
	///	1		|	0		|	0		|| clk / 256
	///	1		|	0		|	1		|| clk / 1024
	///	1		|	1		|	0		|| T0 pin, falling edge
	///	1		|	1		|	1		|| T0 pin, rising edge

	SET_BIT( tccr0b_temp, CS02 );
	//SET_BIT( tccr0b_temp, CS01 );
	SET_BIT( tccr0b_temp, CS00 );

	//Output compare 0a interrupt
	//This interrupt is launched when TCNT0 is equal to OCR0A
	SET_BIT( timsk0_temp, OCIE0A );
	
	//Output compare 0a interrupt
	//This interrupt is launched when TCNT0 is equal to OCR0B
	//SET_BIT( timsk0_temp, OCIE0B );
	
	//overflow interrupt enable
	//is launched when TCNT0 goes in overflow
	//SET_BIT( timsk0_temp, TOIE0 );

	///----------------------------------------------------------------------
	///	CONTROL REGISTER WRITEBACK
	///----------------------------------------------------------------------

	TCCR0A = tccr0a_temp;
	TCCR0B = tccr0b_temp;
	TIMSK0 = timsk0_temp;

	//Generate Compare A interrupt
	OCR0A = 194;
	OCR0B = 0xff;

	return;
}	//end function: timer0_initialisation

/****************************************************************************
** TIMER1 INITIALISATION
*****************************************************************************
**	I generate two independent PWM channels
**	I work in phase correct PWM with 8bit TOP
**	I set inverting mode output since i pass trough an inverter
**	Fclk 		= 20MHz
**	Prescaler	= 2 (double ramp)
**	TOP 		= 0xff
**	Fpwm 		= 39.06KHz
****************************************************************************/

void timer1_init( void )
{
	///----------------------------------------------------------------------
	///	VARS
	///----------------------------------------------------------------------

	uint8_t tccr1a_temp = 0x00;
	uint8_t tccr1b_temp = 0x00;
	uint8_t tccr1c_temp = 0x00;
	uint8_t timsk1_temp = 0x00;

	///--------------------------------------------------------------------------
	//	REGISTER CONFIGURATION
	///--------------------------------------------------------------------------

	//OC1A pin behavior
	//	1	0	|	OC1A
	//-------------------------------
	//	0	0	|	Disconnected
	//	0	1	|	Reserved
	//	1	0	|	Clear on CM when UP, SET on CM when DOWN
	//	1	1	|	Set on CM when UP, CLEAR on CM when DOWN
	SET_BIT( tccr1a_temp, COM1A1 );
	//SET_BIT( tccr1a_temp, COM1A0 );


	//OC1B pin behavior
	//	1	0	|	OC1B
	//-------------------------------
	//	0	0	|	Disconnected
	//	0	1	|	Reserved
	//	1	0	|	Clear on CM when UP, SET on CM when DOWN
	//	1	1	|	Set on CM when UP, CLEAR on CM when DOWN
	SET_BIT( tccr1a_temp, COM1B1 );
	//SET_BIT( tccr1a_temp, COM1B0 );

	//Timer mode: Phase correct 8bit
	//	3	2	1	0	|	TOP		|	MODE
	//--------------------------------------------
	//	0	0	0	0	|	0xffff	|	NORMAL
	//	0	0	0	1	|	0x00ff	|	8bit PWM Phase correct (double ramp)
	//	0	0	1	0	|	0x001f	|	9bit PWM Phase correct (double ramp)
	//	0	0	1	1	|	0x003f	|	10bit PWM Phase correct (double ramp)
	//	0	1	0	0	|	OCR1A	|	CTC
	//	0	1	0	1	|	0x00ff	|	8bit Fast PWM (single ramp)
	//	0	1	1	0	|	0x01ff	|	9bit Fast PWM (single ramp)
	//	0	1	1	1	|	0x03ff	|	10bit Fast PWM (single ramp)
	//	1	0	0	0	|	ICR1	|	Frequency correct PWM Phase correct (double ramp with controlled top)
	//	1	0	0	1	|	OCR1A	|	Frequency correct PWM Phase correct (double ramp with controlled top)
	//	1	0	1	0	|
	//	1	0	1	1	|
	//	1	1	0	0	|
	//	1	1	0	1	|
	//	1	1	1	0	|	ICR1	|	Fast PWM
	//	1	1	1	1	|	OCR1A	|	Fast PWM
	//SET_BIT( tccr1b_temp, WGM13 );
	//SET_BIT( tccr1b_temp, WGM12 );
	//SET_BIT( tccr1a_temp, WGM11 );
	SET_BIT( tccr1a_temp, WGM10 );

	//Input capture noise canceler enabled
	//SET_BIT( tccr1b_temp, ICNC1 );

	//Input capture edge select
	//SET_BIT( tccr1b_temp, ICES1 );

	//Clock and Prescaler selection
	//	2	1	0	|	Mode
	//-------------------------------
	//	0	0	0	|	Disconnected
	//	0	0	1	|	Clk/1
	//	0	1	0	|	Clk/8
	//	0	1	1	|	Clk/64
	//	1	0	0	|	Clk/256
	//	1	0	1	|	Clk/1024
	//	1	1	0	|	T1 pin falling edge
	//	1	1	1	|	T1 pin rising edge
	//SET_BIT( tccr1b_temp, CS12 );
	//SET_BIT( tccr1b_temp, CS11 );
	SET_BIT( tccr1b_temp, CS10 );

	//Input Time Capture Interrupt Enable
	//SET_BIT( timsk1_temp, ICIE1 );

	//Compare Match 1A Interrupt Enable
	//SET_BIT( timsk1_temp, OCIE1A );

	//Compare Match 1B Interrupt Enable
	//SET_BIT( timsk1_temp, OCIE1B );

	//Timer Overflow Interrupt Enable 1
	//SET_BIT( timsk1_temp, TOIE1 );

	///--------------------------------------------------------------------------
	//	REGISTER WRITE-BACK
	///--------------------------------------------------------------------------

	//Configuration registers
	TCCR1A	= tccr1a_temp;
	TCCR1B	= tccr1b_temp;
	TCCR1C	= tccr1c_temp;
	TIMSK1	= timsk1_temp;
	//PWM registers
	OCR1A	= 0x0000;
	OCR1B 	= 0x0000;
	ICR1 	= 0x0000;

	return;
}	//end function: timer1_initialisation

/****************************************************************************
** TIMER2 INITIALISATION
*****************************************************************************
**	Operation Mode:
**	PWM phase correct 8bit
**		FORMULAE
**	Fclk [Hz]	|	system clock
**	Fout [Hz]	|	timer refresh rate
**	N [1]		|	Prescaler
**	OCR			|	PWM register
**
**	Fout = Fclk/N/(OCR+1)/2
**
**	Fclk	Fout [Hz]	N		OCR 
**	20e6	

****************************************************************************/

void timer2_init( void )
{
	///----------------------------------------------------------------------
	///	VARS
	///----------------------------------------------------------------------

	//temp control register variabile
	uint8_t tccr2a_temp = 0x00;
	uint8_t tccr2b_temp = 0x00;
	uint8_t timsk2_temp = 0x00;

	///**********************************************************************
	///	CONTROL REGISTER SETTINGS
	///**********************************************************************

	//	1	0	|	OC2A
	//-------------------------------
	//	0	0	|	Disconnected
	//	0	1	|	Toggle on CM
	//	1	0	|	Clear on CM
	//	1	1	|	Set on CM

	//SET_BIT( tccr2a_temp, COM2A0 );
	SET_BIT( tccr2a_temp, COM2A1 );

	//	1	0	|	O20B
	//-------------------------------
	//	0	0	|	Disconnected
	//	0	1	|	Toggle on CM
	//	1	0	|	Clear on CM
	//	1	1	|	Set on CM

	//SET_BIT( tccr2a_temp, COM2B0 );
	SET_BIT( tccr2a_temp, COM2B1 );

	//Compare Out Mode
	//	2	1	0	|	Mode
	//-------------------------------
	//	0	0	0	|	Normal (TOP = 0xff)
	//	0	0	1	|	PWM Phase Correct (TOP = 0xff)
	//	0	1	0	|	CTC (TOP = OCRA)
	//	0	1	1	|	Fast PWM (TOP = 0xff)
	//	1	0	0	|	Reserved
	//	1	0	1	|	PWM Phase Correct (TOP = OCRA)
	//	1	1	0	|	Reserved
	//	1	1	1	|	Fast PWM (TOP = OCRA)
		
	SET_BIT( tccr2a_temp, WGM20 );
	//SET_BIT( tccr2a_temp, WGM21 );
	//SET_BIT( tccr2b_temp, WGM22 );

	///Clock Source
	///	CS22	|	CS21	|	CS20	|| 
	///-------------------------------------------
	///	0		|	0		|	0		|| no clock
	///	0		|	0		|	1		|| clk / 1
	///	0		|	1		|	0		|| clk / 8
	///	0		|	1		|	1		|| clk / 32
	///	1		|	0		|	0		|| clk / 64
	///	1		|	0		|	1		|| clk / 128
	///	1		|	1		|	0		|| clk / 256
	///	1		|	1		|	1		|| clk / 1024

	SET_BIT( tccr2b_temp, CS20 );
	//SET_BIT( tccr2b_temp, CS21 );
	//SET_BIT( tccr2b_temp, CS22 );

	//Output compare 0a interrupt
	//This interrupt is launched when TCNT0 is equal to OCR0A
	//SET_BIT( timsk2_temp, OCIE2A );
	
	//Output compare 0a interrupt
	//This interrupt is launched when TCNT0 is equal to OCR0B
	//SET_BIT( timsk2_temp, OCIE2B );
	
	//overflow interrupt enable
	//is launched when TCNT0 goes in overflow
	//SET_BIT( timsk2_temp, TOIE2 );

	///**********************************************************************
	///	CONTROL REGISTER WRITEBACK
	///**********************************************************************

	TCCR2A = tccr2a_temp;
	TCCR2B = tccr2b_temp;
	TIMSK2 = timsk2_temp;

	OCR2A = 0x00;
	OCR2B = 0x00;

	return;
}	//end function: timer2_init