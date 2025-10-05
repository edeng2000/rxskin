#include "pch.h"
#include "common\rxskinresourcemgr.h"
#include "main\rxskindialog.h"
#include "common\rxskinutil.h"
#include "common\rximagemodel.h"
#include <Windows.h>
#include <Windowsx.h>
#include <ObjBase.h>
#include "rxfile.h"
#include "rxdir.h"
RX_IMPLEMENT_SINGLETON(CRXSkinResourceMgr);
CRXSkinResourceMgr::CRXSkinResourceMgr(void)
{
	/*m_poUnzip = NULL;*/
}

CRXSkinResourceMgr::~CRXSkinResourceMgr(void)
{
}

tinyxml2::XMLDocument* CRXSkinResourceMgr::GetXml(const TCHAR* pszResoureName)
{
	tinyxml2::XMLDocument* poDoc = NULL;
	HRSRC hr = ::FindResource(NULL, pszResoureName, L"XML");
	if (NULL == hr)
	{
		int ierr = GetLastError();
		return poDoc;
	}

	ULONG nResSize = ::SizeofResource(NULL, hr); // Data size/length
	HGLOBAL hG = ::LoadResource(NULL, hr);
	if (NULL == hG || nResSize <= 0)
	{
		int ierr = GetLastError();
		return poDoc;
	}
	LPBYTE pData = (LPBYTE)LockResource(hG); // Data Ptr
	if (NULL == pData)
	{
		int ierr = GetLastError();
		return poDoc;
	}
	poDoc = new tinyxml2::XMLDocument;
	if (tinyxml2::XML_SUCCESS != poDoc->Parse((const char*)pData, nResSize))
	{
		int ierr = GetLastError();
		delete poDoc;
		return NULL;
	}
	return poDoc;
}
BYTE* CRXSkinResourceMgr::GetRTF(const TCHAR* pszResoureName, uint32_t& dwLen)
{
	dwLen = 0;
	HRSRC hr = ::FindResource(NULL, pszResoureName, L"RTF");
	if (NULL == hr)
	{
		int ierr = GetLastError();
		return NULL;
	}

	ULONG nResSize = ::SizeofResource(NULL, hr); // Data size/length
	HGLOBAL hG = ::LoadResource(NULL, hr);
	if (NULL == hG || nResSize <= 0)
	{
		int ierr = GetLastError();
		return NULL;
	}
	LPBYTE pData = (LPBYTE)LockResource(hG); // Data Ptr
	if (NULL == pData)
	{
		int ierr = GetLastError();
		return NULL;
	}
	dwLen = nResSize;
	return pData;
}
tstring CRXSkinResourceMgr::GetResource(const char* pszValue)
{
	return _RXTUTF82T(pszValue);
}

BYTE* CRXSkinResourceMgr::GetExe(const TCHAR* pszResoureName, uint32_t& dwLen)
{
	dwLen = 0;
	HRSRC hr = ::FindResource(NULL, pszResoureName, L"EXE");
	if (NULL == hr)
	{
		int ierr = GetLastError();
		return NULL;
	}

	ULONG nResSize = ::SizeofResource(NULL, hr); // Data size/length
	HGLOBAL hG = ::LoadResource(NULL, hr);
	if (NULL == hG || nResSize <= 0)
	{
		int ierr = GetLastError();
		return NULL;
	}
	LPBYTE pData = (LPBYTE)LockResource(hG); // Data Ptr
	if (NULL == pData)
	{
		int ierr = GetLastError();
		return NULL;
	}
	dwLen = nResSize;
	return pData;
}


BYTE* CRXSkinResourceMgr::GetPNG(const TCHAR* pszResoureName, uint32_t& dwLen)
{
	dwLen = 0;
	HRSRC hr = ::FindResource(NULL, pszResoureName, L"PNG");
	if (NULL == hr)
	{
		int ierr = GetLastError();
		return NULL;
	}

	ULONG nResSize = ::SizeofResource(NULL, hr); // Data size/length
	HGLOBAL hG = ::LoadResource(NULL, hr);
	if (NULL == hG || nResSize <= 0)
	{
		int ierr = GetLastError();
		return NULL;
	}
	LPBYTE pData = (LPBYTE)LockResource(hG); // Data Ptr
	if (NULL == pData)
	{
		int ierr = GetLastError();
		return NULL;
	}
	dwLen = nResSize;
	return pData;
}

