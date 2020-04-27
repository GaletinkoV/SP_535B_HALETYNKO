#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <locale.h>
#include <iostream>

#define bSize 1024
#define maxNumberOfFiles 10

using namespace std;

void incorrectData() {
	cout << "You made a mistake" << endl;
	cout << "Only 2 arguments available:" << endl;
	cout << "-a => pack files" << endl;
	cout << "-u => unpack files" << endl;
	exit(0);
}

void showError() {
	setlocale(LC_ALL, "Russian");
	cout << "Something went wrong" << endl;

	LPVOID eMessage;
	DWORD eCode = GetLastError();
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		eCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&eMessage, 0, NULL);
	char* errorMessage = (char*)eMessage;
	wprintf(L"ERROR was generated: failed with 0x%x code\n", eCode);
	wprintf(L"ERROR formatted message: %s\n", errorMessage);
}

bool packFiles()
{
	bool result = true;
	LPSTR* inputFiles = new CHAR * [maxNumberOfFiles];
	int inputFilesCount;
	cout << "Input number of files, which will archive: ";
	scanf("%d", &inputFilesCount);
	cout << "Input full path to files, which will archive: ";
	LPSTR fileName = new CHAR[MAX_PATH];

	for (int i = 0; i < inputFilesCount; i++) {
		cout << "Input full path to file:";
		scanf("%s", fileName);
		inputFiles[i] = new CHAR[MAX_PATH];
		strcpy(inputFiles[i], fileName);
	}

	LPSTR archiveName = new CHAR[MAX_PATH];

	cout << "Input full path to new archive:";
	scanf("%s", archiveName);

	HANDLE hInReadPipe;
	HANDLE hInWritePipe;
	HANDLE hOutReadPipe;
	HANDLE hOutWritePipe;

	CreatePipe(
		&hInReadPipe,                      
		&hInWritePipe,         
		NULL,	    
		0);	
	CreatePipe(&hOutReadPipe, &hOutWritePipe, NULL, 0);

	if (hInReadPipe == INVALID_HANDLE_VALUE || hInWritePipe == INVALID_HANDLE_VALUE ||
		hOutReadPipe == INVALID_HANDLE_VALUE || hOutWritePipe == INVALID_HANDLE_VALUE) {
		showError();
		result = false;
	}

	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(startInfo));
	startInfo.cb = sizeof(startInfo);		// Устанавливает размер структуры, в байтах.
	startInfo.hStdInput = hInReadPipe;		// определяет дескриптор ввода для процесса
	startInfo.hStdOutput = hOutWritePipe;		// определяет дескриптор ввода для процесса
	startInfo.hStdError = hOutWritePipe;		// определяет дескриптор ошибки для процесса
	startInfo.dwFlags |= STARTF_USESTDHANDLES;

	PROCESS_INFORMATION procInfo;
	ZeroMemory(&procInfo, sizeof(procInfo));

	LPSTR cmdLine = new CHAR[MAX_PATH];
	ZeroMemory(cmdLine, MAX_PATH);

	strncpy(cmdLine, "7z.exe a ", MAX_PATH - strlen(cmdLine));
	strncat(cmdLine, archiveName, MAX_PATH - strlen(cmdLine));

	for (int i = 0; i < inputFilesCount; i++)
	{
		strncat(cmdLine, " ", MAX_PATH - strlen(cmdLine));
		strncat(cmdLine, inputFiles[i], MAX_PATH - strlen(cmdLine));
	}

	strncat(cmdLine, " ", MAX_PATH - strlen(cmdLine));

	if (!CreateProcessA(NULL, cmdLine, NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startInfo, &procInfo)) {
		showError();
		result = false;
	}

	if (result) {
		WaitForSingleObject(procInfo.hProcess, INFINITE);
		DWORD readBytes = 0;
		LPSTR outBuffer = new CHAR[bSize];
		ZeroMemory(outBuffer, bSize);
		OVERLAPPED overlapped;
		ReadFile(hOutReadPipe, outBuffer, bSize, &readBytes, &overlapped);
		CloseHandle(procInfo.hProcess);
		CloseHandle(procInfo.hThread);
	}

	CloseHandle(hOutReadPipe);
	CloseHandle(hOutWritePipe);
	CloseHandle(hInReadPipe);
	CloseHandle(hInWritePipe);
	return result;
}

bool unpackFiles()
{
	bool result = true;
	LPSTR inputFileName = new CHAR[MAX_PATH];
	LPSTR outputFileName = new CHAR[MAX_PATH];
	cout << "Input full path to archive:";
	scanf("%s", inputFileName);
	cout << "Input full output path:";
	scanf("%s", outputFileName);
	LPSTR cmdLine = new CHAR[MAX_PATH];
	ZeroMemory(cmdLine, MAX_PATH);

	strncpy(cmdLine, "7z.exe e ", MAX_PATH - strlen(cmdLine));
	strncat(cmdLine, inputFileName, MAX_PATH - strlen(cmdLine));
	strncat(cmdLine, " -o", MAX_PATH - strlen(cmdLine));
	strncat(cmdLine, outputFileName, MAX_PATH - strlen(cmdLine));
	strncat(cmdLine, " -y", MAX_PATH - strlen(cmdLine));

	STARTUPINFOA startInfo;
	ZeroMemory(&startInfo, sizeof(startInfo));
	startInfo.cb = sizeof(startInfo);
	startInfo.dwFlags |= STARTF_USESTDHANDLES;
	PROCESS_INFORMATION procInfo;
	ZeroMemory(&procInfo, sizeof(procInfo));
	if (!CreateProcessA(NULL, cmdLine, NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startInfo, &procInfo)) {
		showError();
		result = false;
	}
	return result;
}
int main(int argc, char* argv[])
{
	if (argc != 2 || ((strcmp(argv[1], "-a") != 0) && (strcmp(argv[1], "-u")) != 0)) {
		incorrectData();
	}
	if (strcmp(argv[1], "-a") == 0) {
		(packFiles()) ? printf("The files were archived.") : printf("The files were not archived.");
	}
	else {
		(unpackFiles()) ? printf("The files were unziped.") : printf("The files were not unziped.");
	}
	return 0;
}