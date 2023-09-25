#include "stm32f10x.h" // Device header
#include "PinMap-test.h"
#include "stdio.h"
#include "misc.h"

char	state = 0;

int main(void) 
{
	TIM3_PWM_CH1_init();
	TIM4_PWM_CH1_init();
	TIM2_init();
	
	TIM3_PWM_Pulse_Set(0);		//160 for 80%
	TIM4_PWM_Pulse_Set(40);		//40 for 20%
	
	while(1);
}

void TIM2_IRQHandler(void) 
{
	switch(state)
	{
		case 0:	{
							TIM3_PWM_Pulse_Set(160);		//160 for 80%
							TIM4_PWM_Pulse_Set(0);		//40 for 20%
							state = 1;
							break;
						}
		case 1: {
							TIM3_PWM_Pulse_Set(0);		//160 for 80%
							TIM4_PWM_Pulse_Set(40);		//40 for 20%
							state = 0;
							break;
						}
	}
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}
