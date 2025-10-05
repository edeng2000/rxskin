#include "pch.h"
#include "resize/rxskinresizerect.h"
#include "stdlib.h"

CRXSkinResizeRect::CRXSkinResizeRect()
{
	left = top = right = bottom = 0;
}

//////////////////////////////////////////////////////////////////////

CRXSkinResizeRect::CRXSkinResizeRect(RXSKIN_RESIZE l, RXSKIN_RESIZE t, RXSKIN_RESIZE r, RXSKIN_RESIZE b)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}

CRXSkinResizeRect& CRXSkinResizeRect::operator=(const RECT& rc)
{
	left = (RXSKIN_RESIZE) rc.left;
	top = (RXSKIN_RESIZE) rc.top;
	right = (RXSKIN_RESIZE) rc.right;
	bottom = (RXSKIN_RESIZE) rc.bottom;

	return *this;
}

CRXSkinResizeRect& CRXSkinResizeRect::operator=(const RXSKIN_RESIZERECT& rrc)
{
	left = rrc.left;
	top = rrc.top;
	right = rrc.right;
	bottom = rrc.bottom;

	return *this;
}

CRXSkinResizeRect& CRXSkinResizeRect::operator+=(const RXSKIN_RESIZERECT& rrc)
{
	left += rrc.left;
	top += rrc.top;
	right += rrc.right;
	bottom += rrc.bottom;

	return *this;
}

CRXSkinResizeRect CRXSkinResizeRect::operator & (const RXSKIN_RESIZERECT& rrc)
{
	(IsNormalized() && ((CRXSkinResizeRect&) rrc).IsNormalized());

	CRXSkinResizeRect rrcI(
		__max(left, rrc.left),
		__max(top, rrc.top),
		__min(right, rrc.right),
		__min(bottom, rrc.bottom));

	// only intersection if resulting rect is normalized
	return (rrcI.IsNormalized()) ? rrcI : CRXSkinResizeRect(0, 0, 0, 0);
}
