#include<iostream>
#include"Windows.h"
using namespace std;

struct Shell {
	int* Array;
	int ArraySize;

};

int minIndex = 0;
int maxIndex = 0;
int aver = 0;



CRITICAL_SECTION cs1;



VOID WINAPI minMaxf(LPVOID arg) {

	Shell* shell = (Shell*)arg;

	EnterCriticalSection(&cs1);
	int min = shell->Array[0];
	int max = shell->Array[0];

	for (int i = 0; i < shell->ArraySize; i++)
	{
		if (shell->Array[i] > max) {
			max = shell->Array[i];
			maxIndex = i;
		}
		if (shell->Array[i] < min) {
			min = shell->Array[i];
			minIndex = i;
		}
		Sleep(7);
	}
	cout << "min = " << min << "   " << "max = " << max << endl;
	LeaveCriticalSection(&cs1);
}

VOID WINAPI FindAveragef(LPVOID arg) {

	EnterCriticalSection(&cs1);

	Shell* shell = (Shell*)arg;
	int sum = 0;


	for (int i = 0; i < shell->ArraySize; i++)
	{
		sum += shell->Array[i];
		Sleep(12);
	}

	aver = (double)sum / (double)shell->ArraySize;

	cout << "Average = " << aver<<endl;

	LeaveCriticalSection(&cs1);
}



int main() {

	HANDLE HMinMax;
	HANDLE HAverage;
	DWORD minMaxID;
	DWORD averageID;
	Shell* myShell = new Shell;
	
	InitializeCriticalSection(&cs1);

	cout << "Enter array size: ";
	cin >> myShell->ArraySize;
	//myShell->ArraySize = 6;

	myShell->Array = new int[myShell->ArraySize];
	//int Array[] = { 1,3,5,6,0,8 };

	for (int i = 0; i < myShell->ArraySize; i++)
	{
		cin>>myShell->Array[i];
	}

	

	HMinMax = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)minMaxf, (LPVOID)myShell, NULL, &minMaxID);
	WaitForSingleObject(HMinMax, INFINITE);
	CloseHandle(HMinMax);

	HAverage = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)FindAveragef, (LPVOID)myShell, NULL, &averageID);
	WaitForSingleObject(HAverage, INFINITE);
	CloseHandle(HAverage);

	DeleteCriticalSection(&cs1);

	myShell->Array[minIndex] = aver;
	myShell->Array[maxIndex] = aver;

	for (int i = 0; i < myShell->ArraySize; i++)
	{
		cout << myShell->Array[i] << " ";
	}

	for (;;)
	{

	}


}