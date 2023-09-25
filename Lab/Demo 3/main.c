#include "pinmap.h"

char buffer[50] = {'\0'};
__IO bool pulseHigh = false;
__IO bool generate_pulse = true;
__IO u32 pulseWidth = 0;
__IO u32 captureValue = 0;
__IO u32 distance = 10;
__IO u32 stopdistance = 0;
__IO bool buttonPress = false;
bool firstReading = true;
bool secondReading = false;

int main(void)
{
	Motor_init();
	USART2_init();
	Button_LED_init();	
	TIM3_delay_us(10);
		
	char state = 0;

	while(!buttonPress)
	{
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) == 0)
		{
			buttonPress = true;
		}
	}
	rightforward(1900);
	leftforward(1700);
	
	SR04_init();
	// config TIM1 CH3N I/O (PB15) BUT do not enable TIM1 CH3N
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef init_gpio;
	init_gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	init_gpio.GPIO_Pin = GPIO_Pin_15;
	init_gpio.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &init_gpio);
	
	while(1)
	{
		if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) 
		{
			if(state == 0)  
			{
				GPIO_SetBits(GPIOB, GPIO_Pin_15);
				state = 1;
			} 
			else 
			{
				GPIO_ResetBits(GPIOB, GPIO_Pin_15);
				state = 0;
			}
			
			if(stopdistance > distance)
			{
				Motor_init();
				rightforward(1);
				leftforward(1);
				GPIO_ResetBits(GPIOA, GPIO_Pin_0);
			}
		
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);	
		}
	}
}

// Get the measurement from the ultrasonic sensor
void TIM1_CC_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1, TIM_IT_CC3) != RESET)
	{
		if(!pulseHigh)
		{
			pulseHigh = true;   //pulse starts
			TIM_SetCounter(TIM1, 0);
			//change to detect the falling edge
			TIM_OC3PolarityConfig(TIM1, TIM_ICPolarity_Falling);
		}
		else
		{
			pulseWidth += TIM_GetCounter(TIM1);
			captureValue = pulseWidth;
			// output an integer only not a floating point number
			// very similar to captureValue/2*330/1000000
			distance = (captureValue/58); // in cm
			/*sprintf(buffer, "%d\r\n", distance);
			USARTSend(buffer, sizeof(buffer));*/
			
			if(firstReading)
			{
				if(secondReading)
				{
					if(distance < 40)
					{
						stopdistance = 18;
					}
					else if(distance < 60)
					{
						stopdistance = 26;
					}
					else
					{
						stopdistance = 30;
					}
					firstReading = false;
					secondReading = false;
				}
				secondReading = true;
			}
			//change to detect the rising edge
			TIM_OC3PolarityConfig(TIM1, TIM_ICPolarity_Rising);
			pulseHigh = false;
			pulseWidth = 0;
		}
	}
	//clear interrupt flag
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update|TIM_IT_CC3);
}
