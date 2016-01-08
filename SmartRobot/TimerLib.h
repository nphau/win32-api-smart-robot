#pragma once

#include "resource.h"
#include <stdlib.h>
#include <time.h>
#include <windowsx.h>
#include "Packages.h"

extern HWND hWndMain;
const int MAX_TIMERS = 4;
static int TIMERID[MAX_TIMERS] = { 99, 100, 101, 102 };

void CloseAllTimer(HWND hWnd, int timerID[], DWORD32 max)
{
	for (DWORD32 i = 0; i < max; i++)
	{
		KillTimer(hWnd, timerID[i]);
	}
}

/* Set default value for begin package */
void SetBeginPackValue(PACKAGE& package)
{
	int xLeft = listBox[0].left;
	int yTop = listBox[0].bottom + 10;
	int xRight = xLeft + sizeBox;
	int yBottom = yTop + sizeBox;

	// Begin point
	SetRect(&packBegin.rBegin, xLeft, yTop, xRight, yBottom);
	// End point
	SetRect(&packBegin.rEnd, cxClient / 2, yTop, listBox[0].left + sizeBox, yBottom);

	srand(time(NULL));
	packBegin.id = rand() % 3;
	packBegin.color = colorType[packBegin.id];
	packBegin.total = 0;
}
VOID CALLBACK TimerMainProc(HWND hWnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	if (remaining < 1)
	{
		KillTimer(hWnd, TIMERID[0]);
	}
	HDC hdc = GetDC(hWnd);

	if (!MovePackage(hWnd, hdc, packBegin, DIRECTION::LEFT_TO_RIGHT))
	{
		CopyPackage(packCurrent, packBegin);
		packCurrent.isAvailable = true;
		packCurrent.total = 0;

		SetBeginPackValue(packBegin);
	}
}

void CallMainTimer(HWND hWnd, int timeSeek)
{
	SetBeginPackValue(packBegin);
	SetTimer(hWnd, TIMERID[0], timeSeek, TimerMainProc);
}

VOID CALLBACK Thread0TimerProc(HWND hWnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	if (packCurrent.isAvailable)
	{
		HDC hdc = GetDC(hWnd);

		if (!MovePackage(hWnd, hdc, packCurrent, DIRECTION::BOTTOM_TO_TOP))
		{
			// InvalidateRect(hWnd, &packCurrent.rCurr, true);
			ZeroMemory(&packCurrent, sizeof(PACKAGE));
		}
		ReleaseDC(hWnd, hdc);
	}
}

// Màu đỏ
void CallThread0Timer(HWND hWnd, int time)
{
	int left = listBox[0].right + 10;
	int right = left + sizeBox;

	CopyRect(&packCurrent.rBegin, &packCurrent.rCurr);

	packCurrent.total = 0;
	packCurrent.rBegin.left = left;
	packCurrent.rBegin.right = right;

	SetRect(&packCurrent.rEnd, left, 0, right, sizeBox);
	SetTimer(hWnd, TIMERID[1], time, Thread0TimerProc);
}
VOID CALLBACK Thread2TimerProc(HWND hWnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	if (packCurrent.isAvailable)
	{
		HDC hdc = GetDC(hWnd);

		if (!MovePackage(hWnd, hdc, packCurrent, DIRECTION::LEFT_TO_RIGHT))
		{
			// InvalidateRect(hWnd, &packCurrent.rCurr, true);
			ZeroMemory(&packCurrent, sizeof(PACKAGE));
		}
		ReleaseDC(hWnd, hdc);
	}
}
// Màu xanh
void CallThread2Timer(HWND hWnd, int timeSeek)
{
	int top = listBox[0].bottom + 10;
	int left = listBox[1].left;
	int right = left + sizeBox;
	int bottom = listBox[0].bottom + 10 + sizeBox;
	packCurrent.total = 0;

	CopyRect(&packCurrent.rBegin, &packCurrent.rCurr);
	SetRect(&packCurrent.rEnd, cxClient, listBox[1].bottom, cxClient + sizeBox, listBox[1].bottom + sizeBox);

	SetTimer(hWnd, TIMERID[2], timeSeek, Thread2TimerProc);
}

VOID CALLBACK Thread1TimerProc(HWND hWnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	if (packCurrent.isAvailable)
	{
		HDC hdc = GetDC(hWnd);

		if (!MovePackage(hWnd, hdc, packCurrent, DIRECTION::TOP_TO_BOTTOM))
		{
			ZeroMemory(&packCurrent, sizeof(PACKAGE));
		}
		ReleaseDC(hWnd, hdc);
	}
}

// Màu tím
void CallThread1Timer(HWND hWnd, int timeSeek)
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

	SetTimer(hWnd, TIMERID[3], timeSeek, Thread1TimerProc);
}
