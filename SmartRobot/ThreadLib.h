#pragma once
#include "resource.h"
#include "Packages.h"

#define MAX_THREADS 3

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
void CloseThreadHandle(HANDLE hList[], DWORD32 dMax)
{
	for (DWORD32 i = 0; i < dMax; i++)
	{
		if (WaitForSingleObject(hList[i], INFINITE) == WAIT_OBJECT_0)
		{
			CloseHandle(hList[i]);
		}
	}
}

bool Thread0MoveFunction(HDC hdc)
{
	int left = listBox[0].right + 10;
	int right = left + sizeBox;

	CopyRect(&packCurrent.rBegin, &packCurrent.rCurr);

	packCurrent.total = 0;
	packCurrent.rBegin.left = left;
	packCurrent.rBegin.right = right;

	SetRect(&packCurrent.rEnd, left, 0, right, sizeBox);

	while (true)
	{
		if (!MovePackage(hWndMain, hdc, packCurrent, DIRECTION::BOTTOM_TO_TOP))
		{
			ZeroMemory(&packCurrent, sizeof(PACKAGE));
			remaining--;
			numPackage[0]++;
			ReleaseMutex(ghMutex);
			return true;
		}
		Sleep(300);
	}
	return false;
}
bool Thread2MoveFunction(HDC hdc)
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

	while (true)
	{
		if (!MovePackage(hWndMain, hdc, packCurrent, DIRECTION::TOP_TO_BOTTOM))
		{
			ZeroMemory(&packCurrent, sizeof(PACKAGE));
			remaining--;
			numPackage[2]++;
			ReleaseMutex(ghMutex);
			return true;
		}
		Sleep(300);
	}
	return false;
}
bool Thread1MoveFunction(HDC hdc)
{
	int top = listBox[0].bottom + 10;
	int left = listBox[1].left;
	int right = left + sizeBox;
	int bottom = listBox[0].bottom + 10 + sizeBox;
	packCurrent.total = 0;

	CopyRect(&packCurrent.rBegin, &packCurrent.rCurr);
	SetRect(&packCurrent.rEnd, cxClient, listBox[1].bottom, cxClient + sizeBox, listBox[1].bottom + sizeBox);

	while (true)
	{
		if (!MovePackage(hWndMain, hdc, packCurrent, DIRECTION::LEFT_TO_RIGHT))
		{
			ZeroMemory(&packCurrent, sizeof(PACKAGE));
			remaining--;
			numPackage[1]++;
			ReleaseMutex(ghMutex);
			return true;
		}
		Sleep(300);
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
			HDC hdc = GetDC(data->hWnd);

			switch (data->id)
			{
			case 0: // Red
				data->id = -1;
				return Thread0MoveFunction(hdc);
			case 1: // Violet 
				data->id = -1;
				return Thread2MoveFunction(hdc);
			case 2: // Blue
				data->id = -1;
				return Thread1MoveFunction(hdc);
			}
		}
	}
	return true;
}

DWORD WINAPI MainThreadProc(LPVOID lpParam)
{
	while (true)
	{

		if (remaining < 1)
		{
			CloseThreadHandle(hThread, MAX_THREADS);
			CloseHandle(hThreadMain);

			return false;
		}

		SetBeginPackValue(packBegin);

		HDC hdc = GetDC(hWndMain);

		while (true)
		{
			if (!MovePackage(hWndMain, hdc, packBegin, DIRECTION::LEFT_TO_RIGHT))
			{
				CopyPackage(packCurrent, packBegin);
				packCurrent.isAvailable = true;
				packCurrent.total = 0;
				break;
			}
			Sleep(600);
		}

		tData.id = packCurrent.id;
		ReleaseMutex(ghMutex);
		Sleep(2000);

		DWORD retVal = WaitForMultipleObjects(MAX_THREADS, hThread, FALSE, INFINITE);

		switch (retVal)
		{
		case WAIT_OBJECT_0:
			UpdateInfo(hWndMain, 0);
			break;
		case WAIT_OBJECT_0 + 1:
			UpdateInfo(hWndMain, 1);
			break;
		case WAIT_OBJECT_0 + 2:
			UpdateInfo(hWndMain, 2);
			break;
		}

		UpdateInfo(hWndMain, -1);

	}
	return TRUE;
}