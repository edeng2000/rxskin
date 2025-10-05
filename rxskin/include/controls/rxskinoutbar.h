#pragma once
#include "rxskin.h"
#include "rxskinitem.h"
#include "rxcontrol.h"
#include <map>
#include "tchar.h"
using namespace std;

class IRXSkinOutbarListener
{
public:
	virtual VOID   OnItemSelected(const UINT32 dwItemID) = 0;
	virtual VOID   OnFolderSelected(const UINT32 dwFolderID) = 0;
};

class RXSKIN_API CRXSkinOutbarItem : public CRXSkinItem
{
public:
	CRXSkinOutbarItem();
	~CRXSkinOutbarItem();

	VOID          SetImageFile(const TCHAR* pszImageFile);

	VOID          SetName(const TCHAR* pszName);
	CRXSkinString GetName();

	virtual VOID  OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual CRXSkinItem* HitTest(POINT pt);
	virtual CRXSkinString GetClassType();

	VOID          SetSelected(const BOOL bSelected);
	BOOL          IsSelected();
private:
	CRXSkinString       m_strImageFile;
	CRXSkinString       m_strName;
	IRXImage*     m_poImage;
	BOOL          m_bSelected;
};


class RXSKIN_API CRXSkinOutbarFolder:public CRXSkinItem
{
public:
	typedef map<UINT32,CRXSkinOutbarItem*>    COutbarItemMap;
	typedef COutbarItemMap::iterator          COutbarItemMapItr;

	CRXSkinOutbarFolder();
	~CRXSkinOutbarFolder();

	VOID           SetGroupName(const TCHAR* pszGroupName);
	CRXSkinString  GetGroupName();

	BOOL           AddItem(CRXSkinOutbarItem* poItem);
	CRXSkinOutbarItem*   RemoveItem(const UINT32 dwItemID);
	CRXSkinOutbarItem*   FindItem(const UINT32 dwItemID);

	VOID           SetOpened(const BOOL bOpened);
	BOOL           GetOpened();

	UINT32         GetTotalHeight();

	virtual VOID   OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual CRXSkinItem* HitTest(POINT pt);
	virtual CRXSkinString GetClassType();
	COutbarItemMap& GetItemMap();
private:
	VOID           _DeleteAll();
private:
	CRXSkinString  m_strGroupName;
	COutbarItemMap m_oOutbarItemMap;
	BOOL           m_bOpen;
	UINT32         m_nTotalHeight;
};


class RXSKIN_API CRXSkinOutbar : public CRXControl
{
public:
	typedef map<UINT32,CRXSkinOutbarFolder*>          CRXSkinOutbarFolderMap;
	typedef CRXSkinOutbarFolderMap::iterator		  CRXSkinOutbarFolderMapItr;
	CRXSkinOutbar(void);
	~CRXSkinOutbar(void);

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
	virtual BOOL OnSize(const UINT32 dwX,const UINT32 dwY);
	virtual CRXSkinRect GetInvalidateRect();
	virtual VOID OnMouseLeave();

	BOOL               AddFolder(const UINT32 dwFolderID,const TCHAR* pszFoldername);

	CRXSkinOutbarFolder*      FindFolder(const UINT32 dwFolderID);
	CRXSkinOutbarFolder*      RemoveFolder(const UINT32 dwFolderID);

	BOOL			   AddItem(const UINT32 dwFolderID,
								const UINT32 dwUserID,
								const TCHAR* pszName,
								const TCHAR* pszImagePath);

	CRXSkinOutbarItem*       FindItem(const UINT32 dwUserID);
	CRXSkinOutbarItem*       RemoveItem(const UINT32 dwUserID);

	VOID               SelectItem(const UINT32 dwUserID);
	VOID               SelectFolder(const UINT32 dwFolderID);
	VOID               SetListen(IRXSkinOutbarListener* poOutbarListener);
private:
	VOID               _DeleteAll();
	CRXSkinItem*       _HitTest(POINT pt);
private:
	CRXSkinOutbarFolderMap    m_oOutbarFolderMap;
	CRXSkinItem*       m_poMouseOn;
	CRXSkinItem*       m_poPressed;
	CRXSkinOutbarFolder*      m_poSelectFolder;
	IRXSkinOutbarListener*   m_poOutbarListener;
};
