#pragma once
#include "rxskin.h"

#define		DIR_LEFT		1
#define		DIR_RIGHT		2
#define		DIR_TOP			3
#define		DIR_BOTTOM		4


#define		ANIMATE_NONE				0
#define		ANIMATE_SLIDE_TTB			1
#define		ANIMATE_SLIDE_LTR			2
#define		ANIMATE_UNFOLD_MTTB			3
#define		ANIMATE_UNFOLD_MTLR			4
#define		ANIMATE_SHUTTER_TTB			5
#define		ANIMATE_SHUTTER_LTR			6
#define		ANIMATE_UNFOLD_LTTRB		7
#define		ANIMATE_UNFOLDSLIDE_LTTRB	8
#define		ANIMATE_STRETCH_TTB			9
#define		ANIMATE_STRETCH_LTR			10
#define		ANIMATE_NOISE				11
#define		ANIMATE_FADE				12
#define		ANIMATE_RAND				100


/////////////////////////////////    API   << CHSGraphics >>    /////////////////////////////////////////////

#define			STEP			128//255
#define			COLORNUM		8

 RXSKIN_API void RXAPI AlphaBlend			( HDC hdcResult, int nX, int nY, int nWidth, int nHeight, HDC hdcDest, int nXDest, int nYDest, HDC hdcSrc, int nXSrc, int nYSrc, BYTE bAlpha );

 RXSKIN_API void RXAPI DrawAnimation		( RECT *prect, HDC hDCSrc, HDC hDCDest, int nType, int nStep, int nTimeDelay );

RXSKIN_API  void RXAPI CreateRoundRectRgn	( HRGN hRgn, const RECT &rect, int nWidth, int nHeight, BOOL bLeftTopR, BOOL bLeftBottomR, BOOL bRightTopR, BOOL bRightBottomR);

 RXSKIN_API void RXAPI BitBltTran			( HDC hDC, int nLeft, int nTop, int nWidth, int nHeight, HDC hdcSrc, int nX, int nY, COLORREF clrTran );

 RXSKIN_API void RXAPI DrawShadowIcon		( HDC hDC, int nX, int nY, HICON hIcon, int nWidth, int nHeight, COLORREF clrShadow );
 RXSKIN_API void RXAPI DrawFadeIcon		( HDC hDC, int nX, int nY, HICON hIcon, int nWidth, int nHeight, UINT nRate );
 RXSKIN_API void RXAPI DrawFadeBmp		( HDC hDC, int nX, int nY, HBITMAP hBmp, COLORREF clrTarget, UINT nRate );
 RXSKIN_API void RXAPI DrawTranBmp		( HDC hDC, int nX, int nY, HBITMAP hBmp, int nWidth, int nHeight, COLORREF clrTran );

 RXSKIN_API void RXAPI Draw3DRgnEdge		( HDC hDC, BOOL bHeave, HRGN rgnWnd );
 RXSKIN_API void RXAPI Draw3DLine			( HDC hDC, BOOL bHeave, const POINT& ptStart, const POINT& ptEnd );
 RXSKIN_API void RXAPI Draw3DRectEdge		( HDC hDC, BOOL bHeave, int nLeft, int nTop, int nRight, int nBottom );

 RXSKIN_API void RXAPI DrawRgn			( HDC hDC, COLORREF clrBkgnd, HRGN hRgn );
 RXSKIN_API void RXAPI DrawRgn			( HDC hDC, COLORREF clrBorder, COLORREF clrBkgnd, HRGN hRgn);
 RXSKIN_API void RXAPI DrawFrameRgn		( HDC hDC, HRGN hRgn, COLORREF clrEdge, int nWidth, int nHeight );
 RXSKIN_API void RXAPI DrawPartionRgn		( HDC hDC, COLORREF clrLeftTop, COLORREF clrRightBottom, BOOL bVerticalLine, int nOffset, HRGN hRgn);

 RXSKIN_API void RXAPI DrawRect			( HDC hDC, COLORREF clrBorder, COLORREF clrBkgnd, int nLeft, int nTop, int nRight, int  nBottom );
 RXSKIN_API void RXAPI DrawRect			( HDC hDC, COLORREF clrFill, int nLeft, int nTop, int nRight, int nBottom );
 RXSKIN_API void RXAPI DrawArrow			( HDC hDC, COLORREF clrArrow, int nX, int nY, int nSize, int nDirection = DIR_RIGHT);
 RXSKIN_API void RXAPI DrawPolyline		( HDC hDC, COLORREF clrLine, POINT* pt, int nSize );
 RXSKIN_API void RXAPI DrawEllipse			( HDC hDC, COLORREF clrBorder, COLORREF clrBkgnd, int nLeft, int nTop, int nRight,  int nBottom );
 RXSKIN_API void RXAPI DrawRoundRect		( HDC hDC, COLORREF clrBorder, COLORREF clrBkgnd, const RECT& rect, int nWidth, int nHeight);
 RXSKIN_API void RXAPI DrawRoundRect		( HDC hDC, COLORREF clrBorder, COLORREF clrBkgnd, int nLeft, int nTop, int nRight, int nBottom, int nWidth, int nHeight);
 RXSKIN_API void RXAPI DrawRectEdge		( HDC hDC, const RECT& rect, int nWidth, COLORREF clrEdge, int nStyle = PS_SOLID);

 RXSKIN_API void RXAPI DrawGradientRgn	( HDC hDC, BOOL bVertical, HRGN hRgn, COLORREF clrStart, COLORREF clrEnd );
 RXSKIN_API void RXAPI DrawGradientRect	( HDC hDC, BOOL bVertical, const RECT& rectFill, COLORREF clrStart, COLORREF clrEnd );
 RXSKIN_API void RXAPI DrawLightPointRect	( HDC hDC, const RECT &rectFill, const POINT &pointLight, COLORREF clrBkgnd, COLORREF clrPoint, float fFouce = 0.0f);
 RXSKIN_API void RXAPI DrawLine(HDC hDC, const RECT& rc, int nSize, DWORD dwPenColor, int nStyle = PS_SOLID);
 RXSKIN_API void RXAPI DrawLine(HDC hDC, const POINT& ptStart, const POINT& ptEnd, int nSize, DWORD dwPenColor, int nStyle = PS_SOLID);
 RXSKIN_API void RXAPI GdiplusDrawText(HDC hDC, HFONT hFont, int32_t nHint, int32_t nLeft, int32_t nTop, LPCTSTR pstrText, DWORD dwTextColor, UINT uStyle);
 RXSKIN_API void RXAPI GdiplusDrawText(HDC hDC, HFONT hFont,int32_t nHint, RECT& rc, LPCTSTR pstrText, DWORD dwTextColor, UINT uStyle);
