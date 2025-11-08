#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

// LED和按键引脚定义
// LED引脚: PF9, PF10, PE13, PE14
#define LED0_PIN        GPIO_Pin_9
#define LED0_PORT       GPIOF
#define LED1_PIN        GPIO_Pin_10
#define LED1_PORT       GPIOF
#define LED2_PIN        GPIO_Pin_13
#define LED2_PORT       GPIOE
#define LED3_PIN        GPIO_Pin_14
#define LED3_PORT       GPIOE

// 按键引脚: PA0, PE2, PE3, PE4
#define KEY0_PIN        GPIO_Pin_0
#define KEY0_PORT       GPIOA
#define KEY1_PIN        GPIO_Pin_2
#define KEY1_PORT       GPIOE
#define KEY2_PIN        GPIO_Pin_3
#define KEY2_PORT       GPIOE
#define KEY3_PIN        GPIO_Pin_4
#define KEY3_PORT       GPIOE

// 延时函数
void delay(uint32_t count) {
    while(count--);
}
// 简单但有效的毫秒延时（基于168MHz系统时钟大致校准）
void Delay_ms(uint32_t ms)
{
    uint32_t i;
    for(i = 0; i < ms; i++)
    {
        delay(32000);  // 168MHz 下大约1ms
    }
}
// 按键初始化
void Key_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能GPIO时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    
    // 配置PA0为输入模式 (KEY0)
    GPIO_InitStructure.GPIO_Pin = KEY0_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;        // 输入模式
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        // 上拉电阻
    GPIO_Init(KEY0_PORT, &GPIO_InitStructure);
    
    // 配置PE2为输入模式 (KEY1)
    GPIO_InitStructure.GPIO_Pin = KEY1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;        // 输入模式
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        // 上拉电阻
    GPIO_Init(KEY1_PORT, &GPIO_InitStructure);
    
    // 配置PE3为输入模式 (KEY2)
    GPIO_InitStructure.GPIO_Pin = KEY2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;        // 输入模式
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        // 上拉电阻
    GPIO_Init(KEY2_PORT, &GPIO_InitStructure);
    
    // 配置PE4为输入模式 (KEY3)
    GPIO_InitStructure.GPIO_Pin = KEY3_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;        // 输入模式
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        // 上拉电阻
    GPIO_Init(KEY3_PORT, &GPIO_InitStructure);
}

// LED初始化
void LED_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能GPIO时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    
    // 配置LED0 (PF9)
    GPIO_InitStructure.GPIO_Pin = LED0_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;        // 输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;       // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;    // 速度high
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;     // 不使用上下拉
    GPIO_Init(LED0_PORT, &GPIO_InitStructure);
    
    // 配置LED1 (PF10)
    GPIO_InitStructure.GPIO_Pin = LED1_PIN;
    GPIO_Init(LED1_PORT, &GPIO_InitStructure);
    
    // 配置LED2 (PE13)
    GPIO_InitStructure.GPIO_Pin = LED2_PIN;
    GPIO_Init(LED2_PORT, &GPIO_InitStructure);
    
    // 配置LED3 (PE14)
    GPIO_InitStructure.GPIO_Pin = LED3_PIN;
    GPIO_Init(LED3_PORT, &GPIO_InitStructure);
    
    // 初始化LED为关闭状态
    GPIO_ResetBits(LED0_PORT, LED0_PIN);
    GPIO_ResetBits(LED1_PORT, LED1_PIN);
    GPIO_ResetBits(LED2_PORT, LED2_PIN);
    GPIO_ResetBits(LED3_PORT, LED3_PIN);
}

/**
  * @brief  读取按键状态（修正后的健壮实现）
  * @param  None
  * @retval uint8_t: 0~3（对应KEY0~KEY3按下），0xFF（无按键按下）
  * @note   采用"检测按下+消抖确认+等待松开"的完整逻辑
  */
