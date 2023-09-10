#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#define STACK_SIZE 512
#define PROGRAM_SIZE 512

typedef struct 
{
    int opCode;
    int lex_level;
    int m_address;
} instruction;

char *opCodeName(int opCode);
char *operationName(int m_address);
int find_base_level(int bp, int level);
void fetch();
void execute();

#endif