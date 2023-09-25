/*
 * Debouncing01_polling.c
 *
 * Created: 1/12/2021 4:58:02 PM
 * Author : encccl
 */ 
 #include <avr/io.h>

 // debouncing01_polling.c
 // wait 20 ms after processing

 void delay_1s()
 {
	 TCCR1A = 0b00000000; // CTC TOP = OCR1A
	 TCCR1B = 0b00001011; // Prescaler = 64
	 OCR1A = 12500; // 1 / (16 MHz / 64) = 0.004 ms,  50ms / 0.004 ms = 12500
	 
	 unsigned char count = 0;
	 while (count < 20) // 20 x 50 ms = 1 s
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
			 delay_1s(); // remove the debouncing effect
		 }
	 }
 }

