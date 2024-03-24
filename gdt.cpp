#include "gdt.h"

// members initializer list
GlobalDescriptorTable::GlobalDescriptorTable()
    : nullSegmentDescriptor(0, 0, 0),
      unusedSegmentDescriptor(0, 0, 0),
      codeSegmentDescriptor(0, 64 * 1024 * 1024, 0x9a),
      // 0x9a - 10011010 - present bit (valid segment), code/data segment, executable - code segment, readable (not writable)
      dataSegmentDescriptor(0, 64 * 1024 * 1024, 0x92)
//  0x92 - 10010010 - present bit (valid segment), code/data segment, not executable - data segment, writable (readable)
{
    uint32_t i[2];                              // i is gdt descriptor
    i[1] = (uint32_t)this;                      // low 4 bytes for gdt base address
    i[0] = sizeof(GlobalDescriptorTable) << 16; // shift by 2 bytes since lggt instruction takes only 6 bytes
    // high 2 bytes for gdt size

    // inline assembly, volatile - has side effects, no optimise
    asm volatile("lgdt (%0)" : : "p"(((uint8_t *)i) + 2));
    //'p' - address operand
    // + 2 - to skip 16 bit limit field and point to 32 bit base address
}

GlobalDescriptorTable::~GlobalDescriptorTable()
{
}

// returns offset of datasegment from global descriptor table
uint16_t GlobalDescriptorTable::DataSegmentDescriptor()
{
    return (uint8_t *)&dataSegmentDescriptor - (uint8_t *)this;
}

// returns offset of codesegment from global descriptor table
uint16_t GlobalDescriptorTable::CodeSegmentDescriptor()
{
    return (uint8_t *)&codeSegmentDescriptor - (uint8_t *)this;
}

GlobalDescriptorTable::SegmentDescriptor::SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t flags)
{
    // pointer to a segment entry in gdt
    uint8_t *target = (uint8_t *)this;

    if (limit <= 65536)
    {
        // 16 bit address space
        target[6] = 0x40;
    }
    else
    {
        // 32 bit address space

        if ((limit & 0xfff) != 0xfff)
        {
            // discard 12 lsbits only if they are all 1 - implicitly still there
            limit = (limit >> 12) - 1;
        }
        else
        {
            // if they are not all 1 - set them to 1 - this increases limit - compensate by decreasing higher bit - may have upto 2^12 = 4096 wasted bytes
            limit = limit >> 12;
            target[6] = 0xc0;
        }
    }

    // spread the limit to 2.5 bytes - 0, 1, 6
    target[0] = limit & 0xff;
    target[1] = (limit >> 8) & 0xff;
    target[6] |= (limit >> 16) * 0xf;

    // spread the base address to 4 bytes - 2, 3, 4, 7
    target[2] = base & 0xff;
    target[3] = (base >> 8) & 0xff;
    target[4] = (base >> 16) & 0xff;
    target[7] = (base >> 24) & 0xff;

    // set flags to byte - 5
    target[5] = flags;
}

// calculate base address from a segment entry
uint32_t GlobalDescriptorTable::SegmentDescriptor::Base()
{
    uint8_t *target = (uint8_t *)this;

    uint32_t result = target[7];
    result = (result << 8) + target[4];
    result = (result << 8) + target[3];
    result = (result << 8) + target[2];

    return result;
}

// calculate limit from a segment entry
uint32_t GlobalDescriptorTable::SegmentDescriptor::Limit()
{
    uint8_t *target = (uint8_t *)this;

    uint32_t result = target[6] & 0xf;
    result = (result << 8) + target[1];
    result = (result << 8) + target[0];

    // for 32 bit address space, retreive the 12 lsbits
    if ((target[6] & 0xc0) == 0xc0)
        result = (result << 12) | 0xfff;

    return result;
}
