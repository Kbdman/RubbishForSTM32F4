#include "RTE_Components.h"
#include "stm32f407xx.h"
#include "system_stm32f4xx.h"
#include CMSIS_device_header
int i=0x256;
int flag=0;
void InitPG6()
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
uint32_t clearBit(uint32_t data,uint8_t start,uint8_t len)
{
    if(start+len>=32||len==0)
        return data;
    uint32_t clearbits=1;
    for(int c=1;c<len;c++)
    {
        clearbits=(clearbits<<1)+1;
    }
    clearbits=clearbits<<start;
    clearbits=~clearbits;
    data&=clearbits;
    return data;
}
/**
 * @brief select alternate function for a gpio port
 * 
 * @param gpio 
 * @param port 
 * @param af 
 */
void setGPIOAF(GPIO_TypeDef* gpio,uint8_t port,uint8_t  af)
{
    volatile uint32_t* AFR=&(gpio->AFR[0]);
    if(port>7)
    {
        AFR=&(gpio->AFR[1]);
        port-=7;
    }
    *AFR=clearBit(*AFR,port*4,4)|(af<<(port*4));

}
/**
 * @brief Set mode for a gpio port 
 * 
 * @param gpio 
 * @param port 
 * @param mode 
 */
void setGPIOMode(GPIO_TypeDef* gpio,uint8_t port,uint8_t mode)
{
    gpio->MODER=clearBit(gpio->MODER,port*2,2)|(mode<<(port*2));
}
char data='A';
void USART1_IRQHandler()
{
    if(USART1->SR&USART_SR_TXE)
    {
        USART1->DR=data;
        data+=1;
        if(data>'Z')
        {
            data='A';
        }
    }
}
void initUSART1()
{
    /**
     * @brief GPIO ports can be configured as alternate function by setting GPIO Mode 
     * Each port can select one alternate function from a list of alternate functiosn by setting GPIO AFR
     * The mapping is described in datasheet.
     * For STM32F407, PB6 can be used as USART1 TX and PB7 can be used as USART1 RX
     */
    RCC->APB2ENR|=RCC_APB2ENR_USART1EN;
    RCC->AHB1ENR|=RCC_AHB1ENR_GPIOBEN;
    setGPIOMode(GPIOB,6, 0b10);
    setGPIOMode(GPIOB,7, 0b10);
    setGPIOAF(GPIOB, 6, 0x7);
    setGPIOAF(GPIOB, 7, 0x7);
    //Here we didn't setup any system timer. so the frequency of APB2 is 16000000
    //if we want set baud rate to 115200 with 16x overstamping
    //the num in BRR should be like 8.6805(16000000/115200/16)
    //So the Mantissa should be 0d8,and Fraction should be 11 (11/16)
    USART1->BRR=0x8B;
    //ENABLE USART ,transmitter,interuption for TXE
    USART1->CR1|=USART_CR1_TXEIE|USART_CR1_UE|USART_CR1_TE;
    HAL_NVIC_EnableIRQ(USART1_IRQn);
}
int main() {
    
    RCC->AHB1ENR|=RCC_AHB1ENR_GPIOGEN;
    RCC->APB2ENR|=RCC_APB2ENR_SYSCFGEN;
    InitPG6();
    initUSART1();
    //InitEXTI();
    //initNVIC();
    //HAL_SYSTICK_Config(SystemCoreClock/20);
    //HAL_NVIC_EnableIRQ(SysTick_IRQn);
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
    while(1)
    {
    };
}
