#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>

/**
 * @brief Parses a StackVM instruction
 * @param instr The instruction to parse
 * @return The parsed instruction data
 * @note This function extracts the opcode and operand from the instruction.
 *       It is called in the decode stage of the StackVM execution cycle.
 */
uint32_t parse_instruction(uint32_t instr);

#endif // PARSER_H
