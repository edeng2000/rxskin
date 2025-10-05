#pragma once
#include "RXSkin.h"
#include "rxskinitem.h"
#include "common/rxskindefines.h"
#include <map>
#include <vector>
using namespace std;

class CRXSkinMenu;


class RXSKIN_API CRXSkinMenuItem : public CRXSkinItem
{
public:
	CRXSkinMenuItem();
	~CRXSkinMenuItem();
	virtual VOID  OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual CRXSkinItem* HitTest(POINT pt);

	EMenuItemType   GetType();
	VOID            SetType(EMenuItemType enType);

	UINT32          GetWidth();
	UINT32          GetHeight();

	TCHAR*          GetName();
	VOID            SetName(const TCHAR* pszName);

	VOID            SetImage(const TCHAR* pszImageFile);
	VOID            SetImage(const UINT32 dwImageID);
	void            SetChecked(const BOOL& bChecked) { m_bChecked = bChecked; }
	BOOL            IsChecked() { return m_bChecked; }

	void            SetChildMenu(CRXSkinMenu* poChild) { m_poChildMenu = poChild; }
	CRXSkinMenu* GetChildMenu() { return m_poChildMenu; }
private:
	EMenuItemType   m_enType;
	TCHAR           m_szName[256];
	IRXImage*       m_poImage;
	CRXSkinMenu* m_poChildMenu;
	BOOL            m_bChecked;

private:
	VOID            _DrawString(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	VOID            _DrawSperator(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	VOID            _DrawSubMenu(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	VOID            _DrawCheckedMenu(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
};


class RXSKIN_API CRXSkinMenu: public CRXSkinWnd
{
public:
	typedef map<UINT32,CRXSkinMenuItem*>    CRXSkinMenuItemMap;
	typedef CRXSkinMenuItemMap::iterator    CRXSkinMenuItemMapItr;

	typedef vector<CRXSkinMenuItem*>        CRXSkinMenuItemVect;
	typedef CRXSkinMenuItemVect::iterator   CRXSkinMenuItemVectItr;

	CRXSkinMenu(void);
	~CRXSkinMenu(void);
	virtual	VOID    Destroy();
	virtual BOOL Create(const TCHAR* pszWindowClassName,
		const TCHAR* pszWindowName,
		const UINT32 dwStyle,
		const HWND hParentWnd,
		HICON hIcon = NULL);
	virtual void OnFinalMessage(HWND hWnd);
	virtual BOOL OnMouseMove(UINT nFlags, POINT point);
	virtual LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	virtual BOOL  OnLButtonDown(UINT nFlags, POINT point);
	virtual BOOL  OnLButtonUp(UINT nFlags, POINT point);
	virtual BOOL  OnCreate(LPCREATESTRUCT lpCreate);
	virtual VOID  OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual BOOL  OnSize(WPARAM wParam, LPARAM lParam);
	virtual VOID  OnPaint();
	virtual LRESULT OnKillFocus(WPARAM wParam, LPARAM lParam);
	CRXSkinMenuItem*    AppendMenu(EMenuItemType enType,
								   const UINT32 dwID,
								   const TCHAR* pszName=_T(""),
								   const TCHAR* pszImageFile=_T(""));
	CRXSkinMenuItem*    AppendMenuEx(EMenuItemType enType,
									const UINT32 dwID,
									const TCHAR* pszName=_T(""),
									const UINT32 dwImageID = 0,
		CRXSkinMenu* poChild = NULL);
	CRXSkinMenuItem*    FindMenuItem(const UINT32 dwID);
	CRXSkinMenuItem*    RemoveMenuItem(const UINT32 dwID);
	CRXSkinMenuItemMap& GetItems();
	BOOL        IsWnd() { return m_hWnd != NULL; }
	VOID        ClearAll() { _DeleteAll(); }
	BOOL        ShowMenu(POINT pt,const HWND& hFocusWnd = NULL);
	VOID        HideMenu();
	HWND        GetHWnd() { return m_hWnd; }
	void        SetItemChecked(const UINT32& dwID,const bool& bChecked);
	void        SetParentMenu(CRXSkinMenu* poParenMenu) { m_poParentMenu = poParenMenu; }
private:	
	CRXSkinMenuItem*   m_poMouseOn;
	CRXSkinMenuItem*   m_poPressed;
	CRXSkinMenuItemMap m_mapItems;
	CRXSkinMenuItemVect m_vecItems;
	BOOL        m_bTracking;
	CRXSkinMenu* m_poParentMenu;
private:
	VOID  _DeleteAll();

	UINT32 _GetWidth();
	UINT32 _GetHeight();

	CRXSkinMenuItem*  _HitTest(POINT pt);
};
