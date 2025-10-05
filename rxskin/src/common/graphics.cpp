#include "pch.h"
#include "common/graphics.h"
#include <math.h>
#include "stdlib.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*
	Draw3DLine
	 画一条具有3D外观的直线.
	参数:
	hDC 
*/

void RXAPI Draw3DLine(HDC hDC, BOOL bHeave, const POINT& ptStart, const POINT& ptEnd)
{
	HPEN	hpen;
	HGDIOBJ hpenOld;

	hpen = ::CreatePen( PS_SOLID, 1, GetSysColor( bHeave ? COLOR_3DHILIGHT : COLOR_3DSHADOW  ) );
	hpenOld = ::SelectObject( hDC, hpen );
	::MoveToEx( hDC, ptStart.x, ptStart.y, NULL );
	::LineTo( hDC, ptEnd.x, ptEnd.y );
	::SelectObject( hDC, hpenOld );
	::DeleteObject( hpen );
	
	hpen = ::CreatePen( PS_SOLID, 1, GetSysColor( bHeave ? COLOR_3DSHADOW : COLOR_3DHILIGHT ) );
	hpenOld = ::SelectObject( hDC, hpen );
	::MoveToEx( hDC, ptStart.x, ptStart.y+1, NULL );
	::LineTo( hDC, ptEnd.x, ptEnd.y+1 );
	::SelectObject( hDC, hpenOld );
	::DeleteObject( hpen );
}

void RXAPI DrawRectEdge(HDC hDC, const RECT& rect, int nWidth, COLORREF clrEdge, int nStyle)
{
	HPEN	hpen	= ::CreatePen( nStyle, nWidth, clrEdge );
	HGDIOBJ	hobjOld = ::SelectObject( hDC, hpen );

	POINT	pt[3];

	pt[0].x = pt[1].x = rect.left;							//	 p1	+-----+ p2
	pt[0].y = rect.bottom;									//		|	
	pt[1].y = pt[2].y = rect.top;							//		|
	pt[2].x = rect.right;									//	 p0	+

	::Polyline( hDC, pt, 3 );

	pt[1].x = pt[2].x = rect.right;							//		      + p2
	pt[1].y = rect.bottom;									//			  |	
	pt[2].y = rect.top;										//			  |			
															//	 p0 +-----+	p1	
	::Polyline( hDC, pt, 3 );

	::SelectObject( hDC, hobjOld );
	::DeleteObject( hpen );
}

void RXAPI Draw3DRectEdge(HDC hDC, BOOL bHeave, int nLeft, int nTop, int nRight, int nBottom)
{
	HPEN	hpen;
	HGDIOBJ hpenOld;
	POINT	pt[3];

	pt[0].x = pt[1].x = nLeft;							//	 p1	+-----+ p2
	pt[0].y = nBottom;									//		|	
	pt[1].y = pt[2].y = nTop;							//		|
	pt[2].x = nRight;									//	 p0	+
		
	hpen = ::CreatePen( PS_SOLID, 1, GetSysColor( bHeave ? COLOR_3DHILIGHT : COLOR_3DSHADOW  ) );
	hpenOld = ::SelectObject( hDC, hpen );
	::Polyline( hDC, pt, 3 );
	::SelectObject( hDC, hpenOld );
	::DeleteObject( hpen );

	pt[1].x = pt[2].x = nRight;							//		      + p2
	pt[1].y = nBottom;									//			  |	
	pt[2].y = nTop;										//			  |			
														//	 p0 +-----+	p1	

	hpen = ::CreatePen( PS_SOLID, 1, GetSysColor( bHeave ? COLOR_3DSHADOW : COLOR_3DHILIGHT ) );
	hpenOld = ::SelectObject( hDC, hpen );
	::Polyline( hDC, pt, 3 );
	::SelectObject( hDC, hpenOld );
	::DeleteObject( hpen );
}

void RXAPI Draw3DRgnEdge(HDC hDC, BOOL bHeave, HRGN rgnWnd)
{
	HBRUSH		hbr;
	HRGN		rgnTemp = ::CreateRectRgn( 0, 0, 0, 0 );

	::CombineRgn( rgnTemp, rgnWnd, 0, RGN_COPY );
	::OffsetRgn ( rgnTemp, 1, 1 );
	::CombineRgn( rgnTemp, rgnWnd, rgnTemp, RGN_DIFF );
	hbr = ::CreateSolidBrush( GetSysColor( bHeave ? COLOR_3DHILIGHT : COLOR_3DSHADOW ) );
	::FillRgn( hDC, rgnTemp, hbr );
	::DeleteObject( hbr );
	
	::CombineRgn( rgnTemp, rgnWnd, 0, RGN_COPY );
	::OffsetRgn( rgnTemp, -1, -1 );
	::CombineRgn( rgnTemp, rgnWnd, rgnTemp, RGN_DIFF );
	hbr = ::CreateSolidBrush( GetSysColor( bHeave ? COLOR_3DSHADOW : COLOR_3DHILIGHT ) );
	::FillRgn( hDC, rgnTemp, hbr );
	::DeleteObject( hbr );

	::DeleteObject( rgnTemp );
}


void RXAPI DrawGradientRgn(HDC hDC, BOOL bVertical, HRGN hRgn, COLORREF clrStart, COLORREF clrEnd)
{
	RECT	rect;
	RECT	rcFill;

	::GetRgnBox ( hRgn, &rect );
	int	nWidth  = rect.right  - rect.left;
	int	nHeight = rect.bottom - rect.top; 

	rcFill.left		= 0;
	rcFill.top		= 0;
	rcFill.right	= nWidth;
	rcFill.bottom	= nHeight;

	HRGN	hrgnTmp = ::CreateRectRgn( 0, 0, 0, 0 );
	::CombineRgn( hrgnTmp, hRgn, 0, RGN_COPY );
	::OffsetRgn( hrgnTmp, -rect.left, -rect.top );

	HDC		hdcMem   = ::CreateCompatibleDC( hDC );
	HBITMAP	hbmpMem  = ::CreateCompatibleBitmap( hDC, nWidth, nHeight );
	HGDIOBJ hobjOld  = ::SelectObject( hdcMem, hbmpMem );

	DrawRect( hdcMem, 0x00FFFFFF, rcFill.left, rcFill.top, rcFill.right, rcFill.bottom );

	HGDIOBJ holdRgn = ::SelectObject( hdcMem, hrgnTmp );

	DrawGradientRect( hdcMem, bVertical, rcFill, clrStart, clrEnd );

	BitBltTran( hDC, rect.left, rect.top, nWidth, nHeight, hdcMem, 0, 0, 0x00FFFFFF );

	::SelectObject( hdcMem, holdRgn );
	::SelectObject( hdcMem, hobjOld );
	::DeleteObject( hrgnTmp );	
	::DeleteObject( hbmpMem );
	::DeleteDC( hdcMem );	
}

