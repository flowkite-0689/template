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

void GPIO_Init_Button(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // 1. Enable GPIOA clock
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  // 2. Configure PA0 pin as input
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  // Pull-up resistor

  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void Test_Buzzer_Pins(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 测试PF8引脚
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
    
    // PF8发出测试音 (500ms)
    GPIO_SetBits(GPIOF, GPIO_Pin_8);
    delay_ms(500);
    GPIO_ResetBits(GPIOF, GPIO_Pin_8);
    delay_ms(500);
    
    // 再响一次确认
    GPIO_SetBits(GPIOF, GPIO_Pin_8);
    delay_ms(500);
    GPIO_ResetBits(GPIOF, GPIO_Pin_8);
}

int main(void)
{
  uint8_t led_state = 0;     // Initial LED state (off)
  
  Systick_Init();
  GPIO_Init_LED();
  GPIO_Init_Button();
  
  // 添加蜂鸣器测试 - 程序启动时依次测试各引脚
  Test_Buzzer_Pins();
  
  // Initialize LED states
  GPIO_ResetBits(GPIOF, GPIO_Pin_9);  // Turn off LED initially

  while (1)
  {
    // Read button state (0 when pressed, 1 when released due to pull-up)
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
    {
      delay_ms(20);  // Simple debounce delay
      
      // Check if button is still pressed after debounce
      if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
      {
        // Wait for button release to avoid repeated toggles
        while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0);
        
        // Toggle LED state
        led_state = !led_state;
        
        if (led_state)
        {
          GPIO_SetBits(GPIOF, GPIO_Pin_9);     // Turn on LED
        }
        else
        {
          GPIO_ResetBits(GPIOF, GPIO_Pin_9);   // Turn off LED
        }
      }
    }
    
    delay_ms(10);  // Small delay to reduce CPU usage
  }
}
