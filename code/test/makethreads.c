#include "syscall.h"

void print(int i) {
        if (i % 2)
                PutString("Je suis un nombre impair\n");
        else
                PutString("Je suis un nombre pair\n");
        UserThreadExit();
}
int main() {
        //PutString("Début du main...\n");
        UserThreadCreate(print,(void *) 12);
        UserThreadCreate(print,(void *) 23 );
        PutString("coucou2");
        return 0 ;
}
