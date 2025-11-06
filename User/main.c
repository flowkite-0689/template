#include "stm32f4xx.h"

uint32_t delay_count = 0;
void SysTick_Handler(void)
{
  if (delay_count > 0)
    delay_count--;
}

void Systick_Init(void)
{
  // Configure SysTick for 1ms interrupt
  // SystemCoreClock = 168000000
  // SysTick_Config(SystemCoreClock / 1000) generates 1ms interrupt

  if (SysTick_Config(SystemCoreClock / 1000))
  {
    // Error handling, stay here
    while (1)
      ;
  }

  // Set SysTick interrupt priority to highest
  NVIC_SetPriority(SysTick_IRQn, 0);
}

void delay_ms(uint32_t ms)
{
  delay_count = ms;
  while (delay_count != 0)
    ;
}

void GPIO_Init_LED(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // 1. Enable GPIOF clock
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

  // 2. Configure PF9 and PF10 pins
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

  GPIO_Init(GPIOF, &GPIO_InitStructure);

  // 3. Initialize LED states
  GPIO_SetBits(GPIOF, GPIO_Pin_9);
  GPIO_ResetBits(GPIOF, GPIO_Pin_10);
}

int main(void)
{
  Systick_Init();
  GPIO_Init_LED();

  while (1)
  {
    GPIO_SetBits(GPIOF, GPIO_Pin_9);
    delay_ms(1000);
    GPIO_ResetBits(GPIOF, GPIO_Pin_9);
    delay_ms(1000);
  }
}
