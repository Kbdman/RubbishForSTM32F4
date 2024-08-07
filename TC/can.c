#include "common.h"
void initCan1OnPD()
{
    // Enable GPIO and function
    RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    // Select alternate function for PD0, PD1
    setGPIOMode(GPIOD, 0, 0b10);
    setGPIOMode(GPIOD, 1, 0b10);
    setGPIOAF(GPIOD, 0, 9);
    setGPIOAF(GPIOD, 1, 9);

    // goto initialization mode
    CAN1->MCR |= CAN_MCR_INRQ;
    while ((CAN1->MSR & CAN_MSR_INAK) == 0)
        ;
}