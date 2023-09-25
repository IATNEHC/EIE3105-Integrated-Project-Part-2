#include "stm32f10x.h" // Device header
#include "PinMap.h"
#include "stdio.h"
#include "misc.h"

int main(void) 
{
	int adc_value;
	int state = 0;
	
	ADC1_1channel_init();
	TIM2_PWM_CH3_init();
	TIM3_PWM_CH1_init();
	TIM4_PWM_CH1_init();
	
	// start conversion (will be endless as we are in continuous mode)
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while(1) 
	{
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET )
		{}
		adc_value = ADC_GetConversionValue(ADC1);
		state++;
			
		switch(state)
		{
			case 1:
				if(adc_value > 2000)
				{
					TIM2_PWM_Pulse_Set(200);
					ADC1_1channel_1_change();
				}
				else
				{
					TIM2_PWM_Pulse_Set(0);
					ADC1_1channel_1_change();
				}
				break;
			
			case 2:
				if(adc_value > 2000)
				{
					TIM3_PWM_Pulse_Set(200);
					ADC1_1channel_4_change();
				}
				else
				{
					TIM3_PWM_Pulse_Set(0);
					ADC1_1channel_4_change();
				}
				break;
			
			case 3:
				if(adc_value > 2000)
				{
					TIM4_PWM_Pulse_Set(200);
					ADC1_1channel_0_change();
				}
				else
				{
					TIM4_PWM_Pulse_Set(0);
					ADC1_1channel_0_change();
				}
				state = 0;
				break;
		}
	}
}
