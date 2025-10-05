#include "pch.h"
#include "main/rxskinfileselect.h"

#include "main\rxskinnewfolder.h"
#include "common\rxskinservice.h"
#include "common\rxskinmemdc.h"
#include "fundation\rxstring.h"
#include "common\rxskindefines.h"
#include "fundation\rxutil.h"
#include "fundation\rxstring.h"
#include "common/rxskinservice.h"
tstring FILETIMEToLocalDateEx(FILETIME ft) {
	SYSTEMTIME stUTC, stLocal;

	// 将FILETIME转换为UTC时间
	if (FileTimeToSystemTime(&ft, &stUTC) == 0) {
		return _T("");
	}

	// 转换UTC时间为本地时间
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

	TCHAR szDate[MAX_PATH] = { 0 };
	_RXTsprintf(szDate, _T("%04d-%02d-%02d %02d:%02d:%02d"),
		stLocal.wYear,
		stLocal.wMonth,
		stLocal.wDay,
		stLocal.wHour,
		stLocal.wMinute,
		stLocal.wSecond);
	// 输出本地日期
	return szDate;
}
uint64_t FILETIMEToIntValueEx(FILETIME ft)
{
	ULARGE_INTEGER uliTime;
	uliTime.LowPart = ft.dwLowDateTime;
	uliTime.HighPart = ft.dwHighDateTime;
	uliTime.QuadPart /= 10000;
	return uliTime.QuadPart;
}
#define ID_PICTURE_TIP             4000

CRXSkinFileSelect::CRXSkinFileSelect(void)
{
	m_enType = RXSKIN_FILE_SELECT_DIRECTORY;
	m_dwCurPos = 0;
}

CRXSkinFileSelect::~CRXSkinFileSelect(void)
{
}


VOID CRXSkinFileSelect::Destroy()
{
	CRXSkinDialog::Destroy();
	CRXSkinService::Instance()->GetPreMessages().Empty();
}

