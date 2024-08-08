#include "common.h"
void initCan1OnPD()
{
    // Enable GPIO and function
    RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    // Select Mode and alternate function for PD0, PD1
    setGPIOMode(GPIOD, 0, 0b10);
    setGPIOMode(GPIOD, 1, 0b10);
    setGPIOAF(GPIOD, 0, 9);
    setGPIOAF(GPIOD, 1, 9);

    // goto initialization mode
    CAN1->MCR |= CAN_MCR_INRQ;
    CAN1->MCR ^= CAN_MCR_SLEEP;
    while ((CAN1->MSR & CAN_MSR_INAK) == 0)
        ;
    CAN1->BTR = 0x011c0000;
    // leave Initialization mode
    CAN1->MCR ^= CAN_MCR_INRQ;
    while ((CAN1->MSR & CAN_MSR_INAK) == 0)
        ;

    // set frame type to standard frame
    CAN1->sTxMailBox[0].TIR = clearBit(CAN1->sTxMailBox[0].TIR, CAN_TI0R_IDE_Pos, 1);
    while (1)
    {
        // wait for unfinished sending
        while (CAN1->sTxMailBox[0].TIR & CAN_TI0R_TXRQ == 1)
            ;
        // set data length code and data
        CAN1->sTxMailBox[0].TDTR = setBits(CAN1->sTxMailBox[0].TDTR, 0, 4, 8);
        CAN1->sTxMailBox[0]
            .TDLR = 0x01020304;
        CAN1->sTxMailBox[0].TDHR = 0x05060708;
        // trigger send
        CAN1->sTxMailBox[0].TIR |= CAN_TI0R_TXRQ;
    }
}
// 0x9F42FB44