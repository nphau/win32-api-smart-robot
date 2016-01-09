#pragma once

#include "resource.h"
#include "ThreadLib.h"
#include <Commctrl.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

static int total = 5;
static int remaining = total;
static DWORD32 numPackage[3];

int MAX_TIME_WAIT = 300;

void OnCreate(HWND hWnd)
{
	ZeroMemory(&packBegin, sizeof(PACKAGE));
	ZeroMemory(&packCurrent, sizeof(PACKAGE));
	packBegin.id = packCurrent.id = -1;
	remaining = total;
}

void OnStart(HWND hWnd)
{
	if (!(ghMutex = CreateMutex(NULL, FALSE, NULL)))
	{
		MessageBox(hWnd, L"Không tạo được tiến trình", L"", MB_OK);
		return;
	}

	hThreadMain = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MainThreadProc, &tData, 0, 0);
	
	tData.id = -1;
	tData.hWnd = hWnd;

	for (int i = 0; i < MAX_THREADS; i++)
	{
		hThread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, &tData, 0, 0);
	}
}
void OnReset(HWND hWnd)
{

}
void OnStop(HWND hWnd)
{
	CloseHandle(ghMutex); // Closing mutex handle
	CloseAllThreadHandle(hThread, MAX_THREADS);
}

void OnPaint(HWND hWnd, HDC hdc, RECT rect)
{
	GetClientRect(hWnd, &rect);

	DrawPath(hWnd, hdc, rect, total, remaining);

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

// Message handler for about box.
INT_PTR CALLBACK Speed(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER_SPEED), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(MAX_TIME_WAIT, 500));
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER_SPEED), TBM_SETSEL, (WPARAM)FALSE, (LPARAM)MAKELONG(MAX_TIME_WAIT, 500));
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER_SPEED), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)300);
		SetDlgItemInt(hDlg, IDC_EDIT_TOTAL, total, FALSE);
		return (INT_PTR)TRUE;
	case WM_HSCROLL:
		SetDlgItemInt(hDlg, IDC_SPEED, SendMessage(GetDlgItem(hDlg, IDC_SLIDER_SPEED), TBM_GETPOS, 0, 0), FALSE);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			MAX_TIME_WAIT = SendMessage(GetDlgItem(hDlg, IDC_SLIDER_SPEED), TBM_GETPOS, 0, 0);
			remaining = total = GetDlgItemInt(hDlg, IDC_EDIT_TOTAL, NULL, FALSE);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
