/***
 * 
 * @file: main.c
 * @author: Sagarrajvarman Ladla
 * @date: 2025-08-03
 * @brief: This file contains the main function for the StackVM simulator
 * @version: 1.0
 * @license: MIT License
 * @note: This project is developed using the C23 language standard version.
 * 
 */

#include "defs.h"
#include "stackvm.h"

int main(int argc, char const *argv[])
{
        // printf("Stack VM Simulator\n");
        // printf("This is a simple stack-based virtual machine simulator.\n");
        // printf("It supports basic arithmetic, logical, and control flow operations.\n");
        // printf("You can use it to run stack-based programs written in a custom assembly language.\n");
        // printf("For more information, refer to the documentation.\n");
        // printf("Press any key to exit...\n");

        stackvm_t *vm = stackvm_ctxt();
        if (!vm)
        {
                fprintf(stderr, "Error: Failed to create VM context\n");
                return EXIT_FAILURE; // Exit with error
        }
        uint32_t program[] = {3, 4, GET_OPCODE(DIV), 6, GET_OPCODE(MUL), 8, 9, GET_OPCODE(ADD), GET_OPCODE(SUB), 10, GET_OPCODE(ADD), GET_OPCODE(HALT)}; // Load your program here
        size_t size = sizeof(program) / sizeof(program[0]); // Set the size of the program
        load_program(program, size); // Load the program into the VM memory
        stackvm_run(vm); // Start the VM
        stackvm_free(vm); // Free the VM context
        return EXIT_SUCCESS;
}
