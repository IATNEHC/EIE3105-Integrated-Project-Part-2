/*
 * Debouncing03_int.c
 *
 * Created: 1/12/2021 5:08:45 PM
 * Author : encccl
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

// debouncing03_int.c
// wait 20 ms after processing

unsigned char status = 0;
unsigned char state = 0;
unsigned char count = 0;

int main(void)
{
	DDRB = 0x00;
	DDRD = 0xF8;

	EIMSK = 0b00000001; //INT0 enable
	EICRA = 0b00000010; //INT0 falling edage interrupt ISC01:ISC00 1:0
	
	sei();
	
	
	while(1)
	{
		//TODO:: Please write your application code
	}
}

ISR(TIMER1_COMPA_vect)
{
	count++;
	if (count == 4) // 4 x 50 ms = 200 ms
	{
		if(state == 0)
		{
			state = 1;
			PORTD = 0xF8; // any PDx is ON
		}
		else
		{
			state = 0;
			PORTD = 0x00; // any PDx is OFF
		}
		count = 0;
		status = 0;

		// disable Timer 1
		TCCR1B = 0b00000000;
	}
}

ISR(INT0_vect)
{
	if (status == 0) // first time
	{
		status = 1;

		// enable Timer 1
		TCCR1A = 0b00000000; // CTC TOP = OCR1A
		TCCR1B = 0b00001011; // Prescaler = 64
		OCR1A = 12500; // 1 / (16 MHz / 64) = 0.004 ms,  50ms / 0.004 ms = 12500
		TIMSK1 = 0b00000010; //OCIE1A
	}
	if (status == 1); // not the first time, do nothing
}



