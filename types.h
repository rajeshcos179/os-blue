// check if the macro is defined else define it 
// to prevent redeclarations if file is included multiple times

#ifndef __TYPES_H
#define __TYPES_H

    typedef char int8;
    typedef unsigned char uint8;
    typedef short int16;
    typedef unsigned short uint16;
    typedef int int32;
    typedef unsigned int uint32;
    typedef long long int int64;
    typedef unsigned long long int uint64;

#endif