BOOL CRXSkinFileSelect::OnCreate(LPCREATESTRUCT lpCreate)
{
	CRXSkinDialog::OnCreate(lpCreate);
	::SetWindowLong(GetHWnd(), GWL_EXSTYLE, GetWindowLong(GetHWnd(), GWL_EXSTYLE)
		& ~WS_EX_APPWINDOW | WS_EX_TOOLWINDOW);
	::SetWindowPos(GetHWnd(), NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
	CRXSkinString strPath = CRXSkinService::Instance()->GetResourceRootPath().c_str();
	strPath += _T("configs/file_select.xml");
	ReadConfig(strPath.GetData());

	FILE* hFile = _tfopen(strPath.GetData(), _T("rb"));
	if (hFile)
	{
		tinyxml2::XMLDocument oDoc;
		if (oDoc.LoadFile(hFile) == tinyxml2::XML_SUCCESS)
		{
			tinyxml2::XMLElement* poRoot = oDoc.RootElement();
			if (poRoot)
			{
				tinyxml2::XMLElement* poBackground = poRoot->FirstChildElement("Background");
				if (poBackground)
				{
					tinyxml2::XMLElement* poStatus = poBackground->FirstChildElement("Status");
					if (poStatus)
					{
						const char* pszValue = poStatus->Attribute("area");
						if (pszValue)
						{
							m_rcStatus = RXSkinGetRect(pszValue);
						}
						pszValue = poStatus->Attribute("bk");
						if (pszValue)
						{
							m_clrStatusBK = GetColor(pszValue);
						}
						pszValue = poStatus->Attribute("border");
						if (pszValue)
						{
							m_clrStatusBorder = GetColor(pszValue);
						}
						pszValue = poStatus->Attribute("round");
						if (pszValue)
						{
							string strValue = pszValue;
							INT32 nX = -1;
							INT32 nY = -1;
							INT32 nPos = (INT32)strValue.find(",");
							if (nPos >= 0)
							{
								string strX = strValue.substr(0, nPos);
								nX = atoi(strX.c_str());
								string strY = strValue.substr((size_t)nPos + 1, strValue.size() - nPos - 1);
								nY = atoi(strY.c_str());
							}
							if (nX > 0 && nY > 0)
							{
								m_szRound.cx = nX;
								m_szRound.cy = nY;
							}

						}
					}
				}
			}
		}
		fclose(hFile);
	}


	CenterWindow();
	m_poComFiles = (CRXSkinComboBox*)GetControlMgr().FindControl(ID_COMBO_FILE_SELECT);
	if (m_poComFiles == NULL)
	{
		return FALSE;
	}
	m_poComFiles->SetMaxHeight(200);
	m_poComFiles->SetComboboxType(EComboBoxType::COMBOBOX_DROPLIST_FILE);
	m_poComFiles->SetImageList(CRXSkinService::Instance()->GetImageMgr().FindImageList("self"));
	m_poLstFiles = (CRXSkinListCtrl*)GetControlMgr().FindControl(ID_LIST_CTRL_FILE_SELECT);
	m_poLstFiles->SetImageList(CRXSkinService::Instance()->GetImageMgr().FindImageList("self"));
	m_poLstFiles->SetCanDragged(false);
	m_poLstFiles->SetCanSort(true);
	m_poLstFiles->SetGroupNum(2);
	m_poLstFiles->SetColumnCanResize(0, true);
	m_poLstFiles->SetColumnCanResize(1, true);
	m_poLstFiles->SetColumnCanResize(2, true);
	ReloadLocalComboBox();
	m_btnBack = (CRXImageButton*)GetControlMgr().FindControl(ID_BUTTON_FILE_SELECT_BACK);
	m_btnRefresh = (CRXImageButton*)GetControlMgr().FindControl(ID_BUTTON_FILE_SELECT_REFRESH);
	m_btnNew = (CRXImageButton*)GetControlMgr().FindControl(ID_BUTTON_FILE_SELECT_NEW);
	m_btnUpward = (CRXImageButton*)GetControlMgr().FindControl(ID_BUTTON_FILE_SELECT_UPWARD);
	if (m_strDefaultPath.size()<=0)
	{
		m_strDefaultPath = RXGetModulePath();
	}
	if (m_strDefaultPath.size()>0)
	{
		if (m_strDefaultPath[m_strDefaultPath.size()-1]!=_T('\\'))
		{
			m_strDefaultPath += _T('\\');
		}
	}
	TCHAR* szFullName = (TCHAR*)malloc(sizeof(TCHAR) * MAX_PATH);
	memset(szFullName, 0, sizeof(TCHAR) * MAX_PATH);
	_RXTStrcpy(szFullName, m_strDefaultPath.c_str());
	m_poComFiles->AddString(m_strDefaultPath.c_str(), -1, (LPARAM)szFullName);
	m_poComFiles->SelectString(m_strDefaultPath.c_str());

	ChangeLocalDir(m_strDefaultPath.c_str());
	AddHistory(m_strDefaultPath.c_str(), szFullName);
	return TRUE;
}

VOID CRXSkinFileSelect::OnClickedClose()
{
	Destroy();
	CRXSkinDialog::OnClickedClose();
}

VOID CRXSkinFileSelect::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	CRXSkinDialog::OnPaintBk(hDC);
	DrawRoundRect(hDC, m_clrStatusBorder, m_clrStatusBK,
		m_rcStatus.left,
		m_rcStatus.top, m_rcStatus.right, m_rcStatus.bottom,
		m_szRound.cx, m_szRound.cy);
}

