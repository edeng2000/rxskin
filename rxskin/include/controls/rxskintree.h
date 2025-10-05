#pragma once
#include "rxskin.h"
#include "rxcontrol.h"
#include "rxskinitem.h"
#include "common/rxskindefines.h"
#include "common/rxskinimagelist.h"
#include <map>
using namespace std;
#define ID_TREE_ROOT               0
class CRXSkinTree;

class RXSKIN_API  CRXSkinTreeItem : public CRXSkinItem
{
public:
	CRXSkinTreeItem();
	~CRXSkinTreeItem();
	virtual VOID  OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual int32_t OnDrawEx(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual CRXSkinItem* HitTest(POINT pt);
	void    SetText(const TCHAR* pszTxt) { m_strText = pszTxt; }
	void    SetTooltip(const TCHAR* pszTip) { m_strToolTip = pszTip; }
	const TCHAR* GetToolTip() {
		return m_strToolTip.c_str();
	}
	virtual void Invalidate();
	void    SetExtern(const TCHAR* pszExtern) { m_strExtern = pszExtern; }
	const TCHAR* GetExtern() {
		return m_strExtern.c_str();
	}

	const TCHAR* GetText()
	{
		return m_strText.c_str();
	}
	void SetOwner(CRXSkinTree* poOwner) { m_poOwner = poOwner; }
	CRXSkinTree* GetOwner() { return m_poOwner; }

	void SetParentItem(CRXSkinTreeItem* poItem) { m_poParentItem = poItem; };
	CRXSkinTreeItem* GetParentItem() { return m_poParentItem; }

	void SetPreItem(CRXSkinTreeItem* poItem) { m_poPreItem = poItem; };
	CRXSkinTreeItem* GetPreItem() { return m_poPreItem; }

	void SetNextItem(CRXSkinTreeItem* poItem) { m_poNextItem = poItem; };
	CRXSkinTreeItem* GetNextItem() { return m_poNextItem; }
	void AddChildItem(CRXSkinTreeItem* poItem);
	CRXSkinTreeItem* FindChildItem(const uint32_t& dwID);
	CRXSkinTreeItem* RemoveChildItem(const uint32_t& dwID);
	bool IsOpened() { return m_bIsOpened; }
	void SetIsOpened(const bool& bIsOpened);
	uint32_t GetImageID() { return m_dwImageID; }
	VOID SetImageID(const uint32_t& dwImageID) { m_dwImageID = dwImageID; }

	bool IsRoot() { return m_bIsRoot; }
	void SetIsRoot(const bool& bIsRoot) { m_bIsRoot = bIsRoot; }

	uint32_t GetItemHeight();
	bool IsEmpty() { return m_vecChildItem.size() == 0; }
	int32_t GetBottom();
private:
	tstring m_strText;
	tstring m_strToolTip;
	tstring m_strExtern;
	CRXSkinTree* m_poOwner;
	CRXSkinTreeItem* m_poParentItem;
	CRXSkinTreeItem* m_poPreItem;
	CRXSkinTreeItem* m_poNextItem;
	vector<CRXSkinTreeItem*> m_vecChildItem;

	bool  m_bIsOpened;
	uint32_t m_dwImageID;

	bool  m_bIsRoot;
};

class IRXSkinTreeListener
{
public: 
	virtual void OnTreeItemSelected(CRXSkinTree* poTree, CRXSkinTreeItem* poItem) = 0;
	virtual void OnTreeItemRClick(CRXSkinTree* poTree, CRXSkinTreeItem* poItem) = 0;
};


class RXSKIN_API CRXSkinTree : public CRXControl
{
public:

	typedef  map<UINT32, CRXSkinTreeItem*>      CRXSkinTreeItemMap;
	typedef  CRXSkinTreeItemMap::iterator       CRXSkinTreeItemMapItr;

	CRXSkinTree(void);
	~CRXSkinTree(void);
	virtual VOID OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual VOID ReleaseControl();
	virtual BOOL PtInRect(POINT pt);
	virtual BOOL OnClick(POINT pt);
	virtual BOOL OnMouseMove(POINT pt);
	virtual BOOL OnLButtonDown(POINT pt);
	virtual BOOL OnLButtonUp(POINT pt);
	virtual BOOL OnRButtonDown(POINT pt) { return FALSE; };
	virtual BOOL OnRButtonUp(POINT pt);
	virtual BOOL OnRButtonDblClk(POINT pt) { return FALSE; }
	virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, POINT pt);
	virtual BOOL OnSize(const UINT32 dwX, const UINT32 dwY);
	virtual VOID SetRect(const CRXSkinRect rcItem);
	virtual VOID OnScrollBarChanged(EScrollType enType, INT32 nPercent);
	
	void    SetBkColor(const COLORREF& clr) { m_clrBK = clr; }
	void    SetTextColor(const COLORREF& clr) { m_clrText = clr; }
	COLORREF GetTextColor() { return m_clrText; }
	COLORREF GetBkColor() { return m_clrBK; }

	void    SetBorderColor(const COLORREF& clr) { m_clrBorder = clr; }
	COLORREF GetBorderColor() { return m_clrBorder; }

	uint32_t    AddItem(const uint32_t& dwParentID,
		const TCHAR* pszTxt,
		const TCHAR* pszToolTip = _T(""), 
		const TCHAR* pszExtern = _T(""),
		const uint32_t& dwImageID = 0);
	void    RemoveItem(const UINT32& dwID);

	CRXSkinTreeItem* FindItem(const uint32_t& dwItemID);

	void    SetListener(IRXSkinTreeListener* poListerner) { m_poListener = poListerner; }
	CRXSkinTreeItem* GetSelectedItem() { return m_poPressed; }
	void    SetImageList(CRXSkinImageList* poImageList) { m_poImageList = poImageList; }
	CRXSkinImageList* GetImageList() { return m_poImageList; }
	void    InitRootItem();
	void    ExpandItem(const uint32_t& dwItemID);
	void    CollaspseItem(const uint32_t& dwItemID);
	void    ModifyTotalHeight(const int32_t& nTop, const int32_t& nBottom);
	void    ModifyTotalWidth(const int32_t& nLeft, const int32_t& nRight);
	int32_t GetTotalHeight() { return m_nMaxY - m_nMinY; }
	int32_t GetTotalWidth() { return m_nMaxX - m_nMinX; }
protected:
	COLORREF m_clrBorder;
	COLORREF m_clrBK;
	COLORREF m_clrText;
	CRXSkinTreeItemMap    m_mapItem;
	CRXSkinTreeItem* m_poMouseOn;
	CRXSkinTreeItem* m_poPressed;
	CRXSkinTreeItem* m_poSelected;
	IRXSkinTreeListener* m_poListener;
	CRXSkinImageList* m_poImageList;
	uint32_t m_dwCurItemID;
	CRXSkinTreeItem* m_poRootItem;
	int32_t m_nMinY;
	int32_t m_nMaxY;

	int32_t m_nMinX;
	int32_t m_nMaxX;
	CRXSkinScrollbar    m_oScrollbarVer;
	CRXSkinScrollbar    m_oScrollbarHov;

	INT32     m_nTopOffSet;
	INT32     m_nLeftOffSet;
private:
	void    _DeleteAll();
	CRXSkinTreeItem* _HitTest(POINT pt);
};
