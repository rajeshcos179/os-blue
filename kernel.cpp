#include "types.h"

void printf(char* str)
{
    // using VGA text buffer for displaying output
    // for each 2 bytes, 1st byte is color info, 2nd byte is text to be displayed
    static uint16* video_memory = (uint16*) 0xb8000;
    for(int i=0; str[i]; i++)
    {
        // preserving upper byte and storing text on lower byte of two bytes
        video_memory[i] = (video_memory[i] & 0xff00) | str[i];
    }
}

// extern "C" - to prevent name mangling and ensuring it can be called from assembly code 
extern "C" void kernel_main(void* multiboot_structure, uint32 magic_number)
{
    printf("Hello from operating system - blue :)");
    while(1);   // infinite loop 
}
