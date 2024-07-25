
# logs
#### Thu Jul 25 2024
It starts to be boring to learn peripherals. I decide to stop learning peripherals interfaces after I finishing demos for CAN SPI and IIC.            
Maybe it will be time to start port freertos on my board

#### Mon Jul 22 11:19:55 PM CST 2024
Re-implement log function with DMA mode USART1      
To setup DMA mode USART, both USART and DMA  should be well configured by setup registers

#### Thu Jul 18 11:54:10 PM CST 2024
Implement a "Memory to Memory" data copy function by DMA.
It's meaningless as even though DMA do not cost cpu , I still need to spend cpu on wait for the finish if I need to know when the transfer is finished.
Maybe DMA is useful for some scenarios that I don't care about the finish of the tranfer.
Maybe I can use use the DMA to re-implement the USART1 log

#### Tue Jul 16 10:19:33 PM CST 2024
Implement a simple mechanism to log string via USART1       

#### Mon Jul 15 09:41:27 PM CST 2024     
The reason why I didn't make it last night is the incorrect baud rate  
The USART1 is connected to APB2, it works at APB2's frequency.      
As I didn't setup the clocks for system and any peripheral, so they are all work at the default frequency ,which is 16MHZ for STM32F407.        
To configure USART1's buad rate to 115200, I need to calcualte a coefficient based on the USART1's work frequency and my target baud rate,then set it into USART->BRR

      
#### Sun Jul 14 11:11:24 PM CST 2024     
Tried to send data to a TFT232 usb device via uart1.        
The TFT232 was connected to a linux host, the received data is not what I sent.     
The reason is not clear, need more knowledge.       
