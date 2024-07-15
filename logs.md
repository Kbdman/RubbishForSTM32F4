
# logs

#### Mon Jul 15 09:41:27 PM CST 2024     
The reason why I didn't make it last night is the incorrect baud rate  
The USART1 is connected to APB2, it works at APB2's frequency.      
As I didn't setup the clocks for system and any peripheral, so they are all work at the default frequency ,which is 16MHZ for STM32F407.        
To configure USART1's buad rate to 115200, I need to calcualte a coefficient based on the USART1's work frequency and my target baud rate,then set it into USART->BRR

      
#### Sun Jul 14 11:11:24 PM CST 2024     
Tried to send data to a TFT232 usb device via uart1.        
The TFT232 was connected to a linux host, the received data is not what I sent.     
The reason is not clear, need more knowledge.       