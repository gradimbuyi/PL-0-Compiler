#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "scanner.h"

char *RESERVED_WORDS[] = 
{
    "null", "beggin", "call", "const", "do", "else", "end", "if", "odd",
    "procedure", "read", "then", "var", "while", "write"
};

char *LEXEME_TABLE[100];
int TOKEN_LIST[100];
int NUM_TOKEN = 0;

/*
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
*/

void scanLine(char *buffer, int buffer_length)
{
    char temp;

    for(int i = 0; i < buffer_length; i++)
    {
        //temp = buffer[i];
        if(isalpha(buffer[i])) ;// do something
       
    }
}

void lexicalAnalyzer(char *file_name)
{
    FILE *input_file = fopen(file_name, "r");
    char buffer[strmax];

    fprintf(stderr, "Source Program:\n");

    while(fgets(buffer, strmax, input_file)) 
    {
        fprintf(stderr, "%s", buffer);
        scanLine(buffer, strlen(buffer));
    }

    fprintf(stderr, "\n");
    fclose(input_file);
}

void printLexemeTable() 
{
    fprintf(stderr, "\nLexeme Table:\n\n");
    fprintf(stderr, "lexeme\ttoken type\n");
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
        lexicalAnalyzer(argv[1]);
        printLexemeTable();
    }

    return 0;
}