BOOL CRXSkinFileSelect::OnLButtonDown(UINT nFlags, POINT point)
{
	BOOL bClicked = CRXSkinDialog::OnLButtonDown(nFlags, point);
	if (FALSE == bClicked)
	{
		CRXSkinRect rcCaption = GetCaptionRect();
		if (rcCaption.PtInRect(point))
		{
			::PostMessage(GetHWnd(), WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
			return TRUE;
		}
	}
	return bClicked;
}
BOOL CRXSkinFileSelect::OnCommand(WPARAM wParam, LPARAM lParam)
{
	BOOL bResult = CRXSkinDialog::OnCommand(wParam, lParam);
	if (FALSE == bResult)
	{
 		switch (wParam)
		{
		case IDOK:
		{
			m_nModelRet = IDOK;
			Destroy();
			return TRUE;
		}
		break;
		case IDCANCEL:
		{
			m_nModelRet = IDCANCEL;
			Destroy();
			return TRUE;
		}
		break;
		case IDABORT:
		{
			m_nModelRet = IDABORT;
			Destroy();
			return TRUE;
		}
		break;
		case IDRETRY:
		{
			m_nModelRet = IDRETRY;
			Destroy();
			return TRUE;
		}
		break;
		case IDIGNORE:
		{
			m_nModelRet = IDIGNORE;
			Destroy();
			return TRUE;
		}
		break;
		case IDNO:
		{
			m_nModelRet = IDNO;
			Destroy();
			return TRUE;
		}
		break;
		case IDYES:
		{
			m_nModelRet = IDYES;
			Destroy();
			return TRUE;
		}
		break;
		case ID_BUTTON_FILE_SELECT_BACK:
		{
			CRXSkinFileSelect::SHistoryDir* poHistory = PopLastHistory();
			if (poHistory)
			{
				ReloadLocalComboBox();
				TCHAR* szFullName = (TCHAR*)malloc(sizeof(TCHAR) * MAX_PATH);
				memset(szFullName, 0, sizeof(TCHAR) * MAX_PATH);
				_RXTStrcpy(szFullName, poHistory->szFullPath);
				m_poComFiles->AddString(poHistory->szDisplayName, -1, (LPARAM)szFullName);
				m_poComFiles->SelectString(poHistory->szDisplayName);
				ChangeLocalDir(szFullName);
				AddHistory(poHistory->szDisplayName, szFullName);
			}
	
		}
		break;
		case ID_BUTTON_FILE_SELECT_REFRESH:
		{
			RefreshLocal();
		}
		break;
		case ID_BUTTON_FILE_SELECT_UPWARD:
		{
			TCHAR* pszCurPath = (TCHAR*)m_poComFiles->GetCurSelData();
			if (pszCurPath)
			{
				tstring strCurDir = RXGetUpDirectory(pszCurPath);
				if (strCurDir.size() > 0)
				{
					ChangeLocalDir(strCurDir.c_str());
					ReloadLocalComboBox();


					SHFILEINFO sfi;
					if (SHGetFileInfo((LPCTSTR)strCurDir.c_str(), 0, &sfi, sizeof(sfi),
						SHGFI_DISPLAYNAME | SHGFI_ICON))
					{
						TCHAR* szFullName = (TCHAR*)malloc(sizeof(TCHAR) * MAX_PATH);
						memset(szFullName, 0, sizeof(TCHAR) * MAX_PATH);
						_RXTStrcpy(szFullName, strCurDir.c_str());
						m_poComFiles->AddString(strCurDir.c_str(), sfi.iIcon, (LPARAM)szFullName);
						m_poComFiles->SelectString(strCurDir.c_str());
						AddHistory(strCurDir.c_str(), strCurDir.c_str());
					}

				}
			}
		}
		break;
		case ID_BUTTON_FILE_SELECT_NEW:
		{			
			TCHAR* pszCurPath = (TCHAR*)m_poComFiles->GetCurSelData();
			if (pszCurPath)
			{
				CRXSkinNewFolder* poWnd = new CRXSkinNewFolder;
				poWnd->SetParentPath(pszCurPath);
				poWnd->Create(_T("RXSkinNewFolder"),
					_T("RXSkinNewFolderClass"),
					WS_POPUP | WS_VISIBLE | WS_EX_TOOLWINDOW,
					GetHWnd(),
					NULL);
				poWnd->ShowModal();
				RefreshLocal();
			}
		}
		break;
		case ID_BUTTON_OK:
		{
			SFileInfo* poFileInfo = (SFileInfo*)m_poLstFiles->GetCurFirstRowData();
			if (poFileInfo )
			{
				m_strResult = poFileInfo->szFullPath;		
			}
			else
			{
				TCHAR* szFullPath = (TCHAR*)m_poComFiles->GetCurSelData();
				if (szFullPath)
				{
					m_strResult = szFullPath;
				}
			}
			Destroy();
		}
		break;
		default:
			break;
		}
	}
	return bResult;
}

void  CRXSkinFileSelect::ReloadLocalComboBox()
{
	if (m_poComFiles)
	{
		m_poComFiles->ResetContent();
		LPITEMIDLIST pidl;
		if (FAILED(SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &pidl)))
		{
			return;
		}
		TCHAR* szPath = (TCHAR*)malloc(MAX_PATH * sizeof(TCHAR));
		memset(szPath, 0, sizeof(TCHAR) * MAX_PATH);
		SHGetPathFromIDList(pidl, szPath);

		LPSHELLFOLDER pDesktop;
		if (FAILED(SHGetDesktopFolder(&pDesktop)))
		{
			return;
		}

		SHFILEINFO sfi;
		if (SHGetFileInfo((LPCTSTR)pidl, 0, &sfi, sizeof(sfi),
			SHGFI_PIDL | SHGFI_DISPLAYNAME | SHGFI_ICON))
		{
			m_poComFiles->AddString(sfi.szDisplayName, sfi.iIcon, (LPARAM)szPath);
		}
		DWORD dwDiskCount = 0;
		DWORD dwBitMask = GetLogicalDrives();
		for (DWORD dw = 0; dw < 26; dw++)
		{
			if (dwBitMask & 1)
			{
				dwDiskCount++;
			}
			dwBitMask = dwBitMask >> 1;
		}

		DWORD dwDriveStrLen = 0;
		dwDriveStrLen = GetLogicalDriveStrings(0, NULL);
		TCHAR* szDriveName = new TCHAR[dwDriveStrLen];
		TCHAR* pDriveName = NULL;
		if (GetLogicalDriveStrings(dwDriveStrLen, szDriveName))
		{
			pDriveName = szDriveName;
			while (*pDriveName != NULL)
			{
				switch (GetDriveType(pDriveName))
				{
				case DRIVE_UNKNOWN:
					break;
				case DRIVE_NO_ROOT_DIR:
					break;
				case DRIVE_REMOVABLE:
					break;
				case DRIVE_FIXED:
					break;

				case DRIVE_REMOTE:
					break;
				case DRIVE_CDROM:
					break;
				case DRIVE_RAMDISK:
					break;
				default:
					break;
				}
				TCHAR* szFullName = (TCHAR*)malloc(sizeof(TCHAR) * MAX_PATH);
				memset(szFullName, 0, sizeof(TCHAR) * MAX_PATH);
				_RXTStrcpy(szFullName, pDriveName);
				if (SHGetFileInfo((LPCTSTR)szFullName, 0, &sfi, sizeof(sfi),
					SHGFI_DISPLAYNAME | SHGFI_ICON))
				{
					m_poComFiles->AddString(sfi.szDisplayName, sfi.iIcon, (LPARAM)szFullName);
				}
				pDriveName += lstrlen(pDriveName) + 1;
			}
		}
	}
}

