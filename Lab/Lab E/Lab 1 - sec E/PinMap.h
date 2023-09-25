// Pin Usage
// Function			**		Pin Name	**		Board Pin Out
// TIM4 CH1 IC1	** 		PB6 			**		D10
// TIM3 CH1 PWM ** 		PA6 			** 		D12

// TIM4 CH1 IC1 ** 		PB6 			** 		D10
#define TIM4_CH1_IT1_RCC_GPIO 	RCC_APB2Periph_GPIOB
#define TIM4_CH1_IT1_GPIO 			GPIOB
#define TIM4_CH1_IT1_PIN 				GPIO_Pin_6

//Function prototypes
void TIM4_CH1_IC1_init(void);
void USART2_init(void);
void USARTSend(char *pucBuffer, unsigned long ulCount);
void TIM4_IRQHandler(void);
