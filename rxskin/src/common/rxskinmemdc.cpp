#include "pch.h"
#include "common\RXSkinMemDC.h"

CRXSkinMemDC::CRXSkinMemDC(HDC hDC,RECT rcClient)
{
	m_poSurface = NULL;
	m_rcClient = rcClient;
	m_hDestDC = hDC;
	m_hDC = ::CreateCompatibleDC (hDC);
	SetMapMode(m_hDC,GetMapMode(hDC));
	LPVOID pixels = nullptr;
	int32_t nWidth = m_rcClient.right - m_rcClient.left;
	int32_t nHeight = m_rcClient.bottom - m_rcClient.top;

	m_hBitmap = CreateHBitmap(m_hDestDC,
										 m_rcClient.right - m_rcClient.left,
										 m_rcClient.bottom - m_rcClient.top,true,
		&pixels);
	m_hOldBitmap = ::SelectObject (m_hDC, m_hBitmap);
	SkImageInfo info = SkImageInfo::Make(nWidth, nHeight,
		SkColorType::kN32_SkColorType,
		SkAlphaType::kPremul_SkAlphaType);
	m_poSurface = SkSurface::NewRasterDirect(info, pixels, sizeof(uint32_t) * nWidth);
}

CRXSkinMemDC::~CRXSkinMemDC(void)
{
	if (!m_hDC)
		return;

	if (m_hDestDC)
	{
		::DeleteDC(m_hDC);
		m_hDC = NULL;
	}
	if (m_hBitmap)
	{
		::DeleteObject(m_hBitmap);
		m_hBitmap = NULL;
	}
	if (m_poSurface)
	{
		delete m_poSurface;
	}
	::SelectObject (m_hDC, m_hOldBitmap);
}

HDC CRXSkinMemDC::GetHDC()
{
	return m_hDC;
}

HBITMAP CRXSkinMemDC::CreateHBitmap(HDC hDC, int32_t nWidth,
	int32_t nHeight,
	bool flipHeight,
	LPVOID* pBits) const
{
	ASSERT((nWidth > 0) && (nHeight > 0));
	if (nWidth <= 0 || nHeight <= 0) {
		return nullptr;
	}

	BITMAPINFO bmi = { 0 };
	::ZeroMemory(&bmi, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = nWidth;
	if (flipHeight) {
		bmi.bmiHeader.biHeight = -nHeight;//负数表示位图方向：从上到下，左上角为圆点
	}
	else {
		bmi.bmiHeader.biHeight = nHeight; //正数表示位图方向：从下到上，左下角为圆点
	}
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = nWidth * nHeight * sizeof(DWORD);

	HBITMAP hBitmap = nullptr;


	if (hDC != nullptr) {
		hBitmap = ::CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS, pBits, NULL, 0);
	}
	return hBitmap;
}

void CRXSkinMemDC::TakeSnapshot()
{
	::BitBlt (m_hDestDC,
		m_rcClient.left,
		m_rcClient.top,
		m_rcClient.right - m_rcClient.left,
		m_rcClient.bottom - m_rcClient.top,
		m_hDC, 
		0,
		0, 
		SRCCOPY);
}