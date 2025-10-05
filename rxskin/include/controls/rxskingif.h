#pragma once
#include "rxskin.h"
#include "rxcontrol.h"

#include "common/EasyGifReader.h"

class RXSKIN_API CRXSkinGif : public CRXControl
{
public:
	CRXSkinGif(void);
	~CRXSkinGif(void);
	enum ETimerID
	{
		SHOW_GIF_TIMER_ID = 100,
	};
	virtual VOID OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual VOID ReleaseControl();
	virtual BOOL PtInRect(POINT pt);
	virtual BOOL OnClick(POINT pt);
	virtual BOOL OnMouseMove(POINT pt);
	virtual BOOL OnLButtonDown(POINT pt);
	virtual BOOL OnLButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDown(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDblClk(POINT pt) { return FALSE; }
	virtual VOID SetVisible(const BOOL bIsVisible);
	virtual BOOL OnTimer(WPARAM wParam, LPARAM lParam);
	VOID    SetImage(const TCHAR* pszImageFile);
	VOID    SetImageData(const BYTE* pszImageData,const uint32_t& dwLen);
	virtual void SetRect(const CRXSkinRect rcItem);
	
private:
	BITMAPINFOHEADER m_stHeader;
	tstring   m_strImageFile;
	uint32_t  m_dwWidth;
	uint32_t  m_dwHeight;
	uint32_t  m_dwFrameCount;
	uint32_t  m_dwCurFrame;
	vector<HBITMAP> m_vecBitmap;
};
