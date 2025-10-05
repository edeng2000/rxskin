#include "pch.h"
#include "controls\rxskingif.h"

#include "common\RXSkinService.h"
#include "common\graphics.h"
#include "common\RXSkinMemDC.h"
#include "common\rxskinutil.h"
#include "rxlogger.h"
#include "rxtime.h"
#include <wingdi.h>

CRXSkinGif::CRXSkinGif(void)
{
	m_dwFrameCount = 0;
	m_dwCurFrame = 0;
	m_dwHeight = 0;
	m_dwWidth = 0;
}

CRXSkinGif::~CRXSkinGif(void)
{
	for (uint32_t t=0;t<(uint32_t)m_vecBitmap.size();t++)
	{
		DeleteObject(m_vecBitmap[t]);
	}
	m_vecBitmap.clear();

}

VOID CRXSkinGif::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	SetRect(rcItem);
	if (IsVisible()==FALSE)
	{
		return;
	}
	if (m_vecBitmap.size()>m_dwCurFrame)
	{
		HBITMAP bmp = m_vecBitmap[m_dwCurFrame];
		HDC hmemdc = CreateCompatibleDC(hDC);
		::SelectObject(hmemdc, bmp);

		BLENDFUNCTION bf = { AC_SRC_OVER,0,255,AC_SRC_ALPHA };
		::AlphaBlend(hDC, rcItem.left, rcItem.top,
			m_dwWidth, m_dwHeight,
				hmemdc, 0, 0, m_dwWidth, m_dwHeight, bf);

		DeleteDC(hmemdc);
	}
}

VOID CRXSkinGif::ReleaseControl()
{
	delete this;
}
BOOL CRXSkinGif::PtInRect(POINT pt)
{
	if (IsVisible()&& IsEnabled())
	{
		return GetRect().PtInRect(pt);
	}
	return FALSE;
}

BOOL CRXSkinGif::OnClick(POINT pt)
{
	return FALSE;
}

BOOL CRXSkinGif::OnMouseMove(POINT pt)
{
	return FALSE;
}

BOOL CRXSkinGif::OnLButtonDown(POINT pt)
{
	return FALSE;
}

BOOL CRXSkinGif::OnTimer(WPARAM wParam, LPARAM lParam)
{
	if (SHOW_GIF_TIMER_ID == wParam)
	{
		if (m_dwFrameCount<=0)
		{
			return FALSE;
		}
		if (m_dwCurFrame >= m_dwFrameCount - 1)
		{
			m_dwCurFrame = 0;
		}
		else
			m_dwCurFrame++;
		Invalidate();
		return TRUE;
	}
	return FALSE;
}

void CRXSkinGif::SetRect(const CRXSkinRect rcItem)
{
	CRXControl::SetRect(rcItem);
}

VOID CRXSkinGif::SetImageData(const BYTE* pszImageData, const uint32_t& dwLen)
{
	EasyGifReader* poGif = EasyGifReader::openMemory(pszImageData,dwLen);
	if (poGif == NULL)
	{
		return;
	}

	BITMAPINFO bmi;
	::ZeroMemory(&bmi, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = poGif->width();
	bmi.bmiHeader.biHeight = -poGif->height();
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = poGif->width() * poGif->height() * 4;


	memset(&m_stHeader, 0, sizeof(BITMAPINFOHEADER));
	m_stHeader.biBitCount = 32;
	m_stHeader.biCompression = BI_RGB;
	m_stHeader.biHeight = poGif->height();
	m_stHeader.biPlanes = 1;
	m_stHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_stHeader.biWidth = poGif->width();
	m_dwCurFrame = 0;
	m_dwFrameCount = poGif->frameCount();
	m_vecBitmap.resize(poGif->frameCount(), NULL);
	EasyGifReader::FrameIterator itrCur = poGif->begin();
	LPBYTE pDest = NULL;
	uint32_t dwPos = 0;
	uint32_t dwTime = 0;
	m_dwHeight = poGif->height();
	m_dwWidth = poGif->width();
	for (; itrCur != poGif->end(); itrCur++)
	{
		dwTime = itrCur->duration().milliseconds();
		HBITMAP hBitmap = ::CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pDest, NULL, 0);
		if (!hBitmap) {
			break;
		}
		uint32_t m_nY = poGif->height();
		uint32_t m_nX = poGif->width();
		const EasyGifReader::PixelComponent* framePixels = itrCur->pixels();
		for (int y = 0; y < m_nY; y++)
		{
			for (int x = 0; x < m_nX; x++)
			{
				pDest[m_nX * 4 * y + 4 * x + 3] = framePixels[m_nX * 4 * y + 4 * x + 3];
				if (pDest[m_nX * 4 * y + 4 * x + 3] < 255)
				{
					pDest[m_nX * 4 * y + 4 * x] = (BYTE)(DWORD(framePixels[m_nX * 4 * y + 4 * x + 2]) * framePixels[m_nX * 4 * y + 4 * x + 3] / 255);
					pDest[m_nX * 4 * y + 4 * x + 1] = (BYTE)(DWORD(framePixels[m_nX * 4 * y + 4 * x + 1]) * framePixels[m_nX * 4 * y + 4 * x + 3] / 255);
					pDest[m_nX * 4 * y + 4 * x + 2] = (BYTE)(DWORD(framePixels[m_nX * 4 * y + 4 * x]) * framePixels[m_nX * 4 * y + 4 * x + 3] / 255);
				}
				else
				{
					pDest[m_nX * 4 * y + 4 * x] = framePixels[m_nX * 4 * y + 4 * x + 2];
					pDest[m_nX * 4 * y + 4 * x + 1] = framePixels[m_nX * 4 * y + 4 * x + 1];
					pDest[m_nX * 4 * y + 4 * x + 2] = framePixels[m_nX * 4 * y + 4 * x];
				}
			}
		}
		m_vecBitmap[dwPos] = hBitmap;
		dwPos++;
	}

	delete poGif;
	CRXSkinWnd* poParent = CRXSkinService::Instance()->FindWnd(GetParentWnd());
	if (poParent)
	{
		poParent->SetTimer(this, SHOW_GIF_TIMER_ID, dwTime);
	}
}

