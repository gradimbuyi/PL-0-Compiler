#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int opCode;
    int lex_level;
    int m_address;
} instruction;

/* REGISTERS */
int base_pointer = 0;
int stack_pointer = 0;
int program_counter = 0;
instruction instruction_register;

int main(int argc, char *argv[]) {
    return 0;
}