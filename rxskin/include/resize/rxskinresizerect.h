#if !defined(__RXSKINRESIZERECT_H__)
#define __RXSKINRESIZERECT_H__
//}}AFX_RXIANG_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "rxskin.h"
// ----------------------------------------------------------------------
// Summary:
//     The RXSKIN_RESIZE data type is defined as a <i>float</i> and is
//     used for storing single-precision floating point x and y window
//     coordinates.
// See Also:
//     CRXSkinResizeRect, RXSKIN_RESIZERECT, CRXSkinResizePoint, RXSKIN_RESIZEPOINT
// ----------------------------------------------------------------------
typedef float RXSKIN_RESIZE;

// ---------------------------------------------------------------------
// Summary:
//     RXSKIN_RESIZERECT structure is used by the CRXSkinResizeRect class
//     for defining single -precision floating point coordinates of the
//     upper-left and lower-right corners of a rectangle.
// Example:
//     The following example demonstrates alternate ways to initialize a RXSKIN_RESIZERECT structure:
// <code>
// RXSKIN_RESIZERECT rc1;
// rc1.left = 20;
// rc1.top = 30;
// rc1.right = 180;
// rc1.bottom = 230;
//
// RXSKIN_RESIZERECT rc2 = {20, 30, 180, 230};
// </code>
// See Also:
//     CRXSkinResizeRect, RXSKIN_RESIZE
// ---------------------------------------------------------------------
struct RXSKIN_RESIZERECT
{
	RXSKIN_RESIZE left;     // Specifies the x-coordinate of the upper-left corner of a rectangle.
	RXSKIN_RESIZE top;      // Specifies the y-coordinate of the upper-left corner of a rectangle.
	RXSKIN_RESIZE right;    // Specifies the x-coordinate of the lower-right corner of a rectangle.
	RXSKIN_RESIZE bottom;   // Specifies the y-coordinate of the lower-right corner of a rectangle.
};

//===========================================================================
// Summary:
//     CRXSkinResizeRect is an RXSKIN_RESIZERECT structure derived class. The
//     CRXSkinResizeRect class is similar to an RXSKIN_RESIZERECT structure. CRXSkinResizeRect
//     also includes member functions to manipulate CRXSkinResizeRect objects and
//     RXSKIN_RESIZERECT structures.
// See Also:
//     RXSKIN_RESIZERECT, CRXSkinResizePoint, RXSKIN_RESIZEPOINT, RXSKIN_RESIZE
//===========================================================================
class RXSKIN_API CRXSkinResizeRect: public RXSKIN_RESIZERECT
{
public:

	CRXSkinResizeRect();
	CRXSkinResizeRect(const RECT& rc); // <combine CRXSkinResizeRect::CRXSkinResizeRect>
	CRXSkinResizeRect(const RXSKIN_RESIZERECT& rrc); // <combine CRXSkinResizeRect::CRXSkinResizeRect>
	CRXSkinResizeRect(RXSKIN_RESIZE l, RXSKIN_RESIZE t, RXSKIN_RESIZE r, RXSKIN_RESIZE b); // <combine CRXSkinResizeRect::CRXSkinResizeRect>

	CRXSkinResizeRect& operator=(const RECT& rc);
	CRXSkinResizeRect& operator=(const RXSKIN_RESIZERECT& rrc);
	CRXSkinResizeRect& operator+=(const RXSKIN_RESIZERECT& rrc);
	CRXSkinResizeRect operator+(const RXSKIN_RESIZERECT& rrc);
	CRXSkinResizeRect operator & (const RXSKIN_RESIZERECT& rrc);


	bool operator==(const RXSKIN_RESIZERECT& rrc);


	bool operator!= (const RXSKIN_RESIZERECT& rrc);

	//-----------------------------------------------------------------------
	// Summary:
	//     This operator converts a CRXSkinResizeRect to a CRect. When you use this
	//     function, you do not need the address-of (&) operator. This operator
	//     will be automatically used when you pass a CRXSkinResizeRect object to
	//     a function that expects a CRect.
	//-----------------------------------------------------------------------

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function determines if CRXSkinResizeRect is normalized.
	// Returns:
	//     true if normalized, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsNormalized();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function calculates the width of a CRXSkinResizeRect by subtracting
	//     the left value from the right value. The resulting value can be negative.
	// Returns:
	//     The width of a CRXSkinResizeRect.
	//-----------------------------------------------------------------------
	RXSKIN_RESIZE Width();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function calculates the height of a CRXSkinResizeRect by subtracting
	//     the top value from the bottom value. The resulting value can be negative.
	// Returns:
	//     The height of a CRXSkinResizeRect.
	//-----------------------------------------------------------------------
	RXSKIN_RESIZE Height();
};

