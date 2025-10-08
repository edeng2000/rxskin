#include <iostream>
#include "rxlogger.h"
#include "rxstring.h"
#include "rxfile.h"
#include "rxdir.h"
#include <ShlObj.h>
#include <ShlObj_core.h>
#include <Shlwapi.h>
#include <shtypes.h>
#include <Shellapi.h>
#include <corecrt_io.h>
#include <corecrt.h>
#include <fcntl.h>
#include "tinyxml.h"
struct SBuildItem
{
	char   szName[256];
	char   szBuildPath[256];
	char   szBuildType[256];
	SBuildItem()
	{
		memset(szBuildType, 0, 256);
		memset(szName, 0, 256);
		memset(szBuildPath, 0, 256);
	}
	SBuildItem(const SBuildItem& oItem)
	{
		memset(szBuildType, 0, 256);
		memset(szName, 0, 256);
		memset(szBuildPath, 0, 256);
		strncpy(szBuildPath, oItem.szBuildPath, 256);
		strncpy(szBuildType, oItem.szBuildType, 256);
		strncpy(szName, oItem.szName, 256);
	}
};

vector<SBuildItem>  vecAllItem;

void ReadConfig()
{
	char szConfig[MAX_PATH] = { 0 };
	sprintf(szConfig, "%sconfig.xml", _RXTT2Local(RX::RXGetModulePath()).c_str());
	TiXmlDocument oDoc;
	if (oDoc.LoadFile(szConfig))
	{
		TiXmlElement* poRoot = oDoc.RootElement();
		if (poRoot)
		{
			TiXmlElement* poItem = poRoot->FirstChildElement();
			while (poItem)
			{
				SBuildItem oItem;
				const char* pszValue = poItem->Attribute("name");
				if (pszValue)
				{
					strncpy(oItem.szName, pszValue, 256);
				}
				pszValue = poItem->Attribute("path");
				if (pszValue)
				{
					strncpy(oItem.szBuildPath, pszValue, 256);
				}
				pszValue = poItem->Attribute("type");
				if (pszValue)
				{
					strncpy(oItem.szBuildType, pszValue, 256);
				}
				vecAllItem.push_back(oItem);
				poItem = poItem->NextSiblingElement();
			}
		}
	}
}


int main()
{
	vector<tstring> vecCommand;
	CRXLogger::CreateInstance();
	CRXLogger::Instance()->SetName("build");

	CRXLogger::Instance()->StartThread();
	int nCrt = 0;
	FILE* fp;
	AllocConsole();
	nCrt = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
	fp = _fdopen(nCrt, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);
	CRXLogger::Instance()->SetConselOutput(fp);

	SHFILEINFO sfi;
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
			TCHAR szFullName[MAX_PATH] = { 0 };
			_RXTStrcpy(szFullName, pDriveName);
			if (SHGetFileInfo((LPCTSTR)szFullName, 0, &sfi, sizeof(sfi),
				SHGFI_DISPLAYNAME | SHGFI_ICON))
			{
				LOG_INFO("磁盘分区：%s", _RXTT2Local(szFullName).c_str());
				tstring strPath = szFullName;
				strPath += _T("Program Files (x86)\\Microsoft Visual Studio\\");
				vector<tstring> vecFind;
				RX::RXGetAllFilesEx(strPath.c_str(), _T("exe"), vecFind);
				for (int t = 0; t < vecFind.size(); t++)
				{
					int pos = vecFind[t].find(_T("devenv.exe"));
					if (pos >= 0)
					{
						vecCommand.push_back(vecFind[t]);
					}
					//			m_poFolderTree->AddItem(dwParentID, sfi.szDisplayName, sfi.szDisplayName, szFullName, sfi.iIcon);
				}
				pDriveName += lstrlen(pDriveName) + 1;
			}
		}
	}

	for (int t=0;t<vecCommand.size();t++)
	{
		LOG_INFO("visual studio Path:序号：[%d] %s",t,
			_RXTT2Local(vecCommand[t].c_str()).c_str());
	}
	ReadConfig();
	bool exited = false;
	LOG_INFO("请选定visual studio 路径：");
	while (exited==false)
	{
		int input = fgetchar();
		char c = input;
		if (c == 'e')
		{
			exited = true;
		}
		else
		{
			int sel = input - 48;
			if (sel>=0&&sel<vecCommand.size())
			{
				LOG_INFO("选定编译器：%s", _RXTT2Local(vecCommand[sel].c_str()).c_str());

				tstring strPath =  RX::RXGetModulePath();
				tstring curBat = strPath + _T("build_sln.bat");
				FILE* hFile = fopen(_RXTT2Local(curBat.c_str()).c_str(),"wb");
				if (hFile)
				{
					fprintf(hFile,"@echo  off\n");
					int pos = strPath.find(_T("\\"));
					tstring strRoot = strPath.substr(0, pos);

					for (int t=0;t<vecAllItem.size();t++)
					{
						fprintf(hFile, "echo \"start build %s  %s\"\n",
							vecAllItem[t].szName,
							vecAllItem[t].szBuildType);
						fprintf(hFile, "\"%s\" \"%s%s\"  /rebuild \"%s\" /out \"output.txt\"\n",
							_RXTT2Local(vecCommand[sel].c_str()).c_str(),
							_RXTT2Local(RX::RXGetModulePath()).c_str(),
							vecAllItem[t].szBuildPath,
							vecAllItem[t].szBuildType);
						fprintf(hFile, "echo \"build %s  %s end\"\n",
							vecAllItem[t].szName,vecAllItem[t].szBuildType);

					}

					
					fprintf(hFile, "echo \"build successfully!\"\n");
					fprintf(hFile, "pause\n");
					
					fclose(hFile);

					LOG_INFO("已经生成：build_sln.bat！输入：'e',回车！退出应用，然后双击：build_sln.bat,执行编译，然后在项目根目录的bin目录可找到生成的RXSkin库！");
				}
			}
		}
	}
}
