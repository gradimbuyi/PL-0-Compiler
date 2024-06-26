#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "scanner.h"

//char *LEXEME_TABLE[100];
char *IDENTIFIER_NAMES[100];
int NUMBER_TABLE[100];
int TOKEN_LIST[100];

int NUM_IDENTIFIER = 0;
int TABLE_SIZE = 0;
int NUM_TOKEN = 0;

int readSourceProgram(char *file_name, char **program_memory)
{
    int num_inputs = 0;

    FILE *input_file = fopen(file_name, "r");
    char buffer[strmax];

    while(fgets(buffer, strmax, input_file)) 
    {
        program_memory[num_inputs] = malloc(sizeof(char) * strlen(buffer) + 1);
        strcpy(program_memory[num_inputs], buffer);
        num_inputs++;
    }

    fclose(input_file); return num_inputs;
}

void printSourceProgram(char **program_memory, int num_inputs)
{
    fprintf(stderr, "Source Program:\n");

    for(int i = 0; i < num_inputs; i++)
    {
        fprintf(stderr, "%s", program_memory[i]);
    }
}

int checkTokenType(char *token, int isAllNumber) 
{
    int token_type;

    if(isAllNumber == 0) 
    {
        int number;
        sscanf(token, "%d", &number);
        NUMBER_TABLE[TABLE_SIZE] = number;
        TABLE_SIZE++;
        token_type = numbersym;

        if(number > imax) fprintf(stderr, "%s\t\tError: Too many digits\n", token);
        else fprintf(stderr, "%s\t\t\t%d\n", token, token_type);

    } else {

        if(strcmp(token, "null") == 0)              token_type = nullsym;
        else if(strcmp(token, "skip") == 0)         token_type = skipsym;  
        else if(strcmp(token, "+") == 0)            token_type = plussym;
        else if(strcmp(token, "-") == 0)            token_type = minussym;
        else if(strcmp(token, "*") == 0)            token_type = multsym;
        else if(strcmp(token, "/") == 0)            token_type = slashsym;
        else if(strcmp(token, "ifel") == 0)         token_type = ifelsym;
        else if(strcmp(token, "=") == 0)            token_type = eqlsym;
        else if(strcmp(token, "<>") == 0)           token_type = neqsym;
        else if(strcmp(token, "<") == 0)            token_type = lessym;
        else if(strcmp(token, "<=") == 0)           token_type = leqsym;
        else if(strcmp(token, ">") == 0)            token_type = gtrsym;
        else if(strcmp(token, ">=") == 0)           token_type = geqsym; 
        else if(strcmp(token, "(") == 0)            token_type = lparentsym;
        else if(strcmp(token, ")") == 0)            token_type = rparentsym;
        else if(strcmp(token, ",") == 0)            token_type = commasym;
        else if(strcmp(token, ";") == 0)            token_type = semicolonsym;
        else if(strcmp(token, ".") == 0)            token_type = periodsym;
        else if(strcmp(token, ":=") == 0)           token_type = becomessym;
        else if(strcmp(token, "begin") == 0)        token_type = beginsym;
        else if(strcmp(token, "end") == 0)          token_type = endsym;
        else if(strcmp(token, "if") == 0)           token_type = ifsym;
        else if(strcmp(token, "then") == 0)         token_type = thensym;
        else if(strcmp(token, "while") == 0)        token_type = whilesym; 
        else if(strcmp(token, "do") == 0)           token_type = dosym;
        else if(strcmp(token, "call") == 0)         token_type = callsym; 
        else if(strcmp(token, "const") == 0)        token_type = constsym; 
        else if(strcmp(token, "procedure") == 0)    token_type = procsym; 
        else if(strcmp(token, "write") == 0)        token_type = writesym;
        else if(strcmp(token, "read") == 0)         token_type = readsym; 
        else if(strcmp(token, "else") == 0)         token_type = elsesym; 
        else if(strcmp(token, "var") == 0)          token_type = varsym; 
        else {
            
            token_type = identsym;

            for(int i = 0; i < strlen(token); i++) if(!isalpha(token[i]) && !isdigit(token[i])) 
            {
                token_type = -1;
                break;
            }

            if(isdigit(token[0])) token_type = -2;

            IDENTIFIER_NAMES[NUM_IDENTIFIER] = malloc(sizeof(char) * strlen(token) + 1);
            strcpy(IDENTIFIER_NAMES[NUM_IDENTIFIER], token);
            NUM_IDENTIFIER++;
        }

        if(token_type == -1) fprintf(stderr, "%s\t\t\tError: Not a valid symbol\n", token);
        else if(token_type == -2) fprintf(stderr, "%s\t\t\tError: Not a valid Ident\n", token);
        else if(strlen(token) > cmax) fprintf(stderr, "%s\tError: Ident length too long\n", token);
        else if(strcmp(token, "procedure") == 0) fprintf(stderr, "%s\t\t%d\n", token, token_type);
        else fprintf(stderr, "%s\t\t\t%d\n", token, token_type);
    }
    
    return token_type;
}

