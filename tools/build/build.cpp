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

					// libpng
					fprintf(hFile, "echo \"start build libpng-1.6.45  Debug|x64\"\n");
					fprintf(hFile, "\"%s\" \"%s..\\lib3rd\\libpng-1.6.45\\projects\\vstudio\\vstudio.sln\"  /rebuild \"DebugLibrary|x64\" /out \"%soutput.txt\"\n",
						_RXTT2Local(vecCommand[sel].c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str());
					fprintf(hFile, "echo \"build libpng-1.6.45  Unicode_Debug|x64 end\"\n");

					fprintf(hFile, "echo \" start build libpng-1.6.45  Release|x64\"\n");
					fprintf(hFile, "\"%s\" \"%s..\\lib3rd\\libpng-1.6.45\\projects\\vstudio\\vstudio.sln\"  /rebuild \"ReleaseLibrary|x64\" /out \"%soutput.txt\"\n",
						_RXTT2Local(vecCommand[sel].c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str());
					fprintf(hFile, "echo \"build libpng  Release|x64 end\"\n");

					fprintf(hFile, "echo \"start build libpng-1.6.45  Debug|Win32\"\n");
					fprintf(hFile, "\"%s\" \"%s..\\lib3rd\\libpng-1.6.45\\projects\\vstudio\\vstudio.sln\"  /rebuild \"DebugLibrary|Win32\" /out \"%soutput.txt\"\n",
						_RXTT2Local(vecCommand[sel].c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str());
					fprintf(hFile, "echo \"build libpng-1.6.45  Debug|Win32 end\"\n");

					fprintf(hFile, "echo \" start build libpng-1.6.45  Release|Win32\"\n");
					fprintf(hFile, "\"%s\" \"%s..\\lib3rd\\libpng-1.6.45\\projects\\vstudio\\vstudio.sln\"  /rebuild \"ReleaseLibrary|Win32\" /out \"%soutput.txt\"\n",
						_RXTT2Local(vecCommand[sel].c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str());
					fprintf(hFile, "echo \"build libpng  Release|Win32 end\"\n");

					// giflib
					fprintf(hFile, "echo \"start build giflib-5.2.2  Debug|x64\"\n");
					fprintf(hFile, "\"%s\" \"%s..\\lib3rd\\giflib-5.2.2\\build64\\GifLib.sln\"  /rebuild \"Debug|x64\" /out \"%soutput.txt\"\n",
						_RXTT2Local(vecCommand[sel].c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str());
					fprintf(hFile, "echo \"build libpng-1.6.45  Unicode_Debug|x64 end\"\n");

					fprintf(hFile, "echo \" start build GifLib  Release|x64\"\n");
					fprintf(hFile, "\"%s\" \"%s..\\lib3rd\\giflib-5.2.2\\build64\\GifLib.sln\"  /rebuild \"Release|x64\" /out \"%soutput.txt\"\n",
						_RXTT2Local(vecCommand[sel].c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str());
					fprintf(hFile, "echo \"build GifLib  Release|x64 end\"\n");

					fprintf(hFile, "echo \"start build GifLib  Debug|Win32\"\n");
					fprintf(hFile, "\"%s\" \"%s..\\lib3rd\\giflib-5.2.2\\build32\\GifLib.sln\"  /rebuild \"Debug|Win32\" /out \"%soutput.txt\"\n",
						_RXTT2Local(vecCommand[sel].c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str());
					fprintf(hFile, "echo \"build GifLib  Debug|Win32 end\"\n");

					fprintf(hFile, "echo \" start build GifLib  Release|Win32\"\n");
					fprintf(hFile, "\"%s\" \"%s..\\lib3rd\\giflib-5.2.2\\build32\\GifLib.sln\"  /rebuild \"Release|Win32\" /out \"%soutput.txt\"\n",
						_RXTT2Local(vecCommand[sel].c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str());
					fprintf(hFile, "echo \"build GifLib  Release|Win32 end\"\n");



					fprintf(hFile, "echo \"start build cximage  Unicode_Debug|Win32\"\n");
					fprintf(hFile, "\"%s\" \"%s..\\lib3rd\\cximage\\src\\CxImageFull.sln\"  /rebuild \"Unicode_Debug|Win32\" /out \"%soutput.txt\"\n",
						_RXTT2Local(vecCommand[sel].c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str());
					fprintf(hFile, "echo \"build cximage  Unicode_Debug|Win32 end\"\n");
					
					fprintf(hFile, "echo \" start build cximage  Unicode_Release|Win32\"\n");
					fprintf(hFile, "\"%s\" \"%s..\\lib3rd\\cximage\\src\\CxImageFull.sln\"  /rebuild \"Unicode_Release|Win32\" /out \"%soutput.txt\"\n",
						_RXTT2Local(vecCommand[sel].c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str());
					fprintf(hFile, "echo \"build cximage  Unicode_Release|Win32 end\"\n");
	
					fprintf(hFile, "echo \"start build cximage  Unicode_Debug|x64\"\n");
					fprintf(hFile, "\"%s\" \"%s..\\lib3rd\\cximage\\src\\CxImageFull.sln\"  /rebuild \"Unicode_Debug|x64\" /out \"%soutput.txt\"\n",
						_RXTT2Local(vecCommand[sel].c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str());
					fprintf(hFile, "echo \"build cximage  Unicode_Debug|x64 end\"\n");

					fprintf(hFile, "echo \" start build cximage  Unicode_Release|x64\"\n");
					fprintf(hFile, "\"%s\" \"%s..\\lib3rd\\cximage\\src\\CxImageFull.sln\"  /rebuild \"Unicode_Release|x64\" /out \"%soutput.txt\"\n",
						_RXTT2Local(vecCommand[sel].c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str());
					fprintf(hFile, "echo \"build cximage  Unicode_Release|x64 end\"\n");

				

					fprintf(hFile, "echo \"start build Skia  Debug|x86\"\n");
					fprintf(hFile, "\"%s\" \"%s..\\lib3rd\\skia\\skia.sln\"  /rebuild \"Debug|x86\" /out \"%soutput.txt\"\n",
						_RXTT2Local(vecCommand[sel].c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str());
					fprintf(hFile, "echo \"build skia  Debug|x86 end\"\n");

					fprintf(hFile, "echo \" start build skia  Release|x86\"\n");
					fprintf(hFile, "\"%s\" \"%s..\\lib3rd\\skia\\skia.sln\"  /rebuild \"Release|x86\" /out \"%soutput.txt\"\n",
						_RXTT2Local(vecCommand[sel].c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str());
					fprintf(hFile, "echo \"build skia  Release|x86 end\"\n");


					fprintf(hFile, "echo \"start build Skia  Debug|x64\"\n");
					fprintf(hFile, "\"%s\" \"%s..\\lib3rd\\skia\\skia.sln\"  /rebuild \"Debug|x64\" /out \"%soutput.txt\"\n",
						_RXTT2Local(vecCommand[sel].c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str());
					fprintf(hFile, "echo \"build skia  Debug|x64 end\"\n");

					fprintf(hFile, "echo \" start build skia  Release|x64\"\n");
					fprintf(hFile, "\"%s\" \"%s..\\lib3rd\\skia\\skia.sln\"  /rebuild \"Release|x64\" /out \"%soutput.txt\"\n",
						_RXTT2Local(vecCommand[sel].c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str());
					fprintf(hFile, "echo \"build skia  Release|x64 end\"\n");

					fprintf(hFile, "echo \" start build RXSkin  Debug|x64\"\n");
					fprintf(hFile, "\"%s\" \"%s..\\RXSkin.sln\"  /rebuild \"Debug|x64\" /out \"%soutput.txt\"\n",
						_RXTT2Local(vecCommand[sel].c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str());
					fprintf(hFile, "echo \"build RXSkin  Debug|x64 end\"\n");

					fprintf(hFile, "echo \" start build RXSkin  Release|x64\"\n");
					fprintf(hFile, "\"%s\" \"%s..\\RXSkin.sln\"  /rebuild \"Release|x64\" /out \"%soutput.txt\"\n",
						_RXTT2Local(vecCommand[sel].c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str());
					fprintf(hFile, "echo \"build RXSkin  Release|x64 end\"\n");


					fprintf(hFile, "echo \" start build RXSkin  Debug|Win32\"\n");
					fprintf(hFile, "\"%s\" \"%s..\\RXSkin.sln\"  /rebuild \"Debug|Win32\" /out \"%soutput.txt\"\n",
						_RXTT2Local(vecCommand[sel].c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str());
					fprintf(hFile, "echo \"build RXSkin  Debug|Win32 end\"\n");

					fprintf(hFile, "echo \" start build RXSkin  Release|Win32\"\n");
					fprintf(hFile, "\"%s\" \"%s..\\RXSkin.sln\"  /rebuild \"Release|Win32\" /out \"%soutput.txt\"\n",
						_RXTT2Local(vecCommand[sel].c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str(),
						_RXTT2Local(strPath.c_str()).c_str());
					fprintf(hFile, "echo \"build RXSkin  Release|Win32 end\"\n");
				
					fprintf(hFile, "echo \"build successfully!\"\n");
					fprintf(hFile, "pause\n");
					
					fclose(hFile);

					LOG_INFO("已经生成：build_sln.bat！输入：'e',回车！退出应用，然后双击：build_sln.bat,执行编译，然后在项目根目录的bin目录可找到生成的RXSkin库！");
				}
			}
		}
	}
}
