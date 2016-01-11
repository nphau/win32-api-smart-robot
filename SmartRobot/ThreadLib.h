#pragma once
#include "resource.h"
#include "Packages.h"

#define MAX_THREADS 3

static HANDLE ghMutex = NULL;
static HANDLE hThreadMain = NULL;
static HANDLE hThread[MAX_THREADS];

extern HWND hMain;

extern int total;
extern int remaining;
extern int MAX_TIME_WAIT;


/* Closing list of thread handle */
void CloseAllThreadHandle(HANDLE hList[], DWORD32 dMax)
{
	TerminateThread(hThreadMain, 0);
	for (DWORD32 i = 0; i < dMax; i++)
	{
		TerminateThread(hList[i], 0);
		CloseHandle(hList[i]);
	}
}

bool Thread0MoveFunction()
{
	int left = listBox[0].right + 10;
	int right = left + sizeBox;

	CopyRect(&packCurrent.rBegin, &packCurrent.rCurr);

	packCurrent.total = 0;
	packCurrent.rBegin.left = left;
	packCurrent.rBegin.right = right;

	SetRect(&packCurrent.rEnd, left, 0, right, sizeBox);

	HDC hdc = GetDC(hMain);

	while (true)
	{
		Sleep(MAX_TIME_WAIT);
		if (!MovePackage(hMain, hdc, packCurrent, DIRECTION::BOTTOM_TO_TOP))
		{
			remaining--;
			numPackage[0]++;
			return true;
		}
	}
	return false;
}
bool Thread1MoveFunction()
{
	int top = listBox[2].top;
	int left = listBox[0].right + 10;
	int right = left + sizeBox;
	int bottom = top + sizeBox;

	CopyRect(&packCurrent.rBegin, &packCurrent.rCurr);

	packCurrent.rBegin.left = left;
	packCurrent.rBegin.right = right;
	packCurrent.total = 0;

	SetRect(&packCurrent.rEnd, left, cyClient, right, bottom);

	HDC hdc = GetDC(hMain);

	while (true)
	{
		Sleep(MAX_TIME_WAIT);
		if (!MovePackage(hMain, hdc, packCurrent, DIRECTION::TOP_TO_BOTTOM))
		{
			remaining--;
			numPackage[2]++;
			return true;
		}
	}
	return false;
}
bool Thread2MoveFunction()
{
	int top = listBox[0].bottom + 10;
	int left = listBox[1].left;
	int right = left + sizeBox;
	int bottom = listBox[0].bottom + 10 + sizeBox;
	packCurrent.total = 0;

	CopyRect(&packCurrent.rBegin, &packCurrent.rCurr);
	SetRect(&packCurrent.rEnd, cxClient, listBox[1].bottom, cxClient + sizeBox, listBox[1].bottom + sizeBox);

	HDC hdc = GetDC(hMain);

	while (true)
	{
		Sleep(MAX_TIME_WAIT);
		if (!MovePackage(hMain, hdc, packCurrent, DIRECTION::LEFT_TO_RIGHT))
		{
			remaining--;
			numPackage[1]++;
			return true;
		}
	}
	return false;
}

bool MainThreadMoveFunction()
{
	SetBeginPackValue(packBegin);

	HDC hdc = GetDC(hMain);

	while (true)
	{
		if (!MovePackage(hMain, hdc, packBegin, DIRECTION::LEFT_TO_RIGHT))
		{
			CopyPackage(packCurrent, packBegin);
			packCurrent.total = 0;
			packCurrent.id = -1;
			return true;
		}
		Sleep(MAX_TIME_WAIT);
	}
	return false;
}

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	while (true)
	{
		if (WaitForSingleObject(ghMutex, INFINITE) == WAIT_OBJECT_0)
		{
			if (remaining < 1)
			{
				return FALSE;
			}

			switch (packCurrent.id)
			{
			case 0: // Red
				Thread0MoveFunction();
				ReleaseMutex(ghMutex);
				return TRUE;
			case 1: // Violet 
				Thread1MoveFunction();
				ReleaseMutex(ghMutex);
				return TRUE;
			case 2: // Blue
				Thread2MoveFunction();
				ReleaseMutex(ghMutex);
				return TRUE;
			}
			packCurrent.id = -1;
		}
	}
	return TRUE;
}

DWORD WINAPI MainThreadProc(LPVOID lpParam)
{
	while (true)
	{
		if (remaining < 1)
		{
			CloseAllThreadHandle(hThread, MAX_THREADS);
			CloseHandle(hThreadMain);

			return FALSE;
		}

		MainThreadMoveFunction();

		// 

		Sleep(1000);

		DWORD retVal = WaitForMultipleObjects(MAX_THREADS, hThread, FALSE, INFINITE);

		if (retVal >= WAIT_OBJECT_0 && retVal <= WAIT_OBJECT_0 + MAX_THREADS - 1)
		{
			for (int i = -1; i < MAX_THREADS; i++)
			{
				UpdateInfo(hMain, i, total, remaining);
			}
		}

		ReleaseMutex(ghMutex);
	}
	return TRUE;
}