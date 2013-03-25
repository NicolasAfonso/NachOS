#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"
static Semaphore *readAvail;
static Semaphore *writeDone;
static void ReadAvail(int arg) { readAvail->V(); }
static void WriteDone(int arg) { writeDone->V(); }
                                            
static Semaphore *semPutChar;

SynchConsole::SynchConsole(char *readFile, char *writeFile)
{
  readAvail = new Semaphore("read avail", 0);
  writeDone = new Semaphore("write done", 0);
  console = new Console(readFile,writeFile,ReadAvail,WriteDone,0);
  semPutChar = new Semaphore("PutChar", 1);
}
SynchConsole::~SynchConsole()
{
  delete console;
  delete writeDone;
  delete readAvail;
}
void SynchConsole::SynchPutChar(const char ch)
{
        //console->PutChar('<');
        //writeDone->P();
	semPutChar->P();
        console->PutChar(ch);
        writeDone->P();
	semPutChar->V();
        // console->PutChar('>');
        //writeDone->P();
}

char SynchConsole::SynchGetChar()
{
        readAvail->P();
        return console->GetChar();
}

void SynchConsole::SynchPutString(const char s[])
{
        int i;
        for(i=0;i<MAX_STRING_SIZE && s[i]!='\0';i++) {
                synchconsole->SynchPutChar((char)s[i]);
                //writeDone->P();
        }
        synchconsole->SynchPutChar('\n');
        //writeDone->P();
}

void SynchConsole::SynchGetString(char *s, int n)
{
        int i;
        char c;
        for(i=0;i<n-1;i++) {
                c=synchconsole->SynchGetChar();
                if(c==EOF || c=='\n') break;
                else s[i] = c;
        }
        s[i] = '\0';
}
#endif // CHANGED
