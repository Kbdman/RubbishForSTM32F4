#include "common.h"
char buf[512] = {};
uint32_t buflen = 0;
uint32_t idx = 0;
uint32_t clearBit(uint32_t data, uint8_t start, uint8_t len)
{
    if (start + len >= 32 || len == 0)
        return data;
    uint32_t clearbits = 1;
    for (int c = 1; c < len; c++)
    {
        clearbits = (clearbits << 1) + 1;
    }
    clearbits = clearbits << start;
    clearbits = ~clearbits;
    data &= clearbits;
    return data;
}
/**
 * @brief select alternate function for a gpio port
 *
 * @param gpio
 * @param port
 * @param af
 */
void setGPIOAF(GPIO_TypeDef *gpio, uint8_t port, uint8_t af)
{
    volatile uint32_t *AFR = &(gpio->AFR[0]);
    if (port > 7)
    {
        AFR = &(gpio->AFR[1]);
        port -= 7;
    }
    *AFR = clearBit(*AFR, port * 4, 4) | (af << (port * 4));
}
/**
 * @brief Set mode for a gpio port
 *
 * @param gpio
 * @param port
 * @param mode
 */
void setGPIOMode(GPIO_TypeDef *gpio, uint8_t port, uint8_t mode)
{
    gpio->MODER = clearBit(gpio->MODER, port * 2, 2) | (mode << (port * 2));
}
/**
 * @brief send "ABCD....Z" out via USART1
 *
 */
void USART1_IRQHandler()
{
    if (USART1->SR & USART_SR_TXE)
    {
            USART1->DR = buf[idx];
            if(++idx==buflen)
                HAL_NVIC_DisableIRQ(USART1_IRQn);
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
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    setGPIOMode(GPIOB, 6, 0b10);
    setGPIOMode(GPIOB, 7, 0b10);
    setGPIOAF(GPIOB, 6, 0x7);
    setGPIOAF(GPIOB, 7, 0x7);
    // Here we didn't setup any system timer. so the frequency of APB2 is 16000000
    // if we want set baud rate to 115200 with 16x overstamping
    // the num in BRR should be like 8.6805(16000000/115200/16)
    // So the Mantissa should be 0d8,and Fraction should be 11 (11/16)
    USART1->BRR = 0x8B;
    // ENABLE USART ,transmitter,interuption for TXE
    USART1->CR1 |= USART_CR1_TXEIE | USART_CR1_UE | USART_CR1_TE;
}
void Memcpy(char *dst, const char *src, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
    {
        dst[i] = src[i];
    }
}
uint32_t Strlen(const char *src)
{
    uint32_t res = 0;
    while (src[res] != 0)
        ++res;
    return res;
}
void logBuf(const char *msg, uint32_t len)
{
    if (len == 0 || msg == NULL)
        return;
    HAL_NVIC_DisableIRQ(USART1_IRQn);
    buf[0] = '\r';
    buf[1] = '\n';
    if (len > sizeof(buf) - 2)
        len = sizeof(buf) - 2;
    Memcpy(buf+2, msg, len);

    buflen = len + 2;
    idx = 0;
    HAL_NVIC_EnableIRQ(USART1_IRQn);
}
void logString(const char *msg)
{
    logBuf(msg, Strlen(msg));
}