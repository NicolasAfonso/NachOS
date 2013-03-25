// addrspace.h 
//      Data structures to keep track of executing user programs 
//      (address spaces).
//
//      For now, we don't keep any information about address spaces.
//      The user level CPU state is saved and restored in the thread
//      executing the user program (see thread.h).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef ADDRSPACE_H
#define ADDRSPACE_H

#include "copyright.h"
#include "filesys.h"

#define UserStackSize		1024	// increase this as necessary!

#ifdef CHANGED
#include "synch.h"
#include "bitmap.h"
/**nombre de pages allouees par thread utilisateur*/
#define threadPageNumber 3

#endif//CHANGED

class AddrSpace
{
  public:
    AddrSpace (OpenFile * executable);	// Create an address space,
    // initializing it with the program
    // stored in the file "executable"
    ~AddrSpace ();		// De-allocate an address space

    void InitRegisters ();	// Initialize user-level CPU registers,
    // before jumping to user code

    void SaveState ();		// Save/restore address space-specific
    void RestoreState ();	// info on a context switch 
#ifdef CHANGED
//methodes crees
    int getThreadNumber();//retourne le nombre de threads utilisateur
    void addUserThread();//incremente de nombre de threads
    void deleteUserThread();//decremente de nombre de threads
    int getNextThreadSpace(int *threadid);//retourne le prochain pointeur de pile pour une nouvelle thread
    void do_Exit();
#endif //CHANGED

  private:
    TranslationEntry * pageTable;	// Assume linear page table translation
    // for now!
    unsigned int numPages;	// Number of pages in the virtual 
    // address space
#ifdef CHANGED
    int threadNumber; //nombre de threads operants sur cet espace d adressage
    Semaphore* semThreadNumber; //semaphone pour modifier le nombre de thread

    BitMap *bitMap;
    Semaphore *semBitMap;

    Semaphore *semMainWait;
    //utilisateur en exclusion mutuelle
#endif //CHANGED


};

#endif // ADDRSPACE_H
