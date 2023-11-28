#pragma once

#include <windows.h>
#include <winternl.h>
#include <shlwapi.h>

// Functions
FARPROC ResolveFunction(
    LPCWSTR lpModuleName,
    LPCSTR lpFunctionName
)
{
    // Get PEB
    PEB* Peb = NtCurrentTeb()->ProcessEnvironmentBlock;

    // Iterate
    LIST_ENTRY* Head = &Peb->Ldr->InMemoryOrderModuleList;
    LIST_ENTRY* Next = Head->Flink;

    for (; Head != Next; Next = Next->Flink)
    {
        LDR_DATA_TABLE_ENTRY* entry = CONTAINING_RECORD(Next, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

        if (StrStrW(entry->FullDllName.Buffer, lpModuleName) != NULL)
        {
            PIMAGE_DOS_HEADER pDOS = (PIMAGE_DOS_HEADER)entry->DllBase;
            PIMAGE_NT_HEADERS pNTH = (PIMAGE_NT_HEADERS)(
                (DWORD_PTR)entry->DllBase + pDOS->e_lfanew
            );

            // Export Directory
            PIMAGE_EXPORT_DIRECTORY pExport = (PIMAGE_EXPORT_DIRECTORY)(
                (DWORD_PTR)entry->DllBase + pNTH->OptionalHeader.DataDirectory[0].VirtualAddress
            );

            // EAT Entries
            WORD* wOrdinals = (WORD*)(
                (DWORD_PTR)entry->DllBase + pExport->AddressOfNameOrdinals
            );
            
            DWORD* dwNamesTable = (DWORD*)(
                (DWORD_PTR)entry->DllBase + pExport->AddressOfNames
            );

            DWORD* dwFunctionTable = (DWORD*)(
                (DWORD_PTR)entry->DllBase + pExport->AddressOfFunctions
            );

            // Iterate over functions
            for (int i = 0; i < pExport->NumberOfNames; i++)
            {
                LPCSTR lpFunction = (LPCSTR)(
                    (DWORD_PTR)entry->DllBase + dwNamesTable[i]
                );

                if (StrStrA(lpFunction, lpFunctionName) != NULL)
                {
                    return (FARPROC)((DWORD_PTR)entry->DllBase + dwFunctionTable[wOrdinals[i]]);
                }
            }
        }
    }
}
