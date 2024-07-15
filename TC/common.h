#include "RTE_Components.h"
#include "stm32f407xx.h"
#include "system_stm32f4xx.h"
#include "stm32f4xx_hal.h"
uint32_t clearBit(uint32_t data,uint8_t start,uint8_t len);
/**
 * @brief select alternate function for a gpio port
 * 
 * @param gpio 
 * @param port 
 * @param af 
 */
void setGPIOAF(GPIO_TypeDef* gpio,uint8_t port,uint8_t  af);
/**
 * @brief Set mode for a gpio port 
 * 
 * @param gpio 
 * @param port 
 * @param mode 
 */
void setGPIOMode(GPIO_TypeDef* gpio,uint8_t port,uint8_t mode);
void USART1_IRQHandler();
void initUSART1();
void logMsg(const char* msg,int len);