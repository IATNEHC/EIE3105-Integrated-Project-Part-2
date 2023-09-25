/*
 * Lab_1_Section_E.c
 *
 * Created: 2022/1/20 下午 06:52:51
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
	
	unsigned char ch1 = 0;															// Input from Tera Team
	unsigned char ch2 = 0;
	unsigned char ch3 = 0;
	unsigned char DC = 0;
	
	PORTB = 0xFF;
	TCCR1A = 0;																		//	Timer Mode = Normal
	TCCR0A = (1 << COM0A1) | (0 << COM0A0) | (1 << COM0B1) | (0 << COM0B0) | (1 << WGM01) | (1 << WGM00);
	TCCR0B = (1 << WGM02) | (1 << CS02) | (0 << CS01) | (0 << CS00);
	OCR0A = 124;																	// 500Hz
	
	while(1)
	{
		while((ch3 < 10) | (ch3 > 99))
		{
			Delay200ms();
			if((UCSR0A & (1<<RXC0)))
			{
				ch1 = UDR0;
				usart_send(ch1);
				while(1)
				{
					while(!(UCSR0A & (1<<RXC0)));
					ch2 = UDR0;
					break;
				}
				usart_send(ch2);
			}
			ch3 = ((ch1-48)*10 + (ch2-48));
		}
		
		DC = (ch3*1.26 - 1);
		OCR0B = DC;
		DDRD = 0x20;																// PD5 (OC0B)
		
		ch1 = 0;																	// reset parameter for the looping
		ch2 = 0;
		ch3 = 0;
	}
}