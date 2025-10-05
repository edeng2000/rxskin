#include "pch.h"
#include "common/rximagemodel.h"
#include "fundation/rxstring.h"
#include "fundation/rxfile.h"
#include "main/rxskinapp.h"
#include "common/rxskinresourcemgr.h"
#if defined(USE_STB)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <wingdi.h>
#endif
namespace RX
{
    namespace Image
    {
		typedef int (*GetIconIndexFunc)(LPCWSTR);
		int GetIconIndex(LPCWSTR ext)
		{
			int iconindex = -1;
			tstring strPath = RXGetModulePath();
			strPath += _T("mpciconlib.dll");
			static HMODULE mpciconlib = LoadLibraryW(strPath.c_str());
			if (mpciconlib) {
				static GetIconIndexFunc pGetIconIndexFunc = (GetIconIndexFunc)GetProcAddress(mpciconlib, "get_icon_index");
				if (pGetIconIndexFunc) {
					iconindex = pGetIconIndexFunc(ext);
				}
			}

			return iconindex;
		}



        CRXImage::CRXImage()
        {
            memset(m_szImageFile, 0, sizeof(TCHAR) * 256);
#ifdef USE_FREEIMAGE

#endif
#ifdef USE_STB
			m_hBitmap = NULL;			
			m_nX = 0;
			m_nY = 0;
			m_dwMask = 0;
#endif			

#if defined(USE_CXIMAGE)			
#endif
#ifdef USE_GDI_PLUS
			m_pGdiplusImage = NULL;;
#endif
			m_hIcon = NULL;
        }

        CRXImage::~CRXImage(void)
        {
#ifdef USE_FREEIMAGE
#endif
#ifdef USE_STB
			if (m_hBitmap) {
				::DeleteObject(m_hBitmap);
			}
			m_hBitmap = NULL;

#endif
#if defined(USE_CXIMAGE)

#endif
#ifdef USE_GDI_PLUS
			if (m_pGdiplusImage)
				delete m_pGdiplusImage;
			m_pGdiplusImage = NULL;;
#endif
			if (m_hIcon)
			{
				::DestroyIcon(m_hIcon);
				m_hIcon = NULL;
			}
        }
		void CRXImage::DrawImage(HDC hDC, const RECT& rcDest, HBITMAP hBitmap, const RECT rcSrc, BYTE byAlpha)
		{
// 			HBITMAP bmp = hBitmap;
// 			HDC hmemdc = CreateCompatibleDC(hDC);
// 			::SelectObject(hmemdc, bmp);
// 
// 			BLENDFUNCTION bf = { AC_SRC_OVER,0,byAlpha,AC_SRC_ALPHA };
// 			::AlphaBlend(hDC, rcDest.left, rcDest.top,
// 				rcDest.right - rcDest.left, rcDest.bottom - rcDest.top,
// 					hmemdc, rcSrc.left, rcSrc.top, rcSrc.right - rcSrc.left, rcSrc.bottom - rcSrc.top, bf);
// 
// 			DeleteDC(hmemdc);
		}
		VOID CRXImage::CreateFromICON(HICON hIcon)
		{
#if defined(USE_CXIMAGE)
			m_oxImage.CreateFromHICON(hIcon);
#endif
		}

        INT32 CRXImage::Draw(HDC hDC,
            const RECT& oRect,
            INT32 desx,
            INT32 desy,
            RECT* poClipRect /* = NULL */,
            bool bSmooth /* = FALSE */,
            bool  bFlipY)
        {
#ifdef USE_FREEIMAGE
			m_oImage.draw(hDC, desx, desy);
			return 0;
#endif
#if defined(USE_CXIMAGE)
			return m_oxImage.Draw(hDC, oRect, desx, desy, poClipRect, bSmooth, bFlipY);
#endif

#if defined(USE_STB)
			RECT rcBmpPart = { 0 };
			rcBmpPart.right = m_nX;
			rcBmpPart.bottom = m_nY;
			DrawImage(hDC, oRect, m_hBitmap, rcBmpPart);
#endif
#ifdef USE_GDI_PLUS
			RECT rSrc;
			rSrc.left = rSrc.top = 0;
			rSrc.right = GetWidth();
			rSrc.bottom = GetHeight();
			GDIPlusDrawImage(hDC, rSrc, oRect);
#endif
			return 0;
        }

            INT32 CRXImage::DrawTrans(HDC hDC,
                RGBQUAD rgbTrans,
                const RECT& oRect,
                INT32 desx,
                INT32 desy,
                RECT* poClipRect /* = NULL */,
                bool bSmooth /* = FALSE */,
                bool  bFlipY)
            {
 #ifdef USE_FREEIMAGE
				//m_oImage.drawEx();
				return 0;
#endif
#if defined(USE_CXIMAGE)
				m_oxImage.SetTransColor(rgbTrans);
				return m_oxImage.Draw(hDC, oRect, desx, desy, poClipRect, bSmooth, bFlipY);
#endif
				return 0;
            }
			void CRXImage::Scale(const float& fRate)
			{
		
			}
			void CRXImage::Rotation(const float& fRotation)
			{
#ifdef USE_FREEIMAGE
			
#endif
#if defined(USE_CXIMAGE)   
				m_oxImage.Rotate(fRotation);
#endif
				
			}
            INT32 CRXImage::DrawGray(HDC hDC,
                const RECT& oRect,
                INT32 desx,
                INT32 desy,
                RECT* poClipRect,
                bool  bSmooth,
                bool  bFlipY)
            {
       
#ifdef USE_FREEIMAGE
				return 0;
#endif
#if defined(USE_CXIMAGE)   
				CxImage tmp;
				tmp.Copy(m_oxImage);
				tmp.GrayScale();
				return tmp.Draw(hDC, oRect, desx, desy, poClipRect, bSmooth, bFlipY);
#endif
				return 0;
            }

