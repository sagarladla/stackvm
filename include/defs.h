/***
 * 
 * @file: defs.h
 * @author: Sagarrajvarman Ladla
 * @date: 2025-08-03
 * @brief: This header file defines the primitive instruction set and the flag register
 * @version: 1.0
 * @license: MIT License
 * @note: This project is developed using the C23 language standard version.
 * 
 */


#ifndef DEFS_H
#define DEFS_H

typedef enum PRIMITIVE_INSTRUCTION_TYPE
{
        HALT, // Halt the VM

        // Arithmetic instructions
        ADD,
        SUB,
        MUL,
        DIV,

        // Logical instructions
        AND,
        OR,
        NOT,
        XOR,
        LT,
        GT,
        LE,
        GE,
        EQ,
        NE,

        // Control flow instructions
        BR,
        BRT,
        BRF,
        RET,
} PRIMITIVE_INSTRUCTION_TYPE;


/**
 * FLAG REGISTER
 * ---------------------------------------------------------------------------------
 * |    Z    |    N    |    C    |    V    |    I    |    R    |    R    |    R    |
 * ---------------------------------------------------------------------------------
 * |    7    |    6    |    5    |    4    |    3    |    2    |    1    |    0    |
 * 
 * Z - Zero flag
 * N - Negative flag
 * C - Carry flag
 * V - Overflow flag
 * I - Interrupt flag
 * R - Reserved flag
 * R - Reserved flag
 * R - Reserved flag
 */
// unsigned char FLAG_REGISTER;

#endif // DEFS_H