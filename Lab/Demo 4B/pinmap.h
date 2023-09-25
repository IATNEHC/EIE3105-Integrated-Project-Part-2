#include "stm32f10x.h"                  // Device header
#include "stdbool.h"
#include "stdio.h"

// Function propotypes for initialize
void TIM1_init(void);
void TIM2_init(void);
void TIM1_init_with_SR04(void);
void TIM3_delay_us(unsigned short us);
void TIM3_delay_ms(unsigned short ms);
void Button_LED_init(void);
void USART3_init(void);
void SPI1_init(void);

// Function propotype for sending data
void USARTSend(USART_TypeDef *USARTx, char *pucBuffer, unsigned long ulCount);

// Function propotypes for controlling the motor of cart
void leftforward(unsigned short pwm);
void rightforward(unsigned short pwm);
void leftbackward(unsigned short pwm);
void rightbackward(unsigned short pwm);
void OuterAntiClockwise(char reading);
void OuterClockwise(char reading);

// Function propotypes for reading the floor
void readFloor(void);
