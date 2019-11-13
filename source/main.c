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
#include "io.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
//#define A0 PINA & 0x01

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

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


enum states {wait, press0, press1, both} state;
unsigned char tmpB = 0x07;
void tick() {
	switch(state) {
		
		case wait:
		{
			if ((PINA & 0x03) == 0x00) {
				LCD_WriteData( tmpB + '0');
                                state = wait;
                                break;
                        }
			else if ((PINA & 0x03) == 0x03) {
                                state = both;
                                tmpB = 0;
                                LCD_WriteData( tmpB + '0');
                                break;
                        }
                        else if ((PINA & 0x03) == 0x01) {
                                state = press0;
				if (tmpB < 9) {
					tmpB = tmpB + 1;
				}
                                LCD_WriteData( tmpB + '0');
                                break;
                        }
			else if ((PINA & 0x03) == 0x02) {
                                state = press1;
                                if (tmpB > 0) {
                                        tmpB = tmpB - 1;
                                }
                                LCD_WriteData( tmpB + '0');
                                break;
                        }
		}
		case press0:
		{		
			if ((PINA & 0x03) == 0x00) {
                                state = wait;
                                break;
                        }
                        else if ((PINA & 0x03) == 0x03) {
                                state = both;
                                tmpB = 0;
                                LCD_WriteData( tmpB + '0');
                                break;
                        }
                        else if ((PINA & 0x03) == 0x01) {
				state = press0;
                                if (tmpB < 9) {
                                        tmpB = tmpB + 1;
                                }
                                LCD_WriteData( tmpB + '0');
                                break;
                        }
                        else if ((PINA & 0x03) == 0x02) {
                                state = press1;
                                if (tmpB > 0) {
                                        tmpB = tmpB - 1;
                                }
                                LCD_WriteData( tmpB + '0');
                                break;
                        }	
		}
		case press1:
		{	
		        if ((PINA & 0x03) == 0x00) {
                                LCD_WriteData( tmpB + '0');
				state = wait;
                                break;
                        }
                        else if ((PINA & 0x03) == 0x03) {
                                state = both;
                                tmpB = 0;
                                LCD_WriteData( tmpB + '0');
                                break;
                        }
                        else if ((PINA & 0x03) == 0x01) {
                                state = press0;
                                if (tmpB < 9) {
                                        tmpB = tmpB + 1;
                                }
                                LCD_WriteData( tmpB + '0');
                                break;
                        }
          		else if ((PINA & 0x03) == 0x02) {
				state = press1;
                                if (tmpB > 0) {
                                        tmpB = tmpB - 1;
                                }
                                LCD_WriteData( tmpB + '0');
                                break;
                        }
		}
		case both:
		{	
		        if ((PINA & 0x03) == 0x00) {
				LCD_WriteData( tmpB + '0');
                                state = wait;
                                break;
                        }
                        else {
				LCD_WriteData( tmpB + '0');
                                state = both;
                                break;
                        }
		}	
		default:
		{
			state = press0;
			break;
		}
	}
	switch(state) {
		case wait:
		{
                        break;
		}
                case press0:
		{
                        break;
		}
                
                case press1:
		{
                        break;
		}
                case both:
		{
                        break;
		}
        }
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	state = wait;
	TimerSet(1000);
	TimerOn();
//	unsigned char A0 = 0x00;
	while(1) {
//		A0 = PINA & 0x01;
		tick();
		while (!TimerFlag);
		TimerFlag = 0;
	}
    return 1;
}
