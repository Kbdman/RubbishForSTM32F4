#include "RTE_Components.h"
#include "stm32f407xx.h"
#include "system_stm32f4xx.h"
#include CMSIS_device_header
#include "common.h"
#include "can.h"
int i = 0x256;
int flag = 0;
const char *strs[3] = {"This is the first string",
                       "Print the second",
                       "This third string is a wish that I hope the WW3 will not happen in 2024"};
void InitPG6()
{
    GPIOG->MODER &= ~(0x3 << (2 * 6));
    GPIOG->MODER |= (0x1 << (2 * 6));
    GPIOG->OTYPER &= ~(1 << (1 * 6));
    GPIOG->OTYPER |= (0 << (1 * 6));
    GPIOG->OSPEEDR &= ~(0x3 << (2 * 6));
    GPIOG->OSPEEDR |= (0 << (2 * 6));
    GPIOG->PUPDR &= ~(0x3 << (2 * 6));
    GPIOG->PUPDR |= (1 << (2 * 6));
}
void delight()
{
    GPIOG->BSRR |= 1 << 16 << 6;
}
void toff()
{
    GPIOG->BSRR |= 1 << 6;
}
void InitPD9()
{
    GPIO_InitTypeDef def = {};
    def.Pin = GPIO_PIN_9;
    def.Mode = GPIO_MODE_INPUT;
    def.Pull = GPIO_PULLDOWN;
    def.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &def);
}
void InitPB1()
{
    GPIO_InitTypeDef def = {};
    def.Pin = GPIO_PIN_1;
    def.Mode = GPIO_MODE_IT_RISING;
    def.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOB, &def);
}

int checkPD9()
{
    int re = (*(int *)(GPIOD_BASE + 16)) & (1 << 9);
    return re;
    // return GPIO_PIN_SET==HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_9);
}
void EXTI1_IRQHandler()
{
}
void SysTick_Handler()
{
    flag++;
    if (flag % 2 == 0)
        delight();
    else
        toff();
}
void initNVIC()
{
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);
}

/**
 * @brief output SystemClock onto MCO2
 *
 */
void setupMCO2()
{
    // MCO2 is AF0 of PC9
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    setGPIOMode(GPIOC, 9, 0b10);
    setGPIOAF(GPIOC, 9, 0x0);
    RCC->CFGR = clearBit(RCC->CFGR, 30, 2) | (0b00 << (30));
}

char bufdes[1024] = {0};
int main()
{
    SystemCoreClockUpdate();
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    InitPG6();
    setupMCO2();
    initUSART1();
    initDMA2S7ForM2P();
    initCan1OnPD();
    //   InitEXTI();
    //   initNVIC();
    //   HAL_SYSTICK_Config(SystemCoreClock/20);
    //   HAL_NVIC_EnableIRQ(SysTick_IRQn);
    /*


        HAL_RCC_MCOConfig(RCC_MCO1,RCC_MCO1SOURCE_HSI,RCC_MCODIV_4);
        InitPG6();
        InitPD9();
        for (;;) {
            i++;
            if(i%10000!=0)
                continue;
            if(checkPD9())
               delight();
            else
             toff();

        }
        */
    int sel = 0;
    while (1)
    {
        sel++;
        if (sel % 20 == 0)
        {

            logString(strs[(sel / 20000) % 3]);
        }
    };
}
