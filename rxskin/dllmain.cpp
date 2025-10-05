// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "common/rxskinservice.h"
#include "common/rximagemodel.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
	{
		CRXImageModule::CreateInstance();
	}
	break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:

    case DLL_PROCESS_DETACH:
	{
	//	CRXImageModule::DestroyInstance();
	}
    break;
    }
    return TRUE;
}

