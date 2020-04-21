#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <locale.h>

using namespace std;

#define nNumberOfBytesToRead  2048

void incorrectInput() {
	cout << "You made a mistake" << endl;
	cout << "There are only four arguments available:" << endl;
	cout << "-a => ASCII to UNICODE" << endl;
	cout << "-u => UNICODE to ASCII" << endl;
	cout << "file 1 => full path to source" << endl;
	cout << "file 2 => full path to result" << endl;
	exit(0);
}

void showFilesError() {
	setlocale(LC_ALL, "Russian");
	cout << "Something went wrong" << endl;
	LPVOID errorMessage;
	DWORD errorCode = GetLastError();
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&errorMessage, 0, NULL);
	char* _errorMessage = (char*)errorMessage;
	wprintf(L"ERROR was generated: failed with 0x%x code\n", errorCode);
	wprintf(L"ERROR formatted message: %s\n", _errorMessage);
	exit(0);
}

void codeFromAsciiToUnicode(HANDLE ascii_file, HANDLE new_file) {
	BYTE lpBuffer[nNumberOfBytesToRead];
	DWORD lpNumberOfBytesRead;
	while (ReadFile(ascii_file, lpBuffer, nNumberOfBytesToRead, (LPDWORD)&lpNumberOfBytesRead, NULL))
	{
		if (lpNumberOfBytesRead == 0) {
			break;
		}
		else {
			int stringLength = MultiByteToWideChar(CP_UTF8, 0, (LPCCH)lpBuffer, lpNumberOfBytesRead, NULL, 0);
			wchar_t* _string = new wchar_t[stringLength];

			MultiByteToWideChar(CP_UTF8, 0, (LPCCH)lpBuffer, lpNumberOfBytesRead, _string, stringLength);
			wprintf(_string);
			if (!WriteFile(new_file, _string, stringLength * sizeof(wchar_t), (LPDWORD)&lpNumberOfBytesRead, NULL)) {
				cout << "Something went wrong(Writing failed)" << endl;
				exit(0);
			}
		}
	}
	CloseHandle(ascii_file);
	CloseHandle(new_file);
	exit(0);
}

void codeFromUnicodeToAscii(HANDLE unicode_file, HANDLE new_file) {
	setlocale(LC_ALL, "Russian");
	BYTE lpBuffer[nNumberOfBytesToRead];
	DWORD lpNumberOfBytesRead;

	while (ReadFile(unicode_file, lpBuffer, nNumberOfBytesToRead, (LPDWORD)&lpNumberOfBytesRead, NULL))
	{
		if (lpNumberOfBytesRead == 0) {
			break;
		}
		else {
			int uStringLength = MultiByteToWideChar(CP_UTF8, 0, (LPCCH)lpBuffer, lpNumberOfBytesRead, NULL, 0);
			wchar_t* uString = new wchar_t[uStringLength];

			MultiByteToWideChar(CP_UTF8, 0, (LPCCH)lpBuffer, lpNumberOfBytesRead, uString, uStringLength);

			int aStringLength = WideCharToMultiByte(CP_ACP, 0, uString, uStringLength, NULL, 0, NULL, NULL);
			char* _aString = new char[aStringLength];

			WideCharToMultiByte(CP_ACP, 0, uString, uStringLength, _aString, aStringLength, NULL, NULL);
			printf("%s", _aString);
			if (!WriteFile(new_file, _aString, aStringLength * sizeof(char), (LPDWORD)&lpNumberOfBytesRead, NULL)) {
				cout << "Something went wrong(Writing failed)" << endl;
				exit(0);
			}
		}
	}
	CloseHandle(unicode_file);
	CloseHandle(new_file);
	exit(0);
}

void main(int argc, char* argv[]) {
	if (argc != 4 || ((strcmp(argv[1], "-a") != 0) && (strcmp(argv[1], "-u")) != 0)) {
		incorrectInput();
	}

	HANDLE file = CreateFileA(argv[2], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE result_file = CreateFileA(argv[3], GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

	if (file != INVALID_HANDLE_VALUE && result_file != INVALID_HANDLE_VALUE) {
		(strcmp(argv[1], "-a") == 0) ? codeFromAsciiToUnicode(file, result_file) : codeFromUnicodeToAscii(file, result_file);
	} else {
		showFilesError();
	}
}