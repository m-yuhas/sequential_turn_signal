#include <msp430.h>

typedef enum
{
	SIG_RIGHT_1_ON,
	SIG_RIGHT_1_OFF,
	SIG_RIGHT_2_ON,
	SIG_RIGHT_2_OFF,
	SIG_RIGHT_3_ON,
	SIG_RIGHT_3_OFF,
	SIG_LEFT_1_ON,
	SIG_LEFT_1_OFF,
	SIG_LEFT_2_ON,
	SIG_LEFT_2_OFF,
	SIG_LEFT_3_ON,
	SIG_LEFT_3_OFF
} State;

State prgrm_state = SIG_LEFT_3_OFF;
char last_input;

#pragma vector=PORT1_VECTOR
__interrupt void port_one_interrupt( void )
{
	char input_vector = P1IN & 0xC0;
	char diff_bits = input_vector ^ last_input;
	last_input = input_vector;
	P1IFG = 0x00;
	P1IES = input_vector;
	switch( prgrm_state )
	{
		case SIG_RIGHT_1_ON:
		{
			if ( diff_bits == 0x40 )
			{
				prgrm_state = SIG_RIGHT_1_OFF;
				P1OUT = 0x00;
				return;
			}
			if ( diff_bits == 0xC0 )
			{
				prgrm_state = SIG_LEFT_1_ON;
				P1OUT = 0x30;
				return;
			}
			return;
		}
		case SIG_RIGHT_1_OFF:
		{
			if ( diff_bits == 0x40 )
			{
				prgrm_state = SIG_RIGHT_2_ON;
				P1OUT = 0x05;
				return;
			}
			if ( diff_bits == 0xC0 )
			{
				prgrm_state = SIG_LEFT_1_OFF;
				P1OUT = 0x00;
				return;
			}
			return;
		}
		case SIG_RIGHT_2_ON:
		{
			if ( diff_bits == 0x40 )
			{
				prgrm_state = SIG_RIGHT_2_OFF;
				P1OUT = 0x00;
				return;
			}
			if ( diff_bits == 0xC0 )
			{
				prgrm_state = SIG_LEFT_1_ON;
				P1OUT = 0x30;
				return;
			}
			return;
		}
		case SIG_RIGHT_2_OFF:
		{
			if ( diff_bits == 0x40 )
			{
				prgrm_state = SIG_RIGHT_3_ON;
				P1OUT = 0x06;
				return;
			}
			if ( diff_bits == 0xC0 )
			{
				prgrm_state = SIG_LEFT_1_OFF;
				P1OUT = 0x00;
				return;
			}
			return;
		}
		case SIG_RIGHT_3_ON:
		{
			if ( diff_bits == 0x40 )
			{
				prgrm_state = SIG_RIGHT_3_OFF;
				P1OUT = 0x00;
				return;
			}
			if ( diff_bits == 0xC0 )
			{
				prgrm_state = SIG_LEFT_1_ON;
				P1OUT = 0x30;
				return;
			}
			return;
		}
		case SIG_RIGHT_3_OFF:
		{
			if ( diff_bits == 0x40 )
			{
				prgrm_state = SIG_RIGHT_1_ON;
				P1OUT = 0x03;
				return;
			}
			if ( diff_bits == 0xC0 )
			{
				prgrm_state = SIG_LEFT_1_OFF;
				P1OUT = 0x00;
				return;
			}
			return;
		}
		case SIG_LEFT_1_ON:
		{
			if ( diff_bits == 0x80 )
			{
				prgrm_state = SIG_LEFT_1_OFF;
				P1OUT = 0x00;
				return;
			}
			if ( diff_bits == 0xC0 )
			{
				prgrm_state = SIG_RIGHT_1_ON;
				P1OUT = 0x03;
				return;
			}
			return;
		}
		case SIG_LEFT_1_OFF:
		{
			if ( diff_bits == 0x80 )
			{
				prgrm_state = SIG_LEFT_2_ON;
				P1OUT = 0x28;
				return;
			}
			if ( diff_bits == 0xC0 )
			{
				prgrm_state = SIG_RIGHT_1_OFF;
				P1OUT = 0x00;
				return;
			}
			return;
		}
		case SIG_LEFT_2_ON:
		{
			if ( diff_bits == 0x80 )
			{
				prgrm_state = SIG_LEFT_2_OFF;
				P1OUT = 0x00;
				return;
			}
			if ( diff_bits == 0xC0 )
			{
				prgrm_state = SIG_RIGHT_1_ON;
				P1OUT = 0x03;
				return;
			}
			return;
		}
		case SIG_LEFT_2_OFF:
		{
			if ( diff_bits == 0x80 )
			{
				prgrm_state = SIG_LEFT_3_ON;
				P1OUT = 0x18;
				return;
			}
			if ( diff_bits == 0xC0 )
			{
				prgrm_state = SIG_RIGHT_1_OFF;
				P1OUT = 0x00;
				return;
			}
			return;
		}
		case SIG_LEFT_3_ON:
		{
			if ( diff_bits == 0x80 )
			{
				prgrm_state = SIG_LEFT_3_OFF;
				P1OUT = 0x00;
				return;
			}
			if ( diff_bits == 0xC0 )
			{
				prgrm_state = SIG_RIGHT_1_ON;
				P1OUT = 0x03;
				return;
			}
			return;
		}
		case SIG_LEFT_3_OFF:
		{
			if ( diff_bits == 0x80 )
			{
				prgrm_state = SIG_LEFT_1_ON;
				P1OUT = 0x30;
				return;
			}
			if ( diff_bits == 0xC0 )
			{
				prgrm_state = SIG_RIGHT_1_OFF;
				P1OUT = 0x00;
				return;
			}
			return;
		}
	}
	return;
}

#pragma vector=TIMERA0_VECTOR
__interrupt void timer_a_interrupt( void )
{
	return;
}

/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	P1REN = 0x00;
    P1SEL = 0x00;
	P1DIR = 0x3F;
	P1IES = 0xC0;
	P1IFG = 0x00;
	P1OUT = 0x00;
	P1IE = 0xC0;
	last_input = P1IN & 0xC0;
	prgrm_state = SIG_LEFT_3_OFF;
	__enable_interrupt();
	_BIS_SR( LPM3_bits + GIE );
}
