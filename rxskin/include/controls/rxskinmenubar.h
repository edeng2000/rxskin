#pragma once
#include "rxskin.h"
#include "rxskinitem.h"
#include "rxcontrol.h"
#include "rxskinscrollbar.h"
#include "fundation/rxstring.h"
#include "main/rxskinwnd.h"
#include <map>
using namespace std;

class CRXSkinMenubar;
class RXSKIN_API CRXSkinMenubarItem : public CRXSkinItem
{
public:
	CRXSkinMenubarItem();
	~CRXSkinMenubarItem();
	virtual VOID  OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual CRXSkinItem* HitTest(POINT pt);
	void SetName(const TCHAR* pszName) { m_strName = pszName; }
	const TCHAR* GetName() { return m_strName; }
	void  SetMenubar(CRXSkinMenubar*poBar) { m_poMenubar = poBar; }
	CRXSkinMenu& GetMenu() { return m_oMenu; }
	CRXSkinMenuItem*    AppendMenu(EMenuItemType enType,
		const UINT32 dwID,
		const TCHAR* pszName = _T(""),
		const TCHAR* pszImageFile = _T(""));
	CRXSkinMenuItem*    AppendMenuEx(EMenuItemType enType,
		const UINT32 dwID,
		const TCHAR* pszName = _T(""),
		const UINT32 dwImageID = 0);
	void PopupMenu();
	void   SetChecked(const UINT32& dwID, const bool& bChecked);
private:
	CRXSkinMenubar* m_poMenubar;
	CRXSkinString   m_strName;
	CRXSkinMenu     m_oMenu;
};


class RXSKIN_API CRXSkinMenubar : public CRXControl
{
public:
	
	typedef vector<CRXSkinMenubarItem*>			CRXSkinMenubarItemVec;
	typedef CRXSkinMenubarItemVec::iterator		CRXSkinMenubarItemVecItr;
	CRXSkinMenubar();
	virtual ~CRXSkinMenubar();
	BOOL    Create(const UINT32 dwID, CRXSkinRect rcClient);
	virtual VOID OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual VOID ReleaseControl();
	virtual BOOL PtInRect(POINT pt);
	virtual BOOL OnClick(POINT pt);
	virtual BOOL OnMouseMove(POINT pt);
	virtual BOOL OnLButtonDown(POINT pt);
	virtual BOOL OnLButtonUp(POINT pt);
	virtual BOOL OnRButtonDown(POINT pt);
	virtual BOOL OnRButtonUp(POINT pt);
	virtual BOOL OnRButtonDblClk(POINT pt);
	virtual BOOL OnSize(const UINT32 dwX, const UINT32 dwY);
	virtual VOID SetVisible(const BOOL bIsVisible);
	virtual VOID Enable(const BOOL bEnabled);

	UINT32  GetCount();

	void SetItemNormalTextColor(const COLORREF clr) { m_clrItemNormalTxt = clr; }
	COLORREF GetItemNormalTextColor() { return m_clrItemNormalTxt; }
	void SetItemMouseOnTextColor(const COLORREF clr) { m_clrItemMouseOnTxt = clr; }
	COLORREF GetItemMouseOnTextColor() { return m_clrItemMouseOnTxt; }
	void SetItemPressedTextColor(const COLORREF clr) { m_clrItemPressedTxt = clr; }
	COLORREF GetItemPressedTextColor() { return m_clrItemPressedTxt; }

	void SetItemMouseOnBkColor(const COLORREF clr) { m_clrItemMouseOnBk = clr; }
	COLORREF GetItemMouseOnBkColor() { return m_clrItemMouseOnBk; }
	void SetItemPressedBkColor(const COLORREF clr) { m_clrItemPressedBk = clr; }
	COLORREF GetItemPressedBkColor() { return m_clrItemPressedBk; }
	void SetBkFrom(const COLORREF& clr) { m_clrFrom = clr; };
	void SetBkMiddle(const COLORREF& clr) { m_clrMiddle = clr; };
	void SetBkTo(const COLORREF& clr) { m_clrTo = clr; };


	CRXSkinMenubarItem*   AddSubMenu(const UINT32& dwID, const  TCHAR* pszName, CRXSkinRect rcItem);

	void   SetChecked(const UINT32& dwID, const bool& bChecked);
private:
	void   _DeleteAll();
	CRXSkinMenubarItem* _HitTest(POINT pt);
private:
	CRXSkinMenubarItemVec   m_vecItem;
	CRXSkinMenubarItem* m_poMouseOn;
	CRXSkinMenubarItem* m_poPressed;
	COLORREF  m_clrFrom, m_clrMiddle,m_clrTo;

	COLORREF        m_clrItemNormalTxt;
	COLORREF        m_clrItemMouseOnTxt;
	COLORREF        m_clrItemPressedTxt;

	COLORREF        m_clrItemMouseOnBk;
	COLORREF        m_clrItemPressedBk;
};


