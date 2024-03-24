#include "types.h"
#include "gdt.h"

// size of screen
#define x_max 80
#define y_max 25

void printf(char *str)
{
    // using VGA text buffer for displaying output
    // for each 2 bytes, 1st byte is color info, 2nd byte is text to be displayed
    static uint16_t *video_memory = (uint16_t *)0xb8000;

    // for current xy coord in screen
    static uint8_t x = 0, y = 0;

    for (int i = 0; str[i]; i++)
    {
        switch (str[i])
        {
        // move to new line if character is \n
        case '\n':
            y++;
            x = 0;
            break;

        default:
            // preserving upper byte (text color) and storing text on lower byte of two bytes
            video_memory[x_max * y + x] = (video_memory[x_max * y + x] & 0xff00) | str[i];
            x++;
            // move to next line if current line is full
            if (x >= x_max)
            {
                y++;
                x = 0;
            }
            // erase all lines if screen is full
            if (y >= y_max)
            {
                for (y = 0; y < y_max; y++)
                {
                    for (x = 0; x < x_max; x++)
                    {
                        video_memory[x_max * y + x] = (video_memory[x_max * y + x] & 0x9d00) | ' ';
                    }
                }
                x = 0;
                y = 0;
            }
        }
    }
}

// extern "C" - to prevent name mangling and ensuring it can be called from assembly code
extern "C" void kernel_main(void *multiboot_structure, uint32_t magic_number)
{
    printf("Hello from os - blue :)\n");
    printf("hope you're doing well!");
    GlobalDescriptorTable gdt;
    while (1)
        ; // infinite loop
}