            INT32 CRXImage::Draw(HDC hDC,
                INT32 nX /* = 0 */,
                INT32 nY /* = 0 */,
                INT32 nWidth /* = -1 */,
                INT32 nHeight /* = -1 */,
                INT32 desx,
                INT32 desy,
                RECT* poClipRect /* = 0 */,
                bool bSmooth /* = FALSE */,
                bool  bFlipY)
            {
#ifdef USE_FREEIMAGE
				m_oImage.draw(hDC, nX, nY, nWidth, nHeight);
				return 0;
#endif
#if defined(USE_CXIMAGE) 
				return m_oxImage.Draw(hDC, nX, nY, nWidth, nHeight, desx, desy, poClipRect, bSmooth, bFlipY);
#endif
#ifdef USE_STB
				RECT rc;
				rc.left = nX;
				rc.right = nX +m_nX;
				rc.top = nY;
				rc.bottom = rc.top + m_nY;
				return Draw(hDC, rc, desx, desy);
#endif
#ifdef USE_GDI_PLUS
				RECT rSrc;
				rSrc.left = rSrc.top = 0;
				rSrc.right = GetWidth();
				rSrc.bottom = GetHeight();
				RECT rDest;
				rDest.left = nX;
				rDest.top = nY;
				rDest.right = rDest.left + nWidth;
				rDest.bottom = rDest.top + nHeight;
				GDIPlusDrawImage(hDC, rSrc, rDest);
#endif
				return 0;
            }
            INT32 CRXImage::DrawTrans(HDC hDC,
                RGBQUAD rgbTrans,
                INT32 nX /* = 0 */,
                INT32 nY /* = 0 */,
                INT32 nWidth /* = -1 */,
                INT32 nHeight /* = -1 */,
                INT32 desx,
                INT32 desy,
                RECT* poClipRect /* = 0 */,
                bool bSmooth /* = FALSE */,
                bool  bFlipY)
            {
#ifdef USE_FREEIMAGE
				return 0;
#endif
#if defined(USE_CXIMAGE) 
				m_oxImage.SetTransColor(rgbTrans);
				return m_oxImage.Draw(hDC, nX, nY, nWidth, nHeight, desx, desy, poClipRect, bSmooth, bFlipY);
#endif
#ifdef USE_STB

#endif
				return 0;
            }

            INT32  CRXImage::DrawGray(HDC hDC,
                INT32 nX /* = 0 */,
                INT32 nY /* = 0 */,
                INT32 nWidth /* = -1 */,
                INT32 nHeight /* = -1 */,
                INT32 desx,
                INT32 desy,
                RECT* poClipRect /* = 0 */,
                bool bSmooth /* = FALSE */,
                bool  bFlipY)
            {
#ifdef USE_FREEIMAGE
				return 0;
#endif
#if defined(USE_CXIMAGE)   
				CxImage tmp;
				tmp.Copy(m_oxImage);
				tmp.GrayScale();
				return m_oxImage.Draw(hDC, nX, nY, nWidth, nHeight, desx, desy, poClipRect, bSmooth, bFlipY);
#endif
#ifdef USE_STB

#endif
				return 0;
            }

            INT32 CRXImage::Stretch(HDC hDC,
                const RECT& oRect,
                UINT32 dwRop /* = SRCCOPY */)
            {            
#ifdef USE_FREEIMAGE
				RECT rc = oRect;
				m_oImage.draw(hDC, &rc);
				return 0;
#endif
#if defined(USE_CXIMAGE)   
				return m_oxImage.Stretch(hDC,
					oRect.left,
					oRect.top,
					oRect.right - oRect.left,
					oRect.bottom - oRect.top,
					NULL, FALSE,
					FALSE);
#endif
#ifdef USE_STB
				RECT rc;
				rc.left = rc.top = 0;
				rc.right = m_nX;
				rc.bottom = m_nY;
				return Draw(hDC, oRect, oRect.left, oRect.top);
#endif
#ifdef USE_GDI_PLUS
				RECT rSrc;
				rSrc.left = rSrc.top = 0;
				rSrc.right = GetWidth();
				rSrc.bottom = GetHeight();
				GDIPlusDrawImage(hDC, rSrc, oRect);
#endif
				return 0;
            }

            INT32 CRXImage::StretchGray(HDC hDC, const RECT& oRect, UINT32 dwRop /* = SRCCOPY */)
            {
               
#ifdef USE_FREEIMAGE
				return 0;
#endif
#if defined(USE_CXIMAGE)   
				CxImage tmp;
				tmp.Copy(m_oxImage);
				tmp.GrayScale();
			//	tmp.AlphaPaletteEnable(false);
				return tmp.Stretch(hDC,
					oRect.left,
					oRect.top,
					oRect.right - oRect.left,
					oRect.bottom - oRect.top,
					NULL, FALSE,
					FALSE);
#endif
				return 0;
            }

            INT32 CRXImage::Stretch(HDC hDC, INT32 nXOffset, INT32 nYOffset, INT32 nXSize, INT32 nYSsize, UINT32 dwRop /* = SRCCOPY */)
            {
          
#ifdef USE_FREEIMAGE
				return 0;
#endif
#if defined(USE_CXIMAGE)   
				return m_oxImage.Stretch(hDC,
					nXOffset,
					nYOffset,
					nXSize,
					nYSsize,
					NULL, FALSE,
					FALSE);
#endif
#ifdef USE_STB
				RECT rc;
				rc.left = nXOffset;
				rc.right = nXOffset+nXSize;
				rc.top = nYOffset;
				rc.bottom = nYOffset+nYSsize;
				return Draw(hDC, rc, 0, 0);
#endif
#ifdef USE_GDI_PLUS
				RECT rSrc;
				rSrc.left = rSrc.top = 0;
				rSrc.right = GetWidth();
				rSrc.bottom = GetHeight();
				RECT rDest;
				rDest.left = nXOffset;
				rDest.top = nYOffset;
				rDest.right = rDest.left + nXSize;
				rDest.bottom = rDest.top + nYSsize;
				GDIPlusDrawImage(hDC, rSrc, rDest);
#endif
				return 0;
            }

