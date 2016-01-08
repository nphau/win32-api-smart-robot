#pragma once
#include "resource.h"
#include "TimerLib.h"

#define MAX_THREADS 3
static HANDLE ghMutex = NULL;
static HANDLE hThreadMain = NULL;
static HANDLE hThread[MAX_THREADS];

/* Closing list of thread handle */
void CloseThreadHandle(HANDLE hList[], DWORD32 max)
{
	for (DWORD32 i = 0; i < max; i++)
	{
		CloseHandle(hList[i]);
	}
}

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	UNREFERENCED_PARAMETER(lpParam);

	DWORD dwCount = 0, dwWaitResult = 0;

	while (dwCount < 1)
	{
		dwWaitResult = WaitForSingleObject(ghMutex, INFINITE);

		switch (dwWaitResult)
		{

		case WAIT_OBJECT_0:	// The thread got ownership of the mutex
			__try {

				switch (packCurrent.id)
				{
				case 0: // Red
					CallThread0Timer(hWndMain, 500);

					remaining--;
					Sleep(5000);
					numPackage[0]++;
					break;
				case 1: // Violet 
					CallThread1Timer(hWndMain, 500);
					remaining--;
					Sleep(5000);
					numPackage[1]++;
					break;
				case 2: // Blue
					CallThread2Timer(hWndMain, 500);
					remaining--;
					Sleep(5000);
					numPackage[2]++;
					break;
				}

				dwCount++;

			}

			__finally {
				ReleaseMutex(ghMutex);
			}
			break;
		case WAIT_ABANDONED:
			return FALSE;
		}
	}
	return TRUE;
}

DWORD WINAPI MainThreadProc(LPVOID lpParam)
{
	DWORD retVal = (WaitForMultipleObjects(MAX_THREADS, hThread, TRUE, INFINITE));

	if ((retVal >= WAIT_OBJECT_0) && (retVal <= (WAIT_OBJECT_0 + MAX_THREADS - 1)))
	{

		for (int i = -1; i < 3; i++)
		{
			UpdateInfo(hWndMain, i);
		}

		if (remaining < 0)
		{
			CloseThreadHandle(hThread, MAX_THREADS);
			CloseHandle(hThreadMain);
			ExitProcess(0);
		}

		Sleep(1000);
	}
	return TRUE;
}