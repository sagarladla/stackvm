#include "defs.h"
#include "parser.h"
#include <ctype.h>

#define issign(x) ((x == '-') || (x == '+'))

uint32_t parse_instruction(char *file_path)
{
        FILE *file = fopen(file_path, "r");
        if (!file) {
            perror("Failed to open file");
            return 0;
        }

        char buffer[STACK_MEMORY_SIZE];
        int index = 0;
        while (fgets(buffer, STACK_MEMORY_SIZE, file)) {
                
                while (buffer[index] != ';')
                {
                        if (issign(buffer[index]) && buffer[index + 1] != ' ')
                        {
                                /* code */
                        }
                        
                        fprintf(stdout, "%c", buffer[index]);
                        index++;
                }
                                
        }

        fclose(file);
        return 0;
}

int main(int argc, char const *argv[])
{
        parse_instruction("example/simple.sram");
        // printf("%d", atoi('+'));
        return 0;
}
