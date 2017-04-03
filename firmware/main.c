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

/*
 * Typedefs:
 * Signal - tells whether the left or right signal is active
 */
typedef enum { LEFT_SIG, RIGHT_SIG } Signal;

/*
 * Globals:
 * next_timer - tells whether the left or right signal will fire next
 * next_left - the time remaining until the the left signal fires
 * next_right - the time remaining until the right signal fires
 * previous_input - the previous input from P1 to the system
 */
Signal next_timer;
unsigned int next_left;
unsigned int next_right;
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
	unsigned char input_vector = P1IN & 0xC0;
	TACTL = MC_0;
	P1IES = input_vector;
	P1IFG = 0x00;
	if ( input_vector == 0x40 )
	{
		if ( TAR == 0 || next_timer == RIGHT_SIG )
		{
			CCR0 = 30000;
			next_timer = RIGHT_SIG;
		}
		else
		{
			next_right = TAR;
		}
		TACTL = TASSEL_2 + MC_1 + ID_3;
		P1OUT = ( P1OUT & 0xF8 ) | 0x03;
		return;
	}
	if ( input_vector == 0x80 )
	{
		if ( TAR == 0 || next_timer == LEFT_SIG )
		{
			CCR0 = 30000;
			next_timer = LEFT_SIG;
		}
		else
		{
			next_left = TAR;
		}
		TACTL = TASSEL_2 + MC_1 + ID_3;
		P1OUT = ( P1OUT & 0xC7 ) | 0x30;
		return;
	}
	if ( input_vector == 0xC0 )
	{
		CCR0 = 30000;
		next_left = 10;
		next_timer = RIGHT_SIG;
		TACTL = TASSEL_2 + MC_1 + ID_3;
		P1OUT = ( P1OUT & 0xC0 ) | 0x33;
		return;
	}
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
	if ( next_timer == RIGHT_SIG )
	{
		if ( ( P1OUT & 0x07 ) == 0x03 )
		{
			P1OUT = ( P1OUT & 0xF8 ) | 0x05;
			if ( next_left == 0 )
			{
				CCR0 = 30000;
			}
			else
			{
				CCR0 = next_left;
				next_right = 30000 - next_left;
				next_timer = LEFT_SIG;
			}
			TACTL = TASSEL_2 + MC_1 + ID_3;
			return;
		}
		if ( ( P1OUT & 0x07 ) == 0x05 )
		{
			P1OUT = ( P1OUT & 0xF8 ) | 0x06;
			if ( next_left == 0 )
			{
				CCR0 = 30000;
			}
			else
			{
				CCR0 = next_left;
				next_right = 30000 - next_left;
				next_timer = LEFT_SIG;
			}
			TACTL = TASSEL_2 + MC_1 + ID_3;
			return;
		}
		if ( ( P1OUT & 0x07 ) == 0x06 )
		{
			P1OUT = ( P1OUT & 0xF8 ) | 0x00;
			if ( next_left == 0 )
			{
				TACTL = MC_0;
				TAR = 0;
				return;
			}
			else
			{
				CCR0 = next_left;
				next_timer = LEFT_SIG;
			}
			TACTL = TASSEL_2 + MC_1 + ID_3;
			return;
		}
	}
	else
	{
		if ( ( P1OUT & 0x38 ) == 0x30 )
		{
			P1OUT = ( P1OUT & 0xC7 ) | 0x28;
			if ( next_right == 0 )
			{
				CCR0 = 30000;
			}
			else
			{
				CCR0 = next_right;
				next_left = 30000 - next_right;
				next_timer = RIGHT_SIG;
			}
			TACTL = TASSEL_2 + MC_1 + ID_3;
			return;
		}
		if ( ( P1OUT & 0x38 ) == 0x28 )
		{
			P1OUT = ( P1OUT & 0xC7 ) | 0x18;
			if ( next_right == 0 )
			{
				CCR0 = 30000;
			}
			else
			{
				CCR0 = next_right;
				next_left = 30000 - next_right;
				next_timer = RIGHT_SIG;
			}
			TACTL = TASSEL_2 + MC_1 + ID_3;
			return;
		}
		if ( ( P1OUT & 0x38 ) == 0x18 )
		{
			P1OUT = ( P1OUT & 0xC7 ) | 0x00;
			if ( next_right == 0 )
			{
				TACTL = MC_0;
				TAR = 0;
				return;
			}
			else
			{
				CCR0 = next_right;
				next_timer = RIGHT_SIG;
			}
			TACTL = TASSEL_2 + MC_1 + ID_3;
			return;
		}
	}
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
    next_timer = RIGHT_SIG;
    next_left = 0;
    next_right = 0;
	P1REN = 0x00;
    P1SEL = 0x00;
	P1DIR = 0x3F;
	P1OUT = 0x00;
	previous_input = P1IN & 0xC0;
	P1IES = 0x00;
	P1IFG = 0x00;
	P1IE = 0xC0;
	CCTL0 |= CCIE;
	__enable_interrupt();
	_BIS_SR( LPM0 + GIE );
	return 0;
}
