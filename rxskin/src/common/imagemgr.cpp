#include "pch.h"
#include "common/ImageMgr.h"
#include "fundation/rxstring.h"
#include "common/rxskinutil.h"
#include "fundation/tinyxml2.h"
#include "common/rximagemodel.h"
#include "fundation/rxfile.h"
#include "common/rxskinresourcemgr.h"
#include "rxskinres.h"

extern TCHAR* RXSkinGetFileTypes(BOOL bOpenFileDialog);
extern int RXSkinGetIndexFromType(int nDocType, BOOL bOpenFileDialog);
extern tstring RXSkinGetExtFromType(int nDocType);

extern int RXSkinGetTypeFromIndex(int nIndex, BOOL bOpenFileDialog);
extern int GetIconIndex(LPCWSTR ext);
CImageMgr::CImageMgr(void)
{
	m_poImageModule = NULL;

}

CImageMgr::~CImageMgr(void)
{
	UnInit();
}

BOOL CImageMgr::InitResource(const TCHAR* pszResource)
{
	tinyxml2::XMLDocument* poDoc = CRXSkinResourceMgr::Instance()->GetXml(pszResource);
	if (poDoc)
	{
		tinyxml2::XMLElement* poRoot = poDoc->RootElement();
		if (poRoot)
		{
			tinyxml2::XMLElement* poElementImages = poRoot->FirstChildElement("Images");
			if (poElementImages)
			{
				tinyxml2::XMLElement* poElementImage = poElementImages->FirstChildElement("Image");
				while (poElementImage)
				{
					INT32 nID = 0;
					uint32_t dwResourceID = 0;
					CHAR* pszSrc = (CHAR*)poElementImage->Attribute("src");
					if (pszSrc)
					{
						dwResourceID = atoi(pszSrc);
					}
					CHAR* pszId = (CHAR*)poElementImage->Attribute("id");
					if (pszId)
					{
						nID = atoi(pszId);
					}
					TCHAR* pszResource = MAKEINTRESOURCE(dwResourceID);
					if (NULL  == CRXImageModule::Instance())
					{
						CRXImageModule::CreateInstance();
					}
					m_poImageModule = CRXImageModule::Instance();
					IRXImage* poImage = m_poImageModule->GetImageHandleFromResource(NULL, pszResource);
					if (poImage)
					{
						m_oImageIDMap.insert(make_pair(nID, poImage));
					}
					poElementImage = poElementImage->NextSiblingElement("Image");
				}
			}

		}
		delete poDoc;
	}
	IRXImage* poImage = FindImage(ID_BUTTON_CHECKBOX_NORMAL);
	if (NULL == poImage)
	{
		TCHAR* pszResource = MAKEINTRESOURCE(IDB_PNG_CHECKBOX);
		poImage = m_poImageModule->GetImageHandleFromResource(NULL, pszResource);
		if (poImage)
		{
			m_oImageIDMap.insert(make_pair(ID_BUTTON_CHECKBOX_NORMAL, poImage));
		}
	}
	poImage = FindImage(ID_BUTTON_CHECKBOX_CHECKED);
	if (NULL == poImage)
	{
		TCHAR* pszResource = MAKEINTRESOURCE(IDB_PNG_CHECKBOX_PRESSED);
		poImage = m_poImageModule->GetImageHandleFromResource(NULL, pszResource);
		if (poImage)
		{
			m_oImageIDMap.insert(make_pair(ID_BUTTON_CHECKBOX_CHECKED, poImage));
		}
	}
	poImage = FindImage(ID_BUTTON_CHECKBOX_HOT);
	if (NULL == poImage)
	{
		TCHAR* pszResource = MAKEINTRESOURCE(IDB_PNG_CHECKBOX_HOVER);
		poImage = m_poImageModule->GetImageHandleFromResource(NULL, pszResource);
		if (poImage)
		{
			m_oImageIDMap.insert(make_pair(ID_BUTTON_CHECKBOX_HOT, poImage));
		}
	}
	poImage = FindImage(ID_BROWSER_BTN_NORMAL);
	if (NULL == poImage)
	{
		TCHAR* pszResource = MAKEINTRESOURCE(IDB_PNG_BTN_SEARCH_NORMAL);
		poImage = m_poImageModule->GetImageHandleFromResource(NULL, pszResource);
		if (poImage)
		{
			m_oImageIDMap.insert(make_pair(ID_BROWSER_BTN_NORMAL, poImage));
		}
	}
	poImage = FindImage(ID_BROWSER_BTN_HOT);
	if (NULL == poImage)
	{
		TCHAR* pszResource = MAKEINTRESOURCE(IDB_PNG_BTN_SEARCH_DOWN);
		poImage = m_poImageModule->GetImageHandleFromResource(NULL, pszResource);
		if (poImage)
		{
			m_oImageIDMap.insert(make_pair(ID_BROWSER_BTN_HOT, poImage));
		}
	}
	poImage = FindImage(ID_PICTURE_TIP);
	if (NULL == poImage)
	{
		TCHAR* pszResource = MAKEINTRESOURCE(IDB_PNG_TIP);
		poImage = m_poImageModule->GetImageHandleFromResource(NULL, pszResource);
		if (poImage)
		{
			m_oImageIDMap.insert(make_pair(ID_PICTURE_TIP, poImage));
		}
	}

	poImage = FindImage(ID_MESSAGEBOX_ERROR);
	if (NULL == poImage)
	{
		TCHAR* pszResource = MAKEINTRESOURCE(IDB_PNG_ERROR);
		poImage = m_poImageModule->GetImageHandleFromResource(NULL, pszResource);
		if (poImage)
		{
			m_oImageIDMap.insert(make_pair(ID_MESSAGEBOX_ERROR, poImage));
		}
	}

	poImage = FindImage(ID_MESSAGEBOX_WARN);
	if (NULL == poImage)
	{
		TCHAR* pszResource = MAKEINTRESOURCE(IDB_PNG_WARN);
		poImage = m_poImageModule->GetImageHandleFromResource(NULL, pszResource);
		if (poImage)
		{
			m_oImageIDMap.insert(make_pair(ID_MESSAGEBOX_WARN, poImage));
		}
	}
	poImage = FindImage(ID_MESSAGEBOX_HELP);
	if (NULL == poImage)
	{
		TCHAR* pszResource = MAKEINTRESOURCE(IDB_PNG_HELP);
		poImage = m_poImageModule->GetImageHandleFromResource(NULL, pszResource);
		if (poImage)
		{
			m_oImageIDMap.insert(make_pair(ID_MESSAGEBOX_HELP, poImage));
		}
	}
	poImage = FindImage(ID_SCROLLBAR);
	if (NULL == poImage)
	{
		TCHAR* pszResource = MAKEINTRESOURCE(IDB_PNG_SCROLLBAR);
		poImage = m_poImageModule->GetImageHandleFromResource(NULL, pszResource);
		if (poImage)
		{
			m_oImageIDMap.insert(make_pair(ID_SCROLLBAR, poImage));
		}
	}
	poImage = FindImage(ID_SCROLLBAR_HOR);
	if (NULL == poImage)
	{
		TCHAR* pszResource = MAKEINTRESOURCE(IDB_PNG_SCROLLBAR_HOVER);
		poImage = m_poImageModule->GetImageHandleFromResource(NULL, pszResource);
		if (poImage)
		{
			m_oImageIDMap.insert(make_pair(ID_SCROLLBAR_HOR, poImage));
		}
	}
	poImage = FindImage(ID_BUTTON_RADIO_NORMAL);
	if (NULL == poImage)
	{
		TCHAR* pszResource = MAKEINTRESOURCE(IDB_PNG_RADIO);
		poImage = m_poImageModule->GetImageHandleFromResource(NULL, pszResource);
		if (poImage)
		{
			m_oImageIDMap.insert(make_pair(ID_BUTTON_RADIO_NORMAL, poImage));
		}
	}
	poImage = FindImage(ID_BUTTON_RADIO_HOT);
	if (NULL == poImage)
	{
		TCHAR* pszResource = MAKEINTRESOURCE(IDB_PNG_RADIO_HOVER);
		poImage = m_poImageModule->GetImageHandleFromResource(NULL, pszResource);
		if (poImage)
		{
			m_oImageIDMap.insert(make_pair(ID_BUTTON_RADIO_HOT, poImage));
		}
	}
	poImage = FindImage(ID_BUTTON_RADIO_CHECKED);
	if (NULL == poImage)
	{
		TCHAR* pszResource = MAKEINTRESOURCE(IDB_PNG_RADIO_PRESSED);
		poImage = m_poImageModule->GetImageHandleFromResource(NULL, pszResource);
		if (poImage)
		{
			m_oImageIDMap.insert(make_pair(ID_BUTTON_RADIO_CHECKED, poImage));
		}
	}
	poImage = FindImage(ID_ARROW_RIGHT_NORMAL);
	if (NULL == poImage)
	{
		TCHAR* pszResource = MAKEINTRESOURCE(IDB_PNG_COLLASPSE_NORMAL);
		poImage = m_poImageModule->GetImageHandleFromResource(NULL, pszResource);
		if (poImage)
		{
			m_oImageIDMap.insert(make_pair(ID_ARROW_RIGHT_NORMAL, poImage));
		}
	}

	poImage = FindImage(ID_ARROW_RIGHT_NORMAL);
	if (NULL == poImage)
	{
		TCHAR* pszResource = MAKEINTRESOURCE(IDB_PNG_COLLASPSE_NORMAL);
		poImage = m_poImageModule->GetImageHandleFromResource(NULL, pszResource);
		if (poImage)
		{
			m_oImageIDMap.insert(make_pair(ID_ARROW_RIGHT_NORMAL, poImage));
		}
	}
	return TRUE;
}

