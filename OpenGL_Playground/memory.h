#ifndef MEMORY_H
#define MEMORY_H

#define KB 1024
#define MB (1024 * KB)
#define GB (1024 * MB)

static void* MemoryStore;
static void* MemItr;
_SYSTEM_INFO SystemInfo;
unsigned int NumberOfPages;

void Mem_Initialize()
{
	GetSystemInfo(&SystemInfo);
	NumberOfPages = GB / SystemInfo.dwPageSize;
	NumberOfPages *= 2;

	MemoryStore = VirtualAlloc(0, NumberOfPages * SystemInfo.dwPageSize,
		MEM_COMMIT, PAGE_READWRITE);
	MemItr = MemoryStore;
}

float** Mem_Allocate(float** DataPtr, unsigned int Size)
{
	DataPtr = (float**)MemItr;
	MemItr = (float**)MemItr + Size;
	return DataPtr;
}

unsigned int* Mem_Allocate(unsigned int* DataPtr, unsigned int Size)
{
	DataPtr = (unsigned int*)MemItr;
	MemItr = (unsigned int*)MemItr + Size;
	return DataPtr;
}

char* Mem_Allocate(char* DataPtr, unsigned int Size)
{
	DataPtr = (char*)MemItr;
	MemItr = (char*)MemItr + Size;
	char* End = (char*)MemItr - 1;
	*End = '\0';
	return DataPtr;
}

void Mem_Shutdown()
{
	VirtualFree(MemoryStore, NumberOfPages * SystemInfo.dwPageSize,
		MEM_RELEASE);
}

#endif
