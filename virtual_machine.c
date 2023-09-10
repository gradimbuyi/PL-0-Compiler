#include <stdio.h>
#include <stdlib.h>
#include "virtual_machine.h"

/* Process Address Space */
int stack[STACK_SIZE] = {0};
int instructions[PROGRAM_SIZE];

/* REGISTERS */
int base_pointer = 1;
int stack_pointer = 0;
int program_counter = 0;

int halt = -1;

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
}

char *operationName(int m_address)
{
    if(m_address == 0) return "RTN";
    else if(m_address == 1) return "ADD";
    else if(m_address == 2) return "SUB";
    else if(m_address == 3) return "MUL";
    else if(m_address == 4) return "DIV";
    else if(m_address == 5) return "EQL";
    else if(m_address == 6) return "NEQ";
    else if(m_address == 7) return "LSS";
    else if(m_address == 8) return "LEQ";
    else if(m_address == 9) return "GTR";
    else if(m_address == 10) return "GEQ";
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
    // Pushes a constant value (literal) M onto the stack
    if(instruction_register.opCode == 1) 
    {
        stack_pointer = stack_pointer + 1;
        stack[stack_pointer] = instruction_register.m_address;
    }

    // Operation to be performed on the data at the top of the stack. (or return from function)
    else if(instruction_register.opCode == 2)
    {
        if(instruction_register.m_address == 0)
        {
            stack_pointer = base_pointer - 1;
            base_pointer = stack[stack_pointer + 2];
            program_counter = stack[stack_pointer + 3];
        }
        else if(instruction_register.m_address == 1)
        {
            stack[stack_pointer - 1] = stack[stack_pointer - 1] + stack[stack_pointer];
            stack_pointer = stack_pointer - 1;
        }
        else if(instruction_register.m_address == 2)
        {
            stack[stack_pointer - 1] = stack[stack_pointer - 1] - stack[stack_pointer];
            stack_pointer = stack_pointer - 1;
        }
        else if(instruction_register.m_address == 3)
        {
            stack[stack_pointer - 1] = stack[stack_pointer - 1] * stack[stack_pointer];
            stack_pointer = stack_pointer - 1;
        }
        else if(instruction_register.m_address == 4)
        {
            stack[stack_pointer - 1] = stack[stack_pointer - 1] / stack[stack_pointer];
            stack_pointer = stack_pointer - 1;
        }
        else if(instruction_register.m_address == 5)
        {
            stack[stack_pointer - 1] = stack[stack_pointer - 1] == stack[stack_pointer];
            stack_pointer = stack_pointer - 1;
        }
        else if(instruction_register.m_address == 6)
        {
            stack[stack_pointer - 1] = stack[stack_pointer - 1] != stack[stack_pointer];
            stack_pointer = stack_pointer - 1;
        }
        else if(instruction_register.m_address == 7)
        {
            stack[stack_pointer - 1] = stack[stack_pointer - 1] < stack[stack_pointer];
            stack_pointer = stack_pointer - 1;
        }
        else if(instruction_register.m_address == 8)
        {
            stack[stack_pointer - 1] = stack[stack_pointer - 1] <= stack[stack_pointer];
            stack_pointer = stack_pointer - 1;
        }
        else if(instruction_register.m_address == 9)
        {
            stack[stack_pointer - 1] = stack[stack_pointer - 1] > stack[stack_pointer];
            stack_pointer = stack_pointer - 1;
        }
        else if(instruction_register.m_address == 10)
        {
            stack[stack_pointer - 1] = stack[stack_pointer - 1] >= stack[stack_pointer];
            stack_pointer = stack_pointer - 1;
        }
    }

    // Load value to top of stack from the stack location at offset M in AR located L lexicographical levels down
    else if(instruction_register.opCode == 3)
    {
        stack_pointer = stack_pointer + 1;
        stack[stack_pointer] = stack[find_base_level(base_pointer, instruction_register.lex_level) + instruction_register.m_address];
    }

    // Store value at top of stack in stack location at offset M in AR located L lexicographical levels down
    else if(instruction_register.opCode == 4)
    {
        stack[find_base_level(base_pointer, instruction_register.lex_level) + instruction_register.m_address] = stack[stack_pointer];
        stack_pointer = stack_pointer - 1;
    }

    // Call procedure at code index M (generates new Activation Record and PC <- M)
    else if(instruction_register.opCode == 5)
    {
        stack[stack_pointer + 1] = find_base_level(base_pointer, instruction_register.lex_level);
        stack[stack_pointer + 2] = base_pointer;
        stack[stack_pointer + 3] = program_counter;
        base_pointer = stack_pointer + 1;
        program_counter = instruction_register.m_address;
    }

    // Allocates M memory words (increment SP by M)
    else if(instruction_register.opCode == 6) stack_pointer = stack_pointer + instruction_register.m_address;

    // Jump to instruction M (PC <- M)
    else if(instruction_register.opCode == 7) program_counter = instruction_register.m_address;

    // Jump to instruction M if topstack element is 0
    else if(instruction_register.opCode == 8)
    {
        if(stack[stack_pointer] == 0) program_counter = instruction_register.m_address;
        stack_pointer = stack_pointer - 1;
    }

    // System operations
    else if(instruction_register.opCode == 9)
    {
        // Write the top stack element to the screen
        if(instruction_register.m_address == 1) 
        {
            printf("%d", stack[stack_pointer]);
            stack_pointer = stack_pointer - 1;
        }

        // Read in input from the user and store it on top of the stack
        else if(instruction_register.m_address == 2)
        {
            stack_pointer = stack_pointer + 1;
            printf("%d", stack[stack_pointer]);
        }

        // End of program (Set halt flag to zero)
        else if(instruction_register.m_address == 3)
        {
            halt = 0;
        }
    }
}

int main(int argc, char *argv[])
{
    return 0;
}