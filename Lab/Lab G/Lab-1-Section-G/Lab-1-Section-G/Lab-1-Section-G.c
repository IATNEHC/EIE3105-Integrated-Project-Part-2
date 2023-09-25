/*
 * Lab_1_Section_G.c
 *
 * Created: 2022/1/27 下午 07:58:13
 *  Author: user
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char A1 = 0;
unsigned char A2 = 0;
unsigned int state = 0;

void Timer0init(void)
{
	TCCR0A = (1 << COM0A1) | (0 << COM0A0) | (1 << COM0B1) | (0 << COM0B0) | (1 << WGM01) | (1 << WGM00);			//Timer 0 PWM
	TCCR0B = (1 << WGM02) | (1 << CS02) | (0 << CS01) | (0 << CS00);
	OCR0A = 124;
}

void Timer1init(void)
{
	TCCR1A = (1 << COM1A1) | (0 << COM1A0) | (1 << COM1B1) | (0 << COM1B0) | (1 << WGM11) | (1 << WGM10);			//Timer 1 PWM
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (0 << CS12) | (1 << CS11) | (1 << CS10);
	OCR1A = 0x01F3;
}

void Timer2init(void)
{
	TCCR2A = (1 << COM2A1) | (0 << COM2A0) | (1 << COM2B1) | (0 << COM2B0) | (1 << WGM21) | (1 << WGM20);			//Timer 2 PWM
	TCCR2B = (1 << WGM22) | (1 << CS22) | (1 << CS21) | (0 << CS20);
	OCR2A = 124;
}

int main(void)
{	
	DDRC = 0;
	DDRD = 0xFF;
	DDRB = 0xFF;
	sei();
	
	Timer0init();
	Timer1init();
	Timer2init();
	
	ADCSRA = 0x8F;																							
	ADMUX = 0xC0;																									//ADC for Timer 0 LED
	
	OCR0B = 0;
	OCR1B = 0;
	OCR2B = 0;
	
	ADCSRA |= (1<<ADSC);																							//Start
    while(1);
	return 0;
}

ISR(ADC_vect)
{
	state++;
	A1 = ADCL;																										//Need to read both register
	A2 = ADCH;
	
	switch(state)
	{
		case 1:
			if(A1 > 250)																							//Adjust the duty cycle based on the ADC input
			{
				OCR0B = 124;
				A1 = 0;
				A2 = 0;
				ADMUX = 0xC1;																						//ADC for Timer 1 LED
			}
			else
			{
				OCR0B = 0;
				A1 = 0;
				A2 = 0;
				ADMUX = 0xC1;
			}
			break;
		
		case 2:
			if(A1 > 250)																							//Adjust the duty cycle based on the ADC input
			{
				OCR1B = 0x01F3;
				A1 = 0;
				A2 = 0;
				ADMUX = 0xC2;																						//ADC for Timer 2 LED
			}
			else
			{
				OCR1B = 0;
				A1 = 0;
				A2 = 0;
				ADMUX = 0xC2;
			}
			break;
			
		case 3:
			if(A1 > 250)																							//Adjust the duty cycle based on the ADC input
			{
				OCR2B = 124;
				A1 = 0;
				A2 = 0;
				ADMUX = 0xC0;
				state = 0;																							//ADC for Timer 0 LED
			}
			else
			{
				OCR2B = 0;
				A1 = 0;
				A2 = 0;
				ADMUX = 0xC0;
				state = 0;
			}
			break;
	}
	
	ADCSRA |= (1<<ADSC);
}
