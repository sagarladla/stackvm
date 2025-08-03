/***
 * 
 * @file: stackvm.c
 * @author: Sagarrajvarman Ladla
 * @date: 2025-08-03
 * @brief: This file contains the implementation of the stack virtual machine (StackVM)
 * @version: 1.0
 * @license: MIT License
 * @note: This project is developed using the C23 language standard version.
 * 
 */

#include "defs.h"
#include "stackvm.h"

static stackvm_t *vm;

static int instruction_stage_counter = 0;
static const stage_t instruction_stage[] =
{
        fetch_instruction,   // Fetch stage
        decode_instruction,  // Decode stage
        execute_instruction, // Execute stage
};

stackvm_t *stackvm_ctxt()
{
        if (!vm)
        {
                vm = (stackvm_t *)malloc(sizeof(stackvm_t));    // Allocate memory for the VM context
                if (!vm)
                {
                        return NULL;                            // Return NULL if memory allocation fails
                }
                stackvm_init(vm);                               // Initialize the VM context
        }
        return vm; // Return the VM context
}

void stackvm_init(stackvm_t *vm)
{
        vm->pc     = 0;                                                     // Initialize program counter
        vm->sp     = -1;                                                    // Initialize stack pointer
        vm->memory = (void *)malloc(STACK_MEMORY_SIZE * sizeof(uint32_t));  // Allocate memory
        vm->type   = 0;                                                     // Instruction data type
        vm->data   = 0;                                                     // Instruction data register
        vm->state  = STATE_RESET;                                           // Set initial state
        vm->stage  = NULL;                                                  // Set initial stage to STATE_RESET
        return;
}

void stackvm_free(stackvm_t *vm)
{
        if (vm->memory)
        {
                free(vm->memory);                                           // Free allocated memory
                vm->memory = NULL;                                          // Set pointer to NULL after freeing
        }
        if (vm)
        {
                free(vm);                                                  // Free the VM context
                vm = NULL;                                                 // Set pointer to NULL after freeing
        }
        return;
}

void stackvm_reset(stackvm_t *vm)
{
        vm->pc     = 0;                                                 // Reset program counter
        vm->sp     = -1;                                                // Reset stack pointer
        vm->type   = 0;                                                 // Reset instruction type
        vm->data   = 0;                                                 // Reset data register
        vm->state  = STATE_RESET;                                       // Reset state function pointer to START
        vm->stage  = NULL;                                              // Reset stage to STATE_RESET
        memset(vm->memory, 0, STACK_MEMORY_SIZE * sizeof(uint32_t));    // Clear memory
        return;
}

void stackvm_run(stackvm_t *vm)
{
        vm->pc--;                                                       // Decrement program counter to start from the first instruction
        vm->state = STATE_RUN;                                          // Set the VM state to RUN
        vm->stage = instruction_stage[instruction_stage_counter++];     // Set the initial stage to fetch
        while (vm->state != STATE_HALT)
        {
                if (vm->stage)
                {
                        vm->stage(vm);
                        vm->stage = instruction_stage[
                                instruction_stage_counter++ 
                                % (sizeof(instruction_stage) / sizeof(instruction_stage[0]))
                        ]; // Move to the next stage
                }
                else
                {
                        fprintf(stderr, "Error: Invalid stage function pointer\n");
                        vm->state = STATE_HALT; // Set state to HALT if stage function pointer is invalid
                        break; // Exit the loop if stage function pointer is invalid
                }
        }
        return;
}

void load_program(void *program, size_t size)
{
        if (!program || !vm || !vm->memory)
        {
                fprintf(stderr, "Error: Invalid program or VM context\n");
                return; // Return if the program or VM context is invalid
        }

        // Load the program into the VM memory
        for (size_t i = 0; i < size; i++)
        {
                ((uint32_t *)(vm->memory))[vm->pc + i] = ((uint32_t *)program)[i];
        }
        fprintf(stdout, "Loaded program of size %zd bytes...\n", size);
}

void fetch_instruction(stackvm_t *vm)
{
        vm->pc++; // Increment program counter to point to the current instruction
        return;
}

void decode_instruction(stackvm_t *vm)
{
        uint32_t instruction = ((uint32_t *)(vm->memory))[vm->pc];               // Fetch the instruction from memory
        vm->type             = GET_TYPE(instruction);                            // Decode the type of instruction
        vm->data             = GET_DATA(instruction);                            // Decode the data of instruction
        return;
}

