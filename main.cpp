#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <fstream>

DWORD GetPID(const char* procName)
{
    DWORD procId = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnap != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 procEntry;
        procEntry.dwSize = sizeof(procEntry);

        if (Process32First(hSnap, &procEntry))
        {
            do
            {
                if (!_stricmp((procEntry.szExeFile), procName))
                {
                    procId = procEntry.th32ProcessID;
                    break;
                }
            } while (Process32Next(hSnap, &procEntry));
        }
    }
    CloseHandle(hSnap);
    return procId;
}

int main()
{
    const char* dllPath = "C:\\Users\\Student\\source\\repos\\Project3\\x64\\Release\\Project3.dll";
    const char* exeName = "cmd.exe";
    DWORD PID = 0;

    while (!PID)
    {
        PID = GetPID(exeName);
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, PID);

    if (hProcess && hProcess != INVALID_HANDLE_VALUE)
    {
        void* loc = VirtualAllocEx(hProcess, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        LPVOID addr = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");

        WriteProcessMemory(hProcess, loc, dllPath, strlen(dllPath) + 1, 0);

        HANDLE hThread = CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)addr, loc, 0, 0);

        if (hThread)
        {
            CloseHandle(hThread);
        }
    }

    if (hProcess)
    {
        CloseHandle(hProcess);
    }
    return 0;
}



