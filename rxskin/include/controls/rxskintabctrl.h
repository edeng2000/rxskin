#pragma once
#include "rxskin.h"
#include "rxskinitem.h"
#include "rxcontrol.h"
#include <map>
#include "tchar.h"
using namespace std;


class CRXSkinTabCtrl;
class RXSKIN_API CRXSkinTabItem : public CRXSkinItem
{
public:
	CRXSkinTabItem();
	~CRXSkinTabItem();
	virtual VOID  SetRect(CRXSkinRect rcItem);
	VOID        SetImage(const TCHAR* pszImage);
	VOID        SetTooltip(const TCHAR* pszTooltip);

	VOID        SetChild(CRXControl* poChild);
	CRXControl* GetChild();
	VOID        SetChildWnd(CRXSkinWnd* poChildWnd);
	CRXSkinWnd* GetChildWnd();

	TCHAR*      GetName();
	VOID        SetName(const TCHAR* pszName);
	TCHAR*      GetToolTip();

	SIZE        GetItemSize();

	VOID        SetTabCtrl(CRXSkinTabCtrl* poTabCtrl);

	virtual VOID        OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual CRXSkinItem* HitTest(POINT pt);

	void        SetSelectClose(const BOOL& bSelected);
	BOOL        IsSelectedClose() { return m_bSelectClose; }
	BOOL        PtInClose(POINT pt) {
		return m_rcClose.PtInRect(pt);
	}

	CRXControl* OnMouseOn(POINT pt);
	void        OnClicked(POINT pt);

	void        OnMouseLeave();
	void        AddTabButton(const uint32_t& dwCommandID,
		const TCHAR* pszToolTip,
		const TCHAR* pszNormal, 
		const TCHAR* pszHot,
		const TCHAR* pszDown,
		const RECT rc);
	void        Init();
	void		AddControl(CRXControl* poControl);
	CRXControl* FindControl(const uint32_t& dwID);
	CRXControl* HitTestEx(POINT pt);
	string     GetKey() { return m_strKey; }
	void        SetKey(const CHAR* pszKey) { m_strKey = pszKey; }
private:
	IRXImage*   m_poImage;
	CRXControl* m_poChild;
	CRXSkinWnd* m_poChildWnd;
	TCHAR       m_szName[256];
	TCHAR       m_szToolTip[256];
	CRXSkinTabCtrl* m_poTabCtrl;
	CRXSkinRect m_rcClose;
	BOOL        m_bSelectClose;
	CRXControl* m_poMouseOn;
	CRXControl* m_poPressed;
	CRXControlMgr m_oControlMgr;
	string     m_strKey;
};

class IWndTabCtrlListener
{
public:
	virtual VOID OnTabItemChanged(CRXSkinTabItem* poItem) = 0;
	virtual VOID OnTabItemClosed(CRXSkinTabItem* poItem) = 0;
	virtual VOID OnTabItemClicked(CRXSkinTabItem* poItem,const uint32_t& dwCommandID) = 0;
	virtual VOID OnTabItemMouseOn(CRXSkinTabItem* poItem) = 0;
	virtual VOID OnTabItemMouseLeave() = 0;
};


class RXSKIN_API  CRXSkinTabCtrl : public CRXControl
{
public:
	typedef map<uint64_t,CRXSkinTabItem*>    CRXSkinTabItemMap;
	typedef CRXSkinTabItemMap::iterator    CCRXSkinTabItemMapItr;

