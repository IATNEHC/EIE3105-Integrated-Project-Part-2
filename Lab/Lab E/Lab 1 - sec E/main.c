#include "stm32f10x.h" 				// Device header
#include "PinMap.h"
#include "stdbool.h"
#include "stdio.h"

static char buffer[50] = {'\0'};
static bool pulseHigh = false;
static u32 pulseWidth = 0;

int main(void) 
{
	TIM4_CH1_IC1_init();
	USART2_init();
	
	while(1) 
	{}
}

void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_CC1) !=RESET) 
	{
		if(!pulseHigh)
		{
			pulseHigh = true; 							//pulse starts
			TIM_SetCounter(TIM4,0);
			//change to detect falling
			TIM_OC1PolarityConfig(TIM4,TIM_ICPolarity_Falling);
		}
		else 
		{
			pulseWidth += TIM_GetCounter(TIM4);
			//change to detect raising
			TIM_OC1PolarityConfig(TIM4,TIM_ICPolarity_Rising);
			pulseWidth = pulseWidth/2;
			sprintf(buffer, "%d\r\n", pulseWidth);
			USARTSend(buffer, sizeof(buffer));
			pulseHigh = false;
			pulseWidth = 0;
		}
	}
	//Clear interrupt flag
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update|TIM_IT_CC1);
}
