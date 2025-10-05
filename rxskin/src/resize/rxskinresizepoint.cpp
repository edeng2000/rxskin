#include "pch.h"

#include "resize/rxskinresizerect.h"
#include "resize/rxskinresizepoint.h"



CRXSkinResizePoint::CRXSkinResizePoint()
{
	x = y = 0;
}

CRXSkinResizePoint::CRXSkinResizePoint(RXSKIN_RESIZE cx, RXSKIN_RESIZE cy)
{
	x = cx;
	y = cy;
}
