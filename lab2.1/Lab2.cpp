#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <vector>
#include <locale.h>

using namespace std;

const char FILE_NAME[] = "C:\\Users\\38099\\Desktop\\Lab2\\records.txt";

void printErrors() {
	setlocale(LC_ALL, "Russian");

	cout << endl << "Something went wrong" << endl;

	LPVOID eMessage;
	DWORD eCode = GetLastError();
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		eCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&eMessage, 0, NULL);
	char* errorMessage = (char*)eMessage;
	wprintf(L"ERROR was generated: failed with 0x%x code\n", eCode);
	wprintf(L"ERROR formatted message: %s\n", errorMessage);
	exit(0);
}

struct RECORD {
	int recordNumber;
	FILETIME creation;
	char text[81];
	int counter;
};

struct HEADER {
	int emptyRecordCount;
	int fileSize;
};

void showIncorrectInputError() {
	cout << endl << "You made a mistake" << endl;
	cout << "Only 1 arguments available: " << endl;
	cout << "Number of records" << endl;
	exit(0);
}

void initRecords(int initialNumber) {
	if (initialNumber > 0) {
		HANDLE hFile = CreateFileA(FILE_NAME, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		RECORD r;
		SYSTEMTIME systemTime, systemTime2;
		FILETIME fileTime;
		HEADER headerInformation;

		headerInformation.emptyRecordCount = 0;
		headerInformation.fileSize = sizeof(RECORD) * initialNumber + sizeof(headerInformation);

		GetLocalTime(&systemTime);
		SystemTimeToFileTime(&systemTime, &fileTime);
		FileTimeToSystemTime(&fileTime, &systemTime2);
		r.counter = 0; r.creation = fileTime; strcpy(r.text, "");
		if (hFile != INVALID_HANDLE_VALUE) {

			SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
			WriteFile(hFile, &headerInformation, sizeof(HEADER), NULL, NULL);

			SetFilePointer(hFile, NULL, NULL, FILE_END);
			for (int i = 0; i < initialNumber; i++) {
				r.recordNumber = i;
				WriteFile(hFile, &r, sizeof(RECORD), NULL, NULL);
			}
			CloseHandle(hFile);
		}
		else {
			printErrors();
		}
	}
}

HEADER getHeaderInfo(bool print) {
	HEADER headerInformation;
	DWORD bytes;
	HANDLE hFile = CreateFileA(FILE_NAME, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		SetFilePointer(hFile, NULL, NULL, FILE_BEGIN);
		ReadFile(hFile, &headerInformation, sizeof(HEADER), &bytes, NULL);
		CloseHandle(hFile);
		if (print) {
			cout << "Header information" << endl;
			cout << "File size in bytes: " << headerInformation.fileSize << endl;
			cout << "Not empty record count: " << headerInformation.emptyRecordCount << endl;
		}
		return headerInformation;
	}
	else {
		printErrors();
	}
}

vector<RECORD> getRecordsInfo(int count, bool print) {
	vector<RECORD> records(0);
	RECORD r;
	DWORD bytes;
	HANDLE hFile = CreateFileA(FILE_NAME, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		SetFilePointer(hFile, sizeof(HEADER), NULL, FILE_BEGIN);
		do {
			ReadFile(hFile, &r, sizeof(RECORD), &bytes, NULL);
			records.push_back(r);
		} while (bytes != 0);
		if (print) {
			SYSTEMTIME systemTime;
			for (int i = 0; i < count; i++) {
				cout << "ID: " << records[i].recordNumber << endl;
				cout << "Counter: " << records[i].counter << endl;
				cout << "Text: " << records[i].text << endl;

				FileTimeToSystemTime(&records[i].creation, &systemTime);

				printf("Creation time -> %02d-%02d-%d\n",
					systemTime.wMonth, systemTime.wDay, systemTime.wYear);
			}
		}
		CloseHandle(hFile);
	}
	else {
		printErrors();
	}
	return records;
}

void modifyRecord(int count, bool remove) {
	HEADER header_info = getHeaderInfo(false);
	vector <RECORD> records = getRecordsInfo(count, false);
	SYSTEMTIME systemTime;
	int id;

	cout << "Input id to modify: ";
	scanf("%d", &id);

	if (id >= 0 && id < count) {
		char str[81];
		if (!remove) {
			cout << "Input text (80 symbols without space): ";
			scanf("%s", records[id].text);

			strcpy(str, records[id].text);
			header_info.emptyRecordCount++;
			records[id].counter++;
			FileTimeToSystemTime(&records[id].creation, &systemTime);
		}
		else {
			strcpy(records[id].text, "");
			header_info.emptyRecordCount--;
			records[id].counter = 0;
			FileTimeToSystemTime(&records[id].creation, &systemTime);
		}
		HANDLE hFile = CreateFileA(FILE_NAME, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		SetFilePointer(hFile, NULL, NULL, FILE_BEGIN);
		WriteFile(hFile, &header_info, sizeof(HEADER), NULL, NULL);
		SetFilePointer(hFile, NULL, NULL, FILE_END);

		for (int i = 0; i < count; i++)
			WriteFile(hFile, &records[i], sizeof(RECORD), NULL, NULL);

		CloseHandle(hFile);
	}
	else {
		cout << "Incorrect id";
	}
}

void main(int argc, char* argv[])
{
	if (argc != 2) {
		showIncorrectInputError();
	}
	else {
		while (true)
		{
			int num = atoi(argv[1]);
			char command[3];
			initRecords(num);
			
			cout << "Input command" << endl;
			cout << "rm  => delete record" << endl;
			cout << "mf  => modify record" << endl;
			cout << "pr  => print records" << endl;

			scanf_s("%s", command, 3);
			if (strcmp(command, "rm") == 0) {
				modifyRecord(num, true);
			}
			else if (strcmp(command, "mf") == 0) {
				modifyRecord(num, false);
			}
			else if (strcmp(command, "pr") == 0) {
				getHeaderInfo(true);
				getRecordsInfo(num, true);
			}
			else {
				cout << "Incorrect command" << endl;
				exit(0);
			}
		}
	}
}
