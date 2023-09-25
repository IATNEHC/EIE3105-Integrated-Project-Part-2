#include "stm32f10x.h" 				// Device header
#include "PinMap.h"
#include "stdbool.h"
#include "stdio.h"

static char state = 0;
static char character;
static char ch1 = 0;
static char ch2 = 0;
static char DC = 0;

int main(void) 
{
	TIM3_CH1_PWM_init();
	USART2_init();

	while(1) 
	{}
}
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		switch(state)
		{
			case 0:
				character =	(char)USART_ReceiveData(USART2) & 0xFF;
				ch1 = character;
				USART_SendData(USART2, ch1);
			
				state = 1;
			break;

			case 1:
				character = (char)USART_ReceiveData(USART2) & 0xFF;
				ch2 = character;
				USART_SendData(USART2, ch2);
			
				state = 2;
			break;
		}
	}
	//USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	
	if(USART_GetITStatus(USART2, USART_IT_TC) != RESET)
	{
		switch(state)
		{
			case 0:
			break;
			
			case 1:
			break;
			
			case 2:
				DC = ((ch1-48)*10 + (ch2-48))*2;
				TIM_OCInitTypeDef outputChannelInit;
				outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
				outputChannelInit.TIM_Pulse = DC;
				outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
				outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
				TIM_OC1Init(TIM3, &outputChannelInit);
				TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
			
				state = 0;
			break;
		}
	}
	//USART_ClearITPendingBit(USART2, USART_IT_TC);
}

