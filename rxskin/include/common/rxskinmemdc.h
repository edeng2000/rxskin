#pragma once
#include "RXSkin.h"
#include "core/SkSurface.h"
class RXSKIN_API CRXSkinMemDC
{
public:
	CRXSkinMemDC(HDC hDC,RECT rcClient);
	~CRXSkinMemDC(void);
	HDC          GetHDC();
	VOID         TakeSnapshot();
	HBITMAP		 CreateHBitmap(HDC hDC, int32_t nWidth,
		int32_t nHeight,
		bool flipHeight,
		LPVOID* pBits) const;
	SkSurface* GetSurface() { return m_poSurface; }
private:
	HDC          m_hDC;
	HDC          m_hDestDC;
	HBITMAP      m_hBitmap;
	HGDIOBJ      m_hOldBitmap;
	RECT         m_rcClient;
	SkSurface* m_poSurface;
};