BOOL CImageMgr::Init(const TCHAR* pszConfig)
{
	m_poImageModule = CRXImageModule::Instance();
	tinyxml2::XMLDocument* poDoc = CRXSkinResourceMgr::Instance()->LoadXmlFromResource(pszConfig);
	if (poDoc)
	{
		tinyxml2::XMLElement* poRoot = poDoc->RootElement();
		if (poRoot)
		{
			tinyxml2::XMLElement* poElementImages = poRoot->FirstChildElement("Images");
			if (poElementImages)
			{
				tinyxml2::XMLElement* poElementImage = poElementImages->FirstChildElement("Image");
				while(poElementImage)
				{
					INT32 nID = 0;
					tstring strPath = CRXSkinService::Instance()->GetResourceRootPath();
					CHAR* pszSrc = (CHAR*)poElementImage->Attribute("src");
					if (pszSrc)
					{
						strPath += _RXTUTF82T(pszSrc);
					}
					CHAR* pszId = (CHAR*)poElementImage->Attribute("id");
					if (pszId)
					{
						nID = atoi(pszId);
					}
					IRXImage* poImage = m_poImageModule->GetImageHandle(strPath.c_str());
					if (poImage)
					{
						m_oImageIDMap.insert(make_pair(nID,poImage));
					}
					poElementImage = poElementImage->NextSiblingElement("Image");
				}
			}
		}
		delete poDoc;
	}
	return TRUE;
}

