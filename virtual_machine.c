#include <stdio.h>
#include <stdlib.h>
#include "virtual_machine.h"

/* Starts and stops the program */
int halt = -1;

/* Process Address Space */
int process_address_space[MAX_PROGRAM_SIZE];

/* REGISTERS */
int base_pointer;
int stack_pointer;
int program_counter;

/* Instruction Register */
int instruction_size;
instruction instruction_register;

/* Activation Records */
int record_size = 0;
activation_record records[MAX_ACTIVATION_RECORD_SIZE];

char *opCodeName(int operation)
{
         if(operation == 1) return "LIT";
    else if(operation == 2) return operationName(instruction_register.m_address);
    else if(operation == 3) return "LOD";
    else if(operation == 4) return "STO";
    else if(operation == 5) return "CAL";
    else if(operation == 6) return "INC";
    else if(operation == 7) return "JMP";
    else if(operation == 8) return "JCP";
    else if(operation == 9) return "SYS"; 
    else return NULL;
}

char *operationName(int operation)
{
         if(operation == 0) return "RTN";
    else if(operation == 1) return "ADD";
    else if(operation == 2) return "SUB";
    else if(operation == 3) return "MUL";
    else if(operation == 4) return "DIV";
    else if(operation == 5) return "EQL";
    else if(operation == 6) return "NEQ";
    else if(operation == 7) return "LSS";
    else if(operation == 8) return "LEQ";
    else if(operation == 9) return "GTR";
    else if(operation == 10) return "GEQ";
    else return NULL;
}

/* Find base level */
int findBaseLevel(int bp, int level)
{
    int activation_record_base = bp;
    while(level > 0)
    {
        activation_record_base = process_address_space[activation_record_base];
        level--;
    }

    return activation_record_base;
}

void fetch() 
{
    instruction_register.opCode = process_address_space[program_counter];
    instruction_register.lex_level = process_address_space[program_counter + 1];
    instruction_register.m_address = process_address_space[program_counter + 2];
    program_counter = program_counter + 3;
}