void RXAPI BitBltTran( HDC hDC, int nLeft, int nTop, int nWidth, int nHeight, HDC hdcSrc, int nX, int nY, COLORREF clrTran )
{
	HDC		hdcTmp  = ::CreateCompatibleDC( hDC );
	HBITMAP	bmpTmp  = ::CreateCompatibleBitmap( hDC, nWidth, nHeight );
	HGDIOBJ hbmpOld = ::SelectObject( hdcTmp, bmpTmp );

	HDC		hdcMsk  = ::CreateCompatibleDC( hDC );
	HBITMAP hbmpMsk = ::CreateBitmap( nWidth, nHeight, 1, 1, NULL );
	HGDIOBJ hMskOld = ::SelectObject( hdcMsk, hbmpMsk );

	COLORREF clrBkOld = ::SetBkColor( hdcSrc, clrTran );
	::BitBlt( hdcMsk, 0, 0, nWidth, nHeight, hdcSrc, nX, nY, SRCCOPY );
	::SetBkColor( hdcSrc, clrBkOld );

	::BitBlt( hdcTmp, 0, 0, nWidth, nHeight, hDC, nLeft, nTop, SRCCOPY );

	::BitBlt( hdcTmp, 0, 0, nWidth, nHeight, hdcSrc, nX, nY, SRCINVERT );
	::BitBlt( hdcTmp, 0, 0, nWidth, nHeight, hdcMsk, 0, 0, SRCAND );
	::BitBlt( hdcTmp, 0, 0, nWidth, nHeight, hdcSrc, nX, nY, SRCINVERT );

	::BitBlt( hDC, nLeft, nTop, nWidth, nHeight, hdcTmp, 0, 0, SRCCOPY );

	::SelectObject( hdcMsk, hMskOld );
	::DeleteObject( hbmpMsk );
	::DeleteDC( hdcMsk );

	::SelectObject( hdcTmp, hbmpOld );
	::DeleteObject( bmpTmp );
	::DeleteDC( hdcTmp );	
}

/*
void RXAPI DrawGradientRgn(HDC hDC, BOOL bVertical, HRGN hRgn, COLORREF clrStart, COLORREF clrEnd)
{
	float	fStep;
	RECT	rect;
	HBRUSH	hbrFill;
	HRGN	hRgnFill = ::CreateRectRgn( 0, 0, 0, 0 );
	HRGN	hRgnTemp = ::CreateRectRgn( 0, 0, 0, 0 );

	::GetRgnBox ( hRgn, &rect );
	::CombineRgn( hRgnTemp, hRgn, 0, RGN_COPY );
	::OffsetRgn ( hRgnTemp, -rect.left, -rect.top );

	int	nWidth  = rect.right  - rect.left;
	int	nHeight = rect.bottom - rect.top; 

	HRGN	hRgnRect = ::CreateRectRgn( 0, 0, nWidth, nHeight );

	int nR = GetRValue( clrStart );
	int nG = GetGValue( clrStart );
	int nB = GetBValue( clrStart );

	float fRStep = ( GetRValue( clrEnd ) - GetRValue( clrStart ) ) / (float)STEP;
	float fGStep = ( GetGValue( clrEnd ) - GetGValue( clrStart ) ) / (float)STEP;
	float fBStep = ( GetBValue( clrEnd ) - GetBValue( clrStart ) ) / (float)STEP;

	HDC		hdcMem   = ::CreateCompatibleDC( hDC );
	HBITMAP	hbmpMem     = ::CreateCompatibleBitmap( hDC, nWidth, nHeight );
	HGDIOBJ hobjOld = ::SelectObject( hdcMem, hbmpMem );

	hbrFill = ::CreateSolidBrush( 0x00FFFFFF );	// White
	::FillRgn( hdcMem, hRgnRect, hbrFill );
	::DeleteObject( hbrFill );

	if ( bVertical )
	{
		int nBottom, nTop = 0; 

		fStep = (float)nHeight / (float)STEP;

		for ( int nCount=0; nCount<=STEP; nCount++ )
		{
			nBottom = (int)( (nCount+1) * fStep );

			::SetRectRgn( hRgnRect, 0, nTop, nWidth, nBottom );
			::CombineRgn( hRgnFill, hRgnRect, hRgnTemp, RGN_AND );

			hbrFill = ::CreateSolidBrush( RGB( nR+fRStep*nCount, nG+fGStep*nCount, nB+fBStep*nCount ) );
			::FillRgn( hdcMem, hRgnFill, hbrFill );
			::DeleteObject( hbrFill );

			nTop = nBottom;
		}
	}
	else
	{
		int nRight, nLeft = 0;

		fStep = (float)nWidth / (float)STEP;

		for ( int nCount=0; nCount<=STEP; nCount++ )
		{
			nRight = (int)( (nCount+1) * fStep );

			::SetRectRgn( hRgnRect, nLeft, 0, nRight, nHeight );
			::CombineRgn( hRgnFill, hRgnRect, hRgnTemp, RGN_AND );

			hbrFill = ::CreateSolidBrush( RGB( nR+fRStep*nCount, nG+fGStep*nCount, nB+fBStep*nCount ) );
			::FillRgn( hdcMem, hRgnFill, hbrFill );
			::DeleteObject( hbrFill );

			nLeft = nRight;
		}
	}

	BitBltTran( hDC, rect.left, rect.top, nWidth, nHeight, hdcMem, 0, 0, 0x00FFFFFF );

	::DeleteObject( hRgnRect );
	::DeleteObject( hRgnTemp );
	::DeleteObject( hRgnFill );

	::SelectObject( hdcMem, hobjOld );
	::DeleteObject( hbmpMem );
	::DeleteDC( hdcMem );	
}
*/
void RXAPI DrawGradientRect(HDC hDC, BOOL bVertical, const RECT &rectFill, COLORREF clrStart, COLORREF clrEnd)
{
	float	fStep;
	RECT	rect;
	HBRUSH	hbrFill;

	int		nWidth  = rectFill.right  - rectFill.left;
	int		nHeight = rectFill.bottom - rectFill.top; 

	int nR = GetRValue( clrStart );
	int nG = GetGValue( clrStart );
	int nB = GetBValue( clrStart );

	float fRStep = ( GetRValue( clrEnd ) - GetRValue( clrStart ) ) / (float)STEP;
	float fGStep = ( GetGValue( clrEnd ) - GetGValue( clrStart ) ) / (float)STEP;
	float fBStep = ( GetBValue( clrEnd ) - GetBValue( clrStart ) ) / (float)STEP;

	HDC		hdcMem   = ::CreateCompatibleDC( hDC );
	HBITMAP	hbmpMem  = ::CreateCompatibleBitmap( hDC, nWidth, nHeight );
	HGDIOBJ hobjOld  = ::SelectObject( hdcMem, hbmpMem );

	rect.top = rect.left = 0;

	if ( bVertical )
	{
		rect.right	= nWidth;
		fStep		= (float)nHeight / (float)STEP;

		for ( int nCount=0; nCount<=STEP; nCount++ )
		{
			rect.bottom = (int)( (nCount+1) * fStep );
	
			hbrFill = ::CreateSolidBrush( RGB( nR+fRStep*nCount, nG+fGStep*nCount, nB+fBStep*nCount ) );
			::FillRect( hdcMem, &rect, hbrFill );
			::DeleteObject( hbrFill );

			rect.top = rect.bottom;
		}
	}
	else
	{
		rect.bottom = nHeight;
		fStep		= (float)nWidth / (float)STEP;

		for ( int nCount=0; nCount<=STEP; nCount++ )
		{
			rect.right	= (int)( (nCount+1) * fStep );

			hbrFill = ::CreateSolidBrush( RGB( nR+fRStep*nCount, nG+fGStep*nCount, nB+fBStep*nCount ) );
			::FillRect( hdcMem, &rect, hbrFill );
			::DeleteObject( hbrFill );

			rect.left = rect.right;
		}
	}
	
	::BitBlt( hDC, rectFill.left, rectFill.top, nWidth, nHeight, hdcMem, 0, 0, SRCCOPY );

	::SelectObject( hdcMem, hobjOld );
	::DeleteObject( hbmpMem );
	::DeleteDC( hdcMem );
}

