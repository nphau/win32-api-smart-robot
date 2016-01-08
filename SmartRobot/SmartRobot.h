#pragma once

#include "resource.h"
#include "ThreadLib.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

static HWND hWndMain;
static int total = 2;
static int remaining = total;
static DWORD32 numPackage[3];

void OnCreate(HWND hWnd)
{
	hWndMain = hWnd;

	ZeroMemory(&packBegin, sizeof(PACKAGE));
	ZeroMemory(&packCurrent, sizeof(PACKAGE));
	packBegin.id = packCurrent.id = -1;
	remaining = total;
}

void OnStart(HWND hWnd)
{
	// CallMainTimer(hWndMain, 500);

	if (!(ghMutex = CreateMutex(NULL, FALSE, NULL)))
	{
		MessageBox(hWndMain, L"Không tạo được tiến trình", L"", MB_OK);
		return;
	}

	hThreadMain = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MainThreadProc, NULL, 0, 0);
	
	tData.id = -1;
	tData.hWnd = hWnd;

	for (int i = 0; i < MAX_THREADS; i++)
	{
		hThread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, &tData, 0, 0);
	}
}
void OnDestroy(HWND hWnd)
{
	CloseHandle(ghMutex); // Closing mutex handle
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
