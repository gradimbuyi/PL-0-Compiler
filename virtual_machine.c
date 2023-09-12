#include <stdio.h>
#include <stdlib.h>
#include "virtual_machine.h"

/* Process Address Space */
int stack[MAX_STACK_SIZE] = {0};
int instructions[MAX_PROGRAM_SIZE];
int instruction_size = 0;
int halt = -1;

/* REGISTERS */
int base_pointer = 1;
int stack_pointer = 69;
int program_counter = 0;
instruction instruction_register;

char *opCodeName(int opCode)
{
    if(opCode == 1) return "LIT";
    else if(opCode == 2) return "OPR";
    else if(opCode == 3) return "LOD";
    else if(opCode == 4) return "STO";
    else if(opCode == 5) return "CAL";
    else if(opCode == 6) return "INC";
    else if(opCode == 7) return "JMP";
    else if(opCode == 8) return "JCP";
    else if(opCode == 9) return "SYS"; 
    else return NULL;
}

/* Find base level */
int find_base_level(int bp, int level)
{
    int activation_record_base = bp;
    while(level > 0)
    {
        activation_record_base = stack[activation_record_base];
        level--;
    }

    return activation_record_base;
}

/* Fetch cycle */
void fetch() 
{
    instruction_register.opCode = instructions[program_counter];
    instruction_register.lex_level = instructions[program_counter + 1];
    instruction_register.m_address = instructions[program_counter + 2];
    program_counter = program_counter + 3;
}

/* Execute cycle */
void execute()
{
    int opCode = instruction_register.opCode;
    int lex_level = instruction_register.lex_level;
    int m_address = instruction_register.m_address;

    switch (opCode)
    {
        case 1: // Pushes a constant value (literal) M onto the stack
            stack_pointer = stack_pointer + 1;
            stack[stack_pointer] = m_address;
            break;

        case 2:  // Operation to be performed on the data at the top of the stack. (or return from function)
            switch (m_address)
            {
                case 0:
                    stack_pointer = base_pointer - 1;
                    base_pointer = stack[stack_pointer + 2];
                    program_counter = stack[stack_pointer + 3];
                    break;

                case 1:
                    stack[stack_pointer - 1] = stack[stack_pointer - 1] + stack[stack_pointer];
                    stack_pointer = stack_pointer - 1;
                    break;

                case 2:
                    stack[stack_pointer - 1] = stack[stack_pointer - 1] - stack[stack_pointer];
                    stack_pointer = stack_pointer - 1;
                    break;

                case 3:
                    stack[stack_pointer - 1] = stack[stack_pointer - 1] * stack[stack_pointer];
                    stack_pointer = stack_pointer - 1;
                    break;

                case 4:
                    stack[stack_pointer - 1] = stack[stack_pointer - 1] / stack[stack_pointer];
                    stack_pointer = stack_pointer - 1;
                    break;

                case 5:
                    stack[stack_pointer - 1] = stack[stack_pointer - 1] == stack[stack_pointer];
                    stack_pointer = stack_pointer - 1;
                    break;

                case 6:
                    stack[stack_pointer - 1] = stack[stack_pointer - 1] != stack[stack_pointer];
                    stack_pointer = stack_pointer - 1;
                    break;

                case 7:
                    stack[stack_pointer - 1] = stack[stack_pointer - 1] < stack[stack_pointer];
                    stack_pointer = stack_pointer - 1;
                    break;

                case 8:
                    stack[stack_pointer - 1] = stack[stack_pointer - 1] <= stack[stack_pointer];
                    stack_pointer = stack_pointer - 1;
                    break;

                case 9:
                    stack[stack_pointer - 1] = stack[stack_pointer - 1] > stack[stack_pointer];
                    stack_pointer = stack_pointer - 1;
                    break;

                case 10:
                    stack[stack_pointer - 1] = stack[stack_pointer - 1] >= stack[stack_pointer];
                    stack_pointer = stack_pointer - 1;
                    break;

                default:
                    break;
            }

        case 3: // Load value to top of stack from the stack location at offset M in AR located L lexicographical levels down
            stack_pointer = stack_pointer + 1;
            stack[stack_pointer] = stack[find_base_level(base_pointer, lex_level) + m_address];
            break;

        case 4: // Store value at top of stack in stack location at offset M in AR located L lexicographical levels down
            stack[find_base_level(base_pointer, lex_level) + m_address] = stack[stack_pointer];
            stack_pointer = stack_pointer - 1;
            break;

        case 5: // Call procedure at code index M (generates new Activation Record and PC <- M)
            stack[stack_pointer + 1] = find_base_level(base_pointer, lex_level);
            stack[stack_pointer + 2] = base_pointer;
            stack[stack_pointer + 3] = program_counter;
            base_pointer = stack_pointer + 1;
            program_counter = m_address;
            break;

        case 6: // Allocates M memory words (increment SP by M)
            stack_pointer = stack_pointer + m_address;
            break;

        case 7: // Jump to instruction M (PC <- M)
            program_counter = m_address;
            break;

        case 8: // Jump to instruction M if topstack element is 0
            if(stack[stack_pointer] == 0) program_counter = m_address;
            stack_pointer = stack_pointer - 1;
            break;

        case 9: // System operations
            switch (m_address)
            {
                case 1: // Write the top stack element to the screen
                    printf("%d", stack[stack_pointer]);
                    stack_pointer = stack_pointer - 1;
                    break;
            
                case 2: // Read in input from the user and store it on top of the stack
                    stack_pointer = stack_pointer + 1;
                    printf("%d", stack[stack_pointer]);
                    break;

                case 3: // End of program (Set halt flag to zero)
                    halt = 0;
                    break;

                default:
                    break;
            }
    
        default:
            break;
    }
}

void readELF(char *fileName)
{
    int location = 0;
    FILE *input_file = fopen(fileName, "r");

    do {
        fscanf(input_file, "%d %d %d", &instructions[location], &instructions[location + 1], &instructions[location + 2]);
        location = location + 3;
        instruction_size = instruction_size + 3;

    } while(feof(input_file) == 0);

    fclose(input_file);
}

void virtualMachine()
{
    fprintf(stderr, "\t\t   PC  BP  SP  stack\n");
    fprintf(stderr, "Initial values:    %2d  %2d  %2d\n", program_counter, base_pointer, stack_pointer);

    while(halt != 0)
    {
        fetch();
        execute();
    }
}


int main(int argc, char *argv[])
{
    if(argc == 2) 
    {
        readELF(argv[1]); 
        virtualMachine();
    }

    return 0;
}