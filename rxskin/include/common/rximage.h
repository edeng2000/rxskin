#ifndef _RX_IMAGR_INTERFACE_H_
#define _RX_IMAGR_INTERFACE_H_
#include <string.h>
#include <string>
#include "windows.h"
#ifdef _RX_IMAGE_STATIC
#define RXIMAGEAPI  
#else  
#define RXIMAGEAPI _stdcall
#endif
#ifdef UNICODE
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif

namespace RX
{
    namespace Image
    {
        class IRXImage
        {
        public:

            virtual VOID  CreateFromICON(HICON hIcon) = 0;
            virtual INT32 Draw(HDC hDC,
                INT32 nX = 0,
                INT32 nY = 0,
                INT32 nWidth = -1,
                INT32 nHeight = -1,
                INT32 desx = 0,
                INT32 desy = 0,
                RECT* poClipRect = 0,
                bool  bSmooth = FALSE,
                bool  bFlipY = false) = 0;
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
                bool  bFlipY = false
            ) = 0;
            virtual INT32 DrawGray(HDC hDC,
                INT32 nX = 0,
                INT32 nY = 0,
                INT32 nWidth = -1,
                INT32 nHeight = -1,
                INT32 desx = 0,
                INT32 desy = 0,
                RECT* poClipRect = 0,
                bool  bSmooth = FALSE,
                bool  bFlipY = false) = 0;

            virtual INT32 Draw(HDC hDC,
                const RECT& oRect,
                INT32 desx = 0,
                INT32 desy = 0,
                RECT* poClipRect = NULL,
                bool  bSmooth = FALSE,
                bool  bFlipY = false) = 0;
            virtual INT32 DrawTrans(HDC hDC,
                RGBQUAD rgbTrans,
                const RECT& oRect,
                INT32 desx = 0,
                INT32 desy = 0,
                RECT* poClipRect = NULL,
                bool  bSmooth = FALSE,
                bool  bFlipY = false) = 0;
            virtual INT32 DrawGray(HDC hDC,
                const RECT& oRect,
                INT32 desx = 0,
                INT32 desy = 0,
                RECT* poClipRect = NULL,
                bool  bSmooth = FALSE,
                bool  bFlipY = false) = 0;

            virtual INT32 Stretch(HDC hDC,
                INT32 nXOffset,
                INT32 nYOffset,
                INT32 nXSize,
                INT32 nYSize,

                UINT32 dwRop = SRCCOPY) = 0;
            virtual void  SetTransIndex(int32_t idx) = 0;
            virtual INT32 StretchGray(HDC hDC,
                INT32 nXOffset,
                INT32 nYOffset,
                INT32 nXSize,
                INT32 nYSize,
                UINT32 dwRop = SRCCOPY) = 0;

            virtual INT32 Stretch(HDC hDC,
                const RECT& oRect,
                UINT32 dwRop = SRCCOPY) = 0;

            virtual INT32 StretchGray(HDC hDC,
                const RECT& oRect,
                UINT32 dwRop = SRCCOPY) = 0;

            virtual VOID  Release() = 0;
            virtual INT32 GetWidth() = 0;
            virtual INT32 GetHeight() = 0;
 			virtual void  DrawHor(HDC hDC,
				const COLORREF clrTrans,
				const RECT& rcTarget,
				const UINT32& dwLeft, 
				const UINT32& dwRight) = 0;
			virtual void  DrawVer(HDC hDC,
				const COLORREF clrTrans,
				const RECT& rcTarget,
				const UINT32& dwTop,
				const UINT32& dwBottom) = 0;
            virtual void  Scale(const float& fRate) = 0;
            virtual void  Rotation(const float& fRotation) = 0;
        };
        class IRXImageModule
        {
        public:
            virtual IRXImage* GetImageHandle(const TCHAR* pszFilename) = 0;
            virtual IRXImage* GetIconImageHandle(const TCHAR* pszFilename) = 0;
            virtual IRXImage* GetImageHandleFromResource(HMODULE hModule, const TCHAR* pszResourceName) = 0; 
            virtual VOID Release() = 0;
            virtual BOOL SaveJpg(const TCHAR* pszFrom, const TCHAR* psTo) = 0;
            virtual BOOL SaveICON(const TCHAR* pszFrom, HICON hIcon) = 0;
        };

        /**
    * @brief 根据版本号创建相应的DBModule
    */
        IRXImageModule* RXIMAGEAPI RXImageGetModule();

        /**
        * @brief 定义的指向SDDBGetModule的指针
        */
        typedef IRXImageModule* (RXIMAGEAPI *PFN_RXImageGetModule)();

 
        int RXIMAGEAPI RXGetIndexFromType(int nDocType, BOOL bOpenFileDialog);
        typedef int (RXIMAGEAPI *PFN_RXGetIndexFromType)(int, BOOL);

        tstring RXIMAGEAPI RXGetExtFromType(int nDocType);
        typedef tstring(RXIMAGEAPI *PFN_RXGetExtFromType)(int);

        int RXIMAGEAPI RXGetTypeFromIndex(int nIndex, BOOL bOpenFileDialog);
        typedef int (RXIMAGEAPI *PFN_RXGetTypeFromIndex)(int, BOOL);

    }
}


#endif