#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <locale.h>

using namespace std;

void showCommands()
{
	cout << "Available command" << endl;
	cout << "pwd => print working dir" << endl;
	cout << "cd path => change dir" << endl;
	cout << "mkdir dir_name => print working dir" << endl;
	cout << "pfi file_name => print file info" << endl;
	cout << "cp file_name => copy file" << endl;
	cout << "rmf path => remove file" << endl;
	cout << "rmd path => remove empty dir" << endl;
}

void printError() {
	setlocale(LC_ALL, "Russian");
	cout << "Something went wrong" << endl;
	LPVOID eMessage;
	DWORD eCode = GetLastError();
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		eCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&eMessage, 0, NULL);
	char* errorMessage = (char*)eMessage;
	wprintf(L"ERROR was generated: failed with 0x%x code\n", eCode);
	wprintf(L"ERROR formatted message: %s\n", errorMessage);
	exit(0);
}

void createDir() {
	char dirName[MAX_PATH];
	cout << "Input dir name or path:" << endl;
	scanf_s("%s", dirName, MAX_PATH);
	if (CreateDirectoryA((LPCSTR)dirName, NULL)) {
		cout << "Directory was created" << endl;
	}
	else {
		printError();
	}
}

void showWorkingDirectory() {
	setlocale(LC_ALL, "Russian");
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind;
	char szDirName[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, szDirName);
	hFind = FindFirstFileA(strcat(szDirName, "\\*"), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) {
		printError();
	}
	else {
		do {
			printf("----------------------------------\n");
			SYSTEMTIME systemTime;
			FileTimeToSystemTime(&FindFileData.ftCreationTime, &systemTime);
			printf("Name => %s\n", (char*)FindFileData.cFileName);
			printf("Creation time => %02d-%02d-%d %02d:%02d:%02d\n",
				systemTime.wMonth, systemTime.wDay, systemTime.wYear,
				systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
			cout << "Size in bytes =>" << FindFileData.nFileSizeLow << endl;
		} while (FindNextFileA(hFind, &FindFileData) != 0);
		FindClose(hFind);
	}
}

void showFileInfo() {
	char file_path[MAX_PATH];
	BY_HANDLE_FILE_INFORMATION lpFileInformation;
	cout << "Input file path:";
	scanf_s("%s", file_path, MAX_PATH);
	HANDLE file = CreateFileA(file_path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (GetFileInformationByHandle(file, &lpFileInformation)) {
		wprintf(L"File attributes: %d\n", lpFileInformation.dwFileAttributes);
		SYSTEMTIME systemTime;
		FileTimeToSystemTime(&(lpFileInformation.ftCreationTime), &systemTime);
		wprintf(L"Creation time: %02d-%02d-%d %02d:%02d:%02d\n",
			systemTime.wMonth, systemTime.wDay, systemTime.wYear,
			systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
		FileTimeToSystemTime(&(lpFileInformation.ftLastAccessTime), &systemTime);
		wprintf(L"Last access time: %02d-%02d-%d %02d:%02d:%02d\n",
			systemTime.wMonth, systemTime.wDay, systemTime.wYear,
			systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
		FileTimeToSystemTime(&(lpFileInformation.ftLastWriteTime), &systemTime);
		wprintf(L"Last write time: %02d-%02d-%d %02d:%02d:%02d\n",
			systemTime.wMonth, systemTime.wDay, systemTime.wYear,
			systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
		wprintf(L"Number of links: %d\n", lpFileInformation.nNumberOfLinks);
		wprintf(L"Volume serial number: %d\n", lpFileInformation.dwVolumeSerialNumber);
	}
	else {
		printError();
	}
	CloseHandle(file);
}

void removeFile() {
	char file_path[MAX_PATH];
	cout << "Input file path:";
	scanf_s("%s", file_path, MAX_PATH);
	if (DeleteFileA(file_path)) {
		cout << "File was deleted" << endl;
	}
	else {
		printError();
	}
}

void removeDir() {
	char dir_path[MAX_PATH];
	cout << "Input directory path:";
	scanf_s("%s", dir_path, MAX_PATH);
	if (RemoveDirectoryA(dir_path)) {
		cout << "Directory was deleted" << endl;
	}
	else {
		printError();
	}
}

void copyFIle() {
	char file_path_e[MAX_PATH];
	char file_path_n[MAX_PATH];
	cout << "Input path to existing file:";
	scanf_s("%s", file_path_e, MAX_PATH);
	cout << "Input path to new file:";
	scanf_s("%s", file_path_n, MAX_PATH);
	if (CopyFileA((LPCSTR)file_path_e, (LPCSTR)file_path_n, FALSE)) {
		cout << "File was copied" << endl;
	}
	else {
		printError();
	}
}

void changeDir() {
	DWORD dwNumberOfChar;
	char szDirName[MAX_PATH];
	dwNumberOfChar = GetCurrentDirectoryA(MAX_PATH, szDirName);
	if (dwNumberOfChar != 0) {
		cout << "Current directory name: " << szDirName << endl;
	}
	else {
		printError();
	}
	DWORD dwNumberOfChar2;
	char dir_path[MAX_PATH];
	char szDirName2[MAX_PATH];
	cout << "Input directory path:";
	scanf_s("%s", dir_path, MAX_PATH);
	if (SetCurrentDirectoryA((LPCSTR)dir_path))
	{
		dwNumberOfChar2 = GetCurrentDirectoryA(MAX_PATH, szDirName2);
		if (dwNumberOfChar2 != 0) {
			cout << "Current directory name: " << szDirName2 << endl;
		}
		else {
			printError();
		}
	}
	else {
		printError();
	}
}

void main(int argc, char* argv[])
{
	while (true) {
		showCommands();
		char command[6];
		cout << "Input command:";
		scanf_s("%s", command, 6);
		if (strcmp(command, "cd") == 0) {
			changeDir();
		}
		else if (strcmp(command, "pwd") == 0) {
			showWorkingDirectory();
		}
		else if (strcmp(command, "cp") == 0) {
			copyFIle();
		}
		else if (strcmp(command, "mkdir") == 0) {
			createDir();
		}
		else if (strcmp(command, "pfi") == 0) {
			showFileInfo();
		}
		else if (strcmp(command, "rmd") == 0) {
			removeDir();
		}
		else if (strcmp(command, "rmf") == 0) {
			removeFile();
		}
		else {
			cout << "Incorrect argument" << endl;
		}
	}
	exit(0);
}