void CRXSkinFileSelect::ChangeLocalDir(const TCHAR* pszPath)
{
	TCHAR* szFullePath = (TCHAR*)pszPath;
	GetListFiles()->ResetContent();
	vector<SFileInfo*> vecFiles;
	vector<SFileInfo*> vecDirs;
	RXGetCurDirAllFileName(szFullePath, vecFiles, vecDirs);
	
	for (uint32_t t = 0; t < (uint32_t)vecDirs.size(); t++)
	{
		uint32_t dwCurRow = GetListFiles()->AddRow(0);
		tstring strName = vecDirs[t]->szName;
		GetListFiles()->SetItemText(dwCurRow, 0, vecDirs[t]->szName, LIST_ITEM_TEXT);
		GetListFiles()->SetItemText(dwCurRow, 1, _T(""), LIST_ITEM_TEXT);
		m_poLstFiles->SetItemValueType(dwCurRow, 1, ERXSkinListItemValueType::LIST_CTRL_VALUE_TYPE_INT);
		m_poLstFiles->SetItemValueType(dwCurRow, 2, ERXSkinListItemValueType::LIST_CTRL_VALUE_TYPE_INT);
		tstring strTime = FILETIMEToLocalDateEx(vecDirs[t]->llTime);
		m_poLstFiles->SetItemIntValue(dwCurRow, 2, FILETIMEToIntValueEx(vecDirs[t]->llTime));
		m_poLstFiles->SetItemValueType(dwCurRow, 2, ERXSkinListItemValueType::LIST_CTRL_VALUE_TYPE_INT);
		GetListFiles()->SetItemText(dwCurRow, 2, strTime.c_str(), LIST_ITEM_TEXT);
		GetListFiles()->SetItemText(dwCurRow, 3, _T("文件夹"), LIST_ITEM_TEXT);
		INT32 iImageID = CRXSkinService::Instance()->GetDrawMgr().GetShellImageListIndex(vecDirs[t]->szFullPath);
		GetListFiles()->SetItemImage(dwCurRow, 0, iImageID);
		GetListFiles()->SetRowData(dwCurRow, (LPARAM)vecDirs[t]);
		GetListFiles()->SetItemTextStyle(dwCurRow, 3, DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_CENTER);
		GetListFiles()->SetItemTextStyle(dwCurRow, 1, DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_CENTER);

	}
	vecDirs.clear();
	if (m_enType == RXSKIN_FILE_SELECT_FILE)
	{
		for (uint32_t t = 0; t < (uint32_t)vecFiles.size(); t++)
		{
			uint32_t dwCurRow = GetListFiles()->AddRow(1);
			GetListFiles()->SetRowData(dwCurRow, (LPARAM)vecFiles[t]);
			tstring strName = vecFiles[t]->szName;
			GetListFiles()->SetItemText(dwCurRow, 0, vecFiles[t]->szName, LIST_ITEM_TEXT);
			uint64_t llSize = RXFileSize(vecFiles[t]->szFullPath);
			GetListFiles()->SetItemText(dwCurRow, 1, GetTextByValue(llSize).c_str(), LIST_ITEM_TEXT);
			tstring strTime = FILETIMEToLocalDateEx(vecFiles[t]->llTime);
			m_poLstFiles->SetItemIntValue(dwCurRow, 1, 0);
			m_poLstFiles->SetItemIntValue(dwCurRow, 2, 0);
			m_poLstFiles->SetItemValueType(dwCurRow, 2, ERXSkinListItemValueType::LIST_CTRL_VALUE_TYPE_INT);
			m_poLstFiles->SetItemValueType(dwCurRow, 1, ERXSkinListItemValueType::LIST_CTRL_VALUE_TYPE_INT);
			m_poLstFiles->SetItemIntValue(dwCurRow, 1, llSize);
			GetListFiles()->SetItemText(dwCurRow, 2, strTime.c_str(), LIST_ITEM_TEXT);
			m_poLstFiles->SetItemIntValue(dwCurRow, 2, FILETIMEToIntValueEx(vecFiles[t]->llTime));
			GetListFiles()->SetItemText(dwCurRow, 3, RXFileExtractExt(vecFiles[t]->szFullPath).c_str(), LIST_ITEM_TEXT);
			INT32 iImageID = CRXSkinService::Instance()->GetDrawMgr().GetShellImageListIndex(vecFiles[t]->szFullPath);
			GetListFiles()->SetItemImage(dwCurRow, 0, iImageID);
			GetListFiles()->SetItemTextStyle(dwCurRow, 3, DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_CENTER);
			GetListFiles()->SetItemTextStyle(dwCurRow, 1, DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_CENTER);

		}
		vecFiles.clear();
	}


	GetListFiles()->UpdateScrollBar();
	GetListFiles()->Finished();
}

