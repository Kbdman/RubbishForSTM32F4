#include "common.h"
char buf[512]={};
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
/**
 * @brief send "ABCD....Z" out via USART1
 * 
 */
void USART1_IRQHandler()
{
    static char data='A';
    if(USART1->SR&USART_SR_TXE)
    {
        USART1->DR=data;
        if(data=='\n')
            data='A';
        else if(data=='Z')
        {
            data='\r';
        }else if(data =='\r'){
            data='\n';
        }
        else {
            data++;
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
void logMsg(const char* msg,int len)
{
    buf[0]=msg[0];
}