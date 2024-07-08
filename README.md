# Introduction
Just cache some rubbish code which I write to learn MCU development     
This project are created in VSCode with "Arm Keil Studio Pack" extensions, and depends on CMSIS packages        
The hardware target is a STM32F407ZGT

## Files
### Library
Most files in RTE folder are copied from some CMSIS packages by "CMSIS Solution" extension automatically.       
It's quite convenient that all I need to do is toggling the library which I need by clicking mouse.

### c
gcc_linker_script.ld.src  is the template for link script. It was also copied  or created by extensions.        
I don't remember if I have edited it.
Seems I defined some symbol which are referred by this startup file.
I spent some time learning the link script, but I think I have forgot most of them.     
#### Simple Introduction for load address
It defined how to link a target file with different sections from different objects by defining some sections like
```
SECTIONS
{
    [... expressions]
    sectionname_1 VMA:[ AT (Load address)]
    {
        ... sections from objects.
        [... expressions]
    }
    ...
    sectionname_x VMA:[ AT (Load address)]
    {
        ... sections from objects.
        [... expressions]
    }
    
}
```      
It specifies where a section is localted in the target file by Load address and where the section is after it being loaded into memory by VMA.
The code referring the resource in section will be linked to the VMA of the resource
Refer to https://ftp.gnu.org/old-gnu/Manuals/ld-2.9.1/html_chapter/ld_3.html for more information       
### Why AT
If we do not use the AT , the load address will be same to the VMA.     
But sometimes we do not want this.      
For example ,       
if we are developing program for embbeded device ,our program are flashed to flash which start at address 0x800000.     
And  the global variable have to be located in flash,which address is 0x80xxxx,otherwise they can not be flashed to the flash.   
So does it means when we want to access the global variable, we need to access the flash?       
In this situation, we can specify a VMA which is located in RAM for the globals variables and let them AT a address in flash.       
Then when the device boots, we copy the initialized value of the global variables from flash to the RAM.
As the code referring the variables are linked to the address in the RAM, everything works.


### Main
Seems only the main.c is totally managed by myself. It contains the functions written by myself.
## Startup.
When STM32F4 boots(not pefectly correct), the flash which at 0x8000000 will be mapped to 0x0.        
The the CPU will load a vector which starts at 0x0.      
The first 4 bytes are used to specify the top of stack, and the second 4 bytes are used to specify the start address for execution.
In this project then start address is pointed to Reset_Handler which implemented in startup_xxxxxx.S.
It do some initialization , like copy initialized global data from flash to RAM, then call the main function implemented  in main.c
 