void RXAPI DrawLightPointRect(HDC hDC, const RECT &rectFill, const POINT &pointLight, COLORREF clrBkgnd, COLORREF clrPoint, float fFouce)
{
	HBRUSH	hbrFill;

	int		nStep;

	int nX = pointLight.x - rectFill.left;
	int nY = pointLight.y - rectFill.top;

	int	nWidth  = rectFill.right  - rectFill.left;
	int	nHeight = rectFill.bottom - rectFill.top; 

	int nR = GetRValue( clrPoint );
	int nG = GetGValue( clrPoint );
	int nB = GetBValue( clrPoint );

	float fRStep = ( GetRValue( clrBkgnd ) - GetRValue( clrPoint ) ) / (float)STEP;
	float fGStep = ( GetGValue( clrBkgnd ) - GetGValue( clrPoint ) ) / (float)STEP;
	float fBStep = ( GetBValue( clrBkgnd ) - GetBValue( clrPoint ) ) / (float)STEP;

	HDC		hdcMem   = ::CreateCompatibleDC( hDC );
	HBITMAP	hbmpMem     = ::CreateCompatibleBitmap( hDC, nWidth, nHeight );
	HGDIOBJ hobjOld = ::SelectObject( hdcMem, hbmpMem );

	HRGN	rgnTemp;
	HRGN	rgnRect	 = ::CreateRectRgn( 0, 0, nWidth, nHeight );
	HRGN	rgnLight = ::CreateEllipticRgn( nX-1, nY-1, nX+1, nY+1 );

	hbrFill = ::CreateSolidBrush( clrBkgnd );
	::FillRgn( hdcMem, rgnRect, hbrFill );
	::DeleteObject( hbrFill );

	hbrFill = ::CreateSolidBrush( clrPoint );
	::FillRgn( hdcMem, rgnLight, hbrFill );
	::DeleteObject( hbrFill );

	if ( fFouce == 0.0f )
	{
		float fMax = (float)_hypot( max(abs(nX-rectFill.right),abs(nX-rectFill.left)), max(abs(nY-rectFill.bottom),abs(nY-rectFill.top)) );
		fFouce = ( fMax / (float)STEP );
	}

	for ( int nCount=0; nCount<STEP; nCount++ )
	{
		nStep = (int)( fFouce * ( nCount + 1 ) );

		rgnTemp  = ::CreateEllipticRgn( nX-nStep, nY-nStep, nX+nStep, nY+nStep );
		::CombineRgn( rgnLight, rgnTemp, rgnLight, RGN_DIFF );
//		::CombineRgn( rgnLight, rgnRect, rgnLight, RGN_AND );
		::DeleteObject( rgnTemp );

		hbrFill = ::CreateSolidBrush( RGB( nR+fRStep*nCount, nG+fGStep*nCount, nB+fBStep*nCount ) );
		::FillRgn( hdcMem, rgnLight, hbrFill );
		::DeleteObject( hbrFill );

		::DeleteObject( rgnLight );
		rgnLight = ::CreateEllipticRgn( nX-nStep, nY-nStep, nX+nStep, nY+nStep );
	}

	::BitBlt( hDC, rectFill.left, rectFill.top, nWidth, nHeight, hdcMem, 0, 0, SRCCOPY );

	::SelectObject( hdcMem, hobjOld );
	::DeleteObject( hbmpMem );
	::DeleteObject( rgnRect );
	::DeleteObject( rgnLight );
	::DeleteDC( hdcMem );
}

void RXAPI DrawRect(HDC hDC, COLORREF clrBorder, COLORREF clrBkgnd, int nLeft, int nTop, int nRight, int nBottom)
{
	HPEN	 hpen;
	HBRUSH	 hbr;
	HGDIOBJ	 hpenOld, hbrOld;

	hpen = ::CreatePen( PS_SOLID, 1, clrBorder );
	hbr  = ::CreateSolidBrush( clrBkgnd );

	hpenOld = ::SelectObject( hDC, hpen );
	hbrOld  = ::SelectObject( hDC, hbr );
	// Draw rectangle item	
	::Rectangle( hDC, nLeft, nTop, nRight, nBottom );

	::SelectObject( hDC, hpenOld );
	::SelectObject( hDC, hbrOld );

	::DeleteObject( hpen );
	::DeleteObject( hbr );
}

void RXAPI DrawArrow(HDC hDC, COLORREF clrArrow, int nX, int nY, int nSize, int nDirection)
{
	POINT	pt[2];
	HPEN	hpen;

	pt[0].x = nX;
	pt[0].y = nY;
	pt[1].y = pt[0].y;
	pt[1].x = nX+nSize;
	hpen = ::CreatePen( PS_SOLID, 1, clrArrow );
	HGDIOBJ hpenOld = ::SelectObject( hDC, hpen );

	switch( nDirection )
	{
		case DIR_RIGHT:
			for ( int i=0; i<nSize; i++ )
			{
				Polyline( hDC, pt, 2 );

				pt[0].x--;
				pt[0].y--;
				pt[1].x--;
				pt[1].y++;
			}
			break;
		case DIR_BOTTOM:
			{
				for ( int i=0; i<nSize; i++ )
				{
					Polyline( hDC, pt, 2 );
		
					pt[0].x++;
					pt[0].y++;
					pt[1].x--;
					pt[1].y++;
					if (pt[0].x >pt[1].x)
					{
						break;
					}
				}
				break;
			}
		case DIR_TOP:
			{
				for ( int i=0; i<nSize; i++ )
				{
					Polyline( hDC, pt, 2 );

					pt[0].x++;
					pt[0].y--;
					pt[1].x--;
					pt[1].y--;
					if (pt[0].x >pt[1].x)
					{
						break;
					}
				}
				break;
			}
	}

	::SelectObject( hDC, hpenOld );
	::DeleteObject( hpen );
}

void RXAPI DrawRoundRect(HDC hDC, COLORREF clrBorder, COLORREF clrBkgnd, const RECT &rect, int nWidth, int nHeight)
{
	DrawRoundRect( hDC, clrBorder, clrBkgnd, rect.left, rect.top, rect.right, rect.bottom, nWidth, nHeight );
}

void RXAPI DrawRoundRect(HDC hDC, COLORREF clrBorder, COLORREF clrBkgnd, int nLeft, int nTop, int nRight, int nBottom, int nWidth, int nHeight)
{
	HPEN	 hpen;
	HBRUSH	 hbr;
	HGDIOBJ	 hpenOld, hbrOld;

	hpen = ::CreatePen( PS_SOLID, 1, clrBorder );
	hbr  = ::CreateSolidBrush( clrBkgnd );

	hpenOld = ::SelectObject( hDC, hpen );
	hbrOld  = ::SelectObject( hDC, hbr );
	// Draw rectangle item	
	::RoundRect( hDC, nLeft, nTop, nRight, nBottom, nWidth, nHeight );

	::SelectObject( hDC, hpenOld );
	::SelectObject( hDC, hbrOld );

	::DeleteObject( hpen );
	::DeleteObject( hbr );
}

