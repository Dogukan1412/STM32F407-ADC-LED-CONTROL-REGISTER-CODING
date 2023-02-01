
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"


void CLK_Config(void);
void Led_Config(void);
void ADC_Pot_Config(void);
void ADC_Config(void);
uint16_t Read_Adc();

uint16_t adc_value;

int main(void)
{
	CLK_Config();
	Led_Config();
	ADC_Pot_Config();
	ADC_Config();
  while (1)
  {
	  adc_value = Read_Adc();
	  if(adc_value < 512)
	  {
		  GPIOD->ODR = 0x00003000;   // pins set D12, D13
	  }
	  else if(adc_value >= 512)
	  {
		  GPIOD->ODR = 0x0000C000;   // pins set D14, D15
	  }
  }
}


void CLK_Config() // Clock speed for 168MHz
{
	RCC->CR |= 0x00010000;                 // HSEON ENABLE
	while(!(RCC->CR & 0x00020000));        // HSEON Ready Flag wait
	RCC->CR |= 0x00080000;              // CSS ENABLE
	RCC->CR |= 0x01000000;				// PLL ON
	RCC->PLLCFGR |= 0x00400000;        // PLL SRC HSE is selected
	RCC->PLLCFGR |= 0x00000004;       // PLL M 4
	RCC->PLLCFGR |= 0x00005A00;        // PLL N 168
	RCC->PLLCFGR |= 0x00000000;       // PLL P 2
	RCC->CFGR |= 0x00000000;          // AHB PRESCALER 1
	RCC->CFGR |= 0x00080000;          // APB2 PRESCALER 2
	RCC->CFGR |= 0x00001400;          // APB1 PRESCALER 4
	RCC->CIR |= 0x00080000;             // HSE READY FLAG CLEAR
	RCC->CIR |= 0x00800000;             // CSS FLAG CLEAR
}


void Led_Config(void)  // User led configuration
{
	RCC->AHB1ENR |= 0x1U << 3U; // D port clock enable

	GPIOD->MODER |= 0x55000000; // pins D12, D13, D14, D15 is selected output mode
	GPIOD->OSPEEDR |= 0xFF000000; // very high speed is selected
	GPIOD->PUPDR |= 0x00000000; // no pull up, pull down
}


void ADC_Pot_Config(void)
{
	RCC->AHB1ENR |= 0x1U << 0U; // A port clock enable

	GPIOA->MODER |= 3U << 0U; // pin A0 is selected analog mode
	GPIOA->OSPEEDR |= 3U << 0U; // very high speed is selected
	GPIOA->PUPDR |= 0U << 0U; // no pull up, pull down
}


void ADC_Config(void)
{
	RCC->APB2ENR |= 0x1U << 8U;        // ADC1 clock bus is enabled

	ADC1->CR1 |= 1U << 24U;            // resolution = 10-bit
	ADC1->CR1 |= 0U << 8U;             // scan mode is disabled
	ADC1->CR2 |= 1U << 0U;			   // Enable ADC
	ADC1->CR2 |= 1U << 1U;			   // Continuous conversion mode enable
	ADC1->CR2 |= 0U << 11U;			   // data alignment right
	ADC->CCR  |= 1U << 16U;			   // ADC prescaler divided by 4
	ADC1->SMPR2 |= 0x00000003;		   // Channel 1 sampling time selection 56 cycles
}


uint16_t Read_Adc()
{
	ADC1->CR2 |= 1U << 30U;			   // Start conversion of regular channels
	while(!(ADC1->SR & 0x00000002));      // wait for Conversion complete
	return ADC1->DR;                      // return variable in the data register
}








/*
 * Callback used by stm32f4_discovery_audio_codec.c.
 * Refer to stm32f4_discovery_audio_codec.h for more info.
 */
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

/*
 * Callback used by stm324xg_eval_audio_codec.c.
 * Refer to stm324xg_eval_audio_codec.h for more info.
 */
uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