LRESULT CRXSkinFileSelect::OnHandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT nResult = CRXSkinDialog::OnHandleMessage(message, wParam, lParam);
	switch (message)
	{
	case WM_RXSKIN_COMBOBOX_CHANGED:
	{	
		if (m_poComFiles)
		{
			tstring strTxt = m_poComFiles->GetText();
			TCHAR* pszPath = (TCHAR*)m_poComFiles->GetCurSelData();
			ChangeLocalDir(pszPath);
			AddHistory(strTxt.c_str(), pszPath);
		}

	}
	break;
	case WM_RXSKIN_LISTCTRL_DB_CLIEECKED:
	{
		uint32_t dwRow = (uint32_t)lParam;
		SFileInfo* poFileInfo = (SFileInfo*)m_poLstFiles->GetRowData(dwRow);
		if (poFileInfo && poFileInfo->bIsFile == FALSE)
		{
			tstring strPath = poFileInfo->szFullPath;
			strPath += _T("\\");
			ChangeLocalDir(strPath.c_str());
			ReloadLocalComboBox();
			TCHAR* szFullName = (TCHAR*)malloc(sizeof(TCHAR) * MAX_PATH);
			memset(szFullName, 0, sizeof(TCHAR) * MAX_PATH);
			_RXTStrcpy(szFullName, strPath.c_str());
			m_poComFiles->AddString(strPath.c_str(), -1, (LPARAM)szFullName);
			m_poComFiles->SelectString(strPath.c_str());
			AddHistory(strPath.c_str(), szFullName);
		}
	}
	break;

	}
	return nResult;
}
void CRXSkinFileSelect::RefreshLocal()
{
	tstring strTxt = m_poComFiles->GetText();
	TCHAR* pszPath = (TCHAR*)m_poComFiles->GetCurSelData();
	ChangeLocalDir(pszPath);
}