void RXAPI DrawPartionRgn(HDC hDC, COLORREF clrLeftTop, COLORREF clrRightBottom, BOOL bVerticalLine, int nOffset, HRGN hRgn)
{
	RECT rect;

	::GetRgnBox( hRgn, &rect );

	HRGN hRgnFill = ::CreateRectRgn( 0, 0, 0, 0 );
	HRGN hRgnTemp = ::CreateRectRgn( rect.left, rect.top, rect.right, rect.bottom );

	if ( bVerticalLine )
		 ::OffsetRgn( hRgnTemp, nOffset, 0 );
	else ::OffsetRgn( hRgnTemp, 0, nOffset );

	::CombineRgn( hRgnFill, hRgn, hRgnTemp, RGN_DIFF );
	::DrawRgn( hDC, clrLeftTop, hRgnFill );

	::CombineRgn( hRgnFill, hRgn, hRgnTemp, RGN_AND );
	::DrawRgn( hDC, clrRightBottom, hRgnFill );

	::DeleteObject( hRgnFill );
	::DeleteObject( hRgnTemp );
}

void RXAPI DrawRgn(HDC hDC, COLORREF clrBorder, COLORREF clrBkgnd, HRGN hRgn)
{
	HBRUSH hbrBk = ::CreateSolidBrush( clrBkgnd );
	HBRUSH hbrBo = ::CreateSolidBrush( clrBorder );

	::FillRgn ( hDC, hRgn, hbrBk );
	::FrameRgn( hDC, hRgn, hbrBo, 1, 1 );

	::DeleteObject( hbrBk );	
	::DeleteObject( hbrBo );	
}

void RXAPI DrawRgn(HDC hDC, COLORREF clrBkgnd, HRGN hRgn)
{
	HBRUSH hbr = ::CreateSolidBrush( clrBkgnd );
	::FillRgn( hDC, hRgn, hbr );
	::DeleteObject( hbr );	
}

void RXAPI DrawFrameRgn( HDC hDC, HRGN hRgn, COLORREF clrEdge, int nWidth, int nHeight )
{
	HBRUSH hbr = ::CreateSolidBrush( clrEdge );
	::FrameRgn( hDC, hRgn, hbr, nWidth, nHeight );
	::DeleteObject( hbr );	
}

void RXAPI DrawEllipse(HDC hDC, COLORREF clrBorder, COLORREF clrBkgnd, int nLeft, int nTop, int nRight, int nBottom)
{
	HPEN	 hpen;
	HBRUSH	 hbr;
	HGDIOBJ	 hpenOld, hbrOld;

	hpen = ::CreatePen( PS_SOLID, 1, clrBorder );
	hbr  = ::CreateSolidBrush( clrBkgnd );

	hbrOld  = ::SelectObject( hDC, hbr );
	hpenOld = ::SelectObject( hDC, hpen );

	::Ellipse( hDC, nLeft, nTop, nLeft + 6, nTop + 6 );

	::SelectObject( hDC, hbrOld );
	::SelectObject( hDC, hpenOld );

	::DeleteObject( hpen );
	::DeleteObject( hbr );
}

void RXAPI DrawPolyline(HDC hDC, COLORREF clrLine, POINT *pt, int nSize)
{
	HPEN hpen = ::CreatePen( PS_SOLID, 1, clrLine );
	HGDIOBJ	hpenOld = ::SelectObject( hDC, hpen );

	::Polyline( hDC, pt, nSize );

	::SelectObject( hDC, hpenOld );
	::DeleteObject( hpen );
}

void RXAPI DrawTranBmp(HDC hDC, int nX, int nY, HBITMAP hBmp, int nWidth, int nHeight, COLORREF clrTran)
{
	HDC			hdcMem, hdcMsk;
	HBITMAP		hbmpMsk;
	HGDIOBJ		hbmpOld, hbmpMskOld;

	hdcMem = ::CreateCompatibleDC( hDC );
	hdcMsk = ::CreateCompatibleDC( hDC );

	hbmpOld = ::SelectObject( hdcMem, hBmp );

	hbmpMsk = ::CreateBitmap( nWidth, nHeight, 1, 1, NULL );
	hbmpMskOld = ::SelectObject( hdcMsk, hbmpMsk );

	COLORREF clrBkOld = ::SetBkColor( hdcMem, clrTran );
	::BitBlt( hdcMsk, 0, 0, nWidth, nHeight, hdcMem, 0, 0, SRCCOPY );
	::SetBkColor( hdcMem, clrBkOld );

	::BitBlt( hDC, nX, nY, nWidth, nHeight, hdcMem, 0, 0, SRCINVERT );
	::BitBlt( hDC, nX, nY, nWidth, nHeight, hdcMsk, 0, 0, SRCAND );
	::BitBlt( hDC, nX, nY, nWidth, nHeight, hdcMem, 0, 0, SRCINVERT );

	::SelectObject( hdcMsk, hbmpMskOld );
	::SelectObject( hdcMem, hbmpOld );
	::DeleteObject( hbmpMsk );
	::DeleteDC( hdcMem );
	::DeleteDC( hdcMsk );
}

void RXAPI DrawRect(HDC hDC, COLORREF clrFill, int nLeft, int nTop, int nRight, int nBottom)
{
	RECT	rect;

	rect.left	= nLeft;
	rect.top	= nTop;
	rect.right  = nRight;
	rect.bottom = nBottom;

	HBRUSH	br	= ::CreateSolidBrush( clrFill );
	::FillRect( hDC, &rect, br );
	::DeleteObject( br );
}

void RXAPI CreateRoundRectRgn( HRGN hRgn, const RECT &rect, int nWidth, int nHeight, BOOL bLeftTopR, BOOL bLeftBottomR, BOOL bRightTopR, BOOL bRightBottomR)
{
	int nXCenter = (rect.left+rect.right)/2;
	int nYCenter = (rect.top+rect.bottom)/2;

	HRGN hRect  = ::CreateRectRgn( rect.left, rect.top, nXCenter, nYCenter );
	HRGN hRound = ::CreateRoundRectRgn( rect.left, rect.top, rect.right+1, rect.bottom+1, nWidth, nHeight );

	if ( !bLeftTopR )
	{
		::CombineRgn( hRound, hRound, hRect, RGN_OR );
	}

	if ( !bLeftBottomR )
	{
		::SetRectRgn( hRect, rect.left, nYCenter, nXCenter, rect.bottom );
		::CombineRgn( hRound, hRound, hRect, RGN_OR );
	}

	if ( !bRightTopR )
	{
		::SetRectRgn( hRect, nXCenter, rect.top, rect.right, nYCenter );
		::CombineRgn( hRound, hRound, hRect, RGN_OR );
	}

	if ( !bRightBottomR )
	{
		::SetRectRgn( hRect, nXCenter, nYCenter, rect.right, rect.bottom );
		::CombineRgn( hRound, hRound, hRect, RGN_OR );
	}

	::CombineRgn( hRgn, hRound, 0, RGN_COPY );

	::DeleteObject( hRect );
	::DeleteObject( hRound );
}

