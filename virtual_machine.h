#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#define MAX_STACK_SIZE 2000
#define MAX_PROGRAM_SIZE 2000

typedef struct 
{
    int opCode;
    int lex_level;
    int m_address;
    int line_number;
} instruction;

char *opCodeName(int opCode);
char *operationName(int m_address);

int find_base_level(int bp, int level);

void fetch();
void execute();
void readELF(char *fileName);
void virtualMachine();

#endif