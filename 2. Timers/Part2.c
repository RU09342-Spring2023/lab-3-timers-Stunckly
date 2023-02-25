/*
 * Part2.c
 *
 *  Created on: Feb 11, 2023
 *      Author: Chris Stanski
 *
 *      This code will need to change the speed of an LED between 3 different speeds by pressing a button.
 */

#include <msp430.h>

#define SLOW 50000
#define MID 25000
#define FAST 12500

int state = FAST;

void gpioInit();
void timerInit();

void main(){

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    timerInit();
    gpioInit();

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    __bis_SR_register(LPM3_bits | GIE);


}


void gpioInit(){

    P2OUT |= BIT3;
    P2IES &= ~BIT3;     //
    P2REN |= BIT3;
    P2IE |= BIT3;

    P6OUT &= ~BIT6;
    P6DIR |= BIT6;      //


}

void timerInit(){

       TB1CCTL0 = CCIE;                          // TBCCR0 interrupt enabled
       TB1CCR0 = state;
       TB1CTL = TBSSEL_1 | MC_2;                 // ACLK, continuous mode



    // @TODO Initialize Timer B1 in Continuous Mode using ACLK as the source CLK with Interrupts turned on

}


/*
 * INTERRUPT ROUTINES
 */

// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{

    P2IFG &= ~BIT3;


    switch (state){
        case (SLOW):state=MID;
        break;

        case(MID):state=FAST;
        break;

        default:
        case(FAST):state=SLOW;
        break;

    }




    // @TODO Remember that when you service the GPIO Interrupt, you need to set the interrupt flag to 0.


    // @TODO When the button is pressed, you can change what the CCR0 Register is for the Timer. You will need to track what speed you should be flashing at.

}


// Timer B1 interrupt service routine
#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void)
{

    P6OUT ^= BIT6;
    TB1CCR0 += state;

    // @TODO You can toggle the LED Pin in this routine and if adjust your count in CCR0.
}
