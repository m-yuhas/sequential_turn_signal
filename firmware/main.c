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
 * Definitions:
 * TIMER_RESOLUTION_MS - resolution of turn signal timer in milliseconds
 * TIMER_RESOLUTION_TICKS - resolution of turn signal timer in ticks of timer A
 * P1_INPUT_MASK - input bits on port 1
 * P1_RESET_INTERRUPT_FLAG - reset value for port 1
 * LEFT_SIGNAL - output bits for the left turn signal on port 1
 * LEFT_SIGNAL_1_MASK - output bits for left turn signal pattern 1
 * LEFT_SIGNAL_2_MASK - output bits for left turn signal pattern 2
 * LEFT_SIGNAL_3_MASK - output bits for left turn signal pattern 3
 * LEFT_SIGNAL_4_MASK - output bits for left turn signal pattern 4
 * LEFT_SIGNAL_ALL_MASK - all lights in left signal set to active
 * LEFT_INPUT_MASK - input bit that controls the left turn signal
 * RIGHT_SIGNAL - output bits for the right turn signal on port 1
 * RIGHT_SIGNAL_1_MASK - output bits for right turn signal pattern 1
 * RIGHT_SIGNAL_2_MASK - output bits for right turn signal pattern 2
 * RIGHT_SIGNAL_3_MASK - output bits for right turn signal pattern 3
 * RIGHT_SIGNAL_4_MASK - output bits for right turn signal pattern 4
 * RIGHT_SIGNAL_ALL_MASK - all lights in right signal set to active
 * RIGHT_INPUT_MASK - input bit that controls the right turn signal
 */
#define TIMER_RESOLUTION_MS		1
#define TIMER_RESOLUTION_TICKS	125 * TIMER_RESOLUTION_MS
#define P1_INPUT_MASK			0xC0
#define P1_RESET_INTERRUPT_FLAG 0x00
#define LEFT_SIGNAL				0x38
#define LEFT_SIGNAL_1_MASK		0x30
#define LEFT_SIGNAL_2_MASK		0x28
#define LEFT_SIGNAL_3_MASK		0x18
#define LEFT_SIGNAL_4_MASK		0x00
#define	LEFT_SIGNAL_ALL_MASK	0x00
#define LEFT_INPUT_MASK			0x80
#define RIGHT_SIGNAL			0x07
#define RIGHT_SIGNAL_1_MASK		0x03
#define RIGHT_SIGNAL_2_MASK		0x05
#define RIGHT_SIGNAL_3_MASK		0x06
#define RIGHT_SIGNAL_4_MASK		0x00
#define RIGHT_SIGNAL_ALL_MASK	0x00
#define RIGHT_INPUT_MASK		0x40

/*
 * Typedefs:
 * State - corresponds to the current output pattern of a turn signal
 */
typedef enum { INIT, PATTERN_1, PATTERN_2, PATTERN_3, PATTERN_4 } State;

/*
 * Globals:
 * tick_count - current number of TIMER_RESOLUTION_MS length ticks that have passed
 * input_vector - vector of the input port, set by port 1 interrupt
 */
unsigned int tick_count;
unsigned char input_vector;

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
  TACTL = TASSEL_2 | ID_3 | MC_1;
  __bic_SR_register_on_exit( LPM0_bits );
  return;
}

/*
 * left_state_machine - this function keeps track of the left turn signal state machine
 * arguments:
 * *left_state - pointer to current state
 * *next_left - pointer to an integer representing the next time for a state change
 * tick_count - current tick count of the timer
 * wait_time - amount of time to wait (in ticks) between a state change
 * returns:
 * (none)
 */
