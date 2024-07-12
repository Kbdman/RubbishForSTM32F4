#include "RTE_Components.h"
#include "stm32f407xx.h"
#include "system_stm32f4xx.h"
#include CMSIS_device_header
int i=0x256;
int flag=0;
void InitPD6()
{
    GPIOG->MODER&=~(0x3<<(2*6));
    GPIOG->MODER|=(0x1<<(2*6));
    GPIOG->OTYPER&=~(1<<(1*6));
    GPIOG->OTYPER|=(0<<(1*6));
    GPIOG->OSPEEDR&=~(0x3<<(2*6));
    GPIOG->OSPEEDR|=(0<<(2*6));
    GPIOG->PUPDR&=~(0x3<<(2*6));
    GPIOG->PUPDR|=(1<<(2*6));
}
void delight()
{
    GPIOG->BSRR|=1<<16<<6;
}
void toff()
{
    GPIOG->BSRR|=1<<6;
}
void InitPD9()
{
    GPIO_InitTypeDef def={};
    def.Pin=GPIO_PIN_9;
    def.Mode=GPIO_MODE_INPUT;
    def.Pull=GPIO_PULLDOWN;
    def.Speed=GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &def);
}
void InitPB1()
{
    GPIO_InitTypeDef def={};
    def.Pin=GPIO_PIN_1;
    def.Mode=GPIO_MODE_IT_RISING;
    def.Pull=GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOB, &def);
}


int checkPD9()
{
    int re=(*(int*)(GPIOD_BASE+16))&(1<<9);
    return re;
    //return GPIO_PIN_SET==HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_9);
}
void EXTI1_IRQHandler()
{

}
void SysTick_Handler()
{
        flag++;
        if(flag%2==0)
            delight();
        else
            toff();
}
void initNVIC()
{
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);

}
int main() {
    
    RCC->AHB1ENR|=RCC_AHB1ENR_GPIOGEN;
    RCC->AHB1ENR|=RCC_AHB1ENR_GPIOBEN;
    RCC->APB2ENR|=RCC_APB2ENR_SYSCFGEN;
    InitPD6();
    InitPB1();
    //InitEXTI();
    initNVIC();
    HAL_SYSTICK_Config(SystemCoreClock/20);
    HAL_NVIC_EnableIRQ(SysTick_IRQn);
/*


    HAL_RCC_MCOConfig(RCC_MCO1,RCC_MCO1SOURCE_HSI,RCC_MCODIV_4);
    InitPD6();
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
    while(1)
    {
    };
}
