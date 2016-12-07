#include "m_general.h"
#include "avr/interrupt.h"
#include "m_bus.h"
#include "m_rf.h"
#include "m_usb.h"
#include "puckfind.h"
#include "vals.h"

#define NUM_PTS 8

volatile int ADC_Flag = 0;
enum PT { TopRight = 4, Right = 6, Back = 3, Left = 2, TopLeft = 0, InnerLeft = 5, InnerRight = 1, Down = 7 };
int PTs [8] = { 0, 0, 0, 0, 0, 0, 0, 0 }; //PT ADC values
int ADC_Check = 0; //incr through pt channels

int ptNoise = 0; // ambient noise
int maxPTval = 1023; // maximum PT reading after removing ambient

int hasPuckThreshold = 1000;
int noiseThreshold = 10;
int closeThreshold = 400;

void printValues(void);
void puck_findAngle(void);
void normalizePTs(void);
double approxAngle(double range, int val1, int val2);

void printValues(void) {
	m_usb_tx_string("\nTR: "); m_usb_tx_int(PTs[TopRight]);
	m_usb_tx_string("\tR: ");  m_usb_tx_int(PTs[Right]);
	m_usb_tx_string("\tB: ");  m_usb_tx_int(PTs[Back]);
	m_usb_tx_string("\tL: ");  m_usb_tx_int(PTs[Left]);
	m_usb_tx_string("\tTL: "); m_usb_tx_int(PTs[TopLeft]);
	m_usb_tx_string("\tIL: "); m_usb_tx_int(PTs[InnerLeft]);
	m_usb_tx_string("\tIR: "); m_usb_tx_int(PTs[InnerRight]);
	m_usb_tx_string("\tD: ");  m_usb_tx_int(PTs[Down]);
	m_usb_tx_string("\tAmbient: "); m_usb_tx_int(ptNoise);
}

// returns whether values were found
int puck_getADCValues(void) {
	if (ADC_Flag != 0) {  //If ADCs are being read
		clear(ADCSRA,ADEN); // Disable ADC
		setDriveToPuck();
		if (time % 10 == 0) { printValues(); } // COMMENT THIS LINE IN FINAL VERSION }
		ADC_Flag = 0;
		set(ADCSRA,ADEN); // Re-enable ADC
		set(ADCSRA,ADSC); // Start next conversion
		return 1;
	}
	return 0;
}

void setDriveToPuck(void) {
	normalizePTs();
	int i, maxPT1, maxPT2; maxPT1 = 0; maxPT2 = 1; 
	for (i = 1; i < NUM_PTS - 1; i++) { 
		if (PTs[i] > PTs[maxPT1]) { maxPT2 = maxPT1; maxPT1 = i; }
		else if (PTs[i] > PTs[maxPT2]) { maxPT2 = i; }
	}
	if (PTs[Down] => hasPuckThreshold) { // has the puck
		set(PORTB, 0); gameState = GO_TO_GOAL; 
	} else if (maxPT1 <= noiseThreshold) { // noise
		clear(PORTB, 0); gameState = PATROL;
	} else if ((maxPT1 == TopLeft && maxPT2 == TopRight) {
		left_slow();
	} else if (maxPT1 == TopRight && maxPT2 == TopLeft)) {
		right_slow();
	} else {
		switch(maxPT1) {
			case Back: if (PTs[Back] < closeThreshold) { right(); } else { right_ip(); } break;
			case Right: if (PTs[Right] < closeThreshold) { right(); } else { right_ip(); } break;
			case Left: if (PTs[Left] < closeThreshold) { left(); } else { left_ip(); } break;
			case TopLeft: left_slow(); break;
			case TopRight: right_slow(); break;
			default: fwd_slow(); break;
		}
	}
}

// normalizes PTs by 
void normalizePTs(void) {
	int i;
	for (i = 0; i < NUM_PTS; i++) { // SUBTRACT 1 from NUM_PTS IN REAL VERSION
		if (PTs[i] - ptNoise < 0) { PTs[i] = 0; }
		else { PTs[i] = PTs[i] - ptNoise; }
	}
}

void setAmbient(void) {
	while (!puck_getADCValues());
	ptNoise = PTs[Back] < PTs[Left]  ?  PTs[Back] : PTs[Left];
	ptNoise = ptNoise   < PTs[Right] ?  ptNoise	  : PTs[Right];
	maxPTval = 1023 - ptNoise;
	MAX_THRESHOLD = maxPTval - 10;
}

ISR(ADC_vect){ //Call Interrupt when conversion completes
	clear(ADCSRA,ADEN); //Disable ADC subsystem
	PTs[ADC_Check] = (unsigned int) ADC; //Assign current ADC value to channel in loop
	if (ADC_Check == NUM_PTS) {   //If all channels read, reset loop throuch ADC channels
		ADC_Check = 0; 
		ADC_Flag = 1; 
	}
	else ADC_Check ++;

	switch(ADC_Check){ //Switch case to iterate through phototransistors
		case 0: // Set ADC to F0
		clear(ADCSRB,MUX5);
		clear(ADMUX,MUX2);
		clear(ADMUX,MUX1);
		clear(ADMUX,MUX0);
		break;
		case 1: // Set ADC to D7
		set(ADCSRB,MUX5);
		clear(ADMUX,MUX2);
		set(ADMUX,MUX1);
		clear(ADMUX,MUX0);
		break;
		case 2: // Set ADC to F4
		clear(ADCSRB,MUX5);
		set(ADMUX,MUX2);
		clear(ADMUX,MUX1);
		clear(ADMUX,MUX0);
		break;
		case 3: // Set ADC to F5
		clear(ADCSRB,MUX5);
		set(ADMUX,MUX2);
		clear(ADMUX,MUX1);
		set(ADMUX,MUX0);
		break;
		case 4: // Set ADC to F6
		clear(ADCSRB,MUX5);
		set(ADMUX,MUX2);
		set(ADMUX,MUX1);
		clear(ADMUX,MUX0);
		break;
		case 5: // Set ADC to F7
		clear(ADCSRB,MUX5);
		set(ADMUX,MUX2);
		set(ADMUX,MUX1);
		set(ADMUX,MUX0);
		break;
		case 6: // Set ADC to D4
		set(ADCSRB,MUX5);
		clear(ADMUX,MUX2);
		clear(ADMUX,MUX1);
		clear(ADMUX,MUX0);
		break;
		case 7: // Set ADC to D6
		set(ADCSRB,MUX5);
		clear(ADMUX,MUX2);
		clear(ADMUX,MUX1);
		set(ADMUX,MUX0);
		break;
		default:
		break;
	}
	set(ADCSRA,ADEN); // enable ADC again
	set(ADCSRA,ADSC); //start next conversion
}