uint8_t Key_Read(void)
{
    uint8_t key_pressed = 0xFF; // 初始化为无按键
    uint8_t current_state;

    // 1. 快速检测哪个按键被按下
    for(uint8_t i = 0; i < 4; i++)
    {
        switch(i)
        {
            case 0: current_state = GPIO_ReadInputDataBit(KEY0_PORT, KEY0_PIN); break;
            case 1: current_state = GPIO_ReadInputDataBit(KEY1_PORT, KEY1_PIN); break;
            case 2: current_state = GPIO_ReadInputDataBit(KEY2_PORT, KEY2_PIN); break;
            case 3: current_state = GPIO_ReadInputDataBit(KEY3_PORT, KEY3_PIN); break;
            default: current_state = 1; break; // 默认为松开
        }

        // 2. 检测到按键按下（低电平）
        if(current_state == 0)
        {
            key_pressed = i; // 记录下可能按下的按键
            break; // 跳出循环，不再检测其他按键
        }
    }

    // 3. 如果没有按键按下，直接返回0xFF
    if(key_pressed == 0xFF)
    {
        return 0xFF;
    }

    // 4. 消抖延时：确认是真的按下，而不是抖动
    Delay_ms(20);

    // 5. 再次确认按键是否仍然按下
    switch(key_pressed)
    {
        case 0: current_state = GPIO_ReadInputDataBit(KEY0_PORT, KEY0_PIN); break;
        case 1: current_state = GPIO_ReadInputDataBit(KEY1_PORT, KEY1_PIN); break;
        case 2: current_state = GPIO_ReadInputDataBit(KEY2_PORT, KEY2_PIN); break;
        case 3: current_state = GPIO_ReadInputDataBit(KEY3_PORT, KEY3_PIN); break;
        default: current_state = 1; break;
    }

    // 6. 如果确认按下
    if(current_state == 0)
    {
        // 7. 等待按键松开，这是防止长按和抖动的关键
        while(1)
        {
            switch(key_pressed)
            {
                case 0: current_state = GPIO_ReadInputDataBit(KEY0_PORT, KEY0_PIN); break;
                case 1: current_state = GPIO_ReadInputDataBit(KEY1_PORT, KEY1_PIN); break;
                case 2: current_state = GPIO_ReadInputDataBit(KEY2_PORT, KEY2_PIN); break;
                case 3: current_state = GPIO_ReadInputDataBit(KEY3_PORT, KEY3_PIN); break;
            }
            if(current_state == 1) // 检测到按键松开（高电平）
            {
                break; // 跳出等待循环
            }
            Delay_ms(10); // 小延时，降低CPU占用
        }
        
        // 8. 按键完整的“按下-松开”过程结束，返回按键号
        return key_pressed;
    }

    // 如果消抖后发现按键已经松开，说明是抖动，返回无按键
    return 0xFF;
}

// 控制LED
void LED_Control(uint8_t led_num, uint8_t state) {
    switch(led_num) {
        case 0: // LED0
            if (state) {
                GPIO_SetBits(LED0_PORT, LED0_PIN);      // 开启LED
            } else {
                GPIO_ResetBits(LED0_PORT, LED0_PIN);    // 关闭LED
            }
            break;
        case 1: // LED1
            if (state) {
                GPIO_SetBits(LED1_PORT, LED1_PIN);
            } else {
                GPIO_ResetBits(LED1_PORT, LED1_PIN);
            }
            break;
        case 2: // LED2
            if (state) {
                GPIO_SetBits(LED2_PORT, LED2_PIN);
            } else {
                GPIO_ResetBits(LED2_PORT, LED2_PIN);
            }
            break;
        case 3: // LED3
            if (state) {
                GPIO_SetBits(LED3_PORT, LED3_PIN);
            } else {
                GPIO_ResetBits(LED3_PORT, LED3_PIN);
            }
            break;
    }
}

int main(void) {
    uint8_t key_num;
    static uint8_t led_state[4] = {0, 0, 0, 0}; // 每个LED的状态
    
    // 初始化硬件
    Key_Init();
    LED_Init();
    
    while(1) {
        // 检测按键
        key_num = Key_Read();
        
        // 如果有按键按下，切换对应的LED状态
        if (key_num < 4) {
            led_state[key_num] = !led_state[key_num];
            LED_Control(key_num, led_state[key_num]);
        }
        
        // 添加小延时避免CPU占用过高
       Delay_ms(10);
    }
}
