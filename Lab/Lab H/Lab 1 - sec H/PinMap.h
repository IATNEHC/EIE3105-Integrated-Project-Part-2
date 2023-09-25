// Pin Usage
// Function			** 	Pin Name 	** 	Board Pin Out
// ADC1 CH0 		** 	PA0 			** 	A0
// ADC1 CH1 		** 	PA1 			** 	A1
// ADC1 CH2 		** 	PA4 			** 	A2
// TIM2 CH3 PWM ** 	PB10 			** 	D6
// TIM3 CH1 PWM ** 	PA6 			** 	D12
// TIM4 CH1 PWM ** 	PB6 			** 	D10

//ADC1_0 PA0, channel 0, ADC1
#define ADC1_0_RCC_GPIO RCC_APB2Periph_GPIOA
#define ADC1_0_GPIO GPIOA
#define ADC1_0_PIN GPIO_Pin_0

//ADC1_0 PA1, channel 1, ADC1
#define ADC1_1_RCC_GPIO RCC_APB2Periph_GPIOA
#define ADC1_1_GPIO GPIOA
#define ADC1_1_PIN GPIO_Pin_1

//ADC1_0 PA4, channel 5, ADC1
#define ADC1_4_RCC_GPIO RCC_APB2Periph_GPIOA
#define ADC1_4_GPIO GPIOA
#define ADC1_4_PIN GPIO_Pin_4

//PWM Tim2 Ch3 PA2, show the brightness of a LED
#define TIM2_CH3_PWM_RCC_GPIO RCC_APB2Periph_GPIOB
#define TIM2_CH3_PWM_GPIO GPIOB
#define TIM2_CH3_PWM_PIN GPIO_Pin_10

//PWM Tim3 Ch1 PA6, show the brightness of a LED
#define TIM3_CH1_PWM_RCC_GPIO RCC_APB2Periph_GPIOA
#define TIM3_CH1_PWM_GPIO GPIOA
#define TIM3_CH1_PWM_PIN GPIO_Pin_6

//PWM Tim4 Ch1 PB6, show the brightness of a LED
#define TIM4_CH1_PWM_RCC_GPIO RCC_APB2Periph_GPIOB
#define TIM4_CH1_PWM_GPIO GPIOB
#define TIM4_CH1_PWM_PIN GPIO_Pin_6

void ADC1_1channel_init(void);
void ADC1_1channel_0_change(void);
void ADC1_1channel_1_change(void); 
void ADC1_1channel_4_change(void); 
void TIM2_PWM_CH3_init(void);
void TIM2_PWM_Pulse_Set(int pulse);
void TIM3_PWM_CH1_init(void);
void TIM3_PWM_Pulse_Set(int pulse);
void TIM4_PWM_CH1_init(void);
void TIM4_PWM_Pulse_Set(int pulse);
