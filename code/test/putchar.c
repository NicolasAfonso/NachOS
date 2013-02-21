/*Programme de test de putchar*/
#ifdef CHANGED


#include "syscall.h"


void print(char c, int n) {
        //int i;
        char tab[n];
/*
        for (i = 0; i < n; i++) {
                //PutChar(c+i);
                tab[i]=c+i;
          
        }

        //PutChar('\n');
        tab[n]='\0';
        PutString(tab);
*/
//pour tester get et put string
        PutString("tapez un texte:");
        GetString(tab,n);
        PutString("vous avez ecrit :");
        PutString(tab);
        
}
int
main()
{
  print('a',26);
  Halt();
}


#endif //CHANGED