void RXAPI DrawFadeBmp( HDC hDC, int nX, int nY, HBITMAP hBmp, COLORREF clrTarget, UINT nRate )
{
	BITMAPINFO bmpinfo;
	float fRate = nRate / (float)255;

	::ZeroMemory( &bmpinfo, sizeof( BITMAPINFO ) );
	bmpinfo.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
	::GetDIBits( hDC, hBmp, 0, 0, NULL, &bmpinfo, DIB_RGB_COLORS );
	
	int nWidth   = bmpinfo.bmiHeader.biWidth;
	int nHeight  = bmpinfo.bmiHeader.biHeight;
	int nBmpSize = ( nWidth * bmpinfo.bmiHeader.biBitCount + 31 ) / 32 * 8 *nHeight;

	BYTE *pData, *pBuffer;
	pData = pBuffer = new BYTE[ nBmpSize ];
	::ZeroMemory( pBuffer, nBmpSize );

	bmpinfo.bmiHeader.biBitCount = 24;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage = 0;

	::GetDIBits( hDC, hBmp, 0, nHeight, pBuffer, &bmpinfo, DIB_RGB_COLORS );

	int nR = GetRValue( clrTarget );
	int nG = GetGValue( clrTarget );
	int nB = GetBValue( clrTarget );

	for ( int nBmpY=0; nBmpY<nHeight; nBmpY++ )
		for ( int nBmpX=0; nBmpX<nWidth; nBmpX++ )
		{
//			pData = pBuffer + nBmpY*nWidth*3 + nBmpX*3;
			
			*pData = (BYTE)(  nB + (((*pData) - nB)/(float)STEP) * nRate ); 	pData++;
			*pData = (BYTE)(  nG + (((*pData) - nG)/(float)STEP) * nRate ); 	pData++;
			*pData = (BYTE)(  nR + (((*pData) - nR)/(float)STEP) * nRate );		pData++;
		}

	HBITMAP hBmpTmp = ::CreateDIBitmap( hDC, &bmpinfo.bmiHeader, CBM_INIT, pBuffer, &bmpinfo, DIB_RGB_COLORS ); 
	HDC		hdcMem  = ::CreateCompatibleDC( hDC );
	HBITMAP hbmpOld = (HBITMAP)::SelectObject( hdcMem, hBmpTmp );
	::BitBlt( hDC, nX, nY, nWidth, nHeight, hdcMem, 0, 0, SRCCOPY );
	::SelectObject( hdcMem, hbmpOld );
	::DeleteObject( hBmpTmp );
	::DeleteDC( hdcMem );

	delete[] pBuffer;
}

void RXAPI DrawFadeIcon(HDC hDC, int nX, int nY, HICON hIcon, int nWidth, int nHeight, UINT nRate)
{
	HDC		hdcMem  = ::CreateCompatibleDC( hDC );
	HBITMAP	hbmpMem = ::CreateCompatibleBitmap( hDC, nWidth, nHeight );
	HGDIOBJ	hBmpOld = ::SelectObject( hdcMem, hbmpMem );

	::DrawRect( hdcMem, 0, 0, 0, nWidth, nHeight );
	::DrawIconEx( hdcMem, 0, 0, hIcon, nWidth, nHeight, 0, NULL, DI_NORMAL );
	::DrawFadeBmp( hdcMem, 0, 0, hbmpMem, 0, nRate );
//	::DrawFadeBmp( hDC, nX, nY, hbmpMem, 0, nRate );
//	::BitBlt( hDC, nX, nY, nWidth, nHeight, hdcMem, 0, 0, SRCCOPY );

	::DrawTranBmp( hDC, nX, nY, hbmpMem, nWidth, nHeight, 0 );

	::SelectObject( hdcMem, hBmpOld );
	::DeleteObject( hbmpMem );
	::DeleteDC( hdcMem );
}

void RXAPI DrawShadowIcon(HDC hDC, int nX, int nY, HICON hIcon, int nWidth, int nHeight, COLORREF clrShadow)
{
	HDC		hdcMem  = ::CreateCompatibleDC( hDC );
	HDC		hdcMsk  = ::CreateCompatibleDC( hDC );
	HBITMAP	hbmpMsk = ::CreateCompatibleBitmap( hDC, nWidth, nHeight );
	HBITMAP	hbmpMem = ::CreateCompatibleBitmap( hDC, nWidth, nHeight );
	HGDIOBJ	hBmpOld = ::SelectObject( hdcMem, hbmpMem );
	HGDIOBJ	hMskOld = ::SelectObject( hdcMsk, hbmpMsk );

	::DrawIconEx( hdcMsk, 0, 0, hIcon, nWidth, nHeight, 0, NULL, DI_MASK );
	::DrawRect( hdcMem, clrShadow, 0, 0, nWidth, nHeight );
	::BitBlt( hdcMem, 0, 0, nWidth, nHeight, hdcMsk, 0, 0, SRCPAINT );

	::BitBlt( hDC, nX, nY, nWidth, nHeight, hdcMem, 0, 0, SRCINVERT );
	::BitBlt( hDC, nX, nY, nWidth, nHeight, hdcMsk, 0, 0, SRCAND );
	::BitBlt( hDC, nX, nY, nWidth, nHeight, hdcMem, 0, 0, SRCINVERT );

	::SelectObject( hdcMsk, hMskOld );
	::SelectObject( hdcMem, hBmpOld );
	::DeleteObject( hbmpMem );
	::DeleteObject( hbmpMsk );
	::DeleteDC( hdcMem );
	::DeleteDC( hdcMsk );
}

void RXAPI AlphaBlend( HDC hdcResult, int nX, int nY, int nWidth, int nHeight, HDC hdcDest, int nXDest, int nYDest, HDC hdcSrc, int nXSrc, int nYSrc, BYTE bAlpha )
{
	BITMAPINFOHEADER bih;

	bih.biSize			= sizeof( BITMAPINFOHEADER );
	bih.biWidth			= nWidth;
	bih.biHeight		= nHeight;
	bih.biPlanes		= 1;
	bih.biBitCount		= 32;
	bih.biCompression	= BI_RGB;
	bih.biSizeImage		= 0;
	bih.biXPelsPerMeter	= 0;
	bih.biYPelsPerMeter	= 0;
	bih.biClrUsed		= 0;
	bih.biClrImportant	= 0;

	DWORD	*pdwSrc, *pdwDest;
	HBITMAP	hbmpSrc, hbmpDest;

	hbmpSrc  = ::CreateDIBSection( hdcSrc, (BITMAPINFO*)&bih, DIB_RGB_COLORS, (void**)&pdwSrc, 0, (DWORD)0 );
	hbmpDest = ::CreateDIBSection( hdcDest,(BITMAPINFO*)&bih, DIB_RGB_COLORS, (void**)&pdwDest,0, (DWORD)0 );

	HDC		hdcMem  = ::CreateCompatibleDC( NULL );
	HGDIOBJ hobjOld = ::SelectObject( hdcMem, hbmpDest );
	::BitBlt( hdcMem, 0, 0, nWidth, nHeight, hdcDest, nXDest, nYDest, SRCCOPY );
	::SelectObject( hdcMem, hbmpSrc );
	::BitBlt( hdcMem, 0, 0, nWidth, nHeight, hdcSrc,  nXSrc,  nYSrc,  SRCCOPY );

	for( int i=0; i<nHeight; ++i )
	{
		LPBYTE pbRGBSrc  = (LPBYTE)&pdwSrc [i*nWidth];
		LPBYTE pbRGBDest = (LPBYTE)&pdwDest[i*nWidth];

		for ( int j=0; j<nWidth; ++j )
		{
			pbRGBSrc[0] = (BYTE)(( pbRGBDest[0]*(255-bAlpha)+pbRGBSrc[0]*bAlpha )>>8);
			pbRGBSrc[1] = (BYTE)(( pbRGBDest[1]*(255-bAlpha)+pbRGBSrc[1]*bAlpha )>>8);
			pbRGBSrc[2] = (BYTE)(( pbRGBDest[2]*(255-bAlpha)+pbRGBSrc[2]*bAlpha )>>8);

			pbRGBSrc  += 4;
			pbRGBDest += 4;
		}
	}

	::BitBlt( hdcResult, nX, nY, nWidth, nHeight, hdcMem, 0, 0, SRCCOPY );
	::SelectObject( hdcMem, hobjOld );
	::DeleteDC( hdcMem );
	::DeleteObject( hbmpSrc );
	::DeleteObject( hbmpDest );
}

