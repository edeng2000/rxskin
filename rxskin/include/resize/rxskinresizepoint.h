// RXSkinResizePoint.h: interface for the CRXSkinResizePoint class.
//
// This file is a part of the CONTROLS MFC class library.
// (c)1998-2011 RXiang Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF RXiang Software AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF RXiang Software.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE RXSKIN PRO LICENSE AGREEMENT. RXiang Software GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// edeng_2000@163.com
// http://www.rxiang.com
//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_RXIANG_PRIVATE
#if !defined(__RXSKINRESIZEPOINT_H__)
#define __RXSKINRESIZEPOINT_H__
//}}AFX_RXIANG_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "rxskinresizerect.h"
// -----------------------------------------------------------------
// Summary:
//     RXSKIN_RESIZEPOINT structure is used by the CRXSkinResizePoint
//     class for defining single -precision floating point x- and
//     y-coordinates of a point.
// See Also:
//     CRXSkinResizePoint
// -----------------------------------------------------------------
struct RXSKIN_RESIZEPOINT
{
	RXSKIN_RESIZE x; // Specifies the x-coordinate of a point
	RXSKIN_RESIZE y; // Specifies the y-coordinate of a point
};

//===========================================================================
// Summary:
//     CRXSkinResizePoint is an RXSKIN_RESIZEPOINT structure derived class. The
//     CRXSkinResizePoint class is similar to the RXSKIN_RESIZEPOINT structure.
//     It also includes member functions to manipulate CRXSkinResizePoint and
//     RXSKIN_RESIZEPOINT structures. A CRXSkinResizePoint object can be used wherever
//     an RXSKIN_RESIZEPOINT structure is used.
//===========================================================================
class RXSKIN_API CRXSkinResizePoint : public RXSKIN_RESIZEPOINT
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CRXSkinResizePoint object
	// Parameters:
	//     cx - Specifies the x-coordinate of a point.
	//     cy - Specifies the y-coordinate of a point.
	//-----------------------------------------------------------------------
	CRXSkinResizePoint();
	CRXSkinResizePoint(RXSKIN_RESIZE cx, RXSKIN_RESIZE cy); // <combine CRXSkinResizePoint::CRXSkinResizePoint>
};

//-----------------------------------------------------------------------
// Summary:
//     CRXSkinResizePoint constants used by the CRXSkinResize class for defining the
//     anchor position for a child in a resizable window.
// Parameters:
//     RXSKIN_ANCHOR_TOPLEFT - Specifies a top left position.
//     RXSKIN_ANCHOR_TOPCENTER - Specifies a top center position.
//     RXSKIN_ANCHOR_TOPRIGHT - Specifies a top right position.
//     RXSKIN_ANCHOR_MIDDLELEFT - Specifies a middle left position.
//     RXSKIN_ANCHOR_MIDDLECENTER - Specifies a middle center position.
//     RXSKIN_ANCHOR_MIDDLERIGHT - Specifies a middle right position.
//     RXSKIN_ANCHOR_BOTTOMLEFT - Specifies a bottom left position.
//     RXSKIN_ANCHOR_BOTTOMCENTER - Specifies a bottom center position.
//     RXSKIN_ANCHOR_BOTTOMRIGHT - Specifies a bottom right position.
// Example:
//     The following example demonstrates the use of CRXSkinResizePoint.
// <code>
// // Set control resizing.
// SetResize(IDC_EDIT_MASKEDIT,   RXSKIN_ANCHOR_TOPLEFT,     RXSKIN_ANCHOR_TOPRIGHT);
// SetResize(IDC_TXT_MASK,        RXSKIN_ANCHOR_TOPCENTER,   RXSKIN_ANCHOR_TOPRIGHT);
// SetResize(IDC_LBOX_MASK,       RXSKIN_ANCHOR_TOPCENTER,   RXSKIN_ANCHOR_TOPRIGHT);
// SetResize(IDC_BTN_UPDATE,      RXSKIN_ANCHOR_TOPCENTER,   RXSKIN_ANCHOR_TOPCENTER);
// SetResize(IDC_EDIT_MASK,       RXSKIN_ANCHOR_TOPLEFT,     RXSKIN_ANCHOR_TOPCENTER);
// SetResize(IDC_EDIT_PROMPT,     RXSKIN_ANCHOR_TOPLEFT,     RXSKIN_ANCHOR_TOPCENTER);
// SetResize(IDC_EDIT_LITERAL,    RXSKIN_ANCHOR_TOPLEFT,     RXSKIN_ANCHOR_TOPCENTER);
// SetResize(IDC_EDIT_DEFAULT,    RXSKIN_ANCHOR_TOPLEFT,     RXSKIN_ANCHOR_TOPCENTER);
// SetResize(IDC_GBOX_PROPERTIES, RXSKIN_ANCHOR_TOPLEFT,     RXSKIN_ANCHOR_TOPCENTER);
// SetResize(IDCANCEL,            RXSKIN_ANCHOR_BOTTOMRIGHT, RXSKIN_ANCHOR_BOTTOMRIGHT);
// SetResize(IDOK,                RXSKIN_ANCHOR_BOTTOMRIGHT, RXSKIN_ANCHOR_BOTTOMRIGHT);
// </code>
// See Also:
//     CRXSkinResize, CRXSkinResizeDialog, CRXSkinResizeFormView, CRXSkinResizeGroupBox, CRXSkinResizePropertyPage, CRXSkinResizePropertySheet,  CRXSkinResizePoint, CRXSkinResizeRect
//-----------------------------------------------------------------------
#define RXSKIN_ANCHOR_BOTTOMCENTER CRXSkinResizePoint(.5, 1)
//<COMBINE RXSKIN_ANCHOR_BOTTOMCENTER>
#define RXSKIN_ANCHOR_BOTTOMLEFT   CRXSkinResizePoint(0, 1)
//<COMBINE RXSKIN_ANCHOR_BOTTOMCENTER>
#define RXSKIN_ANCHOR_BOTTOMRIGHT  CRXSkinResizePoint(1, 1)
//<COMBINE RXSKIN_ANCHOR_BOTTOMCENTER>
#define RXSKIN_ANCHOR_MIDDLECENTER CRXSkinResizePoint(.5, .5)
//<COMBINE RXSKIN_ANCHOR_BOTTOMCENTER>
#define RXSKIN_ANCHOR_MIDDLELEFT   CRXSkinResizePoint(0, .5)
//<COMBINE RXSKIN_ANCHOR_BOTTOMCENTER>
#define RXSKIN_ANCHOR_MIDDLERIGHT  CRXSkinResizePoint(1, .5)
//<COMBINE RXSKIN_ANCHOR_BOTTOMCENTER>
#define RXSKIN_ANCHOR_TOPLEFT      CRXSkinResizePoint(0, 0)
//<COMBINE RXSKIN_ANCHOR_BOTTOMCENTER>
#define RXSKIN_ANCHOR_TOPRIGHT     CRXSkinResizePoint(1, 0)
//<COMBINE RXSKIN_ANCHOR_BOTTOMCENTER>
#define RXSKIN_ANCHOR_TOPCENTER    CRXSkinResizePoint(.5, 0)

#endif // !defined(__RXSKINRESIZEPOINT_H__)
