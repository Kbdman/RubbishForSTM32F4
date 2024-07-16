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
/**
 * @brief IRQHandler for USART
 * Keep sending data out from internal buffer
 */
void USART1_IRQHandler();
/**
 * @brief setup usart1
 * Enable USART1
 * Use PB6 PB7 as TX RX port of USART1
 * Enable interrupt for TXE for USART1
 */
void initUSART1();
/**
 * @brief send a string via USART1
 * 
 * @param msg message to be sent
 */
void logString(const char* msg);
/**
 * @brief copy data to internal buffer and trigger send out by enable USART IRQ
 * 
 * @param buf buffer containing the data to be sent
 * @param len length of the data
 */
void logBuf(const char* buf,uint32_t len);
/**
 * @brief count length of a null-terminated string
 * 
 * @param src 
 * @return uint32_t 
 */
uint32_t Strlen(const char *src);
/**
 * @brief memcpy 
 * 
 * @param dst 
 * @param src 
 * @param size 
 */
void Memcpy(char *dst, const char *src, uint32_t size);