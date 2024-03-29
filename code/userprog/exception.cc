// exception.cc 
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.  
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

#ifdef CHANGED
#include "synchconsole.h"
#include "machine.h"
#include "userthread.h"
//defini plus bas
void putStringHandler(char *s);
void copyStringToMachine(char *s,char *to,int size);
#endif //CHANGED
//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC ()
{
    int pc = machine->ReadRegister (PCReg);
    machine->WriteRegister (PrevPCReg, pc);
    pc = machine->ReadRegister (NextPCReg);
    machine->WriteRegister (PCReg, pc);
    pc += 4;
    machine->WriteRegister (NextPCReg, pc);
}


//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions 
//      are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler (ExceptionType which)
{
    int type = machine->ReadRegister (2);
    
#ifdef CHANGED
    if (which == SyscallException) {
            switch(type) {
            
            case(SC_Halt):  {
                    DEBUG ('a', "Shutdown, initiated by user program.\n");
                    interrupt->Halt ();
                    break;
            }
            
            case(SC_Exit): {
                    DEBUG ('a', "EXIT\n");
                    currentThread->space->do_Exit();
                    break;
            }
            
            case(SC_PutChar): {
                    DEBUG ('a', "Putchar\n");
                    synchconsole->SynchPutChar ((char)machine->ReadRegister(4));
                    break;
            }
            case(SC_PutString): {
                    DEBUG ('a', "PutString\n");
                    putStringHandler((char *)machine->ReadRegister(4));
                    break;
            }
            case(SC_GetChar): {
                    DEBUG ('a', "GetChar\n");
                    machine->WriteRegister(2,
                                           (int) synchconsole->SynchGetChar());
                    break;
            }
            case(SC_GetString): {
                    DEBUG ('a', "GetString\n");
                    //adresse du resultat
                    char *to = (char *) machine->ReadRegister(4);
                    int size = (int) machine->ReadRegister(5);
                    char *buffer = new char[MAX_STRING_SIZE];
                    synchconsole->SynchGetString(buffer,size);
                    copyStringToMachine(buffer,to,size);
                    delete [] buffer;
                    break;
            }
            case(SC_UserThreadCreate): {
                    DEBUG ('a', "UserThreadCreate\n");
                    do_UserThreadCreate((int)machine->ReadRegister(4),
                                        (int)machine->ReadRegister(5));
                    break;
            }
            case(SC_UserThreadExit): {
                    DEBUG ('a', "UserThreadExit\n");
                    do_UserThreadExit();
                    break;
            }
/*plus tard
            case(SC_UserThreadJoin): {
                    DEBUG ('a', "UserThreadJoin\n");
                    int threadID = machine -> ReadRegister(4);                    
                    int tt = do_UserThreadJoin(threadID);
                    machine->WriteRegister(2,tt);
                    break;
            }
*/


    
            default: {
                    printf ("Unexpected user mode exception %d %d\n", which, type);
                    ASSERT (FALSE);
            }
    }
}
        
#else 
if ((which == SyscallException && type == SC_Halt))
      {
	  DEBUG ('a', "Shutdown, initiated by user program.\n");
	  interrupt->Halt ();
      }
    else
      {
	  printf ("Unexpected user mode exception %d %d\n", which, type);
	  ASSERT (FALSE);
      }
#endif
    // LB: Do not forget to increment the pc before returning!
    UpdatePC ();
    // End of addition
}

#ifdef CHANGED
void 
copyStringFromMachine (int from, char *to, unsigned size) {
        unsigned i;
        for(i=0;i<size && machine->mainMemory[from+i] !='\0';i++) {
                to[i] = machine->mainMemory[from+i];
        }
        to[i] = '\0';
}

void
putStringHandler(char *s) {
        char *buffer = new char[MAX_STRING_SIZE];
        copyStringFromMachine((int)s,buffer,MAX_STRING_SIZE);
        synchconsole->SynchPutString(buffer);
        delete [] buffer;
}

void 
copyStringToMachine(char *s, char *to, int size) {
        int i;
        for(i=0;i<size-1 && s[i] !='\0';i++) {
                machine->mainMemory[(unsigned)(to+i)] = (char) s[i];
        }
        
        machine->mainMemory[(unsigned)(to+i)] = '\0';
}



#endif //CHANGED
