#include "pch.h"
#include "main/rxskinfolderselect.h"
#include "common\rxskinservice.h"
#include "common\rxskinmemdc.h"
#include "fundation\rxstring.h"
#include "common\rxskindefines.h"
#include "fundation\rxutil.h"
#include "fundation\rxstring.h"
#include "common/rxskinresourcemgr.h"
#include <ShlObj.h>
#include <ShlObj_core.h>
#include <Shlwapi.h>
#include <shtypes.h>
#include "rxskinres.h"
#define ID_TREE_FOLDER         2002
#define ID_BUTTON_OK           2001

CRXSkinFolderSelect::CRXSkinFolderSelect()
{

}

CRXSkinFolderSelect::~CRXSkinFolderSelect()
{

}

VOID CRXSkinFolderSelect::OnDraw(SkSurface* poSurface, HDC hDC, CRXSkinRect rcItem)
{
	CRXSkinDialog::OnPaintBk(hDC);
}

BOOL CRXSkinFolderSelect::OnCreate(LPCREATESTRUCT lpCreate)
{
	CRXSkinDialog::OnCreate(lpCreate);
	::SetWindowLong(GetHWnd(), GWL_EXSTYLE, GetWindowLong(GetHWnd(), GWL_EXSTYLE)
		& ~WS_EX_APPWINDOW | WS_EX_TOOLWINDOW);
	::SetWindowPos(GetHWnd(), NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
	TCHAR* pszResource = MAKEINTRESOURCE(IDR_XML_FOLDER_SELECT);
	BOOL bResult = ReadConfigFromResource(pszResource);
	if (bResult == FALSE)
	{
		ReadConfig(_T("configs/folder_select.xml"));
	}
	tinyxml2::XMLDocument* poDoc = CRXSkinResourceMgr::Instance()->GetXml(pszResource);
	if (NULL == poDoc)
	{
		poDoc = CRXSkinResourceMgr::Instance()->LoadXmlFromResource(_T("configs/folder_select.xml"));
	}
	if (poDoc)
	{
		tinyxml2::XMLElement* poRoot = poDoc->RootElement();
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
		delete poDoc;
	}

	m_poFolderTree = (CRXSkinTree*)GetControlMgr().FindControl(ID_TREE_FOLDER);
	m_poFolderTree->SetListener(this);

	CRXSkinImageList* poImageList = CRXSkinService::Instance()->GetImageMgr().FindImageList("self");
	m_poFolderTree->SetImageList(poImageList);
	m_poFolderTree->InitRootItem();
	_InitTree();

	CenterWindow();
	return TRUE;
}

BOOL CRXSkinFolderSelect::OnLButtonDown(UINT nFlags, POINT point)
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
	return CRXSkinDialog::OnLButtonDown(nFlags, point);
}

BOOL CRXSkinFolderSelect::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case ID_BUTTON_OK:
	{
		m_nModelRet = IDOK;
		OnClickedClose();
	}
	break;
	default:
		break;
	}
	return CRXSkinDialog::OnCommand(wParam, lParam);
}

LRESULT CRXSkinFolderSelect::OnHandleMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	return CRXSkinDialog::OnHandleMessage(message, wParam, lParam);
}

void CRXSkinFolderSelect::_InitTree()
{
	LPITEMIDLIST pidl;
	if (FAILED(SHGetSpecialFolderLocation(NULL, CSIDL_DRIVES, &pidl)))
	{
		int32_t error = GetLastError();
		LOG_INFO("SHGetSpecialFolderLocation:%d", error);
		return;
	}
	TCHAR szPath[MAX_PATH] = {0};
	SHGetPathFromIDList(pidl, szPath);
	uint32_t dwParentID = 0;
	SHFILEINFO sfi;
	if (SHGetFileInfo((LPCTSTR)pidl, 0, &sfi, sizeof(sfi),
		SHGFI_PIDL | SHGFI_DISPLAYNAME | SHGFI_ICON))
	{
		dwParentID = m_poFolderTree->AddItem(ID_TREE_ROOT, sfi.szDisplayName, sfi.szDisplayName, szPath, sfi.iIcon);
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
			TCHAR szFullName[MAX_PATH] = {0};
			_RXTStrcpy(szFullName, pDriveName);
			if (SHGetFileInfo((LPCTSTR)szFullName, 0, &sfi, sizeof(sfi),
				SHGFI_DISPLAYNAME | SHGFI_ICON))
			{	
				m_poFolderTree->AddItem(dwParentID, sfi.szDisplayName, sfi.szDisplayName, szFullName, sfi.iIcon);
			}
			pDriveName += lstrlen(pDriveName) + 1;
		}
	}
	m_poFolderTree->ExpandItem(dwParentID);
}

void CRXSkinFolderSelect::OnTreeItemRClick(CRXSkinTree* poTree, CRXSkinTreeItem* poItem)
{

}

void CRXSkinFolderSelect::OnTreeItemSelected(CRXSkinTree* poTree, CRXSkinTreeItem* poItem)
{
	if (poItem->IsOpened())
	{
		poTree->CollaspseItem((uint32_t)poItem->GetID());
	}
	else
	{
		if (poItem->IsEmpty())
		{
			std::vector<tstring> vecChild;
			_GetCurChildDirectory(poItem->GetExtern(), vecChild);
			for (uint32_t t=0;t<(uint32_t)vecChild.size();t++)
			{
				SHFILEINFO sfi;
				if (SHGetFileInfo((LPCTSTR)vecChild[t].c_str(), 0, &sfi, sizeof(sfi),
					SHGFI_DISPLAYNAME | SHGFI_ICON))
				{
					tstring strPath = vecChild[t].c_str();
					if (strPath.size()>=0)
					{
						if (strPath[strPath.size()-1]!=_T('\\'))
						{
							strPath += _T("\\");
						}
					}
					m_poFolderTree->AddItem((uint32_t)poItem->GetID(), sfi.szDisplayName, sfi.szDisplayName, strPath.c_str(), sfi.iIcon);
				}
			}
		}
		poTree->ExpandItem((uint32_t)poItem->GetID());
	}
	m_strCurPath = poItem->GetExtern();
}

void CRXSkinFolderSelect::_GetCurChildDirectory(const TCHAR* lpPath, std::vector<tstring>& fileList)
{
	TCHAR szFind[MAX_PATH] = { 0 };
	WIN32_FIND_DATA FindFileData;
	_RXTStrcpy(szFind, lpPath);
	_RXTStrcat(szFind, _T("*.*"));
	HANDLE hFind = ::FindFirstFile(szFind, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)  return;
	while (true)
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (FindFileData.cFileName[0] != _T('.'))
			{
				TCHAR szFile[MAX_PATH] = { 0 };
				_RXTStrcpy(szFile, lpPath);
				
				_RXTStrcat(szFile, FindFileData.cFileName);
				fileList.push_back(tstring(szFile));
			}
		}
		if (!FindNextFile(hFind, &FindFileData))  break;
	}
	FindClose(hFind);
}