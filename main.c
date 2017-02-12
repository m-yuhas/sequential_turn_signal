#include <msp430.h>

enum STATE
{
	NOSIG_BREAK,
	NOSIG_REST,
	SIG_RIGHT_1,
	SIG_RIGHT_1_BREAK,
	SIG_RIGHT_1_REST,
	SIG_RIGHT_2,
	SIG_RIGHT_2_BREAK,
	SIG_RIGHT_2_REST,
	SIG_RIGHT_3,
	SIG_RIGHT_3_BREAK,
	SIG_RIGHT_3_REST,
	SIG_LEFT_1,
	SIG_LEFT_1_BREAK,
	SIG_LEFT_1_REST,
	SIG_LEFT_2,
	SIG_LEFT_2_BREAK,
	SIG_LEFT_2_REST,
	SIG_LEFT_3,
	SIG_LEFT_3_BREAK,
	SIG_LEFT_3_REST
} prgrm_state;

#pragma vector=PORT1_VECTOR
__interrupt void port_one_interrupt( void )
{
	/*
	//char a = P1IN & 0x40;
	if ( (P1IN & 0x40) == 0x40 ) {
		P1OUT = 0xFF;
	}
	//char b = P1IN & 0x80;
	if ( (P1IN & 0x80) == 0x80 ) {
		P1OUT = 0x00;
	}
	P1IFG &= 0x00;*/
	char input_vector = P1IN & 0xC0;
	switch( prgrm_state )
	{
		case NOSIG_BREAK:
		{
			switch( input_vector )
			{
				case 0x00:
				{
					P1OUT = 0x3F;
					P1IES = 0x00;
					prgrm_state = NOSIG_REST;
					return;
				}
				case 0x40:
				{
					P1OUT = 0x3B;
					P1IES = 0x40;
					prgrm_state = SIG_RIGHT_1;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x37;
					P1IES = 0x80;
					prgrm_state = SIG_LEFT_1;
					return;
				}
				case 0xC0:
				{
					P1OUT = 0x00;
					P1IES = 0xC0;
					prgrm_state = NOSIG_BREAK;
					return;
				}
			}
		}
		case NOSIG_REST:
		{
			switch( input_vector )
			{
				case 0x00:
				{
					P1OUT = 0x3F;
					P1IES = 0x00;
					prgrm_state = NOSIG_REST;
					return;
				}
				case 0x40:
				{
					P1OUT = 0x3B;
					P1IES = 0x40;
					prgrm_state = SIG_RIGHT_1;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x37;
					P1IES = 0x80;
					prgrm_state = SIG_LEFT_1;
					return;
				}
				case 0xC0:
				{
					P1OUT = 0x00;
					P1IES = 0xC0;
					prgrm_state = NOSIG_BREAK;
					return;
				}
			}
		}
		case SIG_RIGHT_1:
		{
			switch( input_vector )
			{
				case 0x00:
				{
					P1OUT = 0x3F;
					P1IES = 0x00;
					prgrm_state = SIG_RIGHT_1_REST;
					return;
				}
				case 0x40:
				{
					P1OUT = 0x3B;
					P1IES = 0x40;
					prgrm_state = SIG_RIGHT_1;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x37;
					P1IES = 0x80;
					prgrm_state = SIG_LEFT_1;
					return;
				}
				case 0xC0:
				{
					P1OUT = 0x00;
					P1IES = 0xC0;
					prgrm_state = SIG_RIGHT_1_BREAK;
					return;
				}
			}
		}
		case SIG_RIGHT_1_BREAK:
		{
			switch( input_vector )
			{
				case 0x00:
				{
					P1OUT = 0x3F;
					P1IES = 0x00;
					prgrm_state = SIG_RIGHT_1_REST;
					return;
				}
				case 0x40:
				{
					P1OUT = 0x3D;
					P1IES = 0x40;
					prgrm_state = SIG_RIGHT_2;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x37;
					P1IES = 0x80;
					prgrm_state = SIG_LEFT_1;
					return;
				}
				case 0xC0:
				{
					P1OUT = 0x00;
					P1IES = 0xC0;
					prgrm_state = SIG_RIGHT_1_BREAK;
					return;
				}
			}
		}
		case SIG_RIGHT_2:
		{
			switch( input_vector )
			{
				case 0x00:
				{
					P1OUT = 0x3F;
					P1IES = 0x00;
					prgrm_state = SIG_RIGHT_2_REST;
					return;
				}
				case 0x40:
				{
					P1OUT = 0x3D;
					P1IES = 0x40;
					prgrm_state = SIG_RIGHT_2;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x37;
					P1IES = 0x80;
					prgrm_state = SIG_LEFT_1;
					return;
				}
				case 0xC0:
				{
					P1OUT = 0x00;
					P1IES = 0xC0;
					prgrm_state = SIG_RIGHT_2_BREAK;
					return;
				}
			}
		}
		case SIG_RIGHT_2_REST:
		{
			switch( input_vector )
			{
				case 0x00:
				{
					P1OUT = 0x3F;
					P1IES = 0x00;
					prgrm_state = SIG_RIGHT_2_REST;
					return;
				}
				case 0x40:
				{
					P1OUT = 0x3E;
					P1IES = 0x40;
					prgrm_state = SIG_RIGHT_3;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x37;
					P1IES = 0x80;
					prgrm_state = SIG_LEFT_1;
					return;
				}
				case 0xC0:
				{
					P1OUT = 0x00;
					P1IES = 0xC0;
					prgrm_state = SIG_RIGHT_2_BREAK;
					return;
				}
			}
		}
		case SIG_RIGHT_2_BREAK:
		{
			switch( input_vector )
			{
				case 0x00:
				{
					P1OUT = 0x3F;
					P1IES = 0x00;
					prgrm_state = SIG_RIGHT_2_REST;
					return;
				}
				case 0x40:
				{
					P1OUT = 0x3E;
					P1IES = 0x40;
					prgrm_state = SIG_RIGHT_3;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x37;
					P1IES = 0x80;
					prgrm_state = SIG_LEFT_1;
					return;
				}
				case 0xC0:
				{
					P1OUT = 0x00;
					P1IES = 0xC0;
					prgrm_state = SIG_RIGHT_2_BREAK;
					return;
				}
			}
		}
		case SIG_RIGHT_3:
		{
			switch( input_vector )
			{
				case 0x00:
				{
					P1OUT = 0x3F;
					P1IES = 0x00;
					prgrm_state = SIG_RIGHT_3_REST;
					return;
				}
				case 0x40:
				{
					P1OUT = 0x3E;
					P1IES = 0x40;
					prgrm_state = SIG_RIGHT_3;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x37;
					P1IES = 0x80;
					prgrm_state = SIG_LEFT_1;
					return;
				}
				case 0xC0:
				{
					P1OUT = 0x00;
					P1IES = 0xC0;
					prgrm_state = SIG_RIGHT_3_BREAK;
					return;
				}
			}
		}
		case SIG_RIGHT_3_REST:
		{
			switch( input_vector )
			{
				case 0x00:
				{
					P1OUT = 0x3F;
					P1IES = 0x00;
					prgrm_state = SIG_RIGHT_3_REST;
					return;
				}
				case 0x40:
				{
					P1OUT = 0x3B;
					P1IES = 0x40;
					prgrm_state = SIG_RIGHT_1;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x37;
					P1IES = 0x80;
					prgrm_state = SIG_LEFT_1;
					return;
				}
				case 0xC0:
				{
					P1OUT = 0x00;
					P1IES = 0xC0;
					prgrm_state = SIG_RIGHT_3_BREAK;
					return;
				}
			}
		}
		case SIG_RIGHT_3_BREAK:
		{
			switch( input_vector )
			{
				case 0x00:
				{
					P1OUT = 0x3F;
					P1IES = 0x00;
					prgrm_state = SIG_RIGHT_3_REST;
					return;
				}
				case 0x40:
				{
					P1OUT = 0x3B;
					P1IES = 0x40;
					prgrm_state = SIG_RIGHT_1;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x37;
					P1IES = 0x80;
					prgrm_state = SIG_LEFT_1;
					return;
				}
				case 0xC0:
				{
					P1OUT = 0x00;
					P1IES = 0xC0;
					prgrm_state = SIG_RIGHT_3_BREAK;
					return;
				}
			}
		}
		case SIG_LEFT_1:
		{
			switch( input_vector )
			{
				case 0x00:
				{
					P1OUT = 0x3F;
					P1IES = 0x00;
					prgrm_state = SIG_LEFT_1_REST;
					return;
				}
				case 0x40:
				{
					P1OUT = 0x3B;
					P1IES = 0x40;
					prgrm_state = SIG_RIGHT_1;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x37;
					P1IES = 0x80;
					prgrm_state = SIG_LEFT_1;
					return;
				}
				case 0xC0:
				{
					P1OUT = 0x00;
					P1IES = 0xC0;
					prgrm_state = SIG_LEFT_1_BREAK;
					return;
				}
			}
		}
		case SIG_LEFT_1_REST:
		{
			switch( input_vector )
			{
				case 0x00:
				{
					P1OUT = 0x3F;
					P1IES = 0x00;
					prgrm_state = SIG_LEFT_1_REST;
					return;
				}
				case 0x40:
				{
					P1OUT = 0x3B;
					P1IES = 0x40;
					prgrm_state = SIG_RIGHT_1;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x2F;
					P1IES = 0x80;
					prgrm_state = SIG_LEFT_2;
					return;
				}
				case 0xC0:
				{
					P1OUT = 0x00;
					P1IES = 0xC0;
					prgrm_state = SIG_LEFT_1_BREAK;
					return;
				}
			}
		}
		case SIG_LEFT_1_BREAK:
		{
			switch( input_vector )
			{
				case 0x00:
				{
					P1OUT = 0x3F;
					P1IES = 0x00;
					prgrm_state = SIG_LEFT_1_REST;
					return;
				}
				case 0x40:
				{
					P1OUT = 0x3B;
					P1IES = 0x40;
					prgrm_state = SIG_RIGHT_1;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x37;
					P1IES = 0x80;
					prgrm_state = SIG_LEFT_2;
					return;
				}
				case 0xC0:
				{
					P1OUT = 0x00;
					P1IES = 0xC0;
					prgrm_state = SIG_LEFT_1_BREAK;
					return;
				}
			}
		}
		case SIG_LEFT_2:
		{
			switch( input_vector )
			{
				case 0x00:
				{
					P1OUT = 0x3F;
					P1IES = 0x00;
					prgrm_state = SIG_LEFT_2_REST;
					return;
				}
				case 0x40:
				{
					P1OUT = 0x3B;
					P1IES = 0x40;
					prgrm_state = SIG_RIGHT_1;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x2F;
					P1IES = 0x80;
					prgrm_state = SIG_LEFT_2;
					return;
				}
				case 0xC0:
				{
					P1OUT = 0x00;
					P1IES = 0xC0;
					prgrm_state = SIG_LEFT_2_BREAK;
					return;
				}
			}
		}
		case SIG_LEFT_2_REST:
		{
			switch( input_vector )
			{
				case 0x00:
				{
					P1OUT = 0x3F;
					P1IES = 0x00;
					prgrm_state = SIG_LEFT_2_REST;
					return;
				}
				case 0x40:
				{
					P1OUT = 0x3B;
					P1IES = 0x40;
					prgrm_state = SIG_RIGHT_1;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x1F;
					P1IES = 0x80;
					prgrm_state = SIG_LEFT_3;
					return;
				}
				case 0xC0:
				{
					P1OUT = 0x00;
					P1IES = 0xC0;
					prgrm_state = SIG_LEFT_2_BREAK;
					return;
				}
			}
		}
		case SIG_LEFT_2_BREAK:
		{
			switch( input_vector )
			{
				case 0x00:
				{
					P1OUT = 0x3F;
					P1IES = 0x00;
					prgrm_state = SIG_LEFT_2_REST;
					return;
				}
				case 0x40:
				{
					P1OUT = 0x3B;
					P1IES = 0x40;
					prgrm_state = SIG_RIGHT_1;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x1F;
					P1IES = 0x80;
					prgrm_state = SIG_LEFT_3;
					return;
				}
				case 0xC0:
				{
					P1OUT = 0x00;
					P1IES = 0xC0;
					prgrm_state = SIG_LEFT_3_BREAK;
					return;
				}
			}
		}
		case SIG_LEFT_3:
		{
			switch( input_vector )
			{
				case 0x00:
				{
					P1OUT = 0x3F;
					P1IES = 0x00;
					prgrm_state = SIG_LEFT_3_REST;
					return;
				}
				case 0x40:
				{
					P1OUT = 0x3B;
					P1IES = 0x40;
					prgrm_state = SIG_RIGHT_1;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x1F;
					P1IES = 0x80;
					prgrm_state = SIG_LEFT_3;
					return;
				}
				case 0xC0:
				{
					P1OUT = 0x00;
					P1IES = 0xC0;
					prgrm_state = SIG_LEFT_3_BREAK;
					return;
				}
			}
		}
		case SIG_LEFT_3_REST:
		{
			switch( input_vector )
			{
				case 0x00:
				{
					P1OUT = 0x3F;
					P1IES = 0x00;
					prgrm_state = SIG_LEFT_3_REST;
					return;
				}
				case 0x40:
				{
					P1OUT = 0x3B;
					P1IES = 0x40;
					prgrm_state = SIG_RIGHT_1;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x37;
					P1IES = 0x80;
					prgrm_state = SIG_LEFT_1;
					return;
				}
				case 0xC0:
				{
					P1OUT = 0x00;
					P1IES = 0xC0;
					prgrm_state = SIG_LEFT_3_BREAK;
					return;
				}
			}
		}
		case SIG_LEFT_3_BREAK:
		{
			switch( input_vector )
			{
				case 0x00:
				{
					P1OUT = 0x3F;
					P1IES = 0x00;
					prgrm_state = SIG_LEFT_3_REST;
					return;
				}
				case 0x40:
				{
					P1OUT = 0x3B;
					P1IES = 0x40;
					prgrm_state = SIG_RIGHT_1;
					return;
				}
				case 0x80:
				{
					P1OUT = 0x37;
					P1IES = 0x80;
					prgrm_state = SIG_LEFT_1;
					return;
				}
				case 0xC0:
				{
					P1OUT = 0x00;
					P1IES = 0xC0;
					prgrm_state = SIG_LEFT_3_BREAK;
					return;
				}
			}
		}
	}
}


/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	P1REN = 0x00;
    P1SEL = 0x00;
	P1DIR = 0x3F;
	P1IES = 0x00;
	P1IFG = 0x00;
	P1IE = 0xC0;
	__enable_interrupt();

	prgrm_state = NOSIG_REST;

	P1OUT = 0x0F;

	while( 1 )
	{
		//Infinite Loop
	}
}

