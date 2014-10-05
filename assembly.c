#include<stdio.h>
#include<stdlib.h>

int main(){
   // register a, b; // Variable in register
   int a, b;
   a = 200;
/*
Some notes:

 movl %1, %%eax;  \        double % for registers, single % for arguments
 movl %%eax, %0;" \        Move the value into b. 
 : "=r" (b)                Paremeter 0 is a register(register int a)
 : "r" (a)                 Parameter 1 is a register(register int b)
 : "%eax"                  Clobbered register i.e. Modified register 

*/

asm volatile ("          \
        movl %1, %%eax;  \
        addl $100, %%eax;\
        movl %%eax, %0;"
        : "=rm" (b)
        : "rm" (a)
        : "%eax"         
);
printf("A = %d, B = %d\n", a, b);

}
