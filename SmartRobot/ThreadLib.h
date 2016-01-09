#pragma once
#include "resource.h"
#include "Packages.h"

#define MAX_THREADS 3


static HANDLE ghMutex = NULL;
static HANDLE hThreadMain = NULL;
static HANDLE hThread[MAX_THREADS];
extern int total;
extern int remaining;
extern int MAX_TIME_WAIT;

typedef struct tagTHREADDATA
{
	int id;
	HWND hWnd;

}THREADDATA, *PTHREADDATA;

static THREADDATA tData;

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

bool Thread0MoveFunction(HWND hWnd)
{
	int left = listBox[0].right + 10;
	int right = left + sizeBox;

	CopyRect(&packCurrent.rBegin, &packCurrent.rCurr);

	packCurrent.total = 0;
	packCurrent.rBegin.left = left;
	packCurrent.rBegin.right = right;

	SetRect(&packCurrent.rEnd, left, 0, right, sizeBox);

	HDC hdc = GetDC(hWnd);

	while (true)
	{
		if (!MovePackage(hWnd, hdc, packCurrent, DIRECTION::BOTTOM_TO_TOP))
		{
			ZeroMemory(&packCurrent, sizeof(PACKAGE));
			remaining--;
			numPackage[0]++;
			return true;
		}
		Sleep(MAX_TIME_WAIT);
	}
	return false;
}
bool Thread1MoveFunction(HWND hWnd)
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


	HDC hdc = GetDC(hWnd);

	while (true)
	{
		if (!MovePackage(hWnd, hdc, packCurrent, DIRECTION::TOP_TO_BOTTOM))
		{
			ZeroMemory(&packCurrent, sizeof(PACKAGE));
			remaining--;
			numPackage[2]++;
			return true;
		}
		Sleep(MAX_TIME_WAIT);
	}
	return false;
}
bool Thread2MoveFunction(HWND hWnd)
{
	int top = listBox[0].bottom + 10;
	int left = listBox[1].left;
	int right = left + sizeBox;
	int bottom = listBox[0].bottom + 10 + sizeBox;
	packCurrent.total = 0;

	CopyRect(&packCurrent.rBegin, &packCurrent.rCurr);
	SetRect(&packCurrent.rEnd, cxClient, listBox[1].bottom, cxClient + sizeBox, listBox[1].bottom + sizeBox);


	HDC hdc = GetDC(hWnd);

	while (true)
	{
		if (!MovePackage(hWnd, hdc, packCurrent, DIRECTION::LEFT_TO_RIGHT))
		{
			ZeroMemory(&packCurrent, sizeof(PACKAGE));
			remaining--;
			numPackage[1]++;
			return true;
		}
		Sleep(MAX_TIME_WAIT);
	}
	return false;
}

bool MainThreadMoveFunction(HWND hWnd)
{

	SetBeginPackValue(packBegin);

	HDC hdc = GetDC(hWnd);

	while (true)
	{
		if (!MovePackage(hWnd, hdc, packBegin, DIRECTION::LEFT_TO_RIGHT))
		{
			CopyPackage(packCurrent, packBegin);
			packCurrent.isAvailable = true;
			packCurrent.total = 0;
			return true;
		}
		Sleep(MAX_TIME_WAIT);
	}
	return false;
}

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	PTHREADDATA data = (PTHREADDATA)lpParam;

	while (true)
	{
		if (remaining < 1)
		{
			return FALSE;
		}
		if (WaitForSingleObject(ghMutex, INFINITE) == WAIT_OBJECT_0)
		{
			HWND hw = data->hWnd;

			switch (data->id)
			{
			case 0: // Red
				data->id = -1;
				Thread0MoveFunction(hw);
				ReleaseMutex(ghMutex);
				Sleep(MAX_TIME_WAIT);
				return TRUE;
			case 1: // Violet 
				data->id = -1;
				Thread1MoveFunction(hw);
				ReleaseMutex(ghMutex);
				Sleep(MAX_TIME_WAIT);
				return TRUE;
			case 2: // Blue
				data->id = -1;
				Thread2MoveFunction(hw);
				ReleaseMutex(ghMutex);
				Sleep(MAX_TIME_WAIT);
				return TRUE;
			}
		}
	}
	return TRUE;
}

DWORD WINAPI MainThreadProc(LPVOID lpParam)
{
	PTHREADDATA data = (PTHREADDATA)lpParam;

	while (true)
	{

		if (remaining < 1)
		{
			CloseAllThreadHandle(hThread, MAX_THREADS);
			CloseHandle(hThreadMain);

			return FALSE;
		}

		MainThreadMoveFunction(data->hWnd);

		Sleep(MAX_TIME_WAIT * 5);

		tData.id = packCurrent.id;

		ReleaseMutex(ghMutex);


		DWORD retVal = WaitForMultipleObjects(MAX_THREADS, hThread, FALSE, INFINITE);

		if (retVal >= WAIT_OBJECT_0 && retVal <= WAIT_OBJECT_0 + MAX_THREADS - 1)
		{
			for (int i = -1; i < MAX_THREADS; i++)
			{
				UpdateInfo(data->hWnd, i, total, remaining);
			}
		}

	}
	return TRUE;
}