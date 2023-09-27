#ifndef SCANNER_H
#define SCANNER_H

#define norw 15 /* number of reserved words */
#define imax 32767 /* maximum integer value */
#define cmax 11 /* maximum number of chars for idents */
#define strmax 256 /* maximum length of strings */

typedef enum 
{ 
    nulsym = 1, idsym, numbersym, plussym, minussym, multsym, slashsym, oddsym,
    eqsym, neqsym, lessym, leqsym, gtrsym, geqsym, lparentsym, rparentssym, commasym, 
    semicolonsym, periodsym, becomessym, beginsym, endsym, ifsym, thensym, whilesym,
    dosym, callsym, constsym, varsym, procsym, writesym
} token_type;

void readSourceProgram(char *file_name);
void printLexemeTable();

#endif