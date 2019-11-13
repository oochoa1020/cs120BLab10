/*	Author: oocho002
 *  Partner(s) Name: Gurparam
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
unsigned char threeLEDs = 0x01;
unsigned char blinkingLED = 0x01;

void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;

	_avr_timer_cntcurr = _avr_timer_M;

	SREG |= 0x080;
}

void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}


enum states {light0, light1, light2} state1;
enum states2 {on, off} state2;
enum states3 {combine} state3;
void ThreeLEDsSM() {
	switch(state1) {
		
		case light0:
		{
			state1 = light1;
			break;
		}
		case light1:
		{		
			state1 = light2;
                        break;
		}
		case light2:
		{
			state1 = light0;
                        break;
		}
		default:
		{
			state1 = light0;
			break;
		}
	}
	switch(state1) {
		case light0:
		{
			threeLEDs = 0x01;
                        break;
		}
        case light1:
		{
			threeLEDs = 0x02;
                        break;
		}
                
		case light2:
		{
			threeLEDs = 0x04;
                        break;
		}
	}
}
void BlinkingLEDSM() {
	switch(state2) {
		
		case off:
		{
			state2 = on;
			break;
		}
		case on:
		{
			state2 = off;
			break;
		}
		default:
		{
			state2 = off;
			break;
		}
	}
	switch(state2) {
		case off:
		{
			blinkingLED = 0x00;
			break;
		}
		case on:
		{
			blinkingLED = 0x08;
			break;
		}
	}
}
void CombineLEDsSM() {
	switch(state3) {
		
		case combine:
		{
			state3 = combine;
			break;
		}
		default:
		{
			state3 = combine;
			break;
		}
	}
	switch(state3) {
		case combine:
		{
			PORTB = (threeLEDs | blinkingLED);
			break;
		}
	}
}
int main(void) {
//	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	state1 = light0;
	state2 = off;
	state3 = combine;
	TimerSet(1000);
	TimerOn();
	while(1) {
		ThreeLEDsSM();
		BlinkingLEDSM();
		CombineLEDsSM();
		while (!TimerFlag);
		TimerFlag = 0;
	}
    return 1;
}