void RXAPI DrawAnimation( RECT *prect, HDC hDCDest, HDC hDCSrc, int nType, int nStep, int nTimeDelay )
{
	const int nLeft   = prect->left;
	const int nTop    = prect->top;
	const int nRight  = prect->right;
	const int nBottom = prect->bottom;
	const int nWidth  = nRight  - nLeft;
	const int nHeight = nBottom - nTop;

	if ( nStep )//&& nWidth>1 && nHeight>1 )
	switch ( nType)
	{
		case ANIMATE_NONE:
			break;

		case ANIMATE_SLIDE_TTB:
		{
			while( nHeight/nStep == 0 )
				--nStep;

			int nStepHeight = nHeight / nStep;
			for ( int ntmpHeight=nStepHeight; ntmpHeight<nHeight; ntmpHeight+=nStepHeight )
			{
				::BitBlt( hDCDest, nLeft, nTop, nWidth, ntmpHeight, hDCSrc, 0, nHeight-ntmpHeight, SRCCOPY );
				Sleep( nTimeDelay / nStep );
			}
		}	break;
	
		case ANIMATE_SLIDE_LTR:
		{
			while( nWidth/nStep == 0 )
				--nStep;

			int nStepWidth = nWidth / nStep;
			for ( int ntmpWidth=nStepWidth; ntmpWidth<nWidth; ntmpWidth+=nStepWidth )
			{
				::BitBlt( hDCDest, nLeft, nTop, ntmpWidth, nHeight, hDCSrc, nWidth-ntmpWidth, 0, SRCCOPY );
				Sleep( nTimeDelay / nStep );
			}
		}	break;

		case ANIMATE_UNFOLD_MTTB:
		{
			while( nHeight/nStep == 0 )
				--nStep;

			int nMiddleCY	= nHeight / 2;
			int nStepHeight = nHeight / nStep;
			int ntmpHeight  = nStepHeight;
			for( ; ntmpHeight<nHeight; ntmpHeight+=nStepHeight )
			{
				::BitBlt( hDCDest, nLeft, nTop+nMiddleCY-ntmpHeight/2, nWidth, ntmpHeight, hDCSrc, 0, nMiddleCY-ntmpHeight/2, SRCCOPY );
				Sleep( nTimeDelay / nStep );
			}
		}	break;
	
		case ANIMATE_UNFOLD_MTLR:
		{
			while( nWidth/nStep == 0 )
				--nStep;

			int nMiddleCX  = nWidth / 2;
			int nStepWidth = nWidth / nStep;
			int ntmpWidth  = nStepWidth;
			for( ; ntmpWidth<nWidth; ntmpWidth+=nStepWidth )
			{
				::BitBlt( hDCDest, nLeft+nMiddleCX-ntmpWidth/2, nTop, ntmpWidth, nHeight, hDCSrc, nMiddleCX-ntmpWidth/2, 0, SRCCOPY );
				Sleep( nTimeDelay / nStep );
			}
		}	break;

		case ANIMATE_SHUTTER_TTB:
		{
			while( nHeight/nStep/nStep == 0 )
				--nStep;

			int nStepHeight   = nHeight / nStep / nStep;
			int nTotalShutter = nHeight / nStep;
			int nShutterHeight= nHeight / nStep;
			int ntmpHeight	  = nStepHeight;

			for( ; ntmpHeight<nShutterHeight; ntmpHeight+=nStepHeight )
			{
				for( int i=0; i<nTotalShutter; i++ )
					::BitBlt( hDCDest, nLeft, nTop+nShutterHeight*i, nWidth, ntmpHeight, hDCSrc, 0, nShutterHeight*i, SRCCOPY );
				Sleep( nTimeDelay / nStep );
			}
		}	break;

		case ANIMATE_SHUTTER_LTR:
		{
			while( nWidth/nStep/nStep == 0 )
				--nStep;

			int nStepWidth    = nWidth / nStep / nStep;
			int nTotalShutter = nWidth / nStep;
			int nShutterWidth = nWidth / nStep;
			int ntmpWidth	  = nStepWidth;

			for( ; ntmpWidth<nShutterWidth; ntmpWidth+=nStepWidth )
			{
				for( int i=0; i<nTotalShutter; i++ )
					::BitBlt( hDCDest, nLeft+nShutterWidth*i, nTop, ntmpWidth, nHeight, hDCSrc, nShutterWidth*i, 0, SRCCOPY );
				Sleep( nTimeDelay / nStep );
			}
		}	break;
	
		case ANIMATE_UNFOLD_LTTRB:
		{
			for ( int i=0; i<nStep; ++i)
			{
				int ntmpWidth   = nTop  + nWidth  * (i + 1) / nStep;
				int ntmpHeight  = nLeft + nHeight * (i + 1) / nStep;
				::BitBlt( hDCDest, nLeft, nTop, ntmpWidth, ntmpHeight, hDCSrc, 0, 0, SRCCOPY);
				Sleep( nTimeDelay / nStep );
			}
		}	break;
			
		case ANIMATE_UNFOLDSLIDE_LTTRB:
		{
			while( nWidth/nStep == 0 || nHeight/nStep == 0 )
				--nStep;

			int nStepWidth  = nWidth / nStep;
			int nStepHeight = nHeight / nStep;
			int ntmpWidth   = nStepWidth;
			int ntmpHeight  = nStepHeight;

			for ( ; ntmpHeight<nHeight; ntmpWidth+=nStepWidth, ntmpHeight+=nStepHeight )
			{
				::BitBlt( hDCDest, nLeft, nTop, ntmpWidth, ntmpHeight, hDCSrc, 0, nHeight-ntmpHeight, SRCCOPY );
				Sleep( nTimeDelay / nStep );
			}
		}	break;

		case ANIMATE_STRETCH_TTB:
		{
			while( nHeight/nStep == 0 )
				--nStep;

			int nStepHeight = nHeight / nStep;
			for ( int ntmpHeight = nStepHeight; ntmpHeight<nHeight; ntmpHeight+=nStepHeight )
			{
				::StretchBlt( hDCDest, nLeft, nTop, nWidth, ntmpHeight, hDCSrc, 0, 0, nWidth, nHeight, SRCCOPY );
				Sleep( nTimeDelay / nStep );
			}
		}	break;	

		case ANIMATE_STRETCH_LTR:
		{
			while( nWidth/nStep == 0 )
				--nStep;

			int nStepWidth = nWidth / nStep;
			for ( int ntmpWidth = nStepWidth; ntmpWidth<nWidth; ntmpWidth+=nStepWidth )
			{
				::StretchBlt( hDCDest, nLeft, nTop, ntmpWidth, nHeight, hDCSrc, 0, 0, nWidth, nHeight, SRCCOPY );
				Sleep( nTimeDelay / nStep );
			}
		}	break;	

		case ANIMATE_NOISE:
		{
			int nCount = nWidth * nHeight / 2;
			for (int i = 0; i < nCount; ++i)
			{
				int nX = nLeft + rand() % nWidth;
				int nY = nTop  + rand() % nHeight;
				::SetPixel( hDCDest, nX, nY, ::GetPixel( hDCSrc, nX, nY ) );
//				Sleep( nTimeDelay / nStep );
			}
		}	break;

		case ANIMATE_FADE:
		{
			HDC hdcBkgnd = ::CreateCompatibleDC( hDCDest );
			HBITMAP	hbmpBkgnd = ::CreateCompatibleBitmap( hDCDest, nWidth, nHeight );
			HGDIOBJ hobjOldB  = ::SelectObject( hdcBkgnd, hbmpBkgnd );
			::BitBlt( hdcBkgnd, 0, 0, nWidth, nHeight, hDCDest, nLeft, nTop, SRCCOPY );

			HDC hdcFade = ::CreateCompatibleDC( hDCDest );
			HBITMAP	hbmpFade = ::CreateCompatibleBitmap( hDCDest, nWidth, nHeight );
			HGDIOBJ hobjOldF = ::SelectObject( hdcFade, hbmpFade );

			for ( int i=0; i<nStep; ++i )
			{
				AlphaBlend( hdcFade, 0, 0, nWidth, nHeight, hdcBkgnd, 0, 0, hDCSrc, 0, 0, (255*i)/nStep );
				::BitBlt( hDCDest, nLeft, nTop, nWidth, nHeight, hdcFade, 0, 0, SRCCOPY );
				Sleep( nTimeDelay / nStep / 3 );
			}

			::SelectObject( hdcBkgnd, hobjOldB );
			::DeleteObject( hbmpBkgnd );
			::DeleteDC( hdcBkgnd );

			::SelectObject( hdcFade, hobjOldF );
			::DeleteObject( hbmpFade );
			::DeleteDC( hdcFade );
		}	break;

		case ANIMATE_RAND:
		{
			UINT uAnimateType[13] = { ANIMATE_SLIDE_TTB,	ANIMATE_SLIDE_LTR,
									  ANIMATE_UNFOLD_MTTB,	ANIMATE_UNFOLD_MTLR,		
									  ANIMATE_SHUTTER_TTB,	ANIMATE_SHUTTER_LTR,
									  ANIMATE_STRETCH_TTB,	ANIMATE_STRETCH_LTR,
									  ANIMATE_UNFOLD_LTTRB,	ANIMATE_UNFOLDSLIDE_LTTRB, 
									  ANIMATE_NOISE,			
									  ANIMATE_FADE,
									  ANIMATE_NONE };

			DrawAnimation( prect, hDCDest, hDCSrc, uAnimateType[rand()%13], nStep, nTimeDelay );
		}	return;
	}

	::BitBlt( hDCDest, nLeft, nTop, nWidth, nHeight, hDCSrc, 0, 0, SRCCOPY );
}

