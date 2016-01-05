#pragma once

#include "resource.h"
#include "ThreadLib.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

static PACKAGE p0;
static PACKAGE p1;
static PACKAGE p2;
static PACKAGE p3;
static PACKAGE rPackage;

static HWND hWndMain;

static int NumPackage[3];

bool OnCreate(HWND hWnd)
{

	hWndMain = hWnd;

	ZeroMemory(&p0, sizeof(PACKAGE));
	ZeroMemory(&p1, sizeof(PACKAGE));
	ZeroMemory(&p2, sizeof(PACKAGE));
	ZeroMemory(&p3, sizeof(PACKAGE));
	ZeroMemory(&rPackage, sizeof(PACKAGE));

	ZeroMemory(&NumPackage, sizeof(int));
	
	return true;
}

bool OnStart(HWND hWnd)
{
	try{

		if (!(ghMutex = CreateMutex(NULL, FALSE, NULL)))
		{ 
			return false; 
		}

		hThreadMain = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MainThreadProc, NULL, 0, 0);

	}
	catch (...)
	{
		GetLastError();
	}
	return true;
}

void OnDestroy(HWND hWnd)
{
	for (int i = 0; i < MAX_TIMERS; i++)
	{
		KillTimer(hWnd, TIMERID[i]);
	}

	CloseHandle(ghMutex);
	

	PostQuitMessage(0);
}

void OnPaint(HWND hWnd, HDC hdc, RECT rect)
{
	GetClientRect(hWnd, &rect);

	DrawPath(hdc, rect);

}


// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
