#pragma once
#include "rximage.h"

#ifdef USE_FREEIMAGE
#include "FreeImage.h"
#include "FreeImagePlus.h"
#include "TxDIB.h"
#endif

#if defined(USE_CXIMAGE)
#include "ximage.h"
#endif

#include "fundation/rxsingleton.h"
using namespace RX;
using namespace RX::Fundation;
namespace RX
{
    namespace Image
    {
        class CRXImage : public IRXImage
        {
        public:
            CRXImage();
            ~CRXImage(void);

            virtual INT32 Draw(HDC hDC,
                INT32 nX = 0,
                INT32 nY = 0,
                INT32 nWidth = -1,
                INT32 nHeight = -1,
                INT32 desx = 0,
                INT32 desy = 0,
                RECT* poClipRect = 0,
                bool  bSmooth = FALSE,
                bool bFlipY = false);
            virtual INT32 DrawTrans(HDC hDC,
                RGBQUAD rgbTrans,
                INT32 nX = 0,
                INT32 nY = 0,
                INT32 nWidth = -1,
                INT32 nHeight = -1,
                INT32 desx = 0,
                INT32 desy = 0,
                RECT* poClipRect = 0,
                bool  bSmooth = FALSE,
                bool bFlipY = false);
            virtual INT32 DrawGray(HDC hDC,
                INT32 nX = 0,
                INT32 nY = 0,
                INT32 nWidth = -1,
                INT32 nHeight = -1,
                INT32 desx = 0,
                INT32 desy = 0,
                RECT* poClipRect = 0,
                bool  bSmooth = FALSE,
                bool bFlipY = false);

            virtual INT32 Draw(HDC hDC,
                const RECT& oRect,
                INT32 desx = 0,
                INT32 desy = 0,
                RECT* poClipRect = NULL,
                bool  bSmooth = FALSE,
                bool bFlipY = false);
            virtual INT32 DrawTrans(HDC hDC,
                RGBQUAD rgbTrans,
                const RECT& oRect,
                INT32 desx = 0,
                INT32 desy = 0,
                RECT* poClipRect = NULL,
                bool  bSmooth = FALSE,
                bool bFlipY = false);

            virtual INT32 DrawGray(HDC hDC,
                const RECT& oRect,
                INT32 desx = 0,
                INT32 desy = 0,
                RECT* poClipRect = NULL,
                bool  bSmooth = FALSE,
                bool  bFlipY = false);

            virtual INT32 Stretch(HDC hDC,
                INT32 nXOffset,
                INT32 nYOffset,
                INT32 nXSize,
                INT32 nYSsize,
                UINT32 dwRop = SRCCOPY);

            virtual INT32 StretchGray(HDC hDC,
                INT32 nXOffset,
                INT32 nYOffset,
                INT32 nXSize,
                INT32 nYSsize,
                UINT32 dwRop = SRCCOPY);


            virtual INT32 Stretch(HDC hDC,
                const RECT& oRect,
                UINT32 dwRop = SRCCOPY);

            virtual INT32 StretchGray(HDC hDC,
                const RECT& oRect,
                UINT32 dwRop = SRCCOPY);
            virtual VOID  Release();
            virtual INT32 GetWidth();
            virtual INT32 GetHeight();
            virtual void  SetTransIndex(int32_t idx);
            virtual VOID  CreateFromICON(HICON hIcon);
			virtual void  DrawHor(HDC hDC,
				const COLORREF clrTrans,
				const RECT& rcTarget, const UINT32& dwLeft, const UINT32& dwRight);
			virtual void  DrawVer(HDC hDC,
				const COLORREF clrTrans,
				const RECT& rcTarget,
				const UINT32& dwTop,
				const UINT32& dwBottom);
            virtual void  Rotation(const float& fRotation);
            virtual void  Scale(const float& fRate);
            BOOL          OpenFile(const TCHAR* pszFilename);
            BOOL          LoadResource(HRSRC,const int32_t nType);
            void          DrawImage(HDC hDC, const RECT& rcDest, HBITMAP hBitmap, const RECT rcSrc, BYTE byAlpha = 0xFF);
            BOOL          LoadIcon(const TCHAR* pszFilename);
#ifdef USE_GDI_PLUS
            BOOL          GDIPlusDrawImage(HDC hDC, const RECT& rcSrc, const RECT& rcDest);
#endif
        private:
#ifdef USE_FREEIMAGE
            CTxDIB  m_oImage;
#endif
#ifdef USE_STB
            HBITMAP m_hBitmap;
            int     m_nX;
            int     m_nY;
            DWORD   m_dwMask;
#endif
#if defined(USE_CXIMAGE)
			CxImage      m_oxImage;
#endif
#ifdef USE_GDI_PLUS
            Gdiplus::Image* m_pGdiplusImage;
#endif

            TCHAR        m_szImageFile[256];
            HICON        m_hIcon;
        };



        class CRXImageModule : public IRXImageModule
        {
        public:
            RX_DECLARE_SINGLETON(CRXImageModule)
            CRXImageModule();
            ~CRXImageModule();
            virtual IRXImage* GetIconImageHandle(const TCHAR* pszFilename);
            virtual IRXImage* GetImageHandle(const TCHAR* pszFilename);
            virtual IRXImage* GetImageHandleFromResource(HMODULE hModule, const TCHAR* pszResourceName);
            virtual VOID Release();
            virtual BOOL SaveJpg(const TCHAR* pszFrom, const TCHAR* psTo);
            virtual BOOL SaveICON(const TCHAR* pszFrom, HICON hIcon);
        };

    }
}
