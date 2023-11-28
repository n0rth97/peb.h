# Peb.h
A header-only, std independent library to get function address from EAT using peb.

# Usage
```c
#include <stdio.h>
#include "Peb.h"

int main(void)
{
  printf("[Info] NtAllocateVirtualMemory: %#02x\n", ResolveFunction(L"ntdll.dll", "NtAllocateVirtualMemory"));
}
```
