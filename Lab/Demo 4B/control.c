#include "pinmap.h"

// Control left motor in forward direction
void leftforward(unsigned short pwm)
{
	TIM_OCInitTypeDef OC_InitStructure;
	OC_InitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	OC_InitStructure.TIM_Pulse = pwm - 1;
	OC_InitStructure.TIM_OutputState = TIM_OutputState_Enable;
	OC_InitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	OC_InitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  
	TIM_OC1Init(TIM1, &OC_InitStructure);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

// Control right motor in forward direction
void rightforward(unsigned short pwm)
{
	TIM_OCInitTypeDef OC_InitStructure;
	OC_InitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	OC_InitStructure.TIM_Pulse = pwm - 1;
	OC_InitStructure.TIM_OutputState = TIM_OutputState_Disable;
	OC_InitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	OC_InitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  
	TIM_OC2Init(TIM1, &OC_InitStructure);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

// Control left motor in backward direction
void leftbackward(unsigned short pwm)
{
	TIM_OCInitTypeDef OC_InitStructure;
	OC_InitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	OC_InitStructure.TIM_Pulse = pwm - 1;
	OC_InitStructure.TIM_OutputState = TIM_OutputState_Disable;
	OC_InitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	OC_InitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	
	TIM_OC1Init(TIM1, &OC_InitStructure);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

// Control right motor in backward direction
void rightbackward(unsigned short pwm)
{
	TIM_OCInitTypeDef OC_InitStructure;
	OC_InitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	OC_InitStructure.TIM_Pulse = pwm - 1;
	OC_InitStructure.TIM_OutputState = TIM_OutputState_Enable;
	OC_InitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	OC_InitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	TIM_OC2Init(TIM1, &OC_InitStructure);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

// Call variable used by SPI1 in main.c
extern short countPathSensor;

// Get the readings of the floor once
void readFloor(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_7);
	SPI_I2S_SendData(SPI1, 0);
	countPathSensor = 4;
	NVIC_EnableIRQ(SPI1_IRQn);
}
