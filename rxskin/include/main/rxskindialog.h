#pragma once
#include <CommCtrl.h>
#include "RXSkin.h"
#include "common/rximage.h"
#include "main/rxskinwnd.h"
#include "common/rxskinutil.h"
#include "controls/rxcontrol.h"
#include "controls/RXSkinToolTip.h"
#include "controls/RXSkinMenu.h"
using namespace RX;
using namespace RX::Image;

class RXSKIN_API CRXSkinDialog : public CRXSkinWnd
{
public:
	CRXSkinDialog(void);
	~CRXSkinDialog(void);
	virtual VOID Destroy();
	virtual BOOL Create(const TCHAR* pszWindowClassName,
						const TCHAR* pszWindowName,
						const UINT32 dwStyle,
						const HWND hParentWnd,
						HICON hIcon = NULL);
	virtual VOID  OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem) = 0;
	virtual BOOL  OnCreate(LPCREATESTRUCT lpCreate);
	virtual BOOL  OnSize(WPARAM wParam, LPARAM lParam);
	virtual BOOL  OnSizing(WPARAM wParam, LPARAM lParam);
	virtual void  OnFinalMessage(HWND hWnd);
	virtual BOOL  OnMouseMove(UINT nFlags, POINT point);
	virtual BOOL  OnMouseHover(UINT nFlags, POINT point);
	virtual BOOL  OnLButtonDown(UINT nFlags, POINT point);
	virtual BOOL  OnLButtonUp(UINT nFlags, POINT point);
	virtual BOOL  OnRButtonDown(UINT nFlags, POINT point);
	virtual BOOL  OnRButtonUp(UINT nFlags, POINT point);
	virtual BOOL  OnLButtonDblClk(UINT nFlags, POINT point);
	virtual BOOL  OnRButtonDblClk(UINT nFlags, POINT point);
	virtual LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	virtual BOOL  OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL  OnMessage(UINT32 dwMessageID,WPARAM wParam, LPARAM lParam);
	virtual BOOL  OnTimer(WPARAM wParam, LPARAM lParam);
	virtual BOOL  OnMove(int x, int y);
	virtual VOID  OnRButtonSystemToolbar();
	virtual HBRUSH  OnCtlColorControl(uint32_t& dwMsgID, HWND hWndChild, HDC hDC);
	virtual BOOL  OnMouseWheel(UINT nFlags, short zDelta,POINT pt);
	virtual VOID  OnScrollBarChanged(EScrollType enType,INT32 nPercent);
	virtual LRESULT OnSystemCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnKeyDown(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnKeyUp(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnSysChar(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnSysKeydown(WPARAM wParam, LPARAM lParam);
	virtual CRXControl* OnCreateControl(tinyxml2::XMLElement* poElement);
	virtual void    AddAnchor(HWND hWnd, CRXSkinResizePoint ptTopLeft, CRXSkinResizePoint ptBottomRight);

	BOOL    ReadConfig(const TCHAR* pszConfig);
	BOOL    ReadConfigFromResource(const TCHAR* pszResource);
	VOID    SetTitle(const TCHAR* pszTitle);
	TCHAR*  GetTitle();
	VOID    InvalidateRect(CRXSkinRect& rcClient,BOOL bRePaint = TRUE);
	void    SetTimer(const DWORD& dwEventID,const UINT32 dwInterval ,LPARAM lpParam);
	void    KillTimer(const DWORD& dwEventID);
	VOID    GetClientRectEx(CRXSkinRect& oRect);
	INT32   ShowModal();
	VOID    CenterWindow();
	CRXSkinRect GetInitRect() { return m_rcInit; }
	VOID    MoveWindow(CRXSkinRect rcWin,BOOL bRepaint = TRUE);
	void    SetDrawControls(const BOOL& bDrawControls) { m_bDrawControls = bDrawControls; }
	
	VOID    AddMsgFilter(CRXSkinDialog* poWnd);
	VOID    RestoreWindow();
	CRXSkinRect GetCaptionRect();

	BOOL    IsMinimizedDialog();

protected:
	static BOOL TranslateMessage(const LPMSG pMsg);
	virtual LRESULT OnHandleMessage(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL    OnPreMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);
	virtual VOID    OnPaint();
	VOID    _UpdateToolTip(CRXControl* poControl);
protected:
	virtual void    OnClickedClose();
	virtual void    OnClickedMin();
	virtual void    OnClickedMax();
	virtual void    OnClickedRestore();
	virtual VOID    OnPaintBk(HDC hDC);

private:
	class RXSKIN_API CLayout
	{
	public:
		CLayout();
		~CLayout();
		CRXSkinResizePoint   GetTopLeft();
		VOID                 SetTopLeft(CRXSkinResizePoint pt);
		CRXSkinResizePoint   GetBottomRight();
		VOID                 SetBottomRight(CRXSkinResizePoint pt);

		VOID                 SetResize(CRXSkinResizePoint ptTopLeft,CRXSkinResizePoint ptBottomRight);

		VOID                 SetWnd(HWND hWnd);
		HWND                 GetWnd();

		VOID                 SetParentWnd(HWND hParentWnd);

		HDWP                 OnSize(const UINT32 cx,const UINT32 cy,HDWP hdwp);
		CRXSkinResizeRect    GetResize();
	private:
		CRXSkinResizePoint   m_ptTopLeft;
		CRXSkinResizePoint   m_ptBottomRight;
		CRXSkinResizeRect	 m_rrcSizing;
		HWND                 m_hWnd;
		HWND                 m_hParentWnd;
		SIZE                 m_oMarginTopLeft;
		SIZE                 m_oMarginBottomRight;
	};

	typedef map<HWND,CLayout*>    CLayoutMap;
	typedef CLayoutMap::iterator  CLayoutMapItr;


protected:
	CBorder       m_borderTop;
	CBorder       m_borderLeft;
	CBorder       m_borderBottom;
	CBorder       m_borderRight;
	BOOL          m_bBordered;

	INT32         m_nRoundHeight;
	INT32         m_nRoundWidth;

	TCHAR         m_szTitle[256];

	SIZE          m_szMin;
	
	BOOL          m_bTracking;
	CRXControl*   m_poMouseOn;
	CRXControl*   m_poPressed;
	INT32         m_nPosition;
	BOOL          m_bMoving;
	BOOL          m_bMax;

	CLayoutMap    m_mapLayout;
	BOOL          m_bResized;

	CRXSkinToolTip m_oToolTip;
	CRXSkinMenu    m_oMenu;

	CRXSkinRect    m_rcBkTop;
	COLORREF       m_clrBkTopFrom,m_clrBkTopTo;

	CRXSkinRect    m_rcBkBottom;
	COLORREF       m_clrBkBottomFrom,m_clrBkBottomTo;

	CRXSkinRect    m_rcBkMiddle;
	COLORREF       m_clrBkMiddleFrom,m_clrBkMiddleTo;

	CRXSkinRect    m_rcInit;

	CRXSkinStdPtrArray m_aPreMessageFilters;
	INT32          m_nModelRet;  

	BOOL           m_bDrawControls;

	CRXSkinRect    m_rcRestore;
private:
	UINT32       _HitTest(POINT point);
	BOOL         _ReadConfigEx(tinyxml2::XMLDocument* poDoc, const EResourceType& enbFromResource = RESOURCE_FROM_FILE);
};
