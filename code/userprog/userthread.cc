#ifdef CHANGED

#include "userthread.h"
//#include "addrspace.h"
#include "system.h"
#include "syscall.h"
#include "machine.h"
#include "thread.h"


int
do_UserThreadCreate(int f,int a) {
        typeFork *argFork = new typeFork;
        Thread *newThread = new Thread("USER THREAD");

//initialisation de la zone memoire du thread
        newThread->space = currentThread->space;
        
        //mise en place de la structure
        argFork->func = f;
        argFork->args = a;

        //passer en parametre la structure pour le start
        newThread->Fork(StartUserThread,(int)argFork);
        return 0;

}


void 
StartUserThread(int f) {

        
        //mise de PC sur l'adresse de la fonction
        typeFork *t = (typeFork *)f;
        currentThread->space->InitRegisters();
        currentThread->space->RestoreState();
                

        machine->WriteRegister(PCReg, t->func);//on place pc sur f
        machine->WriteRegister(NextPCReg, (t->func)+4);

        //arguments de la fonction dans R4
        machine->WriteRegister(4, t->args);

        //pointeur de pile
        machine->WriteRegister(StackReg,
                               currentThread->space->getNextThreadSpace(&(currentThread->id)));
        
        

        if(machine->ReadRegister(StackReg) == -1) {
                //thread inutilisable !
                //TODO quelque chose
                DEBUG('t',"erreur de creation de thread\n");
                currentThread->Finish();
        }
        else {
                //on lance le thread
                DEBUG('t',"Thread%d->run()\n",currentThread->id);
                //on ajoute la thread dans addrspace
                currentThread->space->addUserThread();
                machine->Run();
        }

}


void 
do_UserThreadExit() {
        DEBUG('t',"do_userthreadExit\n");
        currentThread->space->deleteUserThread();
        currentThread->Finish();
}




#endif
