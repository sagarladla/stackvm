#include "parser.h"

uint32_t parse_instruction(char *file_path)
{
        FILE *file = fopen(file_path, "r");
        if (!file) {
            perror("Failed to open file");
            return 0;
        }

        char *token;
        while (fscanf(file, "%x", &token) == 1) {
                switch (*token)
                {
                case '-':
                        /* code */
                        break;
                case '+':
                        /* code */
                        break;
                case '*':
                        /* code */
                        break;
                case '/':
                        /* code */
                        break;
                case '%':
                        /* code */
                        break;
                case '&':
                        /* code */
                        break;
                case '|':
                        /* code */
                        break;
                case '^':
                        /* code */
                        break;
                case '~':
                        /* code */
                        break;
                case '<':
                        /* code */
                        break;
                case '>':
                        /* code */
                        break;
                case '=':
                        /* code */
                        break;
                case '!':
                        /* code */
                        break;
                default:
                        break;
                }
            uint32_t opcode = GET_OPCODE(instr);
            uint32_t operand = GET_DATA(instr);
            fclose(file);
            return operand;
        }

        fclose(file);
        return 0;
}