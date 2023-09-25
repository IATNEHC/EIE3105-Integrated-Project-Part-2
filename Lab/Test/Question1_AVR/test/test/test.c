/*
 * test.c
 *
 * Created: 2022/8/26 下午 13:58:13
 * Author: user
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>

void Timer0init(void)
{
	TCCR0A = (1 << COM0A1) | (0 << COM0A0) | (1 << COM0B1) | (0 << COM0B0) | (1 << WGM01) | (1 << WGM00);			//Timer 0 PWM
	TCCR0B = (1 << WGM02) | (1 << CS02) | (0 << CS01) | (0 << CS00);
	OCR0A = 124;
}

void Timer2init(void)
{
	TCCR2A = (1 << COM2A1) | (0 << COM2A0) | (1 << COM2B1) | (0 << COM2B0) | (1 << WGM21) | (1 << WGM20);			//Timer 2 PWM
	TCCR2B = (1 << WGM22) | (1 << CS22) | (1 << CS21) | (0 << CS20);
	OCR2A = 124;
}

void Delay1s(void)
{
	OCR1AH = 0x3D;
	OCR1AL = 0x08;
	TCCR1B = 0x0D;
	while((TIFR1 & (1<<OCF1A)) == 0);
	TCCR1B = 0;
	TIFR1 &= (1<<OCF1A);
}

int main(void)
{	
	DDRC = 0;
	DDRD = 0xFF;
	DDRB = 0xFF;
	
	Timer0init();
	Timer2init();
	
	OCR1B = 0;			//25 for 20%
	OCR2B = 99;			//99 for 80%
	
    while(1)
	{
		Delay1s();
		OCR0B = 25;		
		OCR2B = 0;	
		Delay1s();
		OCR0B = 0;
		OCR2B = 99;	
	}
	
	return 0;
}

