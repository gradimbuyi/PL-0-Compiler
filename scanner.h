#ifndef SCANNER_H
#define SCANNER_H

#define norw 15 /* number of reserved words */
#define imax 32767 /* maximum integer value */
#define cmax 11 /* maximum number of chars for idents */
#define strmax 256 /* maximum length of strings */

typedef enum { 
    nullsym, skipsym, identsym, numbersym, plussym, minussym, multsym, slashsym, ifelsym, eqlsym, 
    neqsym, lessym, leqsym, gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym, periodsym, 
    becomessym, beginsym, endsym, ifsym, thensym, whilesym, dosym, callsym, constsym, varsym, procsym, 
    writesym, readsym, elsesym
} token_type;

int readSourceProgram(char *file_name, char **program_memory);
void printSourceProgram(char **program_memory, int num_inputs);
int checkTokenType(char *token, int isAllNumber);
void lexicalAnalyzer(char *file_name);

#endif