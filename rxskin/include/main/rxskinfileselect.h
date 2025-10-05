#pragma once
#include "rxskindialog.h"
#include "common/rxskinutil.h"
#include "rxfile.h"
#include "rxdir.h"
class CRXSkinFileSelect : public CRXSkinDialog
{
public:

	struct SHistoryDir
	{
		TCHAR  szDisplayName[MAX_PATH];
		TCHAR  szFullPath[MAX_PATH];
		SHistoryDir()
		{
			memset(szFullPath, 0, MAX_PATH * sizeof(TCHAR));
			memset(szDisplayName, 0, MAX_PATH * sizeof(TCHAR));
		}

		SHistoryDir(const SHistoryDir& oDir)
		{
			memset(szFullPath, 0, MAX_PATH * sizeof(TCHAR));
			memset(szDisplayName, 0, MAX_PATH * sizeof(TCHAR));
			_RXTStrcpy(szDisplayName, oDir.szDisplayName);
			_RXTStrcpy(szFullPath, oDir.szFullPath);
		}
	};

	CRXSkinFileSelect(void);
	~CRXSkinFileSelect(void);
	virtual VOID OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem);
	virtual BOOL OnCreate(LPCREATESTRUCT lpCreate);
	virtual BOOL OnLButtonDown(UINT nFlags, POINT point);

	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnHandleMessage(UINT message, WPARAM wParam, LPARAM lParam);
	vector<CRXSkinFileSelect::SHistoryDir*>& GetAllHistory() { return m_vecHistory; }
	CRXSkinFileSelect::SHistoryDir* GetCurDir();

	void    AddHistory(const TCHAR* pszDisplayName, const TCHAR* pszFullPath);
	CRXSkinFileSelect::SHistoryDir* PopLastHistory();
	virtual void OnClickedClose();
	virtual VOID Destroy();
	void    ReloadLocalComboBox();
	void    RefreshLocal();
	CRXSkinListCtrl* GetListFiles() { return m_poLstFiles; }
	void	ChangeLocalDir(const TCHAR* pszPath);
	void    SetType(const EFileSelectType& enType) { m_enType = enType; }
	tstring GetResult() { return m_strResult; }
	void    SetDefaultPath(const TCHAR* pszPath) { m_strDefaultPath = pszPath; };
private:
	CRXSkinComboBox* m_poComFiles;
	CRXSkinListCtrl* m_poLstFiles;
	CRXImageButton* m_btnBack;
	CRXImageButton* m_btnRefresh;
	CRXImageButton* m_btnNew;
	CRXImageButton* m_btnUpward;
	tstring        m_strResult;
	tstring         m_strDefaultPath;
	EFileSelectType m_enType;
	CRXSkinRect  m_rcStatus;
	COLORREF m_clrStatusBorder;
	COLORREF m_clrStatusBK;
	SIZE m_szRound;
	vector<CRXSkinFileSelect::SHistoryDir*>  m_vecHistory;
	uint32_t m_dwCurPos;
private:
	void    _DeleteAllHistory();

	void    _DeleteOldHistory();
};
