/*
 * test_Q2.c
 *
 * Created: 2022/8/26 下午 02:59:26
 *  Author: user
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char A1 = 0;
unsigned char A2 = 0;

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
	sei();
	usart_init();
	
	ADCSRA = 0x8F;
	ADMUX = 0xC0;																									//ADC reading A0
	
	ADCSRA |= (1<<ADSC);																							//Start
	
	while(1);
	return 0;
}

ISR(ADC_vect)
{	
	
	A1 = ADCL;																										//Need to read both register
	A2 = ADCH;
	
	if(A1 > 250)
	{
		usart_send('B');
		usart_send('b');
		usart_send('B');
		usart_send('b');
		Delay1s();	
	}
	else
	{
		usart_send('Y');
		usart_send('y');
		usart_send('Y');
		usart_send('y');
		Delay1s();
	}
	
	ADCSRA |= (1<<ADSC);
}