void execute()
{
    switch (instruction_register.opCode)
    {
        case 1: // Pushes a constant value (literal) M onto the stack
            stack_pointer = stack_pointer + 1;
            process_address_space[stack_pointer] = instruction_register.m_address;
            break;

        case 2: // Operation to be performed on the data at the top of the stack. (or return from function)
            switch (instruction_register.m_address)
            {
                case 0: // RTN operation
                    stack_pointer = base_pointer - 1;
                    base_pointer = process_address_space[stack_pointer + 2];
                    program_counter = process_address_space[stack_pointer + 3];
                    record_size = record_size - 1;
                    break;

                case 1: // ADD operation
                    process_address_space[stack_pointer - 1] = 
                    process_address_space[stack_pointer - 1] + process_address_space[stack_pointer];
                    stack_pointer = stack_pointer - 1;
                    break;

                case 2: // SUB operation
                    process_address_space[stack_pointer - 1] = 
                    process_address_space[stack_pointer - 1] - process_address_space[stack_pointer];
                    stack_pointer = stack_pointer - 1;
                    break;

                case 3: // MUL operation
                    process_address_space[stack_pointer - 1] = 
                    process_address_space[stack_pointer - 1] * process_address_space[stack_pointer];
                    stack_pointer = stack_pointer - 1;
                    break;

                case 4: // DIV operation
                    process_address_space[stack_pointer - 1] = 
                    process_address_space[stack_pointer - 1] / process_address_space[stack_pointer];
                    stack_pointer = stack_pointer - 1;
                    break;

                case 5: // EQL operation
                    process_address_space[stack_pointer - 1] = 
                    process_address_space[stack_pointer - 1] == process_address_space[stack_pointer];
                    stack_pointer = stack_pointer - 1;
                    break;

                case 6: // NEQ operation
                    process_address_space[stack_pointer - 1] = 
                    process_address_space[stack_pointer - 1] != process_address_space[stack_pointer];
                    stack_pointer = stack_pointer - 1;
                    break;

                case 7: // LSS operation
                    process_address_space[stack_pointer - 1] = 
                    process_address_space[stack_pointer - 1] < process_address_space[stack_pointer];
                    stack_pointer = stack_pointer - 1;
                    break;

                case 8: // LEQ operation
                    process_address_space[stack_pointer - 1] = 
                    process_address_space[stack_pointer - 1] <= process_address_space[stack_pointer];
                    stack_pointer = stack_pointer - 1;
                    break;

                case 9: // GTR operation
                    process_address_space[stack_pointer - 1] = 
                    process_address_space[stack_pointer - 1] > process_address_space[stack_pointer];
                    stack_pointer = stack_pointer - 1;
                    break;

                case 10: // GEQ operation
                    process_address_space[stack_pointer - 1] = 
                    process_address_space[stack_pointer - 1] >= process_address_space[stack_pointer];
                    stack_pointer = stack_pointer - 1;
                    break;

                default:
                    break;
            }

            break;

        case 3: // Load value to top of stack from the stack location at offset M in AR located L lexicographical levels down
            stack_pointer = stack_pointer + 1;
            process_address_space[stack_pointer] = 
            process_address_space[findBaseLevel(base_pointer, instruction_register.lex_level) + instruction_register.m_address];
            break;

        case 4: // Store value at top of stack in stack location at offset M in AR located L lexicographical levels down
            process_address_space[findBaseLevel(base_pointer, instruction_register.lex_level) + instruction_register.m_address] = 
            process_address_space[stack_pointer];
            stack_pointer = stack_pointer - 1;
            break;

        case 5: // Call procedure at code index M (generates new Activation Record and PC <- M)
            process_address_space[stack_pointer + 1] = findBaseLevel(base_pointer, instruction_register.lex_level);
            process_address_space[stack_pointer + 2] = base_pointer;
            process_address_space[stack_pointer + 3] = program_counter;
            base_pointer = stack_pointer + 1;
            program_counter = instruction_register.m_address;
            record_size = record_size + 1;
            break;

        case 6: // Allocates M memory words (increment SP by M)
            stack_pointer = stack_pointer + instruction_register.m_address;
            break;

        case 7: // Jump to instruction M (PC <- M)
            program_counter = instruction_register.m_address;
            break;

        case 8: // Jump to instruction M if topstack element is 0
            if(process_address_space[stack_pointer] == 0) program_counter = instruction_register.m_address;
            stack_pointer = stack_pointer - 1;
            break;

        case 9: // System operations
            switch (instruction_register.m_address)
            {
                case 1: // Write the top stack element to the screen
                    fprintf(stderr, "Output result is: %2d\n", process_address_space[stack_pointer]);
                    stack_pointer = stack_pointer - 1;
                    break;
            
                case 2: // Read in input from the user and store it on top of the stack
                    stack_pointer = stack_pointer + 1;
                    fprintf(stderr, "Please Enter an Intger: ");
                    scanf("%d", &process_address_space[stack_pointer]);
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

    records[record_size].base_pointer = base_pointer;
    records[record_size].stack_pointer = stack_pointer;
}

void readELF(char *fileName)
{
    FILE *input_file = fopen(fileName, "r");

    instruction_size = 0;

    do {
        fscanf(input_file, "%d %d %d", &process_address_space[instruction_size], 
        &process_address_space[instruction_size + 1], &process_address_space[instruction_size + 2]);

        instruction_size = instruction_size + 3;

    } while(feof(input_file) == 0);

    fclose(input_file);
}

void virtualMachine()
{
    base_pointer = instruction_size;
    stack_pointer = base_pointer - 1;
    program_counter = 0;

    fprintf(stderr, "\t\t  PC   BP   SP   stack\n");
    fprintf(stderr, "Initial values:   %2d   %2d   %2d\n\n", program_counter, base_pointer, stack_pointer);

    while(halt != 0)
    {
        /* During the fetch cycle, data stored in memory is retrieved and saved in instruction register */
        fetch();

        /* During the execution cycle, data in the instruction register are executed */
        execute();

        /* Outputs the result of the execution in the console */
        fprintf(stderr, "    %3s  %2d  %2d", opCodeName(instruction_register.opCode), instruction_register.lex_level, instruction_register.m_address);
        fprintf(stderr, "   %2d   %2d   %2d   ", program_counter, base_pointer, stack_pointer);
        
        //fprintf(stderr, "Num Records: %d ", num_records);
        for(int i = 0; i <= record_size; i++)
        {
            for(int j = records[i].base_pointer; j <= records[i].stack_pointer; j++)
            {
                fprintf(stderr, "%d ", process_address_space[j]);
            }

            if(i != record_size) fprintf(stderr, "| ");
        }
        
        fprintf(stderr, "\n");
    }
}

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("Filename missing!\n");
        exit(1);
    } 
    
    else 
    {
        readELF(argv[1]);
        virtualMachine();
    }

    return 0;
}