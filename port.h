#ifndef __PORT_H
#define __PORT_H

#include "types.h"

// cpu communicate to hardware via ports
// requires portnumber and bandwidth
class Port
{
    // only used by derived classes
protected:
    Port(uint16_t portnumber);
    ~Port();
    uint16_t portnumber;
};

class Port8Bit : public Port
{
public:
    Port8Bit(uint16_t portnumber);
    ~Port8Bit();
    // virtual - if same function name in both base and derived class - use derived
    //  write value to port
    virtual void Write(uint8_t data);
    // read value from port
    virtual uint8_t Read();
};

class Port8BitSlow : public Port8Bit
{
public:
    Port8BitSlow(uint16_t portnumber);
    ~Port8BitSlow();
    virtual void Write(uint8_t data);
};

class Port16Bit : public Port
{
public:
    Port16Bit(uint16_t portnumber);
    ~Port16Bit();
    virtual void Write(uint16_t data);
    virtual uint16_t Read();
};

class Port32Bit : public Port
{
public:
    Port32Bit(uint16_t portnumber);
    ~Port32Bit();
    virtual void Write(uint32_t data);
    virtual uint32_t Read();
};

#endif