#pragma once
#include "resource.h"
#include "Packages.h"

#define MAX_THREADS 3
#define MAX_TIMES 200

static HANDLE ghMutex = NULL;
static HANDLE hThreadMain = NULL;
static HANDLE hThread[MAX_THREADS];
extern int total;
extern int remaining;

typedef struct tagTHREADDATA
{
	int id;
	HWND hWnd;

}THREADDATA, *PTHREADDATA;


THREADDATA tData;

/* Closing list of thread handle */
void CloseAllThreadHandle(HANDLE hList[], DWORD32 dMax)
{
	for (DWORD32 i = 0; i < dMax; i++)
	{
		if (WaitForSingleObject(hList[i], INFINITE) == WAIT_OBJECT_0)
		{
			CloseHandle(hList[i]);
		}
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
			ReleaseMutex(ghMutex);
			return true;
		}
		Sleep(MAX_TIMES);
	}
	return false;
}
bool Thread2MoveFunction(HWND hWnd)
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
			ReleaseMutex(ghMutex);
			return true;
		}
		Sleep(MAX_TIMES);
	}
	return false;
}
bool Thread1MoveFunction(HWND hWnd)
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
			ReleaseMutex(ghMutex);
			return true;
		}
		Sleep(MAX_TIMES);
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
		Sleep(MAX_TIMES);
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
			return false;
		}
		if (WaitForSingleObject(ghMutex, INFINITE) == WAIT_OBJECT_0)
		{
			HWND hw = data->hWnd;

			switch (data->id)
			{
			case 0: // Red
				data->id = -1;
				return Thread0MoveFunction(hw);
			case 1: // Violet 
				data->id = -1;
				return Thread2MoveFunction(hw);
			case 2: // Blue
				data->id = -1;
				return Thread1MoveFunction(hw);
			}
		}
	}
	return true;
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

			return false;
		}
		MainThreadMoveFunction(data->hWnd);
		
		Sleep(1000);

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