#pragma once

#include "resource.h"
#include <stdlib.h>
#include <time.h>
#include <windowsx.h>

#include "GUIFunction.h"


extern PACKAGE p0;
extern PACKAGE p1;
extern PACKAGE p2;
extern PACKAGE p3;
extern PACKAGE rPackage;

extern HWND hWndMain;

static DWORD32 index = 0;
static DWORD32 total = 5;
static DWORD32 remaining = total;

const int MAX_TIMERS = 4;
static int TIMERID[MAX_TIMERS] = { 1, 2, 3, 4 };

bool isRun = false;

VOID CALLBACK TimerMainProc(HWND hWnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	HDC hdc = GetDC(hWnd);

	p0.color = colorType[index];

	if (!MovePackage(hWnd, hdc, p0, DIRECTION::LEFT_TO_RIGHT))
	{
		CopyPackage(rPackage, p0);
		rPackage.isValid = true;
		rPackage.total = 0;
		isRun = true;
		KillTimer(hWnd, TIMERID[0]);
	}
}

void CallMainTimer(int timeSeek)
{
	SetRect(&p0.rBegin,
		listBox[0].left, listBox[0].bottom + 10,
		listBox[0].left + sizeBox, listBox[0].bottom + 10 + sizeBox);

	SetRect(&p0.rEnd,
		cxClient / 2, listBox[0].bottom + 10,
		listBox[0].left + sizeBox, listBox[0].bottom + 10 + sizeBox);

	srand(time(NULL));
	index = rand() % 3;
	SetTimer(hWndMain, TIMERID[0], timeSeek, TimerMainProc);

}

VOID CALLBACK Thread2TimerProc(HWND hWnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	HDC hdc = GetDC(hWnd);

	if (!MovePackage(hWnd, hdc, rPackage, DIRECTION::LEFT_TO_RIGHT))
	{
		KillTimer(hWnd, TIMERID[2]);
		rPackage.isValid = false;
		rPackage.total = 0;
	}
	ReleaseDC(hWnd, hdc);
}

void CallThread2Timer(int timeSeek)
{
	if (rPackage.isValid == true)
	{
		int top = listBox[0].bottom + 10;
		int left = listBox[1].left;
		int right = left + sizeBox;
		int bottom = listBox[0].bottom + 10 + sizeBox;
		rPackage.total = 0;

		CopyRect(&rPackage.rBegin, &rPackage.rCurr);
		SetRect(&rPackage.rEnd, cxClient, listBox[1].bottom, cxClient + sizeBox, listBox[1].bottom + sizeBox);

		SetTimer(hWndMain, TIMERID[2], timeSeek, Thread2TimerProc);
	}
}

VOID CALLBACK Thread3TimerProc(HWND hWnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	HDC hdc = GetDC(hWnd);

	if (!MovePackage(hWnd, hdc, rPackage, DIRECTION::TOP_TO_BOTTOM))
	{
		KillTimer(hWnd, TIMERID[3]);
		rPackage.isValid = false;
		rPackage.total = 0;
	}
	ReleaseDC(hWnd, hdc);
}

void CallThread3Timer(int timeSeek)
{
	if (rPackage.isValid == true)
	{
		int top = listBox[2].top;
		int left = listBox[0].right + 10;
		int right = left + sizeBox;
		int bottom = top + sizeBox;

		CopyRect(&rPackage.rBegin, &rPackage.rCurr);
		
		rPackage.rBegin.left = left;
		rPackage.rBegin.right = right;
		rPackage.total = 0;

		SetRect(&rPackage.rEnd, left, cyClient, right, bottom);

		SetTimer(hWndMain, TIMERID[3], timeSeek, Thread3TimerProc);
	}
}

VOID CALLBACK Thread1TimerProc(HWND hWnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	HDC hdc = GetDC(hWnd);

	if (!MovePackage(hWnd, hdc, rPackage, DIRECTION::BOTTOM_TO_TOP))
	{
		KillTimer(hWnd, TIMERID[1]);
		rPackage.isValid = false;
		rPackage.total = 0;
	}
	ReleaseDC(hWnd, hdc);
}

void CallThread1Timer(int time)
{
	if (rPackage.isValid == true)
	{
		int left = listBox[0].right + 10;
		int right = left + sizeBox;

		CopyRect(&rPackage.rBegin, &rPackage.rCurr);

		rPackage.rBegin.left = left;
		rPackage.rBegin.right = right;
		rPackage.total = 0;
		SetRect(&rPackage.rEnd, left, 0, right, sizeBox);
		SetTimer(hWndMain, TIMERID[1], time, Thread1TimerProc);
	}
}