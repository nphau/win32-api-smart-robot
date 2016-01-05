#pragma once
#include "resource.h"
#include "TimerLib.h"

static HANDLE ghMutex = NULL;

extern int NumPackage[3];

const int MAX_THREADS = 3;

static HANDLE hThread[MAX_THREADS];
static HANDLE hThreadMain;

typedef struct THREADDATA
{
	DWORD32 index;

}THREADDATA, *PTHREADDATA;

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	if (WaitForSingleObject(ghMutex, INFINITE) == 0)
	{
		NumPackage[index] ++;

		if (remaining > 1 && rPackage.isValid == true)
		{
			switch (index)
			{
			case 0:
				CallThread1Timer(500);
				break;
			case 1:
				CallThread3Timer(500);
				break;
			case 2:
				CallThread2Timer(500);
				break;
			}
		}
		remaining--;
		ReleaseMutex(ghMutex);
	}
	return TRUE;
}

DWORD WINAPI MainThreadProc(LPVOID lpParam)
{
	CallMainTimer(500);

	while (!isRun);

	if (isRun)
	{
		for (int i = 0; i < MAX_THREADS; i++)
		{
			if (!(hThread[i] = CreateThread(NULL, 0,
				(LPTHREAD_START_ROUTINE)ThreadProc,
				(LPVOID)&rPackage, 0, 0)))
			{
				return false;
			}
			SetThreadPriority(hThread[i], THREAD_PRIORITY_HIGHEST);
		}
	}

	isRun = false;
}