/***
 * 
 * @file: stackvm.h
 * @author: Sagarrajvarman Ladla
 * @date: 2025-08-03
 * @brief: This header file defines the stack virtual machine (StackVM) structure and its operations
 * @version: 1.0
 * @license: MIT License
 * @note: This project is developed using the C23 language standard version.
 * 
 */

#ifndef STACK_VM_H
#define STACK_VM_H

#define GET_TYPE(instr)         (((instr) & 0xc0000000) >> 30) // Macro to get the type of instruction
#define GET_DATA(instr)         (((instr) & 0x3fffffff))       // Macro to get the data of instruction
#define GET_OPCODE(instr)       (((instr) | 0x40000000))       // Macro to create an instruction from PRIMITIVE_INSTRUCTION_TYPE

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief State of the StackVM
 * @note This enum defines the possible states of the StackVM.
 *       The state can be RESET, RUN, or HALT.
 *       The state is used to control the execution flow of the StackVM.
 *       It determines whether the StackVM is ready to run, currently running,
 *       or has reached a halt condition.
 */
typedef enum state
{
        STATE_RESET             = 0b00,
        STATE_RUN               = 0b01,
        STATE_HALT              = 0b10,
} state_t;

/**
 * @brief Instruction types for the StackVM
 * @note This enum defines the types of instructions that can be executed by the StackVM.
 *       Each instruction type corresponds to a specific operation or data type.
 *       The instruction types are used to determine how the StackVM should handle the instruction.
 */
typedef enum instruction
{
        POSITIVE_INTEGER        = 0b00,
        PRIMITIVE_INSTRUCTION   = 0b01,
        NEGATIVE_INTEGER        = 0b10,
        UNDEFINED_INSTRUCTION   = 0b11,
} instr_t;

/**
 * @brief StackVM structure
 * @note This structure represents the state of the StackVM.
 *       It contains the program counter, stack pointer, memory pointer,
 *       instruction type, data register, current state, and a function pointer
 *       to the current instruction stage function.
 */
typedef struct stackvm
{
        uint32_t pc;                       // Program counter
        uint32_t sp;                       // Stack pointer
        void *memory;                      // Pointer to the memory
        instr_t type;                      // Type of the instruction
        uint32_t data;                     // Data register
        state_t state;                     // Current state of the VM
        void (*stage)(struct stackvm *vm); // Current instruction stage function pointer
} stackvm_t;
typedef void (*stage_t)(stackvm_t *vm);    // current instruction stage [Fetch, Decode, Execute]

/**
 * @brief Initializes the StackVM context
 * @param vm Pointer to the StackVM context
 * @return void
 * @note This function should be called to initialize the StackVM context before using it.
 *       It sets the initial state of the StackVM, allocates memory for the stack and
 *       initializes the program counter and stack pointer.
 *       The function will also set the current instruction stage to the fetch stage.
 *       It is important to call this function before using any other StackVM functions.
 *       After calling this function, the StackVM context will be ready to run a program.
 *       It is recommended to call this function only once when creating a new StackVM context.
 *       If the StackVM context is already initialized, this function will reset the state
 *       and reinitialize the memory and registers.
 */
void stackvm_init(stackvm_t *vm);

/**
 * @brief Frees the StackVM context
 * @param vm Pointer to the StackVM context
 * @return void
 * @note This function should be called to free the resources allocated for the StackVM context.
 *       It is important to call this function to avoid memory leaks.
 *       The function will free the memory allocated for the StackVM context and reset the state.
 *       After calling this function, the StackVM context should not be used anymore.
 *       It is recommended to call this function when the StackVM is no longer needed or before
 *       creating a new StackVM context.
 */
void stackvm_free(stackvm_t *vm);

