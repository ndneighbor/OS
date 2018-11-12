// addrspace.h
//	Data structures to keep track of executing user programs
//	(address spaces).
//
//	For now, we don't keep any information about address spaces.
//	The user level CPU state is saved and restored in the thread
//	executing the user program (see thread.h).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#ifndef ADDRSPACE_H
#define ADDRSPACE_H

#include "copyright.h"
#include "filesys.h"
#include "memorymanager.h"
#include "pcb.h"

class MemoryManager;
class PCB;

#define UserStackSize		1024 	// increase this as necessary!

class AddrSpace {
  public:
    AddrSpace(OpenFile *executable);	// Create an address space,
    AddrSpace(OpenFile *executable, Thread* parent);	// Create an address space,
    AddrSpace(OpenFile *executable, Thread* parent, Thread* selfThread, bool replace); // Create an address space,
    ~AddrSpace();			// De-allocate an address space

    int GetNumPages() {return numPages;};
    void InitRegisters();		// Initialize user-level CPU registers,
    int Translate(int i);               // before jumping to user code
    
    int TranslateDiskLocation(int i);               // before jumping to user code

    void SaveState();			// Save/restore address space-specific
    void RestoreState(); // info on a context switch
    AddrSpace* Fork(int pid);                  // Create duplicate address space
    void FreePages();                   //Free allocated physical pages
    int ReadFile(int virtAddr, OpenFile* file, int size, int fileAddr);
    
    // 
    
    int ReadFileIntoBuffer(int virtAddr, 
			   OpenFile* file,
			   int size,
			   int fileAddr, char *into);
               
    char swap[10];                
    int nextVPage;
    
    TranslationEntry *pageTable;
    
    //
    
    PCB* pcb;
    int codeSize;
    int initDataSize;
    int uninitDataSize;
private:
    AddrSpace();   //empty constructor for Fork Operation
    void init(OpenFile* executable, Thread* parent, Thread* selfThread, bool replace); //moved noff data reading and page table building here
    
    unsigned int numPages;
    MemoryManager* manager;
};

#endif // ADDRSPACE_H
