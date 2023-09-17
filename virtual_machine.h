#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#define MAX_STACK_SIZE 2000
#define MAX_PROGRAM_SIZE 2000
#define ARRAY_SIZE 512
#define ACTIVATION_RECORD_SIZE 100

typedef struct 
{
    int opCode;
    int lex_level;
    int m_address;
    int line_number;
} instruction;

typedef struct
{
   int base_pointer;
   int stack_pointer;
} activation_record;

/**
 * opCodeName(): This function takes one integer parameter denotating the operation code, 
 *               and produces the appropriate 3 letter symbol associated with it.
 * 
 */
char *opCodeName(int operation);

/**
 * operationName(): This function takes one integer parameter denotating the m address, 
 *                  and produces the appropriate 3 letter symbol associated with it.
 * 
 */
char *operationName(int operation);

/**
 * findBaseLevel(): This function takes in two parameters: an integer denotating the base pointer and
 *                  another denoting a level. The variables are then used to calculate the activation
 *                  record base.
 */
int findBaseLevel(int bp, int level);

/**
 * fetch(): The fetch function transfers the data within memory into the instruction register.
 * 
 */
void fetch();


/**
 * execute(): The execute function performs the Virtual Machine instructions.
 * 
 */
void execute();

/** 
 * readELF(): This function takes as it parameter a string containing the name of a file
 *            that used to produce a file pointer. Within the function, the while loop goes 
 *            through the file, scanning and storing each of the instrutions given. 
 */
void readELF(char *fileName);

/**
 * virtualMachine(): This function runs our virtual machine. After the instructions are stored in memory,
 *                   this function is called to fetch, execute, and print the result of our operation. 
 * 
 */
void virtualMachine();

#endif