void execute_instruction(stackvm_t *vm)
{
        if (vm->type == POSITIVE_INTEGER || vm->type == NEGATIVE_INTEGER)
        {
                vm->sp++; // Increment stack pointer after pushing data
                ((uint32_t *)(vm->memory))[vm->sp] = vm->data; // Push data onto the stack
                
        }
        else if (vm->type == PRIMITIVE_INSTRUCTION)
        {
                switch (vm->data)
                {
                case HALT:
                        fprintf(stdout, "Halting the VM\n");
                        vm->state = STATE_HALT; // Set state to HALT
                        break;
                case ADD:
                        fprintf(stdout, "add %d, %d\n", ((uint32_t *)(vm->memory))[vm->sp - 1], ((uint32_t *)(vm->memory))[vm->sp]);
                        ((uint32_t *)(vm->memory))[vm->sp - 1] += ((uint32_t *)(vm->memory))[vm->sp];
                        vm->sp--; // Pop the stack after operation
                        break;
                case SUB:
                        fprintf(stdout, "sub %d, %d\n", ((uint32_t *)(vm->memory))[vm->sp - 1], ((uint32_t *)(vm->memory))[vm->sp]);
                        // *(uint32_t *)(vm->memory + vm->sp - 1) -= *(uint32_t *)(vm->memory + vm->sp);
                        ((uint32_t *)(vm->memory))[vm->sp - 1] -= ((uint32_t *)(vm->memory))[vm->sp];
                        vm->sp--; // Pop the stack after operation
                        break;
                case MUL:
                        fprintf(stdout, "mul %d, %d\n", ((uint32_t *)(vm->memory))[vm->sp - 1], ((uint32_t *)(vm->memory))[vm->sp]);
                        // *(uint32_t *)(vm->memory + vm->sp - 1) *= *(uint32_t *)(vm->memory + vm->sp);
                        ((uint32_t *)(vm->memory))[vm->sp - 1] *= ((uint32_t *)(vm->memory))[vm->sp];
                        vm->sp--; // Pop the stack after operation
                        break;
                case DIV:
                        fprintf(stdout, "div %d, %d\n", ((uint32_t *)(vm->memory))[vm->sp - 1], ((uint32_t *)(vm->memory))[vm->sp]);
                        if (((uint32_t *)(vm->memory))[vm->sp] != 0)
                        {
                                // *(uint32_t *)(vm->memory + vm->sp - 1) /= *(uint32_t *)(vm->memory + vm->sp);
                                ((uint32_t *)(vm->memory))[vm->sp - 1] /= ((uint32_t *)(vm->memory))[vm->sp];
                                vm->sp--; // Pop the stack after operation
                        }
                        else
                        {                                // Handle division by zero
                                fprintf(stderr, "Error: Division by zero\n");
                                vm->state = STATE_HALT; // Set state to HALT if division by zero occurs
                        }
                        break;
                case AND:
                        fprintf(stdout, "and %d, %d\n", ((uint32_t *)(vm->memory))[vm->sp - 1], ((uint32_t *)(vm->memory))[vm->sp]);
                        // *(uint32_t *)(vm->memory + vm->sp - 1) &= *(uint32_t *)(vm->memory + vm->sp);
                        ((uint32_t *)(vm->memory))[vm->sp - 1] &= ((uint32_t *)(vm->memory))[vm->sp];
                        vm->sp--; // Pop the stack after operation
                        break;
                case OR:
                        fprintf(stdout, "or %d, %d\n", ((uint32_t *)(vm->memory))[vm->sp - 1], ((uint32_t *)(vm->memory))[vm->sp]);
                        // *(uint32_t *)(vm->memory + vm->sp - 1) |= *(uint32_t *)(vm->memory + vm->sp);
                        ((uint32_t *)(vm->memory))[vm->sp - 1] |= ((uint32_t *)(vm->memory))[vm->sp];
                        vm->sp--; // Pop the stack after operation
                        break;
                case NOT:
                        fprintf(stdout, "not %d\n", ((uint32_t *)(vm->memory))[vm->sp]);
                        // *(uint32_t *)(vm->memory + vm->sp) = ~(*(uint32_t *)(vm->memory + vm->sp));
                        ((uint32_t *)(vm->memory))[vm->sp] = ~((uint32_t *)(vm->memory))[vm->sp];
                        break;
                case XOR:
                        fprintf(stdout, "xor %d, %d\n", ((uint32_t *)(vm->memory))[vm->sp - 1], ((uint32_t *)(vm->memory))[vm->sp]);
                        // *(uint32_t *)(vm->memory + vm->sp - 1) ^= *(uint32_t *)(vm->memory + vm->sp);
                        ((uint32_t *)(vm->memory))[vm->sp - 1] ^= ((uint32_t *)(vm->memory))[vm->sp];
                        vm->sp--; // Pop the stack after operation
                        break;
                case LT:
                        fprintf(stdout, "lt %d, %d\n", ((uint32_t *)(vm->memory))[vm->sp - 1], ((uint32_t *)(vm->memory))[vm->sp]);
                        // Compare the top two elements of the stack for less than
                        if (((uint32_t *)(vm->memory))[vm->sp - 1] <
                            ((uint32_t *)(vm->memory))[vm->sp])
                        {
                                ((uint32_t *)(vm->memory))[vm->sp - 1] = 1; // Set to true
                        }
                        else
                        {
                                ((uint32_t *)(vm->memory))[vm->sp - 1] = 0; // Set to false
                        }
                        vm->sp--; // Pop the stack after operation
                        break;
                
                case LE:
                        fprintf(stdout, "le %d, %d\n", ((uint32_t *)(vm->memory))[vm->sp - 1], ((uint32_t *)(vm->memory))[vm->sp]);
                        if (((uint32_t *)(vm->memory))[vm->sp - 1] <=
                            ((uint32_t *)(vm->memory))[vm->sp])
                        {
                                ((uint32_t *)(vm->memory))[vm->sp - 1] = 1; // Set to true
                        }
                        else
                        {                                ((uint32_t *)(vm->memory))[vm->sp - 1] = 0; // Set to false
                        }
                        vm->sp--; // Pop the stack after operation
                        break;
                case GT:
                        fprintf(stdout, "gt %d, %d\n", ((uint32_t *)(vm->memory))[vm->sp - 1], ((uint32_t *)(vm->memory))[vm->sp]);
                        if (((uint32_t *)(vm->memory))[vm->sp - 1] >
                            ((uint32_t *)(vm->memory))[vm->sp])
                        {
                                ((uint32_t *)(vm->memory))[vm->sp - 1] = 1; // Set to true
                        }
                        else
                        {
                                ((uint32_t *)(vm->memory))[vm->sp - 1] = 0; // Set to false
                        }
                        vm->sp--; // Pop the stack after operation
                        break;
                case GE:
                        fprintf(stdout, "ge %d, %d\n", ((uint32_t *)(vm->memory))[vm->sp - 1], ((uint32_t *)(vm->memory))[vm->sp]);
                        if (((uint32_t *)(vm->memory))[vm->sp - 1] >=
                            ((uint32_t *)(vm->memory))[vm->sp])
                        {
                                ((uint32_t *)(vm->memory))[vm->sp - 1] = 1; // Set to true
                        }
                        else
                        {
                                ((uint32_t *)(vm->memory))[vm->sp - 1] = 0; // Set to false
                        }
                        vm->sp--; // Pop the stack after operation
                        break;
                case EQ:
                        fprintf(stdout, "eq %d, %d\n", ((uint32_t *)(vm->memory))[vm->sp - 1], ((uint32_t *)(vm->memory))[vm->sp]);
                        if (((uint32_t *)(vm->memory))[vm->sp - 1] ==
                            ((uint32_t *)(vm->memory))[vm->sp])
                        {
                                ((uint32_t *)(vm->memory))[vm->sp - 1] = 1; // Set to true
                        }
                        else
                        {
                                ((uint32_t *)(vm->memory))[vm->sp - 1] = 0; // Set to false
                        }
                        vm->sp--; // Pop the stack after operation
                        break;
                case NE:
                        fprintf(stdout, "ne %d, %d\n", ((uint32_t *)(vm->memory))[vm->sp - 1], ((uint32_t *)(vm->memory))[vm->sp]);
                        if (((uint32_t *)(vm->memory))[vm->sp - 1] !=
                            ((uint32_t *)(vm->memory))[vm->sp])
                        {
                                ((uint32_t *)(vm->memory))[vm->sp - 1] = 1; // Set to true
                        }
                        else
                        {
                                ((uint32_t *)(vm->memory))[vm->sp - 1] = 0; // Set to false
                        }
                        vm->sp--; // Pop the stack after operation
                        break;
                case BR:
                        fprintf(stdout, "br %d\n", vm->data);
                        // Branch instruction logic, modifies the program counter
                        vm->pc = vm->data; // Set program counter to the target address
                        break;
                case BRT:
                        fprintf(stdout, "brt %d\n", vm->data);
                        // Branch if true logic, checks the top of the stack
                        if (((uint32_t *)(vm->memory))[vm->sp] != 0)
                        {
                                vm->pc = vm->data; // Set program counter to the target address if true
                        }
                        vm->sp--; // Pop the stack after checking
                        break;
                case BRF:
                        fprintf(stdout, "brf %d\n", vm->data);
                        // Branch if false logic, checks the top of the stack
                        if (((uint32_t *)(vm->memory))[vm->sp] == 0)
                        {
                                vm->pc = vm->data; // Set program counter to the target address if false
                        }
                        vm->sp--; // Pop the stack after checking
                        break;
                case RET:
                        fprintf(stdout, "ret\n");
                        // Return instruction logic, pops the return address from the stack
                        if (vm->sp > 0)
                        {
                                vm->pc = *((uint32_t *)(vm->memory + vm->sp)); // Set program counter to the return address
                                vm->sp--; // Pop the return address from the stack
                        }
                        break;
                default:
                        // Handle undefined instruction
                        vm->state = STATE_HALT; // Set state to HALT if an undefined instruction is encountered
                        fprintf(stderr, "Error: Undefined primitive instruction encountered\n");
                        return;
                }
        }
        else if (vm->type == UNDEFINED_INSTRUCTION)
        {
                fprintf(stderr, "Error: Undefined instruction encountered\n");
                vm->state = STATE_HALT; // Set state to HALT if an undefined instruction is encountered
        }
        fprintf(stdout, "TOP OF STACK: %d\n", ((uint32_t *)(vm->memory))[vm->sp]); // Debug output
}
