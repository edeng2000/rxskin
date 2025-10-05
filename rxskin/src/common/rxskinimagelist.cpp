#include "pch.h"
#include "common/rxskinimagelist.h"
#include "common/rximagemodel.h"
#include "common/imagemgr.h"
#include "common/rxskinservice.h"
CRXSkinImageList::CRXSkinImageList()
{
	m_poImage = NULL;
	m_dwX = 0;
	m_dwY = 0;

}

CRXSkinImageList::~CRXSkinImageList(void)
{
	if (m_poImage)
	{
		m_poImage->Release();
		m_poImage = NULL;
	}
}

bool CRXSkinImageList::OpenFile(const char* pszName, const TCHAR* pszFilePath, const uint32_t& dwCX, const uint32_t& dwCY)
{
	m_poImage = CRXSkinService::Instance()->GetImageMgr().OpenDirectImage(pszFilePath);
	if (NULL == m_poImage)
	{
		return false;
	}
	m_strFilePath = pszFilePath;
	m_strName = pszName;
	m_dwY = dwCY;
	m_dwX = dwCX;
	m_poImage->SetTransIndex(0);
	return true;
}

IRXImage* CRXSkinImageList::GetImage()
{
	return m_poImage;
}

uint32_t CRXSkinImageList::GetX()
{
	return m_dwX;
}

uint32_t CRXSkinImageList::GetY()
{
	return m_dwY;
}

uint32_t CRXSkinImageList::GetCount()
{
	if (NULL == m_poImage)
	{
		return 0;
	}
	if (m_dwY==0)
	{
		return 0;
	}
	return m_poImage->GetHeight() / m_dwY;
}

bool CRXSkinImageList::CheckImageIndex(const uint32_t& dwImageIndex)
{
	if (NULL == m_poImage)
	{
		return false;
	}
	if (m_dwY == 0)
	{
		return false;
	}
	if (dwImageIndex*m_dwY>= m_poImage->GetHeight())
	{
		return false;
	}
	return true;
}