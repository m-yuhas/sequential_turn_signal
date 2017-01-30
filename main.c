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
	switch( prgm_state )
	{
		case NOSIG_BREAK :
			switch( P1IFG )
			{
				case 0x00 :
					P1OUT = 0x00;
					prgrm_state = NOSIG_REST;
					break;
				case 0x40 :
					P1OUT = 0x04;
					prgm_state = SIG_RIGHT_1;
					break;
			}
			break;
	}
}


/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	P1DIR = 0x3F;
	P1IE = 0xC0;
	P1IFG = 0x00;
	P1IES = 0x00;
	__enable_interrupt();

	prgm_state = NOSIG_REST;

	while( 1 )
	{
		//Infinite Loop
	}

	return 0;
}