            INT32 CRXImage::StretchGray(HDC hDC, INT32 nXOffset, INT32 nYOffset, INT32 nXSize, INT32 nYSsize, UINT32 dwRop /* = SRCCOPY */)
            {
               
#ifdef USE_FREEIMAGE
				return 0;
#endif
#if defined(USE_CXIMAGE)   
				CxImage tmp;
				tmp.Copy(m_oxImage);
				bool bResult = tmp.GrayScale();
				tmp.AlphaPaletteEnable(false);
				return tmp.Stretch(hDC,
					nXOffset,
					nYOffset,
					nXSize,
					nYSsize,
					NULL, FALSE,
					FALSE);
#endif
				return 0;
            }

            VOID CRXImage::Release()
            {
				delete this;
            }

            BOOL CRXImage::OpenFile(const TCHAR* pszFilename)
            {
#ifdef USE_FREEIMAGE				
				return m_oImage.load(pszFilename);
#endif
#if defined(USE_CXIMAGE)   
				tstring strExt = RXFileExtractExt(pszFilename);
				uint32_t dwType = CxImage::GetTypeIdFromName(strExt.c_str());
				if (CRXSkinService::Instance()->IsFromFileResource())
				{
					BOOL bResult = m_oxImage.LoadW(pszFilename, dwType);
					if (FALSE == bResult)
					{
						TCHAR* pszRsource = MAKEINTRESOURCE(_ttoi(pszFilename));
						if (pszRsource)
						{
							HRSRC src = FindResource(NULL, pszRsource, _T("PNG"));
							if (src)
							{
								bResult = m_oxImage.LoadResource(src, dwType);
								return bResult;
							}
						}
						
					}
					return bResult;
				}
				if (CRXSkinService::Instance()->IsFromZipResource())
				{
					std::vector<unsigned char> vec;
					CRXSkinResourceMgr::Instance()->LoadResource(pszFilename, vec);
					if (vec.size() > 0)
					{
						BOOL bResult = m_oxImage.LoadMemory((const char*)&vec[0],vec.size(), dwType);
						vec.clear();
						if (FALSE == bResult)
						{
							TCHAR* pszRsource = MAKEINTRESOURCE(_ttoi(pszFilename));
							if (pszRsource)
							{
								HRSRC src = FindResource(NULL, pszRsource, _T("PNG"));
								if (src)
								{
									bResult = m_oxImage.LoadResource(src, dwType);
									return bResult;
								}
							}
						}
						return bResult;
					}
					vec.clear();
					TCHAR* pszRsource = MAKEINTRESOURCE(_ttoi(pszFilename));
					if (pszRsource)
					{
						HRSRC src = FindResource(NULL, pszRsource, _T("PNG"));
						if (src)
						{
							BOOL bResult = m_oxImage.LoadResource(src, dwType);
							return bResult;
						}
					}
				}
				BOOL bResult = m_oxImage.LoadW(pszFilename, dwType);
				if (FALSE == bResult)
				{
					TCHAR* pszRsource = MAKEINTRESOURCE(_ttoi(pszFilename));
					if (pszRsource)
					{
						HRSRC src = FindResource(NULL, pszRsource, _T("PNG"));
						if (src)
						{
							bResult = m_oxImage.LoadResource(src, dwType);
							return bResult;
						}
					}

				}
				return bResult;
				return FALSE;
#endif
#if defined(USE_STB)
// 				uint64_t llSize = RXFileSize(pszFilename);
// 				if (llSize<=0)
// 				{
// 					return FALSE;
// 				}
// 				FILE* hFile = ::_tfopen(pszFilename, _T("rb"));
// 				if (hFile==NULL)
// 				{
// 					return FALSE;
// 				}
// 				fseek(hFile, 0, SEEK_SET);
// 				BYTE* pData = (BYTE*)malloc(llSize);
// 				fread(pData, 1, llSize, hFile);
// 				fclose(hFile);
				
				LPBYTE pImage = NULL;
				int n;
				pImage = stbi_load(_RXTT2A(pszFilename).c_str(), &m_nX, &m_nY, &n, STBI_rgb_alpha);
			//	free(pData);
				if (!pImage) {
					return NULL;
				}

				BITMAPINFO bmi;
				::ZeroMemory(&bmi, sizeof(BITMAPINFO));
				bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
				bmi.bmiHeader.biWidth = m_nX;
				bmi.bmiHeader.biHeight = -m_nY;
				bmi.bmiHeader.biPlanes = 1;
				bmi.bmiHeader.biBitCount = 32;
				bmi.bmiHeader.biCompression = BI_RGB;
				bmi.bmiHeader.biSizeImage = m_nX * m_nY * 4;

				bool bAlphaChannel = false;
				LPBYTE pDest = NULL;
				m_hBitmap = ::CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pDest, NULL, 0);
				if (!m_hBitmap) {
					return FALSE;
				}
	
