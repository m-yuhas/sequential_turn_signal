#include <msp430.h>

typedef enum
{
	NOSIG_REST,
	NOSIG_BREAK,
	SIG_RIGHT_1,
	SIG_RIGHT_1B,
	SIG_RIGHT_1_BREAK,
	SIG_RIGHT_1_REST,
	SIG_RIGHT_2,
	SIG_RIGHT_2B,
	SIG_RIGHT_2_BREAK,
	SIG_RIGHT_2_REST,
	SIG_RIGHT_3,
	SIG_RIGHT_3B,
	SIG_RIGHT_3_BREAK,
	SIG_RIGHT_3_REST,
	SIG_LEFT_1,
	SIG_LEFT_1B,
	SIG_LEFT_1_BREAK,
	SIG_LEFT_1_REST,
	SIG_LEFT_2,
	SIG_LEFT_2B,
	SIG_LEFT_2_BREAK,
	SIG_LEFT_2_REST,
	SIG_LEFT_3,
	SIG_LEFT_3B,
	SIG_LEFT_3_BREAK,
	SIG_LEFT_3_REST
} State;

State prgrm_state = SIG_RIGHT_3_BREAK;

#pragma vector=PORT1_VECTOR
__interrupt void port_one_interrupt( void )
{
	char input_vector = P1IN & 0xC0;
	P1IFG = 0x00;
	P1IES = input_vector;
	switch( prgrm_state )
	{
		case SIG_RIGHT_1:
		{
			switch( input_vector )
			{
				case 0x00:
				{
					P1OUT = 0x3F;
					prgrm_state = SIG_RIGHT_1_REST;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x37;
					prgrm_state = SIG_LEFT_1;
					return;
				}
				case 0xC0:
				{
					P1OUT = 0x00;
					prgrm_state = SIG_RIGHT_1_BREAK;
					return;
				}
			}
			break;
		}
		case SIG_RIGHT_1_REST:
		{
			switch( input_vector )
			{
				case 0x40:
				{
					P1OUT = 0x3D;
					prgrm_state = SIG_RIGHT_2;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x37;
					prgrm_state = SIG_LEFT_1;
					return;
				}
				case 0xC0:
				{
					P1OUT = 0x00;
					prgrm_state = SIG_RIGHT_1_BREAK;
					return;
				}
			}
			break;
		}
		case SIG_RIGHT_1_BREAK:
		{
			switch( input_vector )
			{
				case 0x00:
				{
					P1OUT = 0x3F;
					prgrm_state = SIG_RIGHT_1_REST;
					return;
				}
				case 0x40:
				{
					P1OUT = 0x37;
					prgrm_state = SIG_LEFT_1;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x3D;
					prgrm_state = SIG_RIGHT_2;
					return;
				}
			}
			break;
		}
		case SIG_RIGHT_2:
		{
			switch( input_vector )
			{
				case 0x00:
				{
					P1OUT = 0x3F;
					prgrm_state = SIG_RIGHT_2_REST;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x37;
					prgrm_state = SIG_LEFT_1;
					return;
				}
				case 0xC0:
				{
					P1OUT = 0x00;
					prgrm_state = SIG_RIGHT_2_BREAK;
					return;
				}
			}
			break;
		}
		case SIG_RIGHT_2_REST:
		{
			switch( input_vector )
			{
				case 0x40:
				{
					P1OUT = 0x3E;
					prgrm_state = SIG_RIGHT_3;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x37;
					prgrm_state = SIG_LEFT_1;
					return;
				}
				case 0xC0:
				{
					P1OUT = 0x00;
					prgrm_state = SIG_RIGHT_2_BREAK;
					return;
				}
			}
			break;
		}
		case SIG_RIGHT_2_BREAK:
		{
			switch( input_vector )
			{
				case 0x00:
				{
					P1OUT = 0x3F;
					prgrm_state = SIG_RIGHT_2_REST;
					return;
				}
				case 0x40:
				{
					P1OUT = 0x37;
					prgrm_state = SIG_LEFT_1;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x3E;
					prgrm_state = SIG_RIGHT_3;
					return;
				}
			}
			break;
		}
		case SIG_RIGHT_3:
		{
			switch( input_vector )
			{
				case 0x00:
				{
					P1OUT = 0x3F;
					prgrm_state = SIG_RIGHT_3_REST;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x37;
					prgrm_state = SIG_LEFT_1;
					return;
				}
				case 0xC0:
				{
					P1OUT = 0x00;
					prgrm_state = SIG_RIGHT_3_BREAK;
					return;
				}
			}
			break;
		}
		case SIG_RIGHT_3_REST:
		{
			switch( input_vector )
			{
				case 0x40:
				{
					P1OUT = 0x3B;
					prgrm_state = SIG_RIGHT_1;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x37;
					prgrm_state = SIG_LEFT_1;
					return;
				}
				case 0xC0:
				{
					P1OUT = 0x00;
					prgrm_state = SIG_RIGHT_3_BREAK;
					return;
				}
			}
			break;
		}
		case SIG_RIGHT_3_BREAK:
		{
			switch( input_vector )
			{
				case 0x00:
				{
					P1OUT = 0x3F;
					prgrm_state = SIG_RIGHT_3_REST;
					return;
				}
				case 0x40:
				{
					P1OUT = 0x37;
					prgrm_state = SIG_LEFT_1;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x3B;
					prgrm_state = SIG_RIGHT_1;
					return;
				}
			}
			break;
		}
		case SIG_LEFT_1:
		{
			switch( input_vector )
			{
				case 0x00:
				{
					P1OUT = 0x3F;
					prgrm_state = SIG_LEFT_1_REST;
					return;
				}
				case 0x40:
				{
					P1OUT = 0x3B;
					prgrm_state = SIG_RIGHT_1;
					return;
				}
				case 0xC0:
				{
					P1OUT = 0x00;
					prgrm_state = SIG_LEFT_1_BREAK;
					return;
				}
			}
			break;
		}
		case SIG_LEFT_1_REST:
		{
			switch( input_vector )
			{
				case 0x40:
				{
					P1OUT = 0x3B;
					prgrm_state = SIG_RIGHT_1;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x2F;
					prgrm_state = SIG_LEFT_2;
					return;
				}
				case 0xC0:
				{
					P1OUT = 0x00;
					prgrm_state = SIG_LEFT_1_BREAK;
					return;
				}
			}
			break;
		}
		case SIG_LEFT_1_BREAK:
		{
			switch( input_vector )
			{
				case 0x00:
				{
					P1OUT = 0x3F;
					prgrm_state = SIG_LEFT_1_REST;
					return;
				}
				case 0x40:
				{
					P1OUT = 0x37;
					prgrm_state = SIG_LEFT_2;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x3B;
					prgrm_state = SIG_RIGHT_1;
					return;
				}
			}
			break;
		}
		case SIG_LEFT_2:
		{
			switch( input_vector )
			{
				case 0x00:
				{
					P1OUT = 0x3F;
					prgrm_state = SIG_LEFT_2_REST;
					return;
				}
				case 0x40:
				{
					P1OUT = 0x3B;
					prgrm_state = SIG_RIGHT_1;
					return;
				}
				case 0xC0:
				{
					P1OUT = 0x00;
					prgrm_state = SIG_LEFT_2_BREAK;
					return;
				}
			}
			break;
		}
		case SIG_LEFT_2_REST:
		{
			switch( input_vector )
			{
				case 0x40:
				{
					P1OUT = 0x3B;
					prgrm_state = SIG_RIGHT_1;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x1F;
					prgrm_state = SIG_LEFT_3;
					return;
				}
				case 0xC0:
				{
					P1OUT = 0x00;
					prgrm_state = SIG_LEFT_2_BREAK;
					return;
				}
			}
			break;
		}
		case SIG_LEFT_2_BREAK:
		{
			switch( input_vector )
			{
				case 0x00:
				{
					P1OUT = 0x3F;
					prgrm_state = SIG_LEFT_2_REST;
					return;
				}
				case 0x40:
				{
					P1OUT = 0x1F;
					prgrm_state = SIG_LEFT_3;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x3B;
					prgrm_state = SIG_RIGHT_1;
					return;
				}
			}
			break;
		}
		case SIG_LEFT_3:
		{
			switch( input_vector )
			{
				case 0x00:
				{
					P1OUT = 0x3F;
					prgrm_state = SIG_LEFT_3_REST;
					return;
				}
				case 0x40:
				{
					P1OUT = 0x3B;
					prgrm_state = SIG_RIGHT_1;
					return;
				}
				case 0xC0:
				{
					P1OUT = 0x00;
					prgrm_state = SIG_LEFT_3_BREAK;
					return;
				}
			}
			break;
		}
		case SIG_LEFT_3_REST:
		{
			switch( input_vector )
			{
				case 0x40:
				{
					P1OUT = 0x3B;
					prgrm_state = SIG_RIGHT_1;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x37;
					prgrm_state = SIG_LEFT_1;
					return;
				}
				case 0xC0:
				{
					P1OUT = 0x00;
					prgrm_state = SIG_LEFT_3_BREAK;
					return;
				}
			}
			break;
		}
		case SIG_LEFT_3_BREAK:
		{
			switch( input_vector )
			{
				case 0x00:
				{
					P1OUT = 0x3F;
					prgrm_state = SIG_LEFT_3_REST;
					return;
				}
				case 0x40:
				{
					P1OUT = 0x37;
					prgrm_state = SIG_LEFT_1;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x3B;
					prgrm_state = SIG_RIGHT_1;
					return;
				}
			}
			break;
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
	__enable_interrupt();

	//prgrm_state = SIG_RIGHT_1_BREAK;

	_BIS_SR( LPM3_bits + GIE );
	return 0;
	//while( 1 )
	//{
		//Infinite Loop
	//}
}
