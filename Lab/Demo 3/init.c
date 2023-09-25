#include "pinmap.h"

// TIM1 Channel 3 and Channel 3N initialization
void SR04_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	// config TIM1 CH3 I/O
	// echo inpute capture
	GPIO_InitTypeDef init_gpio;
	init_gpio.GPIO_Mode = GPIO_Mode_IPD; // the mode is not AF_PP
	init_gpio.GPIO_Pin = GPIO_Pin_10; // TIM1 CH3 pin
	init_gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &init_gpio);

	//set TIM1 in 1000Hz
	//pulse range: 0 to 1000
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	// 20ms per cycle, 50Hz
	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	
	timerInitStructure.TIM_Prescaler = 72-1; // 1(72MHz/72) = 1us, 72
	timerInitStructure.TIM_Period = 10000-1; // 1us*10000 = 10ms, 100Hz
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &timerInitStructure);
	TIM_Cmd(TIM1, ENABLE);

	//enable TIM1 CH3 Input Capture
	TIM_ICInitTypeDef inputCapture_init;
	inputCapture_init.TIM_Channel = TIM_Channel_3; //seclect IC1
	inputCapture_init.TIM_ICPolarity = TIM_ICPolarity_Rising; //capture rising edge
	inputCapture_init.TIM_ICSelection = TIM_ICSelection_DirectTI; //map to TI3
	inputCapture_init.TIM_ICPrescaler = TIM_ICPSC_DIV1; //configure input frequency
	inputCapture_init.TIM_ICFilter = 0; //no filter
	TIM_ICInit(TIM1, &inputCapture_init);

	//enable input capture interrput
	NVIC_InitTypeDef nvic_init;
	nvic_init.NVIC_IRQChannel = TIM1_CC_IRQn; //TIM1 capture compare interrput
	//preemptive priority level 2
	nvic_init.NVIC_IRQChannelPreemptionPriority = 2;
	//from the priority level 0
	nvic_init.NVIC_IRQChannelSubPriority = 0;
	//the IRQ channel is enabled
	nvic_init.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_init);
	//allow update to interrput, allows the CC3IE to capture interrput
	TIM_ITConfig(TIM1, TIM_IT_Update|TIM_IT_CC3, ENABLE);
}

// TIM1 initialize, left & right motors
void Motor_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	// GPIO config setup
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// TIM 1 CH1 I/O config, A8
	// Left motor forward
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// TIM 1 CH1N I/O config, B13
	// Left motor backward
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// TIM 1 CH2 I/O config, A9
	// Right motor backward
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// TIM 1 CH2N I/O config, B14
	// Right motor forward
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// TIM 1 config setup
	// set TIM1 100Hz
	// range of PWM: 0 to 10000
	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	
	timerInitStructure.TIM_Prescaler = 72 - 1;
	timerInitStructure.TIM_Period = 10000 - 1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &timerInitStructure);
	TIM_Cmd(TIM1, ENABLE);
	
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
}

// TIM3 internal timer, unit in us
void TIM3_delay_us(unsigned short period) 
{ 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseInitTypeDef timerInitStructure; 
	timerInitStructure.TIM_Prescaler = 72-1;  //1/(72Mhz/72)=1ms
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = period;  //1ms*period = delay
	timerInitStructure.TIM_ClockDivision = 0; //TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &timerInitStructure);
	TIM_Cmd(TIM3, ENABLE);
	//Enable update event for Timer3
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
}

// Button and LED initialize
void Button_LED_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
	
	// GPIO config setup
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// Button I/O config, A12
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// LED I/O config, B6
	// Green LED on control board
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// LED I/O config, A0
	// Yellow LED on control board
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// Turn on Green LED by default
	GPIO_ResetBits(GPIOB, GPIO_Pin_6);
	
	// Turn off Yellow LED by default
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
}

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
}