TCHAR* CImageMgr::GetFileTypes(BOOL bOpenFileDialog)
{
	return RXSkinGetFileTypes(bOpenFileDialog);
}

int CImageMgr::GetIndexFromType(int nDocType, BOOL bOpenFileDialog)
{
	return RXSkinGetIndexFromType(nDocType,bOpenFileDialog);
}

tstring CImageMgr::GetExtFromType(int nDocType)
{
	return RXSkinGetExtFromType(nDocType);
}

int CImageMgr::GetTypeFromIndex(int nIndex, BOOL bOpenFileDialog)
{
	
	return RXSkinGetTypeFromIndex(nIndex,bOpenFileDialog);
}

IRXImage* CImageMgr::FindImage(const UINT32 dwID)
{
	CImageIDMapItr itr = m_oImageIDMap.find(dwID);
	if (itr!=m_oImageIDMap.end())
	{
		return itr->second;
	}
	return NULL;
}

VOID CImageMgr::UnInit()
{
	_DeleteAll();
	if (m_poImageModule)
	{
		m_poImageModule->Release();
		m_poImageModule = NULL;
	}

}

BOOL CImageMgr::SaveJpg(const TCHAR* pszFrom,const TCHAR* psTo)
{
	if (m_poImageModule)
	{
		return m_poImageModule->SaveJpg(pszFrom,psTo);
	}
	return FALSE;
}