BYTE* CRXSkinResourceMgr::GetResource(const TCHAR* pszResoureName, const TCHAR* pszType, uint32_t& dwLen)
{
	dwLen = 0;
	HRSRC hr = ::FindResource(NULL, pszResoureName, pszType);
	if (NULL == hr)
	{
		int ierr = GetLastError();
		return NULL;
	}

	ULONG nResSize = ::SizeofResource(NULL, hr); // Data size/length
	HGLOBAL hG = ::LoadResource(NULL, hr);
	if (NULL == hG || nResSize <= 0)
	{
		int ierr = GetLastError();
		return NULL;
	}
	LPBYTE pData = (LPBYTE)LockResource(hG); // Data Ptr
	if (NULL == pData)
	{
		int ierr = GetLastError();
		return NULL;
	}
	dwLen = nResSize;
	return pData;
}

BYTE* CRXSkinResourceMgr::GetGif(const TCHAR* pszResoureName, uint32_t& dwLen)
{
	dwLen = 0;
	HRSRC hr = ::FindResource(NULL, pszResoureName, L"GIF");
	if (NULL == hr)
	{
		int ierr = GetLastError();
		return NULL;
	}

	ULONG nResSize = ::SizeofResource(NULL, hr); // Data size/length
	HGLOBAL hG = ::LoadResource(NULL, hr);
	if (NULL == hG || nResSize <= 0)
	{
		int ierr = GetLastError();
		return NULL;
	}
	LPBYTE pData = (LPBYTE)LockResource(hG); // Data Ptr
	if (NULL == pData)
	{
		int ierr = GetLastError();
		return NULL;
	}
	dwLen = nResSize;
	return pData;
}

bool CRXSkinResourceMgr::OpenZipResource(const TCHAR* pszPath, const char* pszPassword)
{
	
	BOOL bResult = m_oZip.Open(pszPath);
	m_oZip.SetPassword((LPCSTR)pszPassword);
	int nCount = m_oZip.GetEntries();
	return bResult;
}

bool CRXSkinResourceMgr::LoadResourceFromZip(const TCHAR* pszPath, std::vector<unsigned char>& vec)
{
	tstring strPath = pszPath;
	strPath = replace_all(strPath, _T("/"), _T("\\"));
	CZipFile zf;
	if (!m_oZip.GetFile(strPath.c_str(), zf))
		return FALSE;
	vec.resize(zf.GetSize());
	memcpy(&vec[0], zf.GetData(), zf.GetSize());
	return true;
}
bool CRXSkinResourceMgr::LoadResource(const TCHAR* pszPath, std::vector<unsigned char>& vec)
{
	if (CRXSkinService::Instance()->GetResourceModel() ==  RESOURCE_FROM_FILE)
	{
		TCHAR szFullPath[MAX_PATH] = { 0 };
		_RXTsprintf(szFullPath, _T("%s%s"),CRXSkinService::Instance()->GetResourceRootPath().c_str(),
			pszPath);
		uint32_t dwFileSize = RXFileSize(szFullPath);
		if (dwFileSize<=0)
		{
			return false;
		}
		vec.resize(dwFileSize, 0);
		FILE* hFile = _tfopen(szFullPath, _T("rb"));
		if (hFile)
		{
			fread(&vec[0], 1, dwFileSize, hFile);
			fclose(hFile);
		}
		return true;
	}
	if (CRXSkinService::Instance()->GetResourceModel() == RESOURCE_FROM_ZIP)
	{
		tstring strPath = pszPath;
		strPath = replace_all(strPath, _T("/"), _T("\\"));
		CZipFile zf;
		if (!m_oZip.GetFile(strPath.c_str(), zf))
			return FALSE;
		vec.resize(zf.GetSize());
		memcpy(&vec[0], zf.GetData(), zf.GetSize());
		return true;
	}
	return false;
}

tinyxml2::XMLDocument* CRXSkinResourceMgr::LoadXmlFromResource(const TCHAR* pszResoureName)
{
	std::vector<unsigned char> vec;
	LoadResource(pszResoureName,vec);
	if (vec.size()>0)
	{
		tinyxml2::XMLDocument* poDoc = new tinyxml2::XMLDocument;
		char* pszData = (char*)malloc(vec.size() + 1);
		memset(pszData, 0, vec.size() + 1);
		memcpy(pszData, &vec[0], vec.size());
		if (tinyxml2::XML_SUCCESS == poDoc->Parse((const char*)pszData))
		{
			free(pszData);
			vec.clear();
			return poDoc;
		}
		free(pszData);
		vec.clear();
	}
	return NULL;
}