				for (int y = 0; y < m_nY; y++)
				{
					for (int x=0;x<m_nX;x++)
					{
						pDest[m_nX * 4 * y + 4 * x + 3] = pImage[m_nX * 4 * y + 4 * x + 3];
						if (pDest[m_nX * 4 * y + 4 * x + 3] < 255)
						{
							pDest[m_nX * 4 * y + 4 * x] = (BYTE)(DWORD(pImage[m_nX * 4 * y + 4 * x + 2]) * pImage[m_nX * 4 * y + 4 * x + 3] / 255);
							pDest[m_nX * 4 * y + 4 * x + 1] = (BYTE)(DWORD(pImage[m_nX * 4 * y + 4 * x + 1]) * pImage[m_nX * 4 * y + 4 * x + 3] / 255);
							pDest[m_nX * 4 * y + 4 * x + 2] = (BYTE)(DWORD(pImage[m_nX * 4 * y + 4 * x]) * pImage[m_nX * 4 * y + 4 * x + 3] / 255);
							bAlphaChannel = true;
						}
						else
						{
							pDest[m_nX * 4 * y + 4 * x] = pImage[m_nX * 4 * y + 4 * x + 2];
							pDest[m_nX * 4 * y + 4 * x + 1] = pImage[m_nX * 4 * y + 4 * x + 1];
							pDest[m_nX * 4 * y + 4 * x + 2] = pImage[m_nX * 4 * y + 4 * x];
						}
					}
				}

				stbi_image_free(pImage);
#endif
#ifdef USE_GDI_PLUS
				// GdiPlus
				HANDLE hFile = ::CreateFile(pszFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, \
					FILE_ATTRIBUTE_NORMAL, NULL);
				if (hFile == INVALID_HANDLE_VALUE) return FALSE;
				uint32_t dwSize = ::GetFileSize(hFile, NULL);
				if (dwSize == 0) return FALSE;
				BYTE *pData = new BYTE[dwSize];
				DWORD dwRead = 0;
				::ReadFile(hFile, pData, dwSize, &dwRead, NULL);
				::CloseHandle(hFile);

				if (dwRead != dwSize) {
					delete[] pData;
					pData = NULL;
					return FALSE;
				}
				HGLOBAL hMem = ::GlobalAlloc(GMEM_FIXED, dwSize);
				BYTE* pMem = (BYTE*)::GlobalLock(hMem);
				memcpy(pMem, pData, dwSize);
				IStream* pStm = NULL;
				::CreateStreamOnHGlobal(hMem, TRUE, &pStm);

				m_pGdiplusImage = Gdiplus::Image::FromStream(pStm);
				if (!m_pGdiplusImage || m_pGdiplusImage->GetLastStatus() != Gdiplus::Ok)
				{
					m_pGdiplusImage = NULL;

					pStm->Release();
					::GlobalUnlock(hMem);
				}

				delete[] pData;
				pData = NULL;

				if (m_pGdiplusImage == NULL) {
					return FALSE;
				}

#endif
				return TRUE;
            }

			BOOL   CRXImage::LoadResource(HRSRC hSrc, const int32_t nType)
			{
#ifdef USE_FREEIMAGE
				return 0;
#endif
#if defined(USE_CXIMAGE)  
				return  m_oxImage.LoadResource(hSrc, nType);
#endif
#if defined(USE_STB)
				ULONG nResSize = ::SizeofResource(NULL, hr); // Data size/length
				HGLOBAL hG = ::LoadResource(NULL, hr);
				if (NULL == hG || nResSize <= 0)
				{
					int ierr = GetLastError();
					return false;
				}
				LPBYTE pData = (LPBYTE)LockResource(hG); // Data Ptr
				if (NULL == pData)
				{
					int ierr = GetLastError();
					return false;
				}
				LPBYTE pImage = NULL;
				int n;
				pImage = stbi_load_from_memory((stbi_uc const*)pData, nResSize, &m_nX, &m_nY, &n, STBI_rgb_alpha);
				//	free(pData);
				if (!pImage) {
					return NULL;
				}

				BITMAPINFO bmi;
				::ZeroMemory(&bmi, sizeof(BITMAPINFO));
				bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
				bmi.bmiHeader.biWidth = m_nX;
				bmi.bmiHeader.biHeight = -m_nY;
				bmi.bmiHeader.biPlanes = 1;
				bmi.bmiHeader.biBitCount = 32;
				bmi.bmiHeader.biCompression = BI_RGB;
				bmi.bmiHeader.biSizeImage = m_nX * m_nY * 4;

				bool bAlphaChannel = false;
				LPBYTE pDest = NULL;
				m_hBitmap = ::CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pDest, NULL, 0);
				if (!m_hBitmap) {
					return FALSE;
				}

				for (int y = 0; y < m_nY; y++)
				{
					for (int x = 0; x < m_nX; x++)
					{
						pDest[m_nX * 4 * y + 4 * x + 3] = pImage[m_nX * 4 * y + 4 * x + 3];
						if (pDest[m_nX * 4 * y + 4 * x + 3] < 255)
						{
							pDest[m_nX * 4 * y + 4 * x] = (BYTE)(DWORD(pImage[m_nX * 4 * y + 4 * x + 2]) * pImage[m_nX * 4 * y + 4 * x + 3] / 255);
							pDest[m_nX * 4 * y + 4 * x + 1] = (BYTE)(DWORD(pImage[m_nX * 4 * y + 4 * x + 1]) * pImage[m_nX * 4 * y + 4 * x + 3] / 255);
							pDest[m_nX * 4 * y + 4 * x + 2] = (BYTE)(DWORD(pImage[m_nX * 4 * y + 4 * x]) * pImage[m_nX * 4 * y + 4 * x + 3] / 255);
							bAlphaChannel = true;
						}
						else
						{
							pDest[m_nX * 4 * y + 4 * x] = pImage[m_nX * 4 * y + 4 * x + 2];
							pDest[m_nX * 4 * y + 4 * x + 1] = pImage[m_nX * 4 * y + 4 * x + 1];
							pDest[m_nX * 4 * y + 4 * x + 2] = pImage[m_nX * 4 * y + 4 * x];
						}
					}
				}

