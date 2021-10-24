/*	Author: lab
 *  Partner(s) Name: Charles Hong
 *	Lab Section: 022
 *	Assignment: Lab #6  Exercise #1
 *	Exercise Description: 1 2 3 1 2 3 1 2 3
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  
 *  Demo Link: https://drive.google.com/file/d/15eI90OvEeHgiREDOufNQESF-pZUqVSyC/view?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0; //TimerISR sets it to 1, programmer sets it to 0
unsigned long _avr_timer_M = 0; //start count from here, down to 0. Default 1ms
unsigned long _avr_timer_cntcurr = 0; //current internal count of 1ms ticks

void TimerOn() {
    TCCR1B = 0x0B;
    OCR1A = 125;
    TIMSK1 = 0x02;
    TCNT1  = 0;
    _avr_timer_cntcurr = _avr_timer_M;
    SREG |= 0x80;
}

void TimerOff() {
    TCCR1B = 0x00;
}

void TimerISR() {
    TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
    _avr_timer_cntcurr--;
    if(_avr_timer_cntcurr == 0) {
        TimerISR();
        _avr_timer_cntcurr = _avr_timer_M;
    }
}

void TimerSet(unsigned long M) {
    _avr_timer_M = M;
    _avr_timer_cntcurr = _avr_timer_M;
}

enum State {start, s1, s2, s3} state;
unsigned char tmpB = 0x00;

void Tick() {
    switch(state) {
        case start:
        state = s1;
        break;
        case s1:
        state = s2;
        break;
        case s2:
        state = s3;
        break;
        case s3:
        state = s1;
        break;
    }
    switch(state) {
        case start:
        break;
        case s1:
        tmpB = 0x01;
        break;
        case s2:
        tmpB = 0x02;
        break;
        case s3:
        tmpB = 0x04;
        break;
    }
}

int main(void) {
    DDRB = 0xFF; PORTB = 0x00;
    TimerSet(1000); //1 second
    TimerOn();
    state = start;
    while(1) {
        Tick();
        PORTB = tmpB;
        while(!TimerFlag);
        TimerFlag = 0;
    }
}
