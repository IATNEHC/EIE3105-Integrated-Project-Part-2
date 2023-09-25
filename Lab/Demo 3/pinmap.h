#include "stm32f10x.h"                  // Device header
#include "stdbool.h"
#include "stdio.h"

// Function propotypes for initialize
void SR04_init(void);
void Motor_init(void);
void TIM3_delay_us(unsigned short us);
void Button_LED_init(void);
void USART2_init(void);
void USARTSend(char *pucBuffer, unsigned long ulCount);


// Function propotypes for controlling the motor of cart
void leftforward(unsigned short pwm);
void rightforward(unsigned short pwm);
void leftbackward(unsigned short pwm);
void rightbackward(unsigned short pwm);

