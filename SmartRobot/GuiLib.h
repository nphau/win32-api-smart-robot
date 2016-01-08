#pragma once

#include "resource.h"

const int sizeBox = 60;

static int cWeight = 0;
static int cHeight = 0;
static int cxClient = 0;
static int cyClient = 0;


extern DWORD32 total;
extern DWORD32 remaining;
extern DWORD32 numPackage[3];
extern HWND hWndMain;

enum DIRECTION {
	LEFT_TO_RIGHT,
	RIGHT_TO_LEFT,
	BOTTOM_TO_TOP,
	TOP_TO_BOTTOM
};

const int MAX_PACKAGE_TYPE = 3;

static COLORREF colorType[MAX_PACKAGE_TYPE] = {
	RGB(255, 0, 0),		// Red
	RGB(232, 12, 122),	// Violet
	RGB(0, 0, 255) };	// Blue

const int MAX_LISTBOX = 4;
static RECT listBox[MAX_LISTBOX];

/* Draw a box in screen */
void DrawBox(HDC hdc, RECT rect, COLORREF color)
{
	SelectObject(hdc, GetStockObject(DC_BRUSH));
	SetDCBrushColor(hdc, color);
	Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
}

/* Update info */
void UpdateInfo(HWND hWnd, int id)
{
	WCHAR wInfo[MAX_PATH] = L"Total = ";
	HDC hdc = GetDC(hWnd);
	switch (id)
	{
	case -1:

		swprintf(wInfo, L"Total = %d ", total);
		TextOutW(hdc, 10, listBox[0].bottom - 40, wInfo, 20);
		swprintf(wInfo, L"Remaining = %d", remaining);
		TextOutW(hdc, 10, listBox[0].bottom - 20, wInfo, 20);

		break;
	case 0:

		// Count
		SetTextColor(hdc, colorType[0]);
		swprintf(wInfo, L"(Count=%d)", numPackage[0]);
		TextOutW(hdc, listBox[0].right + 10, 5, wInfo, 10);

		// LINE 1
		wcscpy(wInfo, L"LINE 1");
		TextOutW(hdc, listBox[0].right + 10, 20, wInfo, 6);

		break;
	case 1:
		// Count
		SetTextColor(hdc, colorType[2]);
		wcscpy(wInfo, L"LINE 2");
		TextOutW(hdc, listBox[1].right - sizeBox - 20,listBox[1].bottom + sizeBox / 2, wInfo, 6);

		// LINE 2
		swprintf(wInfo, L"(Count=%d)", numPackage[1]);
		TextOutW(hdc, listBox[1].right - sizeBox - 20, listBox[1].bottom + sizeBox / 1.2, wInfo, 10);

		break;
	case 2:

		// LINE 3
		SetTextColor(hdc, colorType[1]);
		wcscpy(wInfo, L"LINE 3");
		TextOutW(hdc, listBox[2].right + 10, listBox[2].bottom - sizeBox / 1.5, wInfo, 6);

		// Count 
		swprintf(wInfo, L"(Count=%d)", numPackage[2]);
		TextOutW(hdc, listBox[2].right + 10, listBox[2].bottom - sizeBox / 3, wInfo, 10);


		break;
	}
}

/* Draw paths in screen */
void DrawPath(HDC hdc, RECT rect)
{
	cxClient = rect.right - rect.left;
	cyClient = rect.bottom - rect.top;

	cWeight = cxClient / 2;
	cHeight = cyClient / 2;

	listBox[0].left = listBox[0].top = 0;
	listBox[0].bottom = cHeight - sizeBox;
	listBox[0].right = cWeight - sizeBox;

	listBox[1].top = 0;
	listBox[1].left = cxClient - listBox[0].right - sizeBox / 2;
	listBox[1].bottom = listBox[0].bottom;
	listBox[1].right = cxClient;

	listBox[2].top = listBox[0].bottom + 3 * sizeBox / 2;
	listBox[2].left = 0;
	listBox[2].bottom = cyClient;
	listBox[2].right = listBox[0].right;

	listBox[3].top = listBox[2].top;
	listBox[3].left = listBox[1].left;
	listBox[3].bottom = cyClient;
	listBox[3].right = cxClient;

	SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB(0, 0, 255));

	// Draw 4 rectangle
	for (int i = 0; i < MAX_LISTBOX; i++)
	{
		Rectangle(hdc, listBox[i].left, listBox[i].top, listBox[i].right, listBox[i].bottom);
	}

	// Update info
	for (int i = -1; i < 3; i++)
	{
		UpdateInfo(hWndMain, i);
	}

}
