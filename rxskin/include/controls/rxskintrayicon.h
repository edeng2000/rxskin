#pragma once
#include "rxskin.h"
#include "fundation/rxstring.h"
#include "shellapi.h"
class ITrayIconListener;

class RXSKIN_API CRXSkinTrayIcon
{
public:
	CRXSkinTrayIcon(const TCHAR* name=_T("tray_icon"), 
					bool visible=false,
					HICON hIcon=NULL,
					bool destroy_icon_in_destructor=false);
	virtual ~CRXSkinTrayIcon();

	virtual void SetName(const TCHAR* name);
	const TCHAR* GetName() const			{ return m_Name.c_str(); }
	virtual bool SetVisible(bool visible);
	bool IsVisible() const					{ return m_Visible; }

	virtual void SetIcon(HICON hNewIcon, bool destroy_current_icon=true);
	HICON GetIcon() const						{ return m_hIcon; }

	void SetDestroyIconInDestructor(bool b)	{ m_DestroyIconInDestructor = b; }
	bool GetDestroyIconInDestructor() const { return m_DestroyIconInDestructor; }

	enum ETooltipIcon
	{
		eTI_None,			// NIIF_NONE(0)
		eTI_Info,			// NIIF_INFO(1)
		eTI_Warning,		// NIIF_WARNING(2)
		eTI_Error			// NIIF_ERROR(3)
	};
	// ShowBalloonTooltip() works only on win2k and later
	bool ShowBalloonTooltip(const TCHAR* title, const TCHAR* msg, ETooltipIcon icon=eTI_None);

	typedef void (*POnMessageFunc)(CRXSkinTrayIcon* pTrayIcon, UINT uMsg);
	void SetListener(POnMessageFunc pOnMessageFunc) { m_pOnMessageFunc = pOnMessageFunc; }
	void SetListener(ITrayIconListener *pListener) { m_pListener = pListener; }

protected:
	virtual void OnMessage(UINT uMsg);

private:
	void FillNotifyIconData(NOTIFYICONDATA& data);
	// Never returns NULL! If GetIcon()==NULL, then this returns a system icon
	HICON InternalGetIcon() const;
	bool AddIcon();
	bool RemoveIcon();
	void OnTaskbarCreated();

private:
	UINT m_Id;
	tstring m_Name;
	bool m_Visible;
	HICON m_hIcon;
	bool m_DestroyIconInDestructor;
	POnMessageFunc m_pOnMessageFunc;
	ITrayIconListener* m_pListener;

	static LRESULT CALLBACK MessageProcessorWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static HWND GetMessageProcessorHWND();
};


//-------------------------------------------------------------------------------------------------


class ITrayIconListener
{
public:
	virtual void OnTrayIconMouseMove(CRXSkinTrayIcon* pTrayIcon) {}

	virtual void OnTrayIconLButtonDown(CRXSkinTrayIcon* pTrayIcon) {}
	virtual void OnTrayIconLButtonUp(CRXSkinTrayIcon* pTrayIcon) {}
	virtual void OnTrayIconLButtonDblClk(CRXSkinTrayIcon* pTrayIcon) {}

	virtual void OnTrayIconRButtonDown(CRXSkinTrayIcon* pTrayIcon) {}
	virtual void OnTrayIconRButtonUp(CRXSkinTrayIcon* pTrayIcon) {}
	virtual void OnTrayIconRButtonDblClk(CRXSkinTrayIcon* pTrayIcon) {}

	virtual void OnTrayIconMButtonDown(CRXSkinTrayIcon* pTrayIcon) {}
	virtual void OnTrayIconMButtonUp(CRXSkinTrayIcon* pTrayIcon) {}
	virtual void OnTrayIconMButtonDblClk(CRXSkinTrayIcon* pTrayIcon) {}

	// WinXP and later
	virtual void OnTrayIconSelect(CRXSkinTrayIcon* pTrayIcon) {}
	virtual void OnTrayIconBalloonShow(CRXSkinTrayIcon* pTrayIcon) {}
	virtual void OnTrayIconBalloonHide(CRXSkinTrayIcon* pTrayIcon) {}
	virtual void OnTrayIconBalloonTimeout(CRXSkinTrayIcon* pTrayIcon) {}
	virtual void OnTrayIconBalloonUserClick(CRXSkinTrayIcon* pTrayIcon) {}

	// Use GetCursorPos() if you need the location of the cursor.
	virtual void OnTrayIconMessage(CRXSkinTrayIcon* pTrayIcon, UINT uMsg)
	{
		switch (uMsg)
		{
		case WM_MOUSEMOVE: OnTrayIconMouseMove(pTrayIcon); break;
		case WM_LBUTTONDOWN: OnTrayIconLButtonDown(pTrayIcon); break;
		case WM_LBUTTONUP: OnTrayIconLButtonUp(pTrayIcon); break;
		case WM_LBUTTONDBLCLK: OnTrayIconLButtonDblClk(pTrayIcon); break;
		case WM_RBUTTONDOWN: OnTrayIconRButtonDown(pTrayIcon); break;
		case WM_RBUTTONUP: OnTrayIconRButtonUp(pTrayIcon); break;
		case WM_RBUTTONDBLCLK: OnTrayIconRButtonDblClk(pTrayIcon); break;
		case WM_MBUTTONDOWN: OnTrayIconMButtonDown(pTrayIcon); break;
		case WM_MBUTTONUP: OnTrayIconMButtonUp(pTrayIcon); break;
		case WM_MBUTTONDBLCLK: OnTrayIconMButtonDblClk(pTrayIcon); break;

#ifdef NIN_SELECT
		case NIN_SELECT: OnTrayIconSelect(pTrayIcon); break;
		case NIN_BALLOONSHOW: OnTrayIconBalloonShow(pTrayIcon); break;
		case NIN_BALLOONHIDE: OnTrayIconBalloonHide(pTrayIcon); break;
		case NIN_BALLOONTIMEOUT: OnTrayIconBalloonTimeout(pTrayIcon); break;
		case NIN_BALLOONUSERCLICK: OnTrayIconBalloonUserClick(pTrayIcon); break;
#endif
		}
	}
};

