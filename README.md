# Peb.h
A header-only, std independent library to get function address from EAT using peb.

# Usage
```c
#include "peb.h"

int main(void)
{
  printf("[Info] NtAllocateVirtualMemory: %#02x\n", ResolveFunction(L"ntdll.dll", "NtAllocateVirtualMemory"));
}
```
