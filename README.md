# PL/0 Compiler
This is an implmentation for a small PL/0 Compiler writing in C.
## Virtual Machine (virtual_machine.c)
The virtual machine or P-Machine is a stack machine that has one memory area called the process address space. The PAS is dived into sections: the text which contains the list of instructions to be performed by the machine, and the stack which organizes the data to be used by the CPU. 
### Instruction Set Architecture (ISA)
There are 10 arithmetic/logical operations that manipulate the data within the stack segment.
```
01 LIT 0, M Pushes a constant value (literal) M onto the stack
02 OPR 0, M Operation to be performed on the data at the top of the stack (or return from function)
03 LOD L, M Load value to top of stack from the stack location at offset M in AR located L lexicographical levels down
04 STO L, M Store value at top of stack in the stack location at offset M in AR located L lexicographical levels down
05 CAL L, M Call procedure at code index M (generates new Activation Record and PC <- M)
06 INC 0, M Allocates M memory words (increment SP by M)
07 JMP 0, M Jump to instruction M (PC <- M)
08 JCP 0, M Jump to instruction M if top stack element is 0
09 SYS 0, 1 Write the top stack element to the screen
   SYS 0, 2 Read in input from the user and store it on top of the stack
   SYS 0, 3 End of program (Set Halt flag to zero)
```
### Vocabulary
```
AR - Activation Record
OPR - Operation Code
PAS - Process Address Space
L - Lexicographical level
M - Depending on the operators indicates:
    a. a number (when OP is LIT or INC)
    b. a program address (when OP is JMP, JCP, or CAL)
    c. a data address (when OP is LOD, STO)
    d. the identify of the arithmetic operation associated to the OPR op-code.
PC - Program Counter
IR - Instruction Register
BP - Base Pointer
SP - Stack Pointer
```
### Scanner (scanner.c)