	CRXSkinTabCtrl(void);
	~CRXSkinTabCtrl(void);
	virtual VOID OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual VOID ReleaseControl();
	virtual BOOL PtInRect(POINT pt);
	virtual BOOL OnClick(POINT pt);
	virtual BOOL OnMouseMove(POINT pt);
	virtual BOOL OnLButtonDown(POINT pt);
	virtual BOOL OnLButtonUp(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonDown(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonUp(POINT pt);
	virtual BOOL OnRButtonDblClk(POINT pt) { return FALSE; }
	virtual BOOL OnSize(const UINT32 dwX,const UINT32 dwY);
	virtual CRXSkinRect GetInvalidateRect();
	virtual VOID OnMouseLeave();

	COLORREF     GetTabItemNormalFromBk();
	VOID         SetTabItemNormalFromBk(COLORREF clr);

	COLORREF     GetTabItemNormalToBk();
	void         SetTabItemNormalToBk(COLORREF clr);

	COLORREF     GetTabItemNormalText();
	VOID         SetTabItemNormalText(COLORREF clr);

	COLORREF     GetTabItemMouseOnFromBk();
	VOID         SetTabItemMouseOnFromBk(COLORREF clr);

	COLORREF     GetTabItemMouseOnToBk();
	VOID         SetTabItemMouseOnToBk(COLORREF clr);

	COLORREF     GetTabItemMouseOnText();
	VOID         SetTabItemMouseOnText(COLORREF clr);

	COLORREF     GetTabItemPressedFromBk();
	VOID         SetTabItemPressedFromBk(COLORREF clr);

	COLORREF     GetTabItemPressedToBk();
	VOID         SetTabItemPressedToBk(COLORREF clr);

	COLORREF     GetTabItemPressedText();
	VOID         SetTabItemPressedText(COLORREF clr);

	COLORREF     GetTabItemSelectedFromBk();
	VOID         SetTabItemSelectedFromBk(COLORREF clr);

	COLORREF     GetTabItemSelectedToBk();
	VOID         SetTabItemSelectedToBk(COLORREF clr);

	COLORREF     GetTabItemSelectedText();
	VOID         SetTabItemSelectedText(COLORREF clr);

	COLORREF     GetTabItemBorder();
	VOID         SetTabItemBorder(COLORREF clr);

	SIZE         GetTabCtrlRound();
	VOID         SetTabCtrlRound(SIZE sz);

	UINT32       GetTabCtrlItemHeight();
	VOID         SetTabCtrlItemHeight(UINT32 dwHeight);

	UINT32       GetTabItemOffSetX();
	VOID         SetTabItemOffSetX(UINT32 dwOffSetX);

	CRXSkinTabItem* AddTab(
						   const TCHAR* pszName,
						   const char*  pszKey,
						   const TCHAR* pszToolTip=_T(""),
						   const TCHAR* pszImageFile=_T(""),
						   CRXControl* poChild = NULL,
		CRXSkinWnd* poChildWnd = NULL);

	void            AddTabButton(const char* pszKey, const uint32_t& dwCommandID,
		const TCHAR* pszToolTip, const TCHAR* pszNormal, const TCHAR* pszHot, const TCHAR* pszDown,const RECT rc);

	CRXSkinTabItem* FindTab(const uint64_t dwID);
	CRXSkinTabItem* RemoveTab(const uint64_t dwID);
	CRXSkinTabItem* SelectTab(const uint64_t dwID);

	CRXSkinTabItem* FindTabByKey(const char* pszKey);
	CRXSkinTabItem* RemoveTabByKey(const char* pszKey);
	CRXSkinTabItem* SelectTabByKey(const char* pszKey);

	CRXSkinTabItemMap& GetAllItem() { return m_mapItems; }
	void            SetTabTooltip(const uint64_t dwID, const TCHAR* pszText);
	void            SetTabText(const uint64_t dwID, const TCHAR* pszText);
	VOID            SetListener(IWndTabCtrlListener* poListener);
	IWndTabCtrlListener* GetListener() { return m_poListener; }

	uint32_t        GetTabCount() { return m_mapItems.size(); }
	void            Clear() { _RemoveAll(); };
private:
	COLORREF      m_clrTabItemNormalFromBk;
	COLORREF      m_clrTabItemNormalToBk;
	COLORREF      m_clrTabItemNormalText;

	COLORREF      m_clrTabItemMouseOnFromBk;
	COLORREF      m_clrTabItemMouseOnToBk;
	COLORREF      m_clrTabItemMouseOnText;

	COLORREF      m_clrTabItemPressedFromBk;
	COLORREF      m_clrTabItemPressedToBk;
	COLORREF      m_clrTabItemPressedText;

	COLORREF      m_clrTabItemSelectedFromBk;
	COLORREF      m_clrTabItemSelectedToBk;
	COLORREF      m_clrTabItemSelectedText;

	COLORREF      m_clrTabItemBorder;

	UINT32        m_dwTabItemHeight;
	UINT32        m_dwTabItemOffSetX;
	SIZE          m_szTabCtrlRound;

	CRXSkinTabItemMap m_mapItems;
	CRXSkinTabItem*   m_poSelected;
	CRXSkinTabItem*   m_poMouseOn;
	CRXSkinTabItem*   m_poPressed;
	IWndTabCtrlListener* m_poListener;
	
	CRXSkinRect       m_rcInvalidate;

	UINT32            m_dwCurID;
private:
	VOID              _RemoveAll();
	CRXSkinTabItem*   _HitTest(POINT pt);
};