void RXAPI DrawLine(HDC hDC, const RECT& rc, int nSize, DWORD dwPenColor, int nStyle /*= PS_SOLID*/)
{
	LOGPEN lg;
	lg.lopnColor = RGB(GetBValue(dwPenColor), GetGValue(dwPenColor), GetRValue(dwPenColor));
	lg.lopnStyle = nStyle;
	lg.lopnWidth.x = nSize;
	HPEN hPen = CreatePenIndirect(&lg);
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
	POINT ptTemp = { 0 };
	::MoveToEx(hDC, rc.left, rc.top, &ptTemp);
	::LineTo(hDC, rc.right, rc.bottom);
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen);
}

void RXAPI DrawLine(HDC hDC, const POINT& ptStart, const POINT& ptEnd, int nSize, DWORD dwPenColor, int nStyle)
{
	LOGPEN lg;
	lg.lopnColor = RGB(GetRValue(dwPenColor), GetGValue(dwPenColor), GetBValue(dwPenColor));
	lg.lopnStyle = nStyle;
	lg.lopnWidth.x = nSize;
	HPEN hPen = CreatePenIndirect(&lg);
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
	POINT ptTemp = { 0 };
	::MoveToEx(hDC, ptStart.x, ptStart.y, &ptTemp);
	::LineTo(hDC, ptEnd.x, ptEnd.y);
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen);
}

void RXAPI GdiplusDrawText(HDC hDC, HFONT hFont, int32_t nHint, RECT& rc, LPCTSTR pstrText, DWORD dwTextColor, UINT uStyle)
{
	ASSERT(::GetObjectType(hDC) == OBJ_DC || ::GetObjectType(hDC) == OBJ_MEMDC);
	if (pstrText == NULL) return;

	HFONT hOldFont = (HFONT)::SelectObject(hDC, hFont);
	Gdiplus::Graphics graphics(hDC);
	Gdiplus::Font font(hDC, hFont);
	Gdiplus::TextRenderingHint trh = Gdiplus::TextRenderingHintSystemDefault;
	switch (nHint)
	{
	case 0: {trh = Gdiplus::TextRenderingHintSystemDefault; break; }
	case 1: {trh = Gdiplus::TextRenderingHintSingleBitPerPixelGridFit; break; }
	case 2: {trh = Gdiplus::TextRenderingHintSingleBitPerPixel; break; }
	case 3: {trh = Gdiplus::TextRenderingHintAntiAliasGridFit; break; }
	case 4: {trh = Gdiplus::TextRenderingHintAntiAlias; break; }
	case 5: {trh = Gdiplus::TextRenderingHintClearTypeGridFit; break; }
	}
	graphics.SetTextRenderingHint(trh);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);

	Gdiplus::RectF rectF((Gdiplus::REAL)rc.left, (Gdiplus::REAL)rc.top, (Gdiplus::REAL)(rc.right - rc.left), (Gdiplus::REAL)(rc.bottom - rc.top));
	Gdiplus::SolidBrush brush(Gdiplus::Color(254, GetBValue(dwTextColor), GetGValue(dwTextColor), GetRValue(dwTextColor)));

	Gdiplus::StringFormat stringFormat = Gdiplus::StringFormat::GenericTypographic();

	if ((uStyle & DT_END_ELLIPSIS) != 0) {
		stringFormat.SetTrimming(Gdiplus::StringTrimmingEllipsisCharacter);
	}

	int formatFlags = 0;
	if ((uStyle & DT_NOCLIP) != 0) {
		formatFlags |= Gdiplus::StringFormatFlagsNoClip;
	}
	if ((uStyle & DT_SINGLELINE) != 0) {
		formatFlags |= Gdiplus::StringFormatFlagsNoWrap;
	}
	if ((uStyle & DT_NOPREFIX) != 0) {
		stringFormat.SetHotkeyPrefix(Gdiplus::HotkeyPrefixHide);
	}
	else {
		stringFormat.SetHotkeyPrefix(Gdiplus::HotkeyPrefixShow);
	}

	stringFormat.SetFormatFlags(formatFlags);

	if ((uStyle & DT_LEFT) != 0) {
		stringFormat.SetAlignment(Gdiplus::StringAlignmentNear);
	}
	else if ((uStyle & DT_CENTER) != 0) {
		stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
	}
	else if ((uStyle & DT_RIGHT) != 0) {
		stringFormat.SetAlignment(Gdiplus::StringAlignmentFar);
	}
	else {
		stringFormat.SetAlignment(Gdiplus::StringAlignmentNear);
	}
	stringFormat.GenericTypographic();
	if ((uStyle & DT_TOP) != 0) {
		stringFormat.SetLineAlignment(Gdiplus::StringAlignmentNear);
	}
	else if ((uStyle & DT_VCENTER) != 0) {
		stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
	}
	else if ((uStyle & DT_BOTTOM) != 0) {
		stringFormat.SetLineAlignment(Gdiplus::StringAlignmentFar);
	}
	else {
		stringFormat.SetLineAlignment(Gdiplus::StringAlignmentNear);
	}
