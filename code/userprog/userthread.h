#ifdef CHANGED 

#ifndef USERTHREAD_H
#define USERTHREAD_H
/*
 *structure pour le fork
 */
typedef struct {
        int func;
        int args;
} typeFork;

extern int do_UserThreadCreate(int f, int a);
void StartUserThread(int f);
extern void do_UserThreadExit();

#endif
#endif//changed
