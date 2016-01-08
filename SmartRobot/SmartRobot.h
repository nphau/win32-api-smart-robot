#pragma once

#include "resource.h"
#include "ThreadLib.h"
#include "TimerLib.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

static HWND hWndMain;
static DWORD32 total = 2;
static DWORD32 remaining = total;
static DWORD32 numPackage[3];

void OnCreate(HWND hWnd)
{
	hWndMain = hWnd;

	ZeroMemory(&packBegin, sizeof(PACKAGE));
	ZeroMemory(&packCurrent, sizeof(PACKAGE));
}

void OnStart(HWND hWnd)
{
	CallMainTimer(hWndMain, 500);

	if (!(ghMutex = CreateMutex(NULL, FALSE, NULL)))
	{
		MessageBox(hWndMain, L"Không tạo được tiến trình", L"", MB_OK);
		return;
	}

	hThreadMain = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MainThreadProc, NULL, 0, 0);
	
	for (int i = 0; i < MAX_THREADS; i++)
	{
		hThread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, NULL, 0, 0);
	}
}
void OnDestroy(HWND hWnd)
{
	CloseHandle(ghMutex); // Closing mutex handle
	CloseAllTimer(hWnd, TIMERID, MAX_TIMERS); // Closing all timer
	PostQuitMessage(0);	// Exit message
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