				stbi_image_free(pImage);
				return TRUE;
#endif
				return FALSE;
			}
			BOOL CRXImage::LoadIcon(const TCHAR* pszFilename)
			{
#ifdef USE_FREEIMAGE
				return 0;
#endif
#if defined(USE_CXIMAGE)   
				tstring strExt = RXFileExtractExt(pszFilename);
				strExt = _T(".") + strExt;
				int iconID = GetIconIndex(strExt.c_str());
				if (iconID < 0)
				{
					iconID = 0;
			}
				if (iconID >= 0)
				{
					tstring strPath = RXGetModulePath();
					strPath += _T("mpciconlib.dll");
					m_hIcon = ExtractIconW(m_poApp->GetInstance(), strPath.c_str(), iconID);
					if (m_hIcon)
					{
						m_oxImage.CreateFromHICON(m_hIcon);
					}
				}
#endif			
				return TRUE;
			}
#ifdef USE_GDI_PLUS
			BOOL CRXImage::GDIPlusDrawImage(HDC hDC, const RECT& rSrc, const RECT& rDest)
			{
				if (NULL == m_pGdiplusImage) return FALSE;
				Gdiplus::Graphics g(hDC);

				//设置画图时的滤波模式为消除锯齿现象
				g.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
				uint8_t uFade = 255;
				Gdiplus::ImageAttributes imageAtt;
				if (uFade != 255) {
					Gdiplus::ColorMatrix colorMatrix =
					{
						1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 0.0f, uFade / 255.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
					};
					imageAtt.SetColorMatrix(&colorMatrix, Gdiplus::ColorMatrixFlagsDefault, Gdiplus::ColorAdjustTypeBitmap);
				}

				Gdiplus::RectF rcDest(rDest.left, rDest.top, rDest.right - rDest.left, rDest.bottom - rDest.top);
				Gdiplus::RectF rcSrc(rSrc.left, rSrc.top, rSrc.right - rSrc.left, rSrc.bottom - rSrc.top);
				g.DrawImage(m_pGdiplusImage, rcDest, rcSrc.GetLeft(), rcSrc.GetTop(), rcSrc.Width, rcSrc.Height, Gdiplus::UnitPixel, &imageAtt);
				g.ReleaseHDC(hDC);
				return TRUE;
			}