VOID CRXSkinGif::SetImage(const TCHAR* pszImageFile)
{	
	std::vector<unsigned char> vec;
	CRXSkinResourceMgr::Instance()->LoadResource(pszImageFile, vec);
	if (vec.size()<=0)
	{
		return;
	}
	EasyGifReader* poGif = EasyGifReader::openMemory(&vec[0],vec.size());
	if (poGif == NULL)
	{
		vec.clear();
		return;
	}
	vec.clear();
	BITMAPINFO bmi;
	::ZeroMemory(&bmi, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = poGif->width();
	bmi.bmiHeader.biHeight = -poGif->height();
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = poGif->width() * poGif->height() * 4;

	m_strImageFile = pszImageFile;
	memset(&m_stHeader, 0, sizeof(BITMAPINFOHEADER));
	m_stHeader.biBitCount = 32;
	m_stHeader.biCompression = BI_RGB;
	m_stHeader.biHeight = poGif->height();
	m_stHeader.biPlanes = 1;
	m_stHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_stHeader.biWidth = poGif->width();
	m_dwCurFrame = 0;
	m_dwFrameCount = poGif->frameCount();
	m_vecBitmap.resize(poGif->frameCount(), NULL);
	EasyGifReader::FrameIterator itrCur = poGif->begin();
	LPBYTE pDest = NULL;
	uint32_t dwPos = 0;
	uint32_t dwTime = 0;
	m_dwHeight = poGif->height();
	m_dwWidth = poGif->width();
	for (; itrCur!= poGif->end(); itrCur++)
	{
		dwTime = itrCur->duration().milliseconds();
		HBITMAP hBitmap = ::CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pDest, NULL, 0);
		if (!hBitmap) {
			break;
		}
		uint32_t m_nY = poGif->height();
		uint32_t m_nX = poGif->width();
		const EasyGifReader::PixelComponent* framePixels = itrCur->pixels();
		for (int y = 0; y < m_nY; y++)
		{
			for (int x = 0; x < m_nX; x++)
			{
				pDest[m_nX * 4 * y + 4 * x + 3] = framePixels[m_nX * 4 * y + 4 * x + 3];
				if (pDest[m_nX * 4 * y + 4 * x + 3] < 255)
				{
					pDest[m_nX * 4 * y + 4 * x] = (BYTE)(DWORD(framePixels[m_nX * 4 * y + 4 * x + 2]) * framePixels[m_nX * 4 * y + 4 * x + 3] / 255);
					pDest[m_nX * 4 * y + 4 * x + 1] = (BYTE)(DWORD(framePixels[m_nX * 4 * y + 4 * x + 1]) * framePixels[m_nX * 4 * y + 4 * x + 3] / 255);
					pDest[m_nX * 4 * y + 4 * x + 2] = (BYTE)(DWORD(framePixels[m_nX * 4 * y + 4 * x]) * framePixels[m_nX * 4 * y + 4 * x + 3] / 255);
				}
				else
				{
					pDest[m_nX * 4 * y + 4 * x] = framePixels[m_nX * 4 * y + 4 * x + 2];
					pDest[m_nX * 4 * y + 4 * x + 1] = framePixels[m_nX * 4 * y + 4 * x + 1];
					pDest[m_nX * 4 * y + 4 * x + 2] = framePixels[m_nX * 4 * y + 4 * x];
				}
			}
		}
		m_vecBitmap[dwPos] = hBitmap;
		dwPos++;
	}

	delete poGif;
	CRXSkinWnd* poParent = CRXSkinService::Instance()->FindWnd(GetParentWnd());
	if (poParent)
	{
		poParent->SetTimer(this, SHOW_GIF_TIMER_ID, dwTime);
	}
}

VOID CRXSkinGif::SetVisible(const BOOL bIsVisible)
{
	CRXControl::SetVisible(bIsVisible);
}

