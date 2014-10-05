 #include <stdio.h>
 #include <stdlib.h>
//exchange - newvalue, comperand is old/expected value
/*
 * Function actually does the following thing - if the value at *dest is equal to oldvalue then replace it by newvalue else leave it unchanged : do all these atomically
 *
 * There are two options for return value
 *  1.is initial value of *dest and leave the burden of calling fxn to compare it with oldval
 *  2. do it over here and return 0 or 1, this should be more efficient
 * */

/* later change it into macro  */
int cas(int* dest, int oldvalue, int newvalue){
        printf("(%d,%d,%d)",*dest,oldvalue,newvalue);
        /* int cas(int dest, int oldvalue, int newvalue){ */
        /* int cas(int dest, int newvalue, int oldvalue){ */
        int result= 1;/* 1 shows that cas succeeded and 0 shows that it failed  */
        /* btw need to set cc for flag clobbering !  */
        __asm__ __volatile__(
                        "movl %2, %%eax\n\t"
                        "movl %3, %%ebx\n\t"
                        "movl %0, %%ecx\n\t"
                        "LOCK\n\t"
                        "CMPXCHG %%ebx, (%%ecx)\n\t"  /* should LOCK be on the same line  */
                        "jz DONE\n\t"
                        "movl $0, %1\n\t "
                        "DONE: \n\t"
                        :"=m"(dest),"=g"(result)
                        :"g" (oldvalue),"g" (newvalue),"m"(dest)
                        :"%eax","%ebx","ecx","cc"
                        );
        printf("(%d,%d,%d)",*dest,oldvalue,newvalue);
        return result; 
}


/* TODO
 * write another asm fxn which puts above fxn in a while loop and keep trying unless it succeeds*/

int main(){
        int a= 5,b= 6;
        int* c = (int*) malloc(sizeof(int));
        *c= 6;
        /* int c= 6; */
        printf("%d\n",cas(c,b,b));
        printf("%d\n",cas(c,b,a));
        printf("%d\n",cas(c,a,a));
        printf("%d\n",cas(c,b,b));
        *c= 6;
        /* c= 5; */
        printf("changing value of *c to %d\n",*c);
        printf("%d\n",cas(c,b,b));
        printf("%d\n",cas(c,b,a));
        printf("%d\n",cas(c,a,a));
        printf("%d\n",cas(c,a,b));
        printf("%d\n",cas(c,b,a));
        return 0;
}