void left_state_machine( State *left_state, unsigned int *next_left, unsigned int tick_count, unsigned int wait_time )
{
  // Moore type state machine
  switch ( *left_state )
  {
    // Initialization state
    case INIT :
      // Check if the left turn signal was activated
      if ( ( input_vector & LEFT_INPUT_MASK ) == LEFT_INPUT_MASK )
      {
        *left_state = PATTERN_1;
        *next_left = tick_count + wait_time;
        P1OUT = ( P1OUT & RIGHT_SIGNAL ) | LEFT_SIGNAL_1_MASK;
        // If the timer hasn't been started yet, start the timer
        if ( TACTL == MC_0 )
        {
          TAR = 0;
          CCR0 = TIMER_RESOLUTION_TICKS;
          TACTL = TASSEL_2 + ID_3 + MC_1;
        }
      }
      break;
    // Pattern 1 on left signal
    case PATTERN_1 :
      // Check if the left turn signal was turned off
      if ( ( input_vector & LEFT_INPUT_MASK ) != LEFT_INPUT_MASK )
      {
        *left_state = INIT;
        P1OUT = ( P1OUT & RIGHT_SIGNAL ) | LEFT_SIGNAL_ALL_MASK;
        return;
      }
      // Check if the time limit in Pattern 1 was reached, if so proceed to pattern 2
      if ( tick_count == *next_left )
      {
        *left_state = PATTERN_2;
        *next_left = tick_count + wait_time;
        P1OUT = ( P1OUT & RIGHT_SIGNAL ) | LEFT_SIGNAL_2_MASK;
      }
      break;
    // Pattern 2 on left signal
    case PATTERN_2 :
      // Check if the left turn signal was turned off
      if ( ( input_vector & LEFT_INPUT_MASK ) != LEFT_INPUT_MASK )
      {
        *left_state = INIT;
        P1OUT = ( P1OUT & RIGHT_SIGNAL ) | LEFT_SIGNAL_ALL_MASK;
        return;
      }
      // Check if the time limit in Pattern 2 was reached, if so proceed to pattern 3
      if ( tick_count == *next_left )
      {
        *left_state = PATTERN_3;
        *next_left = tick_count + wait_time;
        P1OUT = ( P1OUT & RIGHT_SIGNAL ) | LEFT_SIGNAL_3_MASK;
      }
      break;
    // Pattern 3 on left signal
    case PATTERN_3 :
      // Check if the left turn signal was turned off
      if ( ( input_vector & LEFT_INPUT_MASK ) != LEFT_INPUT_MASK )
      {
        *left_state = INIT;
        P1OUT = ( P1OUT & RIGHT_SIGNAL ) | LEFT_SIGNAL_ALL_MASK;
        return;
      }
      // Check if the time limit in Pattern 3 was reached, if so proceed to pattern 4
      if ( tick_count == *next_left )
      {
        *left_state = PATTERN_4;
        *next_left = tick_count + wait_time;
        P1OUT = ( P1OUT & RIGHT_SIGNAL ) | LEFT_SIGNAL_4_MASK;
      }
      break;
    // Pattern 4 on left signal
    case PATTERN_4 :
      // Check if the left turn signal was turned off
      if ( ( input_vector & LEFT_INPUT_MASK ) != LEFT_INPUT_MASK )
      {
        *left_state = INIT;
        P1OUT = ( P1OUT & RIGHT_SIGNAL ) | LEFT_SIGNAL_ALL_MASK;
        return;
      }
      break;
    // Default case: reset to Initialization
    default :
      *left_state = INIT;
      P1OUT = ( P1OUT & RIGHT_SIGNAL ) | LEFT_SIGNAL_ALL_MASK;
      break;
  }
  return;
}

/*
 * right_state_machine - this function keeps track of the right turn signal state machine
 * arguments:
 * *right_state - pointer to current state
 * *next_right - pointer to an integer representing the next time for a state change
 * tick_count - current tick count of the timer
 * wait_time - amount of time to wait (in ticks) between a state change
 * returns:
 * (none)
 */
