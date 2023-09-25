// Pin Usage
// Function			**		Pin Name	**		Board Pin Out
// TIM4 CH1 IC1	** 		PB6 			**		D10
// TIM3 CH1 PWM ** 		PA6 			** 		D12

// TIM3 CH1 PWM ** 		PA6 			** 		D12
#define TIM3_CH1_PWM_RCC_GPIO 	RCC_APB2Periph_GPIOA
#define TIM3_CH1_PWM_GPIO 			GPIOA
#define TIM3_CH1_PWM_PIN 				GPIO_Pin_6

//Function prototypes
void TIM3_CH1_PWM_init(void);
void USART2_init(void);
void USART2_IRQHandler(void);
