#include "stm32f10x.h"


//IM1 CH1 **A8
#define TIM1_CH1_PWM_RCC_GPIO RCC_APB2Periph_GPIOA
#define TIM1_CH1_PWM_PIN      GPIO_Pin_8
#define TIM1_CH1_PWM_GPIO     GPIOA

//TIM1 CH1N **B13
#define TIM1_CH1N_PWM_RCC_GPIO RCC_APB2Periph_GPIOB
#define TIM1_CH1N_PWM_PIN      GPIO_Pin_13
#define TIM1_CH1N_PWM_GPIO     GPIOB

//TIM1 CH2 **A9
#define TIM1_CH2_PWM_RCC_GPIO RCC_APB2Periph_GPIOA
#define TIM1_CH2_PWM_PIN      GPIO_Pin_9
#define TIM1_CH2_PWM_GPIO     GPIOA

//TIM1 CH2N **B14
#define TIM1_CH2N_PWM_RCC_GPIO RCC_APB2Periph_GPIOB
#define TIM1_CH2N_PWM_PIN      GPIO_Pin_14
#define TIM1_CH2N_PWM_GPIO     GPIOB

unsigned char state = 0;

void init(void)
{
	RCC_APB2PeriphClockCmd(TIM1_CH1_PWM_RCC_GPIO|TIM1_CH1N_PWM_RCC_GPIO|TIM1_CH2_PWM_RCC_GPIO|TIM1_CH2N_PWM_RCC_GPIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	//conifg GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
  
  /* channel 2 & channel 2N must be initialized at the same time */
	
	//ch1
	GPIO_InitStructure.GPIO_Pin = TIM1_CH1_PWM_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(TIM1_CH1_PWM_GPIO,&GPIO_InitStructure);
	
  
	//ch2
	GPIO_InitStructure.GPIO_Pin = TIM1_CH2_PWM_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(TIM1_CH2_PWM_GPIO,&GPIO_InitStructure);
	
	//ch1n
	GPIO_InitStructure.GPIO_Pin = TIM1_CH1N_PWM_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(TIM1_CH1N_PWM_GPIO,&GPIO_InitStructure);
	
	//ch2n
	GPIO_InitStructure.GPIO_Pin = TIM1_CH2N_PWM_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(TIM1_CH2N_PWM_GPIO,&GPIO_InitStructure);
	
	
	//set TIM1 1000Hz 
  	//range of PWM: 0 to 1000
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Prescaler = 72-1; // 0.001ms
	timerInitStructure.TIM_Period = 1000-1; //1ms
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &timerInitStructure);
	TIM_Cmd(TIM1, ENABLE);
  
	TIM_ARRPreloadConfig(TIM1, ENABLE);
}

// Enable USART2 to receive character
void USART2_init(void)
{
  	//USART2 TX RX
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
  
  	//TX pin, PA2
  	GPIO_InitTypeDef gpio_init;
  	gpio_init.GPIO_Pin = GPIO_Pin_2;
  	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
  	gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_Init(GPIOA, &gpio_init);
  
  	//RX pin, PA3
  	gpio_init.GPIO_Pin = GPIO_Pin_3;
  	gpio_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_Init(GPIOA, &gpio_init);
  
  	//USART2 ST-LINK USB
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
  	USART_InitTypeDef usart_init;
  	usart_init.USART_BaudRate = 9600;
  	usart_init.USART_WordLength = USART_WordLength_8b;
  	usart_init.USART_StopBits = USART_StopBits_1;
  	usart_init.USART_Parity = USART_Parity_No;
  	usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	usart_init.USART_Mode = USART_Mode_Rx |USART_Mode_Tx;
  	USART_Init(USART2, &usart_init);
  	USART_Cmd(USART2,ENABLE);
		
		// Enable the USART2 RX Interrupt
		NVIC_InitTypeDef NVIC_InitStructure;
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		// Enable the USART2 TX Interrupt
		USART_ITConfig(USART2, USART_IT_TC, ENABLE); 
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
}

// a function to enable the left wheel moving forward 
void leftforward(unsigned short pwm)
{
	TIM_OCInitTypeDef leftforward;
	leftforward.TIM_OCMode = TIM_OCMode_PWM1;
	leftforward.TIM_Pulse = pwm-1;
	leftforward.TIM_OutputState = TIM_OutputState_Enable;
	leftforward.TIM_OutputNState = TIM_OutputNState_Disable;
	leftforward.TIM_OCPolarity = TIM_OCPolarity_High;
  
	TIM_OC1Init(TIM1, &leftforward);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
  
}

