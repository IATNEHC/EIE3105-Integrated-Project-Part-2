/*
 * Debouncing02_polling.c
 *
 * Created: 1/12/2021 5:05:08 PM
 * Author : encccl
 */ 

#include <avr/io.h>

// debouncing02_polling.c
// wait 20 ms and check again, not good

void delay_200ms()
{
	TCCR1A = 0b00000000; // CTC TOP = OCR1A
	TCCR1B = 0b00001011; // Prescaler = 64
	OCR1A = 12500; // 1 / (16 MHz / 64) = 0.004 ms,  50ms / 0.004 ms = 12500
	
	unsigned char count = 0;
	while (count < 4) // 4 x 50 ms = 200 ms
	{
		while(!(TIFR1 & (1<<OCF1A))) ;
		TIFR1 = (1<<OCF1A);
		count++;
	}
}

int main(void)
{
	DDRB = 0x00;
	DDRD = 0xFF;
	
	unsigned char state = 0;
	
	while(1)
	{
		//TODO:: Please write your application code

		// Timer 1 setting
		if(PINB & (1 << 0)) // PB0 is pressed
		{
			delay_200ms(); // remove the debouncing effect
			if(PINB & (1 << 0))
			{
				if(state == 0)
				{
					state = 1;
					PORTD = 0xFF; // any PDx is ON
				}
				else
				{
					state = 0;
					PORTD = 0x00; // any PDx is OFF
				}

			}
		}
	}
}