void lexicalAnalyzer(char *file_name)
{
    int isAllNumber = 0;

    char **program_memory = malloc(sizeof(char *) * 100);
    int num_inputs = readSourceProgram(file_name, program_memory);

    printSourceProgram(program_memory, num_inputs);

    fprintf(stderr, "\n\nLexeme Table:\n\n");
    fprintf(stderr, "lexeme\t\t\ttoken type\n");

    for(int i = 0; i < num_inputs; i++)
    {
        char token[strmax]; int token_size = 0;

        for(int j = 0; j < strlen(program_memory[i]); j++)
        {
            if((program_memory[i][j] == ' ' || program_memory[i][j] == '\t' || program_memory[i][j] == '\n') && token_size != 0)
            {
                TOKEN_LIST[NUM_TOKEN++] = checkTokenType(token, isAllNumber);
                token_size = 0;
                isAllNumber = 0;
            }

            else if(program_memory[i][j] == '/' && j + 1 < strlen(program_memory[i]) && program_memory[i][j + 1] == '*')
            {
                fprintf(stderr, "This is the current token: %s", token);

                char current = program_memory[i][j];
                char next = program_memory[i][j + 1]; j++;

                do {
                    j += 1;

                    if(j < strlen(program_memory[i]) && j + 1 < strlen(program_memory[i])) 
                    {
                        current = program_memory[i][j];
                        next = program_memory[i][j + 1];
                    }

                    else if(j < strlen(program_memory[i]) && i < num_inputs)
                    {
                        current = program_memory[i][j];
                        next = program_memory[i + 1][0];
                        i++; j = 0;

                        fprintf(stderr, "stuck two\n");
                    }

                } while(current != '*' && next != '/');
            }

            else 
            {
                if(token_size > 0 && (isalpha(token[token_size - 1]) || isdigit(token[token_size - 1]))) 
                {
                    if(!(isalpha(program_memory[i][j]) || isdigit(program_memory[i][j])))
                    {
                        TOKEN_LIST[NUM_TOKEN++] = checkTokenType(token, isAllNumber);
                        token_size = 0;
                        isAllNumber = 0;
                    }
                }

                if(isAllNumber == 0 && !isdigit(program_memory[i][j])) isAllNumber = -1;

                if(!((program_memory[i][j] == ' ' || program_memory[i][j] == '\t' || program_memory[i][j] == '\n')))
                {
                    token[token_size] = program_memory[i][j];
                    token[token_size + 1] = '\0'; 
                    token_size++;
                }
            }
        }

        if(i + 1 == num_inputs && token_size > 0) TOKEN_LIST[NUM_TOKEN++] = checkTokenType(token, isAllNumber);
        
    }
    
    free(program_memory);
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
    }

    return 0;
}