#endif
            INT32 CRXImage::GetHeight()
            {
#ifdef USE_FREEIMAGE
				return m_oImage.getHeight();
#endif
#if defined(USE_CXIMAGE)   
				return m_oxImage.GetHeight();
#endif
#ifdef USE_STB
				return m_nY;
#endif
#ifdef USE_GDI_PLUS
				if (m_pGdiplusImage)
				{
					return	m_pGdiplusImage->GetHeight();
				}
#endif
				return 0;
            }

			void CRXImage::SetTransIndex(int32_t idx)
			{				
#ifdef USE_FREEIMAGE
				
#endif
#if defined(USE_CXIMAGE)   
				m_oxImage.SetTransIndex(idx);
#endif
			}

            INT32 CRXImage::GetWidth()
            {
#ifdef USE_FREEIMAGE
				return m_oImage.getWidth();
#endif
#if defined(USE_CXIMAGE)   
				return m_oxImage.GetWidth();
#endif
#ifdef USE_STB
				return m_nX;
#endif
#ifdef USE_GDI_PLUS
				if (m_pGdiplusImage)
				{
					return	m_pGdiplusImage->GetWidth();
				}
#endif
				return 0;
            }


			void CRXImage::DrawHor(HDC hDC,
				const COLORREF clrTrans,
				const RECT& rcTarget, const UINT32& dwLeft, const UINT32& dwRight)
			{
#ifdef USE_FREEIMAGE
				
#endif
#if defined(USE_CXIMAGE)   
				if (rcTarget.right - rcTarget.left <= (LONG)dwLeft)
				{
					return;
				}
				RECT rcDesc;
				rcDesc.left = rcTarget.left;
				rcDesc.top = rcTarget.top;
				rcDesc.right = dwLeft + rcDesc.left;
				rcDesc.bottom = rcDesc.top + GetHeight();
				RGBQUAD rcTrans;
				rcTrans.rgbRed = GetRValue(clrTrans);
				rcTrans.rgbGreen = GetGValue(clrTrans);
				rcTrans.rgbBlue = GetBValue(clrTrans);
				rcTrans.rgbReserved = 0;
				DrawTrans(hDC, rcTrans, rcDesc, 0, 0);

				INT32 nWidth = GetWidth() - dwRight;
				rcDesc.left = rcTarget.right - nWidth;
				rcDesc.top = rcTarget.top;
				rcDesc.right = rcDesc.left + dwRight;
				rcDesc.bottom = rcDesc.top + GetHeight();
				DrawTrans(hDC,
					rcTrans,
					rcDesc,
					GetWidth() - dwRight,
					0);
				INT32 nMiddleWidth = GetWidth() - nWidth - dwLeft;
				INT32 nLessWidth = rcTarget.right - rcTarget.left - dwLeft - nWidth;
				if (nLessWidth <= 0)
				{
					return;
				}
				INT32 nCount = nLessWidth / nMiddleWidth;
				if (nLessWidth % nMiddleWidth)
				{
					nCount++;
				}
				INT32 nLeft = dwLeft + rcTarget.left;
				for (INT32 n = 0; n < nCount; n++)
				{
					rcDesc.left = nLeft;
					rcDesc.top = rcTarget.top;
					rcDesc.right = rcDesc.left + nMiddleWidth;
					rcDesc.bottom = rcDesc.top + GetHeight();
					DrawTrans(hDC,
						rcTrans,
						rcDesc,
						dwLeft,
						0);
					nLeft += nMiddleWidth;
				}
				if (nLessWidth % nMiddleWidth)
				{
					rcDesc.top = rcTarget.top;
					rcDesc.left = rcTarget.right - nMiddleWidth - nWidth;
					rcDesc.right = rcDesc.left + nMiddleWidth;
					rcDesc.bottom = rcDesc.top + GetHeight();
					DrawTrans(hDC,
						rcTrans,
						rcDesc,
						dwLeft,
						0);
				}
#endif 			
			}


			void CRXImage::DrawVer(HDC hDC, 
				const COLORREF clrTrans, 
				const RECT& rcTarget, 
				const UINT32& dwTop,
				const UINT32& dwBottom)
			{
				RECT rcDesc;
				rcDesc.left = rcTarget.left + (rcTarget.right - rcTarget.left - GetWidth()) / 2;
				rcDesc.top = rcTarget.top;
				rcDesc.right = GetWidth() + rcDesc.left;
				rcDesc.bottom = dwTop;
				RGBQUAD rcTrans;
				rcTrans.rgbRed = GetRValue(clrTrans);
				rcTrans.rgbGreen = GetGValue(clrTrans);
				rcTrans.rgbBlue = GetBValue(clrTrans);
				rcTrans.rgbReserved = 0;
				DrawTrans(hDC, rcTrans, rcDesc, 0, 0);

				INT32 nHeight = dwBottom;
				UINT32 dwWidth = rcTarget.right - rcTarget.left;
				rcDesc.left = rcTarget.left + (dwWidth - GetWidth()) / 2;
				rcDesc.right = GetWidth() + rcDesc.left;
				rcDesc.top = rcTarget.bottom - nHeight;
				rcDesc.bottom = rcTarget.bottom;
				RECT rcCopy;
				rcCopy.left = 0;
				rcCopy.right = GetWidth();
				rcCopy.top = dwTop;
				rcCopy.bottom = GetHeight() - dwBottom;
				DrawTrans(hDC,
					rcTrans,
					rcDesc,
					0,
					GetHeight() - nHeight);
				INT32 nMiddleHeight = GetHeight() - dwBottom - dwTop;
				INT32 nLessHeight = rcTarget.bottom - rcTarget.top - dwBottom - nHeight;
				INT32 nCount = nLessHeight / nMiddleHeight;
				for (INT32 n = 0; n < nCount; n++)
				{
					rcDesc.left = 0;
					rcDesc.top = rcTarget.top +	dwTop +	n * nMiddleHeight;
					rcDesc.right = rcDesc.left + GetWidth();
					rcDesc.bottom = rcDesc.top + nMiddleHeight;
					DrawTrans(hDC,
						rcTrans,
						rcDesc,
						0,
						dwTop);
				}
				if (nLessHeight % nMiddleHeight)
				{
					rcDesc.top = rcTarget.bottom - dwBottom - nMiddleHeight;
					rcDesc.left = rcTarget.left + (rcTarget.right - rcTarget.left - GetWidth()) / 2;
					rcDesc.right = rcDesc.left + GetWidth();
					rcDesc.bottom = rcDesc.top + nMiddleHeight;
					DrawTrans(hDC,
						rcTrans,
						rcDesc,
						0,
						dwTop);
				}
			}
        }

        RX_IMPLEMENT_SINGLETON(CRXImageModule);
        CRXImageModule::CRXImageModule()
        {
        }
        CRXImageModule::~CRXImageModule()
        {
        }

        IRXImage* CRXImageModule::GetImageHandle(const TCHAR* pszFilename)
        {
            CRXImage* poImage = new CRXImage;
            if (NULL == poImage)
            {
                return NULL;
            }
            if (FALSE == poImage->OpenFile(pszFilename))
            {
                poImage->Release();
                return NULL;
            }
            return poImage;
        }

		IRXImage* CRXImageModule::GetImageHandleFromResource(HMODULE hModule, const TCHAR* pszResourceName)
		{
			HRSRC hr = ::FindResource(hModule, pszResourceName, L"PNG");
			if (NULL == hr)
			{
				int ierr = GetLastError();
				return NULL;
			}
			CRXImage* poImage = new CRXImage;
			if (NULL == poImage)
			{
				return NULL;
			}
			if (FALSE == poImage->LoadResource(hr, CXIMAGE_FORMAT_PNG))
			{
				poImage->Release();
				return NULL;
			}
			return poImage;
		}

		IRXImage* CRXImageModule::GetIconImageHandle(const TCHAR* pszFilename)
		{
			CRXImage* poImage = new CRXImage;
#ifdef USE_FREEIMAGE
			return poImage;
#endif
#if defined(USE_CXIMAGE)
			if (NULL == poImage)
			{
				return NULL;
			}
			if (FALSE == poImage->LoadIcon(pszFilename))
			{
				poImage->Release();
				return NULL;
			}
#endif
			return poImage;
		}

        VOID CRXImageModule::Release()
        {
            CRXImageModule::DestroyInstance();
        }

        BOOL CRXImageModule::SaveJpg(const TCHAR* pszFrom, const TCHAR* psTo)
        {
#ifdef USE_FREEIMAGE
			return TRUE;
#endif
#if defined(USE_CXIMAGE)
			CxImage oxImage;
			BOOL bResult = oxImage.LoadA(_RXTT2Local(pszFrom).c_str());
			if (bResult)
			{
				oxImage.SetJpegQuality(75);
				bResult = oxImage.Save(psTo, CXIMAGE_FORMAT_JPG);
		}
#endif
			return TRUE;
        }

		BOOL CRXImageModule::SaveICON(const TCHAR* pszFrom, HICON hIcon)
		{
#ifdef USE_FREEIMAGE
			return TRUE;
#endif
#if defined(USE_CXIMAGE)
			CxImage oxImage;
			oxImage.CreateFromHICON(hIcon);
			oxImage.Save(pszFrom, 0);
			return TRUE;
#endif
			return 0;
		}
        IRXImageModule* RXIMAGEAPI RXImageGetModule()
        {
            if (NULL == CRXImageModule::Instance())
            {
                if (FALSE == CRXImageModule::CreateInstance())
                {
                    return NULL;
                }
            }
            return CRXImageModule::Instance();
        }



		struct DocType
		{
		public:
			int nID;
			BOOL bRead;
			BOOL bWrite;
			tstring description;
			tstring ext;
		};

