#pragma once
#include "rxskindialog.h"
#include "common/rxskinutil.h"
#include "rxfile.h"
#include "rxdir.h"
class CRXSkinFolderSelect : public CRXSkinDialog ,public IRXSkinTreeListener
{
public:
	CRXSkinFolderSelect();
	~CRXSkinFolderSelect();
	virtual VOID OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual BOOL OnCreate(LPCREATESTRUCT lpCreate);
	virtual BOOL OnLButtonDown(UINT nFlags, POINT point);
	virtual void OnTreeItemSelected(CRXSkinTree* poTree, CRXSkinTreeItem* poItem);
	virtual void OnTreeItemRClick(CRXSkinTree* poTree, CRXSkinTreeItem* poItem);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnHandleMessage(UINT message, WPARAM wParam, LPARAM lParam);
	tstring GetCurPath() { return m_strCurPath; }
private:
	tstring        m_strResult;
	tstring         m_strDefaultPath;
	EFileSelectType m_enType;
	CRXSkinRect  m_rcStatus;
	COLORREF m_clrStatusBorder;
	COLORREF m_clrStatusBK;
	SIZE m_szRound;
	tstring m_strCurPath;
	CRXSkinTree* m_poFolderTree;
private:
	void  _InitTree();
	void  _GetCurChildDirectory(const TCHAR* lpPath, std::vector<tstring>& fileList);
};