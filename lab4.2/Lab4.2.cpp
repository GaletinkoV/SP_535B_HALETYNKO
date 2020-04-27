#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <iostream>

using namespace std;

#define amountOfNumbers 5

CRITICAL_SECTION criticalSection;
HANDLE* threadHandles;
int dwTlsIndex;

void incorrectData() {
	cout << "You made a mistake" << endl;
	cout << "Only 1 arguments available:" << endl;
	cout << "Number of threads => more then 1" << endl;
	exit(0);
}

int countLargestDivisor(int number)
{
	int n = number / 2;
	for (int i = n; i >= 2; i--) {
		if (number % i == 0)
			return i;
	}
}

void fillLargestDivisors(int* initArr, int* resArr)
{
	for (int i = 0; i < amountOfNumbers; i++) {
		resArr[i] = countLargestDivisor(initArr[i]);
	}
}

void printArr(int* arr, int size)
{
	for (int i = 0; i < size; i++) {
		cout << " " << arr[i];
	}
}

int countSum(int* arr, int size)
{
	int result = 0;
	for (int i = 0; i < size; i++) {
		result += arr[i];
	}

	return result;
}

DWORD WINAPI executedByThread(LPVOID arr)
{
	EnterCriticalSection(&criticalSection);

	cout << "ID: " << GetCurrentThreadId() << endl;

	int* array = (int*)arr;
	TlsSetValue(dwTlsIndex, (LPVOID)(new int[amountOfNumbers]));
	int sum = 0;

	cout << "Generated array => ";
	printArr(array, amountOfNumbers);
	fillLargestDivisors(array, (int*)TlsGetValue(dwTlsIndex));

	cout << endl << "Modyfied array => ";
	printArr((int*)TlsGetValue(dwTlsIndex), amountOfNumbers);
	sum = countSum((int*)TlsGetValue(dwTlsIndex), amountOfNumbers);

	cout << "Sum =>" << sum << endl;
	LeaveCriticalSection(&criticalSection);
	return 0;
}

void runThreads(int threadsCount)
{
	cout << "Number of threads =>" << threadsCount << endl;
	InitializeCriticalSection(&criticalSection);
	threadHandles = new HANDLE[threadsCount];
	DWORD* ids = new DWORD[threadsCount];
	int** arrays = new int* [threadsCount];
	time_t t;
	srand((unsigned)time(&t));
	
	dwTlsIndex = TlsAlloc();
	for (int i = 0; i < threadsCount; i++)
	{
		arrays[i] = new int[amountOfNumbers];
		for (int j = 0; j < amountOfNumbers; j++) {
			arrays[i][j] = rand() % 90 + 10;
		}
		threadHandles[i] = CreateThread(
			NULL,	
			0,			 
			executedByThread, 	  
			arrays[i],		  
			NULL,			 
			&ids[i]);		  
	}
}

int main(int argc, char* argv[])
{
	if (argc != 2) {
		incorrectData();
	}
	else {
		int threadsCount = atoi(argv[1]);
		runThreads(threadsCount);
		WaitForMultipleObjects(threadsCount, threadHandles, TRUE, INFINITE);
		DeleteCriticalSection(&criticalSection);
	}
	return 0;
}