#if defined(USE_CXIMAGE)
		DocType doctypes[CMAX_IMAGE_FORMATS] =
		{
			{ -1, TRUE, TRUE, _T("支持的文件格式"), _T("*.bmp;*.gif;*.jpg;*.jpeg;*.png;*.ico;*.tif;*.tiff;*.tga;*.pcx;*.wbmp;*.wmf;*.emf;*.j2k;*.jp2;*.jbg;*.j2c;*.jpc;*.pgx;*.pnm;*.pgm;*.ppm;*.ras;*.mng;*.jng;*.ska;*.nef;*.crw;*.cr2;*.mrw;*.raf;*.erf;*.3fr;*.dcr;*.raw;*.dng;*.pef;*.x3f;*.arw;*.sr2;*.mef;*.orf;*.psd") },
#if CXIMAGE_SUPPORT_BMP
			{ CXIMAGE_FORMAT_BMP, TRUE, TRUE, _T("BMP 文件"), _T("*.bmp") },
#endif
#if CXIMAGE_SUPPORT_GIF
			{ CXIMAGE_FORMAT_GIF, TRUE, TRUE, _T("GIF 文件"), _T("*.gif") },
#endif
#if CXIMAGE_SUPPORT_JPG
			{ CXIMAGE_FORMAT_JPG, TRUE, TRUE, _T("JPG 文件"), _T("*.jpg;*.jpeg") },
#endif
#if CXIMAGE_SUPPORT_PNG
			{ CXIMAGE_FORMAT_PNG, TRUE, TRUE, _T("PNG 文件"), _T("*.png") },
#endif
#if CXIMAGE_SUPPORT_MNG
			{ CXIMAGE_FORMAT_MNG, TRUE, TRUE, _T("MNG 文件"), _T("*.mng;*.jng;*.png") },
#endif
#if CXIMAGE_SUPPORT_ICO
			{ CXIMAGE_FORMAT_ICO, TRUE, TRUE, _T("ICO CUR 文件"), _T("*.ico;*.cur") },
#endif
#if CXIMAGE_SUPPORT_TIF
			{ CXIMAGE_FORMAT_TIF, TRUE, TRUE, _T("TIF 文件"), _T("*.tif;*.tiff") },
#endif
#if CXIMAGE_SUPPORT_TGA
			{ CXIMAGE_FORMAT_TGA, TRUE, TRUE, _T("TGA 文件"), _T("*.tga") },
#endif
#if CXIMAGE_SUPPORT_PCX
			{ CXIMAGE_FORMAT_PCX, TRUE, TRUE, _T("PCX 文件"), _T("*.pcx") },
#endif
#if CXIMAGE_SUPPORT_WBMP
			{ CXIMAGE_FORMAT_WBMP, TRUE, TRUE, _T("WBMP 文件"), _T("*.wbmp") },
#endif
#if CXIMAGE_SUPPORT_WMF
			{ CXIMAGE_FORMAT_WMF, TRUE, FALSE, _T("WMF EMF 文件"), _T("*.wmf;*.emf") },
#endif
#if CXIMAGE_SUPPORT_JBG
			{ CXIMAGE_FORMAT_JBG, TRUE, TRUE, _T("JBG 文件"), _T("*.jbg") },
#endif
#if CXIMAGE_SUPPORT_JP2
			{ CXIMAGE_FORMAT_JP2, TRUE, TRUE, _T("JP2 文件"), _T("*.j2k;*.jp2") },
#endif
#if CXIMAGE_SUPPORT_JPC
			{ CXIMAGE_FORMAT_JPC, TRUE, TRUE, _T("JPC 文件"), _T("*.j2c;*.jpc") },
#endif
#if CXIMAGE_SUPPORT_PGX
			{ CXIMAGE_FORMAT_PGX, TRUE, TRUE, _T("PGX 文件"), _T("*.pgx") },
#endif
#if CXIMAGE_SUPPORT_RAS
			{ CXIMAGE_FORMAT_RAS, TRUE, TRUE, _T("RAS 文件"), _T("*.ras") },
#endif
#if CXIMAGE_SUPPORT_PNM
			{ CXIMAGE_FORMAT_PNM, TRUE, TRUE, _T("PNM 文件"), _T("*.pnm;*.pgm;*.ppm") },
#endif
#if CXIMAGE_SUPPORT_SKA
			{ CXIMAGE_FORMAT_SKA, TRUE, TRUE, _T("SKA 文件"), _T("*.ska") },
#endif

#if CXIMAGE_SUPPORT_PSD
			{ CXIMAGE_SUPPORT_PSD, TRUE, TRUE, _T("PSD 文件"), _T("*.psd") },
#endif

#if CXIMAGE_SUPPORT_RAW
			{ CXIMAGE_FORMAT_RAW, TRUE, FALSE, _T("RAW 文件"), _T("*.nef;*.crw;*.cr2;*.mrw;*.raf;*.erf;*.3fr;*.dcr;*.raw;*.dng;*.pef;*.x3f;*.arw;*.sr2;*.mef;*.orf") }
#endif
		};