#ifdef UNICODE
	if ((uStyle & DT_CALCRECT) != 0)
	{
		Gdiplus::RectF bounds;

		graphics.MeasureString(pstrText, -1, &font, rectF, &stringFormat, &bounds);

		// MeasureString存在计算误差，这里加一像素
		rc.bottom = rc.top + (long)bounds.Height + 1;
		rc.right = rc.left + (long)bounds.Width + 1;
	}
	else
	{
		graphics.DrawString(pstrText, -1, &font, rectF, &stringFormat, &brush);
	}
#else
	DWORD dwSize = MultiByteToWideChar(CP_ACP, 0, pstrText, -1, NULL, 0);
	WCHAR* pcwszDest = new WCHAR[dwSize + 1];
	memset(pcwszDest, 0, (dwSize + 1) * sizeof(WCHAR));
	MultiByteToWideChar(CP_ACP, NULL, pstrText, -1, pcwszDest, dwSize);
	if (pcwszDest != NULL)
	{
		if ((uStyle & DT_CALCRECT) != 0)
		{
			Gdiplus::RectF bounds;
			graphics.MeasureString(pcwszDest, -1, &font, rectF, &stringFormat, &bounds);
			rc.bottom = rc.top + (long)(bounds.Height * 1.06);
			rc.right = rc.left + (long)(bounds.Width * 1.06);
		}
		else
		{
			graphics.DrawString(pcwszDest, -1, &font, rectF, &stringFormat, &brush);
		}
		delete[]pcwszDest;
	}
#endif
	::SelectObject(hDC, hOldFont);
}

void RXAPI GdiplusDrawText(HDC hDC, HFONT hFont, int32_t nHint, int32_t nLeft,int32_t nTop, LPCTSTR pstrText, DWORD dwTextColor, UINT uStyle)
{
	ASSERT(::GetObjectType(hDC) == OBJ_DC || ::GetObjectType(hDC) == OBJ_MEMDC);
	if (pstrText == NULL) return;

	HFONT hOldFont = (HFONT)::SelectObject(hDC, hFont);
	Gdiplus::Graphics graphics(hDC);
	Gdiplus::Font font(hDC, hFont);
	Gdiplus::TextRenderingHint trh = Gdiplus::TextRenderingHintSystemDefault;
	switch (nHint)
	{
	case 0: {trh = Gdiplus::TextRenderingHintSystemDefault; break; }
	case 1: {trh = Gdiplus::TextRenderingHintSingleBitPerPixelGridFit; break; }
	case 2: {trh = Gdiplus::TextRenderingHintSingleBitPerPixel; break; }
	case 3: {trh = Gdiplus::TextRenderingHintAntiAliasGridFit; break; }
	case 4: {trh = Gdiplus::TextRenderingHintAntiAlias; break; }
	case 5: {trh = Gdiplus::TextRenderingHintClearTypeGridFit; break; }
	}
	graphics.SetTextRenderingHint(trh);
	graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);

	Gdiplus::SolidBrush brush(Gdiplus::Color(254, GetBValue(dwTextColor), GetGValue(dwTextColor), GetRValue(dwTextColor)));

	Gdiplus::StringFormat stringFormat = Gdiplus::StringFormat::GenericTypographic();

	if ((uStyle & DT_END_ELLIPSIS) != 0) {
		stringFormat.SetTrimming(Gdiplus::StringTrimmingEllipsisCharacter);
	}

	int formatFlags = 0;
	if ((uStyle & DT_NOCLIP) != 0) {
		formatFlags |= Gdiplus::StringFormatFlagsNoClip;
	}
	if ((uStyle & DT_SINGLELINE) != 0) {
		formatFlags |= Gdiplus::StringFormatFlagsNoWrap;
	}
	if ((uStyle & DT_NOPREFIX) != 0) {
		stringFormat.SetHotkeyPrefix(Gdiplus::HotkeyPrefixHide);
	}
	else {
		stringFormat.SetHotkeyPrefix(Gdiplus::HotkeyPrefixShow);
	}

	stringFormat.SetFormatFlags(formatFlags);

	if ((uStyle & DT_LEFT) != 0) {
		stringFormat.SetAlignment(Gdiplus::StringAlignmentNear);
	}
	else if ((uStyle & DT_CENTER) != 0) {
		stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
	}
	else if ((uStyle & DT_RIGHT) != 0) {
		stringFormat.SetAlignment(Gdiplus::StringAlignmentFar);
	}
	else {
		stringFormat.SetAlignment(Gdiplus::StringAlignmentNear);
	}
	stringFormat.GenericTypographic();
	if ((uStyle & DT_TOP) != 0) {
		stringFormat.SetLineAlignment(Gdiplus::StringAlignmentNear);
	}
	else if ((uStyle & DT_VCENTER) != 0) {
		stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
	}
	else if ((uStyle & DT_BOTTOM) != 0) {
		stringFormat.SetLineAlignment(Gdiplus::StringAlignmentFar);
	}
	else {
		stringFormat.SetLineAlignment(Gdiplus::StringAlignmentNear);
	}
#ifdef UNICODE
	if ((uStyle & DT_CALCRECT) != 0)
	{
		Gdiplus::RectF bounds;
	}
	else
	{
		Gdiplus::PointF origin;
		origin.X = nLeft;
		origin.Y = nTop;
		graphics.DrawString(pstrText, -1, &font, origin, &stringFormat, &brush);
	}
#else
	DWORD dwSize = MultiByteToWideChar(CP_ACP, 0, pstrText, -1, NULL, 0);
	WCHAR* pcwszDest = new WCHAR[dwSize + 1];
	memset(pcwszDest, 0, (dwSize + 1) * sizeof(WCHAR));
	MultiByteToWideChar(CP_ACP, NULL, pstrText, -1, pcwszDest, dwSize);
	if (pcwszDest != NULL)
	{
		if ((uStyle & DT_CALCRECT) != 0)
		{
			Gdiplus::RectF bounds;
			graphics.MeasureString(pcwszDest, -1, &font, rectF, &stringFormat, &bounds);
			rc.bottom = rc.top + (long)(bounds.Height * 1.06);
			rc.right = rc.left + (long)(bounds.Width * 1.06);
		}
		else
		{
			Gdiplus::PointF origin;
			origin.X = nLeft;
			origin.Y = nTop;
			graphics.DrawString(pcwszDest, -1, &font, rectF, &stringFormat, &brush);
		}
		delete[]pcwszDest;
	}
#endif
	::SelectObject(hDC, hOldFont);
}