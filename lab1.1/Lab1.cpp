#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <locale.h>

using namespace std;

void getInfo() {
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);

    cout << "Hardware information:" << endl;
    cout << "Hardware information:" << systemInfo.dwOemId << endl;
    cout << "Number of processors:" << systemInfo.dwNumberOfProcessors << endl;
    cout << "Page size:" << systemInfo.dwPageSize << endl;
    cout << "Processor type:" << systemInfo.dwProcessorType << endl;
    cout << "Minimum application address:" << systemInfo.lpMinimumApplicationAddress << endl;
    cout << "Maximum application address:" << systemInfo.lpMaximumApplicationAddress << endl;
    cout << "Active processor mask:" << systemInfo.dwActiveProcessorMask << endl;
    exit(0);
}

void generateError() {
    setlocale(LC_ALL, "Russian");
    MEMORYSTATUS memoryInfo;

    GlobalMemoryStatus(&memoryInfo);
    HLOCAL allocated_memory_object = LocalAlloc(LPTR, memoryInfo.dwTotalVirtual * 1000);

    if (allocated_memory_object == NULL) {
        LPVOID errorMessage;
        DWORD eCode = GetLastError();
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
            eCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&errorMessage, 0, NULL);
        char* _errorMessage = (char*)errorMessage;
        wprintf(L"ERROR was generated: failed with 0x%x code\n", eCode);
        wprintf(L"ERROR formatted message: %s\n", _errorMessage);
    }
    exit(0);
}

void incorrectInput() {
    cout << "You made a mistake" << endl;
    cout << "There are only two arguments available:" << endl;
    cout << "-e = generate error & print error" << endl;
    cout << "-s = print system information" << endl;
    exit(0);
}

void main(int argc, char* argv[]) {
 
     if (argc != 2 || ((strcmp(argv[1], "-e") != 0) && (strcmp(argv[1], "-s"))) != 0) {
         incorrectInput();
     } else {
         if (strcmp(argv[1], "-e") == 0) {
             generateError();
         } else {
             getInfo();
         }
     }
}