#endif



		int RXIMAGEAPI RXGetIndexFromType(int nDocType, BOOL bOpenFileDialog)
		{
			int nCnt = 0;

#if defined(USE_CXIMAGE)
			for (int i = 0; i < CMAX_IMAGE_FORMATS; i++) {
				if (bOpenFileDialog ? doctypes[i].bRead : doctypes[i].bWrite) {
					if (doctypes[i].nID == nDocType) return nCnt;
					nCnt++;
				}
			}
#endif
			return -1;
		}

		tstring RXIMAGEAPI RXGetExtFromType(int nDocType)
		{

#if defined(USE_CXIMAGE)
			for (int i = 0; i < CMAX_IMAGE_FORMATS; i++) {
				if (doctypes[i].nID == nDocType)
					return doctypes[i].ext;
			}
#endif
			return tstring(_T(""));
		}




		int RXIMAGEAPI RXGetTypeFromIndex(int nIndex, BOOL bOpenFileDialog)
		{
			int nCnt = 0;

#if defined(USE_CXIMAGE)
			for (int i = 0; i < CMAX_IMAGE_FORMATS; i++) {
				if (bOpenFileDialog ? doctypes[i].bRead : doctypes[i].bWrite) {
					if (nCnt == nIndex)
						//              return i; // PJO - Buglet ?
						return doctypes[i].nID;
					nCnt++;
				}
			}
#endif
			return -1;
		}


    }
	int RXSkinGetTypeFromIndex(int nIndex, BOOL bOpenFileDialog)
	{
		int nCnt = 0;

#if defined(USE_CXIMAGE)
		for (int i = 0; i < CMAX_IMAGE_FORMATS; i++) {
			if (bOpenFileDialog ? doctypes[i].bRead : doctypes[i].bWrite) {
				if (nCnt == nIndex)
					//              return i; // PJO - Buglet ?
					return doctypes[i].nID;
				nCnt++;
			}
		}
#endif
		return -1;
	}
	tstring RXSkinGetExtFromType(int nDocType)
	{
#if defined(USE_CXIMAGE)
		for (int i = 0; i < CMAX_IMAGE_FORMATS; i++) {
			if (doctypes[i].nID == nDocType)
				return doctypes[i].ext;
		}
#endif
		return tstring(_T(""));
	}

	int RXSkinGetIndexFromType(int nDocType, BOOL bOpenFileDialog)
	{
		int nCnt = 0;

#if defined(USE_CXIMAGE)
		for (int i = 0; i < CMAX_IMAGE_FORMATS; i++) {
			if (bOpenFileDialog ? doctypes[i].bRead : doctypes[i].bWrite) {
				if (doctypes[i].nID == nDocType) return nCnt;
				nCnt++;
			}
		}
#endif
		return -1;
	}

	TCHAR* RXSkinGetFileTypes(BOOL bOpenFileDialog)
	{
		static tstring str;
#if defined(USE_CXIMAGE)
		static BOOL bFinished = FALSE;
		if (bFinished == FALSE)
		{
			str.resize(1024, _T('\0'));
			bFinished = TRUE;
			UINT32 dwPos = 0;
			for (int i = 0; i < CMAX_IMAGE_FORMATS; i++) {
				if (bOpenFileDialog && doctypes[i].bRead) {
					_RXTStrcpy((TCHAR*)(str.c_str() + dwPos), doctypes[i].description.c_str());
					dwPos +=(UINT32) doctypes[i].description.size();
					str[dwPos] = _T('\0');
					dwPos++;
					_RXTStrcpy((TCHAR*)(str.c_str() + dwPos), doctypes[i].ext.c_str());
					dwPos += (UINT32)doctypes[i].ext.size();
					str[dwPos] = _T('\0');
					dwPos++;
					str[dwPos] = _T('\0');
				}
				else if (!bOpenFileDialog && doctypes[i].bWrite) {
					_RXTStrcpy((TCHAR*)(str.c_str() + dwPos), doctypes[i].description.c_str());
					dwPos += (UINT32)doctypes[i].description.size();
					str[dwPos] = _T('\0');
					dwPos++;
					_RXTStrcpy((TCHAR*)(str.c_str() + dwPos), doctypes[i].ext.c_str());
					dwPos += (UINT32)doctypes[i].ext.size();
					str[dwPos] = _T('\0');
				}
			}
		}
#endif
		//memcpy(szTypes, str.c_str(), str.size() * sizeof(TCHAR));
		return (TCHAR*)str.c_str();
	}

	RXSKIN_API TCHAR* RXAPI RXGetFileTypes(BOOL bOpenFileDialog)
	{
		tstring str;
#if defined(USE_CXIMAGE)
		static TCHAR szTypes[1024] = { 0 };
		memset(szTypes, 0, sizeof(TCHAR) * 1024);
		for (int i = 0; i < CMAX_IMAGE_FORMATS; i++) {
			if (bOpenFileDialog && doctypes[i].bRead) {
				str += doctypes[i].description;
				str += (TCHAR)NULL;
				str += doctypes[i].ext;
				str += (TCHAR)NULL;
			}
			else if (!bOpenFileDialog && doctypes[i].bWrite) {
				str += doctypes[i].description;
				str += (TCHAR)NULL;
				str += doctypes[i].ext;
				str += (TCHAR)NULL;
			}
		}
		str += _T("\0");
		memcpy(szTypes, str.c_str(), sizeof(TCHAR) * str.size());
#endif
		return(TCHAR*)_T("");
	}
