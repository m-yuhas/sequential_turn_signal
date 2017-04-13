/*
 * Sequential Turn Signal - main.c
 * This file contains the microcontroller code to implement sequential turn signals on the Corvette
 * (c) 2017 Michael Yuhas
 */

/*
 * Includes:
 * msp430.h - constants and pin mappings for msp430
 */
#include <msp430.h>
#include <stdint.h>

/*
 * Definitions:
 */
#define TIMER_RESOLUTION_MS		1
#define TIMER_RESOLUTION_TICKS	125 * TIMER_RESOLUTION_MS
#define P1_INTERRUPT_MASK		0x01
#define P1_INPUT_MASK			0xC0
#define P1_RESET_INTERRUPT_FLAG 0x00
#define LEFT_SIGNAL_1_MASK		0x30
#define LEFT_SIGNAL_2_MASK		0x28
#define LEFT_SIGNAL_3_MASK		0x18
#define LEFT_SIGNAL_4_MASK		0x00
#define	LEFT_SIGNAL_ALL_MASK	0x00
#define RIGHT_SIGNAL_1_MASK		0x03
#define RIGHT_SIGNAL_2_MASK		0x05
#define RIGHT_SIGNAL_3_MASK		0x06
#define RIGHT_SIGNAL_4_MASK		0x00
#define RIGHT_SIGNAL_ALL_MASK	0x00

/*
 * Typedefs:
 * Signal - tells whether the left or right signal is active
 */
typedef enum { INIT, PATTERN_1, PATTERN_2, PATTERN_3, PATTERN_4 } State;

/*
 * Globals:
 * next_timer - tells whether the left or right signal will fire next
 * next_left - the time remaining until the the left signal fires
 * next_right - the time remaining until the right signal fires
 * previous_input - the previous input from P1 to the system
 */
State left_state;
State right_state;
unsigned int next_left;
unsigned int next_right;
volatile unsigned int tick_count;
unsigned char interrupt_flag;
volatile unsigned char input_vector;
unsigned char previous_input;

/*
 * port_one_interrupt - this function executes everytime an interrupt event occurs on P1
 * arguments:
 * (none)
 * returns:
 * (none)
 */
#pragma vector=PORT1_VECTOR
__interrupt void port_one_interrupt( void )
{
	input_vector = P1IN & P1_INPUT_MASK;
	interrupt_flag |= P1_INTERRUPT_MASK;
	P1IES = input_vector;
	P1IFG = P1_RESET_INTERRUPT_FLAG;
	__bic_SR_register_on_exit( LPM0_bits );
	return;
}

/*
 * timer_a_interrupt - this function executes everytime timer A reaches the value in CCR0
 * arguments:
 * (none)
 * returns:
 * (none)
 */
#pragma vector=TIMERA0_VECTOR
__interrupt void timer_a_interrupt( void )
{
	TACTL = MC_0;
	tick_count++;
	CCR0 = TIMER_RESOLUTION_TICKS;
	TAR = 0;
	TACTL = TASSEL_2 + ID_3 + MC_1;
	__bic_SR_register_on_exit( LPM0_bits );
	return;
}

unsigned int left_state_machine( State *left_state, unsigned int *next_left, unsigned int *tick_count, unsigned int *wait_time, unsigned int *last_start )
{
	switch ( *left_state )
	{
		case INIT :
			if ( ( input_vector & 0x80 ) == 0x80 )
			{
				*left_state = PATTERN_1;
				*next_left = *tick_count + *wait_time;
				*last_start = *tick_count;
				P1OUT = ( P1OUT & 0x03 ) | LEFT_SIGNAL_1_MASK;
				if ( TACTL == MC_0 )
				{
					TAR = 0;
					CCR0 = TIMER_RESOLUTION_TICKS;
					TACTL = TASSEL_2 + ID_3 + MC_1;
				}
			}
			break;
		case PATTERN_1 :
			if ( ( input_vector & 0x80 ) != 0x80 )
			{
				*left_state = INIT;
				P1OUT = ( P1OUT & 0x03 ) | LEFT_SIGNAL_ALL_MASK;
				if ( *tick_count < *last_start )
				{
					return *tick_count + UINT16_MAX - *last_start;
				}
				return *tick_count - *last_start;
			}
			if ( *tick_count == *next_left )
			{
				*left_state = PATTERN_2;
				*next_left = *tick_count + *wait_time;
				P1OUT = ( P1OUT & 0x03 ) | LEFT_SIGNAL_2_MASK;
			}
			break;
		case PATTERN_2 :
			if ( ( input_vector & 0x80 ) != 0x80 )
			{
				*left_state = INIT;
				P1OUT = ( P1OUT & 0x03 ) | LEFT_SIGNAL_ALL_MASK;
				if ( *tick_count < *last_start )
				{
					return *tick_count + UINT16_MAX - *last_start;
				}
				return *tick_count - *last_start;
			}
			if ( *tick_count == *next_left )
			{
				*left_state = PATTERN_3;
				*next_left = *tick_count + *wait_time;
				P1OUT = ( P1OUT & 0x03 ) | LEFT_SIGNAL_3_MASK;
			}
			break;
		case PATTERN_3 :
			if ( ( input_vector & 0x80 ) != 0x80 )
			{
				*left_state = INIT;
				P1OUT = ( P1OUT & 0x03 ) | LEFT_SIGNAL_ALL_MASK;
				if ( *tick_count < *last_start )
				{
					return *tick_count + UINT16_MAX - *last_start;
				}
				return *tick_count - *last_start;
			}
			if ( *tick_count == *next_left )
			{
				*left_state = PATTERN_4;
				*next_left = *tick_count + *wait_time;
				P1OUT = ( P1OUT & 0x03 ) | LEFT_SIGNAL_4_MASK;
			}
			break;
		case PATTERN_4 :
			if ( ( input_vector & 0x80 ) != 0x80 )
			{
				*left_state = INIT;
				P1OUT = ( P1OUT & 0x03 ) | LEFT_SIGNAL_ALL_MASK;
				if ( *tick_count < *last_start )
				{
					return *tick_count + UINT16_MAX - *last_start;
				}
				return *tick_count - *last_start;
			}
			break;
		default :
			*left_state = INIT;
			P1OUT = ( P1OUT & 0x03 ) | LEFT_SIGNAL_ALL_MASK;
			break;
	}
	return 0;
}


/*
 * main - code entry point
 * arguments:
 * (none)
 * returns:
 * integer status code (should never return in practice)
 */
int main( void )
{
    WDTCTL = WDTPW | WDTHOLD;
    next_left = 0;
    next_right = 0;
	P1REN = 0x00;
    P1SEL = 0x00;
	P1DIR = 0x3F;
	P1OUT = 0x00;
	P1IES = 0x00;
	P1IFG = 0x00;
	P1IE = 0xC0;
	TACTL = MC_0;
	CCTL0 |= CCIE;
	unsigned int left_past[16];
	left_state = INIT;
	input_vector = 0x00;
	unsigned int wait_time = 300;
	unsigned int last_start_left = 0;
	__enable_interrupt();

	while ( 1 )
	{
		//if ( tick_count >= max_tick_count )
		//{
		//	tick_count -= max_tick_count;
		//}
		left_state_machine( &left_state, &next_left, &tick_count, &wait_time, &last_start_left );
		__bis_SR_register( LPM0_bits + GIE );
	}
}
