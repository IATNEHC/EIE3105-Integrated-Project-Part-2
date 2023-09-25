#include "pinmap.h"

// TIM1 initialize, left & right motors
void TIM1_init(void)
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

// TIM1 initialize, left & right motors, sr04 module
void TIM1_init_with_SR04(void)
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
	
	// TIM 1 CH3 I/O config, A10
	// Ultrasonic module input capture, SR04
	// PWM mode: GPIO_Mode_Out_PP, IC mode: GPIO_Mode_IPD
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// TIM 1 CH3N I/O config, B15
	// Ultrasonic module trigger pin, SR04
	// NOT using TIM1 to trigger, USING TIM3 to trigger
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
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
	
	// TIM 1 CH3 input capture config setup
	TIM_ICInitTypeDef inputCapture_init;
	
	inputCapture_init.TIM_Channel = TIM_Channel_3;
	inputCapture_init.TIM_ICPolarity = TIM_ICPolarity_Rising;
	inputCapture_init.TIM_ICSelection = TIM_ICSelection_DirectTI;
	inputCapture_init.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	inputCapture_init.TIM_ICFilter = 0;
	TIM_ICInit(TIM1, &inputCapture_init);
		
	// NVIC config setup
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// TIM1 update interrupt config setup
	// Preemptive priority = 2
	// Sub priority = 0
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// TIM1 input capture interrupt config setup
	// Preemptive priority = 3
	// Sub priority = 0
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM1, TIM_IT_Update|TIM_IT_CC3, ENABLE);
}

// TIM2 initialize
void TIM2_init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// TIM 2 config setup
	// set TIM2 10Hz
	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	
	timerInitStructure.TIM_Prescaler = 18000 - 1;
	timerInitStructure.TIM_Period = 400 - 1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &timerInitStructure);
	TIM_Cmd(TIM2, ENABLE);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	//NVIC_EnableIRQ(TIM2_IRQn);
	// NVIC config setup
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// TIM1 update interrupt config setup
	// Preemptive priority = 1
	// Sub priority = 0
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

// TIM3 internal timer, unit in us
void TIM3_delay_us(unsigned short us)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM3_init;
	TIM3_init.TIM_Prescaler = 72 - 1;
	TIM3_init.TIM_Period = us - 1;
	TIM3_init.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM3_init.TIM_CounterMode = TIM_CounterMode_Up;
	TIM3_init.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM3_init);
	TIM_Cmd(TIM3, ENABLE);
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	while(TIM_GetITStatus(TIM3, TIM_IT_Update) == RESET);
}

// TIM3 internal timer, unit in ms
void TIM3_delay_ms(unsigned short ms)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM3_init;
	TIM3_init.TIM_Prescaler = 7200 - 1;
	TIM3_init.TIM_Period = (ms*10) - 1;
	TIM3_init.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM3_init.TIM_CounterMode = TIM_CounterMode_Up;
	TIM3_init.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM3_init);
	TIM_Cmd(TIM3, ENABLE);
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	while(TIM_GetITStatus(TIM3, TIM_IT_Update) == RESET);
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

// USART3 initialize
void USART3_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	// GPIO config setup
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// USART3 TX I/O config, B10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// USART3 RX I/O config, B11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// USART3 config setup
	// Baud rate: 115200
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART3, &USART_InitStructure);
	USART_Cmd(USART3, ENABLE);
}

void SPI1_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_SPI1EN, ENABLE);
	
	// GPIO config setup
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// SPI1 SCK I/O config, A5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// SPI1 MOSI I/O config, A6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// SPI1 MISO I/O config, A7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// SPI1 config setup
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);
	SPI_Cmd(SPI1, ENABLE);
}

// use polling to send out a string through USART2
void USARTSend(USART_TypeDef *USARTx, char *pucBuffer, unsigned long ulCount)
{
	//loop until the whole string is sent
	while(ulCount--)
	{
		USART_SendData(USARTx, *pucBuffer++);
		/* loop until the end of transmission */
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	}
}
