# constants to identify a multiboot-compliant kernel

.set MAGIC, 0x1badb002
.set FLAGS, (1<<0 | 1<<1)
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
    .long MAGIC
    .long FLAGS
    .long CHECKSUM

.section .text
.extern kernel_main
.extern callConstructors
.global loader

loader:
    # stack pointer is not set at the start of bootloader, (.cpp needs stack pointer set)
    # manually set stack pointer to kernel stack
    mov $kernel_stack ,%esp 
    # to initialise global/static objects
    call callConstructors
    # saving multiboot_structure and magic_number given by bootloader on stack
    push %eax
    push %ebx
    # transfer control to kernel
    call kernel_main

# infinite loop 
_stop: 
    cli # clear interrupt flag , ignores maskable interrupts
    hlt # halt the CPU until next interrupt
    jmp _stop

# 2 MiB for kernel_stack
.section .bss
.space 2*1024*1024 

kernel_stack:
