/*
 * Lab_1_Section_F.c
 *
 * Created: 2022/1/20 下午 07:16:15
 *  Author: user
 */ 


#include <avr/io.h>

void usart_init(void)
{
	UCSR0B = (1<<TXEN0)|(1<<RXEN0);
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
	UBRR0 = 0x67;
}

void usart_send(unsigned char ch)
{
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = ch;
}

void Delay200ms(void)
{
	OCR1AH = 0x06;
	OCR1AL = 0x1A;
	TCCR1B = 0x0D;
	while((TIFR1 & (1<<OCF1A)) == 0);
	TCCR1B = 0;
	TIFR1 &= (1<<OCF1A);
}

int main()
{
	usart_init();

	unsigned char ME1 = 0;															// Record the input capture measurement
	unsigned char ME2 = 0;
	unsigned char ME3 = 0;
	unsigned char C1 = 0;															// Translate from acsii back to decimal
	unsigned char C2 = 0;
	
	while(1)
	{
		Delay200ms();
		while((ME3 == 0))
		{
			while((ME1 == 0))														// while loop is introduced to reduce the error when capturing input
			{
				TCCR1B = (1 <<ICES1) | (1 << CS12) | (0 << CS11) | (0 << CS10);		//	rising edge, prescaler = 256, no noise canceller
				TIFR1 = (1<<ICF1);													//	clear ICF1 (The Input Capture Flag)
				while ((TIFR1&(1<<ICF1)) == 0);										//	wait while ICF1 is clear
				ME1 = ICR1L;														//	first edge value (ICR, low byte)
			}
			
			while((ME2 == 0))
			{
				TIFR1 = (1<<ICF1);													//	clear ICF1
				TCCR1B = (0 <<ICES1) | (1 << CS12) | (0 << CS11) | (0 << CS10);		//	falling edge
				while ((TIFR1&(1<<ICF1)) == 0);										//	wait while ICF1 is clear
				ME2 = ICR1L;														//	pulse width = falling - rising
				TIFR1 = (1<<ICF1);													//	clear ICF1
			}
			
			ME3 = (ME2-ME1+1)/1.26;
		}
		
		C1 = (ME3/10 + 48);
		C2 = (ME3%10 + 48);
		usart_send(C1);
		usart_send(C2);
		
		ME1 = 0;																	// reset parameter for the looping
		ME2 = 0;
		ME3 = 0;
	}
}