IRXImage* CImageMgr::OpenImage(const TCHAR* pszFilename)
{
	CImageMapItr itr = m_oImageMap.find(tstring(pszFilename));
	if (itr!=m_oImageMap.end())
	{
		return itr->second;
	}
	if (NULL == m_poImageModule)
	{
		return NULL;
	}
	IRXImage* poImage = m_poImageModule->GetImageHandle(pszFilename);
	if (poImage)
	{
		m_oImageMap.insert(make_pair(tstring(pszFilename),poImage));
	}
	return poImage;
}
IRXImage* CImageMgr::OpenImageFromResource(const TCHAR* pszResource)
{
	CImageMapItr itr = m_oImageMap.find(tstring(pszResource));
	if (itr!=m_oImageMap.end())
	{
		return itr->second;
	}
	if (NULL == m_poImageModule)
	{
		return NULL;
	}
	IRXImage* poImage = m_poImageModule->GetImageHandleFromResource(NULL,pszResource);
	if (poImage)
	{
		m_oImageMap.insert(make_pair(tstring(pszResource),poImage));
	}
	return poImage;
}
IRXImage* CImageMgr::OpenDirectImage(const TCHAR* pszImageFile)
{
	if (m_poImageModule == NULL)
	{
		CRXImageModule::CreateInstance();
		m_poImageModule = CRXImageModule::Instance();
	}
	IRXImage* poImage = m_poImageModule->GetImageHandle(pszImageFile);;
	if (poImage==NULL)
	{
		TCHAR* pszResource = MAKEINTRESOURCE(_ttoi(pszImageFile));
		poImage = m_poImageModule->GetImageHandleFromResource(NULL,pszResource);
	}
	return poImage;
}

IRXImage* CImageMgr::OpenDirectImageFromResource(const TCHAR* pszResource)
{
	if (m_poImageModule == NULL)
	{
		CRXImageModule::CreateInstance();
		m_poImageModule = CRXImageModule::Instance();
	}
	return m_poImageModule->GetImageHandleFromResource(NULL, pszResource);
}

IRXImage* CImageMgr::LoadIconDirectImage(const TCHAR* pszImageFile)
{
	tstring strExt = RX::RXFileExtractExt(pszImageFile);
	strExt = _T(".") + strExt;
	int nIconID = GetIconIndex(strExt.c_str());
	if (nIconID<0)
	{
		nIconID = 0;
	}
	CImageIDMapItr itrFind = m_mapIcons.find(nIconID);
	if (itrFind!=m_mapIcons.end())
	{
		return itrFind->second;
	}
	IRXImage* poImage = m_poImageModule->GetIconImageHandle(pszImageFile);
	if (poImage)
	{
		m_mapIcons.insert(make_pair(nIconID, poImage));
	}
	return poImage;
}

VOID CImageMgr::RemoveImage(const TCHAR* pszFilename)
{
	CImageMapItr itr = m_oImageMap.find(tstring(pszFilename));
	if (itr!=m_oImageMap.end())
	{
		IRXImage* poImage = itr->second;
		m_oImageMap.erase(itr);
		if (poImage)
		{
			poImage->Release();
		}
	}
}

VOID CImageMgr::_DeleteAll()
{
	CImageMapItr itr = m_oImageMap.begin();
	for (;itr!=m_oImageMap.end();itr++)
	{
		IRXImage* poImage = itr->second;
		if (poImage)
		{
			poImage->Release();
		}
	}
	m_oImageMap.clear();

	CImageIDMapItr itrID = m_oImageIDMap.begin();
	for (;itrID!=m_oImageIDMap.end();itrID++)
	{
		IRXImage* poImage = itrID->second;
		if (poImage)
		{
			poImage->Release();
		}
	}
	m_oImageIDMap.clear();

	CImageIDMapItr itrIcon = m_mapIcons.begin();
	for (; itrIcon != m_mapIcons.end(); itrIcon++)
	{
		IRXImage* poImage = itrIcon->second;
		if (poImage)
		{
			poImage->Release();
		}
	}
	m_mapIcons.clear();

	CRXSkinImageListMapItr itrList = m_mapImageList.begin();
	for (; itrList != m_mapImageList.end(); itrList++)
	{
		delete itrList->second;
	}
	m_mapImageList.clear();
}

CRXSkinImageList* CImageMgr::AddImageList(const char* pszName, const TCHAR* pszFilePath, const uint32_t& dwCX, const uint32_t& dwCY)
{	
	CRXSkinImageList* poList = FindImageList(pszName);
	if (poList)
	{
		return poList;
	}
	poList = new CRXSkinImageList;
	if (false==poList->OpenFile(pszName,pszFilePath,dwCX,dwCY))
	{
		delete poList;
		return NULL;
	}
	m_mapImageList.insert(make_pair(string(pszName), poList));
	return poList;
}

CRXSkinImageList* CImageMgr::FindImageList(const char* pszName)
{
	CRXSkinImageListMapItr itr = m_mapImageList.find(string(pszName));
	if (itr==m_mapImageList.end())
	{
		return NULL;
	}
	return itr->second;
}