CRXSkinFileSelect::SHistoryDir* CRXSkinFileSelect::GetCurDir()
{
	if (m_vecHistory.size() < 1)
	{
		return NULL;
	}
	return (m_vecHistory[m_vecHistory.size() - 1]);
}

void CRXSkinFileSelect::AddHistory(const TCHAR* pszDisplayName, const TCHAR* pszFullPath)
{
	_DeleteOldHistory();
	if (m_vecHistory.size() > 0)
	{
		CRXSkinFileSelect::SHistoryDir* poLast = m_vecHistory[m_vecHistory.size() - 1];
		if (_RXTStrcmp(poLast->szFullPath, pszFullPath) == 0)
		{
			return;
		}
		if (m_dwCurPos < m_vecHistory.size())
		{
			poLast = m_vecHistory[m_dwCurPos];
			if (_RXTStrcmp(poLast->szFullPath, pszFullPath) == 0)
			{
				return;
			}
		}
	}
	CRXSkinFileSelect::SHistoryDir* poDir = new CRXSkinFileSelect::SHistoryDir;
	_RXTStrcpy(poDir->szDisplayName, pszDisplayName);
	_RXTStrcpy(poDir->szFullPath, pszFullPath);
	m_vecHistory.push_back(poDir);
	if (m_vecHistory.size() >= 1)
	{
		m_dwCurPos = (uint32_t)m_vecHistory.size() - 1;
	}
}

void  CRXSkinFileSelect::_DeleteAllHistory()
{
	for (uint32_t t = 0; t < (uint32_t)m_vecHistory.size(); t++)
	{
		delete m_vecHistory[t];
	}
	m_vecHistory.clear();
}

void CRXSkinFileSelect::_DeleteOldHistory()
{
	while (m_vecHistory.size() > 10)
	{
		vector<CRXSkinFileSelect::SHistoryDir*>::iterator itr = m_vecHistory.begin();
		CRXSkinFileSelect::SHistoryDir* poHistory = *itr;
		delete poHistory;
		m_vecHistory.erase(itr);
	}
}

CRXSkinFileSelect::SHistoryDir* CRXSkinFileSelect::PopLastHistory()
{
	if (m_dwCurPos < m_vecHistory.size())
	{
		if (m_dwCurPos > 0)
		{
			m_dwCurPos--;
		}
		return m_vecHistory[m_dwCurPos];
	}
	else
	{
		if (m_vecHistory.size() >= 2)
		{
			m_dwCurPos = (uint32_t)m_vecHistory.size() - 2;
			return m_vecHistory[m_dwCurPos];
		}
	}
	return NULL;
}