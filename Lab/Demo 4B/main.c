#include "pinmap.h"

char buffer[50] = {'\0'};
__IO bool isModePWM = true;
__IO bool pulseHigh = false;
__IO bool generate_pulse = true;
__IO u32 pulseWidth = 0;
__IO u32 captureValue = 0;
__IO u32 distance = 100;
__IO bool buttonPress = false;
bool isPathSensorReady = false;
short countPathSensor = 0;
char cart = 0;
unsigned short left = 1;
unsigned short right = 1;
unsigned int CrossCounter = 0;
unsigned int AntiCounter = 0;
unsigned int DistanceCounter = 0;
unsigned int DelayCounterAcross= 0;
unsigned int DelayCounterWait = 0;
bool CrossCounterReady = true;
bool AntiCounterReady = true;
bool AntiClockwise = true;
bool turnLeft = true;
unsigned int mode = 1;

void OuterAnticlockwise(char reading)
{
	switch(reading)
	{
		case 24: { TIM3_delay_us(20); left = 50; right = 135; leftforward(left); rightforward(right); break; } 														//00011000
		case 16: { TIM3_delay_us(20); left = 40; right = 145; leftforward(left); rightforward(right); turnLeft = true; break; } 					//00010000
		case 8: { TIM3_delay_us(20); left = 90; right = 120; leftforward(left); rightforward(right); turnLeft = false; break; } 					//00001000
		case 0: { TIM3_delay_us(20);																																																			//00000000
							if(turnLeft)
							{
								left = 100; right = 120; leftbackward(left); rightforward(right);
							}
							else
							{
								left = 100; right = 120; leftforward(left); rightbackward(right);
							}
							break;
						} 																																																													
	}
}

void OuterClockwise(char reading)
{
	switch(reading)
	{
		case 24: { TIM3_delay_us(20); left = 3000; right = 2400; leftforward(left); rightforward(right); break; } 														//00011000
		case 16: { TIM3_delay_us(20); left = 3400; right = 2400; leftforward(left); rightforward(right); turnLeft = true; break; } 						//00010000
		case 8: { TIM3_delay_us(20); left = 2400; right = 4300; leftforward(left); rightforward(right); turnLeft = false; break; } 						//00001000
		case 0: { TIM3_delay_us(20);																																																					//00000000
							if(turnLeft)
							{
								left = 1; right = 3400; leftbackward(left); rightforward(right);
							}
							else
							{
								left = 3400; right = 1; leftforward(left); rightbackward(right);
							}
							break;
						} 																																																													
	}
}

void GoAcross(void)
{
	left = 100; right = 120; leftforward(left); rightforward(right);
	DelayCounterAcross = DelayCounterAcross + 1;
}

void Wait(void)
{
	left=1; right=1; leftforward(left); rightbackward(right);
	DelayCounterWait = DelayCounterWait + 1;
}

int main(void)
{
	// Initialize USART3, Button and LED
	USART3_init();
	Button_LED_init();
	
	while(!buttonPress)
	{
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) == 0)
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_0);
			buttonPress = true;
		}
	}
	
	TIM1_init();
	TIM2_init();
	SPI1_init();
	
	while(1);
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		readFloor();
		
		if(CrossCounter < 2)
		{
			//reset CrossCounterReady
			if(!((~(~cart | 0x7F)) & 128) && !((~(~cart | 0xFE)) & 1))
			{
				CrossCounterReady = true;
			}
			//update CrossCounter
			if(((~(~cart | 0x7F)) & 128) && !((~(~cart | 0xFE)) & 1))
			{
				if(CrossCounterReady)
				{
					CrossCounter = CrossCounter + 1;
					CrossCounterReady = false;
				}
			}
			//cart movement
			OuterAnticlockwise(~(~cart | 0x81));
		}
		else if(CrossCounter == 2)
		{	
			GPIO_SetBits(GPIOA, GPIO_Pin_0);
			//Run GoAcross() until DelayCounterAcross >= 1400
			if(DelayCounterAcross < 1400)
			{
				GoAcross();
			}
			else
			{
				//changing into clockwise parameter and initialize SR04 pins
				if(AntiClockwise)
				{
					turnLeft = false;
					TIM1_init_with_SR04();
					AntiClockwise = false;
				}
				
				switch(mode)
				{
					case 0:		/*OuterClockwise(~(~cart | 0x81));*/
										if(distance < 20)
										{
											DistanceCounter =  DistanceCounter + 1;
											if(DistanceCounter == 3)
											{
												mode = 1;
											}
										}
										break;
					case 1: 	if(DelayCounterWait < 2000)
										{
											Wait();
											break;
										}
										mode = 2;
										break;
					/*case 2:		OuterClockwise(~(~cart | 0x81));
										break;*/
				}
			}
		}
		else //CrossCounter > 2
		{
			
		}
	}
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}

// Get the value from the path sensor
void SPI1_IRQHandler(void)
{
	char state = ~(~((char) SPI_I2S_ReceiveData(SPI1) & 0xff) | 0x66);
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_7) == 1)
	{
		if (countPathSensor != 0)
		{
			SPI_I2S_SendData(SPI1, 0xff);
			countPathSensor--;
		}
		else
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_7);
			SPI_I2S_SendData(SPI1, 0);
			isPathSensorReady = true;
		}
	}
	else
	{
		if (countPathSensor == 0 && isPathSensorReady)
		{
			isPathSensorReady = false;
			cart = state;
			sprintf(buffer, "SPI:%d\r\n", state);
			//USARTSend(USART3, buffer, sizeof(buffer));
		}
		NVIC_DisableIRQ(SPI1_IRQn);
	}
}

void TIM1_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
	{
		isModePWM = !isModePWM;
		if (!isModePWM)
		{
			// Send trigger pulse to SR04 module
			GPIO_SetBits(GPIOB, GPIO_Pin_15);
			TIM3_delay_us(10);
			GPIO_ResetBits(GPIOB, GPIO_Pin_15);
		}
	}
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
}

// Get the measurement from the ultrasonic sensor
void TIM1_CC_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1, TIM_IT_CC3) != RESET)
	{
		if (!pulseHigh)
		{
			pulseWidth = 0;
			pulseHigh = true;
			TIM_SetCounter(TIM1, 0);
			// Change to detect the falling edge
			TIM_OC3PolarityConfig(TIM1, TIM_ICPolarity_Falling);
		}
		else
		{
			pulseWidth = TIM_GetCounter(TIM1);
			distance = pulseWidth / 58;
			sprintf(buffer, "%dcm\r\n", distance);
			USARTSend(USART3, buffer, sizeof(buffer));
			
			// Change to detect the rising edge
			pulseHigh = false;
			TIM_OC3PolarityConfig(TIM1, TIM_ICPolarity_Rising);
		}
	}
	// Clear interrupt flag
	TIM_ClearITPendingBit(TIM1, TIM_IT_CC3);
}