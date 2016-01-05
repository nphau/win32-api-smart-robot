#pragma once

#include "resource.h"

const int sizeBox = 50;

static int cxClient = 0;
static int cyClient = 0;
static int cWeight = 0;
static int cHeight = 0;

enum DIRECTION {
	LEFT_TO_RIGHT,
	RIGHT_TO_LEFT,
	BOTTOM_TO_TOP,
	TOP_TO_BOTTOM
};

/* Package information */
typedef struct PACKAGE
{
	RECT rEnd;			// End point
	RECT rCurr;			// Current point
	RECT rBegin;		// Begin point
	DWORD32 total;
	COLORREF color;
	bool isValid;

} PACKAGE, *PPACKAGE;

const int MAX_PACKAGE_TYPE = 3;

static COLORREF colorType[MAX_PACKAGE_TYPE] = {
	RGB(255, 0, 0),		// Red
	RGB(155, 89, 182),	// Violet
	RGB(0, 0, 255) };	// Blue

const int MAX_LISTBOX = 4;
static RECT listBox[MAX_LISTBOX];


void CopyPackage(PACKAGE &dest, PACKAGE src)
{
	dest.color = src.color;
	dest.total = src.total;
	dest.isValid = src.isValid;
	
	CopyRect(&dest.rEnd, &src.rEnd);
	CopyRect(&dest.rBegin, &src.rBegin);
	CopyRect(&dest.rCurr, &src.rCurr);
}

void DrawBox(HDC hdc, RECT rect, COLORREF color)
{
	SelectObject(hdc, GetStockObject(DC_BRUSH));
	SetDCBrushColor(hdc, color);

	Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
}

/**/
bool MovePackage(HWND hWnd, HDC hdc, PACKAGE& package, DIRECTION direction)
{
	DWORD32 step = 0;

	switch (direction)
	{
	case DIRECTION::LEFT_TO_RIGHT:

		if (package.rEnd.left < package.rBegin.left){ return false; }

		step = (package.rEnd.left - package.rBegin.left) / sizeBox + 1;

		if (package.total <= step)
		{
			InvalidateRect(hWnd, &package.rCurr, true);

			SetRect(&package.rCurr, package.rBegin.left + package.total * sizeBox,
				package.rBegin.top, package.rBegin.right, package.rBegin.bottom);

			DrawBox(hdc, package.rCurr, package.color);

			package.total++;
		}
		else
		{
			return false;
		}
		break;
	case DIRECTION::RIGHT_TO_LEFT:

		if (package.rEnd.left > package.rBegin.left) { return false; }

		step = (package.rBegin.left - package.rEnd.left) / sizeBox + 1;

		if (package.total <= step)
		{
			// 
			InvalidateRect(hWnd, &package.rCurr, true);

			SetRect(&package.rCurr,
				package.rBegin.left - package.total * sizeBox,
				package.rBegin.top, package.rBegin.right, package.rBegin.bottom);

			DrawBox(hdc, package.rCurr, package.color);

			package.total++;
		}
		else
		{
			return false;
		}
		break;
	case DIRECTION::BOTTOM_TO_TOP:

		if (package.rEnd.bottom > package.rBegin.bottom) { return false; }

		step = (package.rBegin.bottom - package.rEnd.bottom) / sizeBox + 1;

		if (package.total <= step)
		{
			// 
			InvalidateRect(hWnd, &package.rCurr, true);

			SetRect(&package.rCurr,
				package.rBegin.left,
				package.rBegin.top - package.total * sizeBox,
				package.rBegin.right, package.rBegin.bottom);

			DrawBox(hdc, package.rCurr, package.color);

			package.total++;
		}
		else
		{
			return false;
		}
		break;
	case DIRECTION::TOP_TO_BOTTOM:

		if (package.rEnd.top < package.rBegin.top) { return false; }

		step = (package.rEnd.top - package.rBegin.top) / sizeBox + 1;

		if (package.total <= step)
		{
			// 
			InvalidateRect(hWnd, &package.rCurr, true);

			SetRect(&package.rCurr,
				package.rBegin.left,
				package.rBegin.top + package.total * sizeBox,
				package.rBegin.right,
				package.rBegin.bottom);

			DrawBox(hdc, package.rCurr, package.color);

			package.total++;
		}
		else
		{
			return false;
		}
		break;
	}
	return true;
}

/**/
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

	for (int i = 0; i < MAX_LISTBOX; i++)
	{
		Rectangle(hdc, listBox[i].left, listBox[i].top, listBox[i].right, listBox[i].bottom);
	}

}