//////////////////////////////////////////////////////////////////////

_inline CRXSkinResizeRect::CRXSkinResizeRect(const RECT& rc) {
	(operator=)(rc);
}
_inline CRXSkinResizeRect::CRXSkinResizeRect(const RXSKIN_RESIZERECT& rrc) {
	(operator=)(rrc);
}
_inline CRXSkinResizeRect CRXSkinResizeRect::operator+(const RXSKIN_RESIZERECT& rrc) {
	return CRXSkinResizeRect(left + rrc.left, top + rrc.top, right + rrc.right, bottom + rrc.bottom);
}
_inline bool CRXSkinResizeRect::IsNormalized() {
	return ((left <= right) && (top <= bottom));
}
_inline bool CRXSkinResizeRect::operator==(const RXSKIN_RESIZERECT& rrc) {
	return left == rrc.left && top == rrc.top && right == rrc.right && bottom == rrc.bottom;
}
_inline bool CRXSkinResizeRect::operator!= (const RXSKIN_RESIZERECT& rrc) {
	return !operator==(rrc);
}

_inline RXSKIN_RESIZE CRXSkinResizeRect::Width() {
	return right - left;
}
_inline RXSKIN_RESIZE CRXSkinResizeRect::Height() {
	return bottom - top;
}

//-----------------------------------------------------------------------
// Summary:
//     CRXSkinResizeRect constants used by the CRXSkinResize class for defining the
//     resize attributes for a child in a resizable window.
// Parameters:
//     RXSKIN_ATTR_RESIZE - Resize.
//     RXSKIN_ATTR_REPOS - Reposition.
//     RXSKIN_ATTR_HORRESIZE - Horizontal resize.
//     RXSKIN_ATTR_HORREPOS - Horizontal reposition.
//     RXSKIN_ATTR_VERRESIZE - Vertical resize.
//     RXSKIN_ATTR_VERREPOS - Vertical reposition.
// Example:
//     The following example demonstrates how to use CRXSkinResizeRect.
// <code>
// // Set control resizing.
// SetResize(ID_WIZBACK, RXSKIN_ATTR_REPOS(1));
// SetResize(ID_WIZNEXT, RXSKIN_ATTR_REPOS(1));
// SetResize(ID_WIZFINISH, RXSKIN_ATTR_REPOS(1));
// SetResize(ID_WIZLINE, RXSKIN_ANCHOR_BOTTOMLEFT, RXSKIN_ANCHOR_BOTTOMRIGHT);
// </code>
// See Also:
//     CRXSkinResize, CRXSkinResizeDialog, CRXSkinResizeFormView, CRXSkinResizeGroupBox, CRXSkinResizePropertyPage, CRXSkinResizePropertySheet,  CRXSkinResizePoint, CRXSkinResizeRect
//-----------------------------------------------------------------------
#define RXSKIN_ATTR_RESIZE(x)    CRXSkinResizeRect(0, 0, x, x)
// <COMBINE RXSKIN_ATTR_RESIZE>
#define RXSKIN_ATTR_REPOS(x)     CRXSkinResizeRect(x, x, x, x)
// <COMBINE RXSKIN_ATTR_RESIZE>
#define RXSKIN_ATTR_HORRESIZE(x) CRXSkinResizeRect(0, 0, x, 0)
// <COMBINE RXSKIN_ATTR_RESIZE>
#define RXSKIN_ATTR_HORREPOS(x)  CRXSkinResizeRect(x, 0, x, 0)
// <COMBINE RXSKIN_ATTR_RESIZE>
#define RXSKIN_ATTR_VERRESIZE(x) CRXSkinResizeRect(0, 0, 0, x)
// <COMBINE RXSKIN_ATTR_RESIZE>
#define RXSKIN_ATTR_VERREPOS(x)  CRXSkinResizeRect(0, x, 0, x)

#endif // !defined(__RXSKINRESIZERECT_H__)
