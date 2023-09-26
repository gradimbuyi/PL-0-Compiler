#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"

char *reserved_words[] = 
{
    "null", "beggin", "call", "const", "do", "else", "end", "if", "odd",
    "procedure", "read", "then", "var", "while", "write"
};

void readSourceProgram(char *file_name)
{
    FILE *input_file = fopen(file_name, "r");

    char buffer[strmax];

    fprintf(stderr, "Source Program:\n");

    while(fgets(buffer, strmax, input_file)) 
    {
        fprintf(stderr, "%s", buffer);
    }

    fprintf(stderr, "\n");
    fclose(input_file);
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
        readSourceProgram(argv[1]);
    }

    return 0;
}