// a function to enable the right wheel moving forward 
void rightforward(unsigned short pwm)
{
	TIM_OCInitTypeDef rightforward;
	rightforward.TIM_OCMode = TIM_OCMode_PWM1;
	rightforward.TIM_Pulse = pwm-1;
	rightforward.TIM_OutputNState = TIM_OutputNState_Enable;
	rightforward.TIM_OutputState = TIM_OutputState_Disable;
	rightforward.TIM_OCNPolarity = TIM_OCNPolarity_High;
	
	TIM_OC2Init(TIM1, &rightforward);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

// a function to enable the left wheel moving backward 
void leftbackward(unsigned short pwm)
{
	TIM_OCInitTypeDef leftbackward;
	leftbackward.TIM_OCMode = TIM_OCMode_PWM1;
	leftbackward.TIM_Pulse = pwm-1;
	leftbackward.TIM_OutputNState = TIM_OutputNState_Enable;
	leftbackward.TIM_OutputState = TIM_OutputState_Disable;
	leftbackward.TIM_OCNPolarity = TIM_OCNPolarity_High;
	
	TIM_OC1Init(TIM1, &leftbackward);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
  	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

// a function to enable the right wheel moving backward 
void rightbackward(unsigned short pwm)
{
	TIM_OCInitTypeDef rightbackward;
	rightbackward.TIM_OCMode = TIM_OCMode_PWM1;
	rightbackward.TIM_Pulse = pwm-1;
	rightbackward.TIM_OutputState = TIM_OutputState_Enable;
	rightbackward.TIM_OutputNState = TIM_OutputNState_Disable;
	rightbackward.TIM_OCPolarity = TIM_OCPolarity_High;
	
	TIM_OC2Init(TIM1, &rightbackward);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
  	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

// a function to generate a time delay (period in us) by using TIM3
void TIM3_delay(unsigned short period)
{
 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  	TIM_TimeBaseInitTypeDef initStructure;
  	initStructure.TIM_Prescaler = 7200; // 1(72MHz/72000) = 0.1ms
  	initStructure.TIM_CounterMode = TIM_CounterMode_Up;
  	initStructure.TIM_Period = period; // 1us*period = delay us
  	initStructure.TIM_ClockDivision = 0;
  	initStructure.TIM_RepetitionCounter = 0;
  	TIM_TimeBaseInit(TIM3, &initStructure);
  	TIM_Cmd(TIM3, ENABLE);
  	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
}


int main()
{
  	init();
		USART2_init(); 
	
		//TIM3_delay(10000);  
  	//__IO uint8_t state = 0; // variable to count the delay
  
  	while(1)
	{ 
    		//set to 50% duty cycle, frequency = 1000 Hz
    		/*switch(state)
		{
      			case 1:
			        leftforward(500);
      				break;
      			case 3:
        			rightforward(500);
      				break;
      			case 5:
        			leftbackward(500);
      				break;
      			case 7:
        			rightbackward(500);
      				break;
      			default:
			        leftforward(1);
        			rightforward(1);
        			leftbackward(1);
        			rightbackward(1);
    		}
    
    		if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
		{
       			if(state == 8)
         			state = 1;
       			else
         			state++;
       			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
     		} */
  	}
}

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		state =	(char)USART_ReceiveData(USART2) & 0xFF;
	}
	//USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	
	if(USART_GetITStatus(USART2, USART_IT_TC) != RESET)
	{	
		switch(state) 
		{
			case 'f':
			{ 
				leftforward(400);
				rightforward(360);
				break;
			}
			case 'F':
			{ 
				leftforward(600);
				rightforward(560);
				break;
			}
			case 'b':
			{ 
				leftbackward(340);
				rightbackward(400);
				break;
			}
			case 'B':
			{ 
				leftbackward(540);
				rightbackward(600);
				break;
			}
			case 'l':
			{ 
				leftforward(270);
				rightforward(400);
				break;
			}
			case 'L':
			{ 
				leftbackward(350);
				rightforward(350);
				break;
			}
			case 'r': 
			{
				leftforward(400);
				rightforward(270);
				break;
			}
			case 'R':
			{
				leftforward(350);
				rightbackward(350);
				break;
			}
			case 'N':
			{
				leftforward(1);
				rightforward(1);
				break;
			}
		}
	}
	//USART_ClearITPendingBit(USART2, USART_IT_TC);
}