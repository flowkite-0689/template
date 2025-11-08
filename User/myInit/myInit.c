/**
 * @file myInit.c
 * @brief STM32F4xx GPIO初始化及延时函数实现
 * @author [作者姓名]
 * @version v1.0
 * @date [创建日期]
 */

#include "myInit.h"

/**
 * @brief 简单的循环延时函数
 * @param count 延时循环次数
 * @note 这是一个简单的软件延时，延时时间取决于CPU频率
 */
void delay(uint32_t count)
{
  while (count--)
    ;
}

/**
 * @brief 毫秒级延时函数
 * @param ms 延时时间（单位：毫秒）
 * @note 基于168MHz系统时钟的简单延时，约1ms=32000个循环周期
 * @warning 此函数为软件延时，精度较低，不适合需要精确计时的场景
 */
void Delay_ms(uint32_t ms)
{
  uint32_t i;
  for (i = 0; i < ms; i++)
  {
    delay(32000); // 168MHz 下大约1ms
  }
}

/**
 * @brief GPIO通用初始化函数
 * @param GPIOx GPIO端口（如GPIOA、GPIOB等）
 * @param GPIO_Pin GPIO引脚（如GPIO_Pin_0、GPIO_Pin_1等）
 * @param GPIO_Mode GPIO模式（输入、输出、复用、模拟）
 * @param GPIO_Speed GPIO速度（2MHz、25MHz、50MHz、100MHz）
 * @param GPIO_OType GPIO输出类型（推挽、开漏）
 * @param GPIO_PuPd GPIO上下拉配置（上拉、下拉、无上下拉）
 * @note 函数会根据传入的GPIOx参数自动使能对应的AHB1时钟
 */
void GPIO_MyInit(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode, 
                 GPIOSpeed_TypeDef GPIO_Speed, GPIOOType_TypeDef GPIO_OType, 
                 GPIOPuPd_TypeDef GPIO_PuPd)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // 根据实际使用的GPIO端口使能对应时钟
  if (GPIOx == GPIOA) {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  } else if (GPIOx == GPIOB) {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  } else if (GPIOx == GPIOC) {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  } else if (GPIOx == GPIOD) {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  } else if (GPIOx == GPIOE) {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  } else if (GPIOx == GPIOF) {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
  } else if (GPIOx == GPIOG) {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
  } else if (GPIOx == GPIOH) {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
  } else if (GPIOx == GPIOI) {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);
  }

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode;
  GPIO_InitStructure.GPIO_OType = GPIO_OType;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd;
  GPIO_Init(GPIOx,&GPIO_InitStructure);
}

/**
 * @brief 按键GPIO初始化函数
 * @param KEY_PIN 按键引脚（如GPIO_Pin_0等）
 * @param GPIOx 按键所在GPIO端口（如GPIOA等）
 * @note 配置为高速输入模式，推挽输出类型，无上下拉
 * @note 建议根据实际硬件设计配置上下拉电阻
 */
void KEY_Init(uint32_t KEY_PIN, GPIO_TypeDef *GPIOx)
{
  GPIO_MyInit(GPIOx, KEY_PIN, GPIO_Mode_IN, GPIO_High_Speed, GPIO_OType_PP, GPIO_PuPd_NOPULL);
}

/**
 * @brief LED GPIO初始化函数
 * @param LED_PIN LED引脚（如GPIO_Pin_9等）
 * @param GPIOx LED所在GPIO端口（如GPIOF等）
 * @note 配置为高速输出模式，推挽输出，无上下拉
 * @note 适用于驱动LED等低功耗设备
 */
void LED_Init(uint32_t LED_PIN, GPIO_TypeDef *GPIOx)
{
  GPIO_MyInit(GPIOx, LED_PIN, GPIO_Mode_OUT, GPIO_High_Speed, GPIO_OType_PP, GPIO_PuPd_NOPULL);
}

/**
 * @brief 蜂鸣器GPIO初始化函数
 * @param BEEP_PIN 蜂鸣器引脚（如GPIO_Pin_8等）
 * @param GPIOx 蜂鸣器所在GPIO端口（如GPIOA等）
 * @note 配置为50MHz输出模式，推挽输出，无上下拉
 * @note 适用于驱动蜂鸣器等中等频率设备
 */
void BEEP_Init(uint32_t BEEP_PIN, GPIO_TypeDef *GPIOx)
{
  GPIO_MyInit(GPIOx, BEEP_PIN, GPIO_Mode_OUT, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL);
}