/**
 * @brief Resets the StackVM context
 * @param vm Pointer to the StackVM context
 * @return void
 * @note This function resets the StackVM context to its initial state.
 *       It clears the stack, resets the program counter and stack pointer,
 *       and sets the current instruction stage to the fetch stage.
 *       This function is useful when you want to run a new program on the same StackVM context
 *       without creating a new context. It allows you to reuse the existing StackVM context
 *       and start fresh without having to free and reinitialize it.
 *       After calling this function, the StackVM context will be ready to run a new program
 *       with the same memory and registers.
 *       It is recommended to call this function before loading a new program into the StackVM.
 *       This function does not free the memory allocated for the StackVM context,
 *       it only resets the state and clears the stack.
 *       If you want to completely free the StackVM context, you should call `stackvm_free(vm)` instead.
 *       This function is typically called when you want to restart the StackVM without
 *       creating a new context, for example, when running multiple programs in a loop.
 */
void stackvm_reset(stackvm_t *vm);

/**
 * @brief Runs the StackVM
 * @param vm Pointer to the StackVM context
 * @return void
 * @note This function starts the execution of the StackVM.
 *       It will fetch, decode, and execute instructions until the VM reaches a HALT state.
 *       The function will continuously call the current instruction stage function
 *       until the state of the StackVM is set to HALT.
 *       It is important to call this function after loading a program into the StackVM memory.
 *       The function will handle the execution flow of the StackVM, including fetching instructions,
 *       decoding them, and executing the corresponding operations.
 *       The function will also handle any errors that may occur during the execution of the program.
 *       If an error occurs, the function will print an error message and set the state to HALT.
 *       After the execution is complete, the function will return control to the caller.
 *       It is recommended to call this function only once after loading a program into the StackVM
 *       and initializing the StackVM context.
 */
void stackvm_run(stackvm_t *vm);

/**
 * @brief Creates a new StackVM context
 * @return Pointer to the StackVM context on success, NULL on failure
 * @note This function allocates memory for a new StackVM context and initializes it.
 *       It is important to call this function before using any other StackVM functions.
 *       The function will allocate memory for the StackVM context, initialize the registers,
 *       set the initial state to RESET, and set the current instruction stage to the fetch stage
 *       It is recommended to call this function only once when creating a new StackVM context.
 *       After calling this function, the StackVM context will be ready to run a program.
 */
stackvm_t *stackvm_ctxt();

/**
 * @brief Loads a program into the StackVM memory
 * @param program Pointer to the program to be loaded
 * @param size Size of the program
 * @return void
 * @note This function loads a program into the StackVM memory.
 *       It is important to call this function after creating a StackVM context
 *       and before running the StackVM.
 */
void load_program(void *program, size_t size);

/**
 * @brief Fetches the next instruction from the StackVM memory
 * @param vm Pointer to the StackVM context
 * @return void
 * @note This function fetches the next instruction from the StackVM memory
 *       and updates the program counter (pc) accordingly.
 *       It is called in the fetch stage of the StackVM execution cycle.
 */
void fetch_instruction(stackvm_t *vm);

/**
 * @brief Decodes the fetched instruction
 * @param vm Pointer to the StackVM context
 * @return void
 * @note This function decodes the fetched instruction and updates the StackVM state accordingly.
 */
void decode_instruction(stackvm_t *vm);

/**
 * @brief Executes the decoded instruction
 * @param vm Pointer to the StackVM context
 * @return void
 * @note This function executes the decoded instruction based on the current state of the StackVM.
 *       It performs the necessary operations based on the instruction type and updates the StackVM state.
 *       The function will handle different types of instructions, such as arithmetic operations,
 *       logical operations, control flow instructions, and stack manipulation instructions.
 *       It is called in the execute stage of the StackVM execution cycle.
 *       The function will also handle any errors that may occur during the execution of the instruction.
 *       If an error occurs, the function will print an error message and set the state to HALT.
 *       After executing the instruction, the function will update the program counter (pc)
 *       to point to the next instruction to be fetched.
 */
void execute_instruction(stackvm_t *vm);

#endif // STACK_VM_H
