#pragma once

#include "resource.h"
#include "GuiLib.h"

/* Package information */
typedef struct tagPACKAGE
{
	RECT rEnd;			// End point
	RECT rCurr;			// Current point
	RECT rBegin;		// Begin point
	LONG total;
	DWORD32	id;			// Identify number
	COLORREF color;		// Color of package
	bool isAvailable;	// Is A package available?

} PACKAGE, *PPACKAGE;

static PACKAGE packBegin;
static PACKAGE packCurrent;

/* Copy infomation */
void CopyPackage(PACKAGE &dest, const PACKAGE src)
{
	dest.id = src.id;
	dest.color = src.color;
	dest.total = src.total;
	dest.isAvailable = src.isAvailable;

	CopyRect(&dest.rEnd, &src.rEnd);
	CopyRect(&dest.rBegin, &src.rBegin);
	CopyRect(&dest.rCurr, &src.rCurr);
}

/* Moving package is showed in client screen */
bool MovePackage(HWND hWnd, HDC hdc, PACKAGE& package, DIRECTION direction)
{
	switch (direction)
	{
	case DIRECTION::LEFT_TO_RIGHT:

		// Check begin point and finish point are correct or not
		if ((package.rEnd.left < package.rBegin.left) ||
			(package.total >(package.rEnd.left - package.rBegin.left) / sizeBox))
		{
			InvalidateRect(hWnd, &packCurrent.rCurr, true);
			return false;
		}

		// Erase previous object on screen
		InvalidateRect(hWnd, &package.rCurr, true);

		// Create new object
		SetRect(&package.rCurr, package.rBegin.left + package.total * sizeBox,
			package.rBegin.top, package.rBegin.right, package.rBegin.bottom);

		// Coloring and showing
		DrawBox(hdc, package.rCurr, package.color);

		package.total++;

		break;
	case DIRECTION::RIGHT_TO_LEFT:

		if ((package.rEnd.left > package.rBegin.left) ||
			(package.total > (package.rBegin.left - package.rEnd.left) / sizeBox) )
		{
			InvalidateRect(hWnd, &packCurrent.rCurr, true);
			return false;
		}

		InvalidateRect(hWnd, &package.rCurr, true);

		SetRect(&package.rCurr,
			package.rBegin.left - package.total * sizeBox,
			package.rBegin.top, package.rBegin.right, package.rBegin.bottom);

		DrawBox(hdc, package.rCurr, package.color);

		package.total++;

		break;
	case DIRECTION::BOTTOM_TO_TOP:

		if ((package.rEnd.bottom > package.rBegin.bottom) ||
			(package.total > (package.rBegin.bottom - package.rEnd.bottom) / sizeBox) )
		{
			InvalidateRect(hWnd, &packCurrent.rCurr, true);
			return false;
		}

		InvalidateRect(hWnd, &package.rCurr, true);

		SetRect(&package.rCurr,
			package.rBegin.left,
			package.rBegin.top - package.total * sizeBox,
			package.rBegin.right, package.rBegin.bottom);

		DrawBox(hdc, package.rCurr, package.color);

		package.total++;

		break;
	case DIRECTION::TOP_TO_BOTTOM:

		if ((package.rEnd.top < package.rBegin.top) ||
			(package.total > (package.rEnd.top - package.rBegin.top) / sizeBox))
		{
			InvalidateRect(hWnd, &packCurrent.rCurr, true);
			return false;
		}

		InvalidateRect(hWnd, &package.rCurr, true);

		SetRect(&package.rCurr, package.rBegin.left,
			package.rBegin.top + package.total * sizeBox, package.rBegin.right, package.rBegin.bottom);

		DrawBox(hdc, package.rCurr, package.color);

		package.total++;

		break;
	}
	return true;
}