void right_state_machine( State *right_state, unsigned int *next_right, unsigned int tick_count, unsigned int wait_time )
{
  // Moore type state machine
  switch ( *right_state )
  {
    // Initialization state
    case INIT :
      // Check if the right turn signal was activated
      if ( ( input_vector & RIGHT_INPUT_MASK ) == RIGHT_INPUT_MASK )
      {
        *right_state = PATTERN_1;
        *next_right = tick_count + wait_time;
        P1OUT = ( P1OUT & LEFT_SIGNAL ) | RIGHT_SIGNAL_1_MASK;
        // If the timer hasn't been started yet, start the timer
        if ( TACTL == MC_0 )
        {
          TAR = 0;
          CCR0 = TIMER_RESOLUTION_TICKS;
          TACTL = TASSEL_2 + ID_3 + MC_1;
        }
      }
      break;
    // Pattern 1 on right signal
    case PATTERN_1 :
      // Check if the right turn signal was turned off
      if ( ( input_vector & RIGHT_INPUT_MASK ) != RIGHT_INPUT_MASK )
      {
        *right_state = INIT;
        P1OUT = ( P1OUT & LEFT_SIGNAL ) | RIGHT_SIGNAL_ALL_MASK;
        return;
      }
      // Check if the time limit in Pattern 1 was reached, if so proceed to pattern 2
      if ( tick_count == *next_right )
      {
        *right_state = PATTERN_2;
        *next_right = tick_count + wait_time;
        P1OUT = ( P1OUT & LEFT_SIGNAL ) | RIGHT_SIGNAL_2_MASK;
      }
      break;
    // Pattern 2 on right signal
    case PATTERN_2 :
      // Check if the right turn signal was turned off
      if ( ( input_vector & RIGHT_INPUT_MASK ) != RIGHT_INPUT_MASK )
      {
        *right_state = INIT;
        P1OUT = ( P1OUT & LEFT_SIGNAL ) | RIGHT_SIGNAL_ALL_MASK;
        return;
      }
      // Check if the time limit in Pattern 2 was reached, if so proceed to pattern 3
      if ( tick_count == *next_right )
      {
        *right_state = PATTERN_3;
        *next_right = tick_count + wait_time;
        P1OUT = ( P1OUT & LEFT_SIGNAL ) | RIGHT_SIGNAL_3_MASK;
      }
      break;
    // Pattern 3 on right signal
    case PATTERN_3 :
      // Check if the right turn signal was turned off
      if ( ( input_vector & RIGHT_INPUT_MASK ) != RIGHT_INPUT_MASK )
      {
        *right_state = INIT;
        P1OUT = ( P1OUT & LEFT_SIGNAL ) | RIGHT_SIGNAL_ALL_MASK;
        return;
      }
      // Check if the time limit in Pattern 2 was reached, if so proceed to pattern 4
      if ( tick_count == *next_right )
      {
        *right_state = PATTERN_4;
        *next_right = tick_count + wait_time;
        P1OUT = ( P1OUT & LEFT_SIGNAL ) | RIGHT_SIGNAL_4_MASK;
      }
      break;
    // Pattern 4 on right signal
    case PATTERN_4 :
      // Check if the right turn signal was turned off
      if ( ( input_vector & RIGHT_INPUT_MASK ) != RIGHT_INPUT_MASK )
      {
        *right_state = INIT;
        P1OUT = ( P1OUT & LEFT_SIGNAL ) | RIGHT_SIGNAL_ALL_MASK;
        return;
      }
      break;
    // Default case: reset to Initialization
    default :
      *right_state = INIT;
      P1OUT = ( P1OUT & LEFT_SIGNAL ) | RIGHT_SIGNAL_ALL_MASK;
      break;
  }
  return;
}


/*
 * main - code entry point
 * arguments:
 * (none)
 * returns:
 * (none)
 */
void main( void )
{
  WDTCTL = WDTPW | WDTHOLD;
  P1REN = 0x00;
  P1SEL = 0x00;
  P1DIR = 0x3F;
  P1OUT = 0x00;
  P1IES = 0x00;
  P1IFG = 0x00;
  P1IE = 0xC0;
  TACTL = MC_0;
  CCTL0 |= CCIE;
  input_vector = 0x00;
  State left_state = INIT;
  State right_state = INIT;
  unsigned int next_left = 0;
  unsigned int next_right = 0;
  unsigned int wait_time = 200;
  WDTCTL = WDTPW | WDT_MRST_8;
  __enable_interrupt();
  // Calculate Next State -> Set Outputs -> Sleep
  while ( 1 )
  {
    WDTCTL = WDTPW | WDTCNTCL | WDT_MRST_8;
    left_state_machine( &left_state, &next_left, tick_count, wait_time);
    right_state_machine( &right_state, &next_right, tick_count, wait_time);
    __bis_SR_register( LPM0_bits + GIE );
  }
}
