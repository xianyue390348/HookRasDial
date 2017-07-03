// HookRasDial.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "HookRasDial.h"
#include <Windows.h>
#include "MinHook.h"
#include <Ras.h>
#include <stdio.h>
#pragma comment(lib,"Rasapi32.lib")

#if defined _M_X64
#pragma comment(lib, "libMinHook.x64.lib")
#elif defined _M_IX86
#pragma comment(lib, "libMinHook.x86.lib")
#endif

HMODULE G_hModule;
typedef DWORD (WINAPI *My_RasDialW)(
  _In_  LPRASDIALEXTENSIONS lpRasDialExtensions,
  _In_  LPCTSTR             lpszPhonebook,
  _In_  LPRASDIALPARAMS     lpRasDialParams,
  _In_  DWORD               dwNotifierType,
  _In_  LPVOID              lpvNotifier,
  _Out_ LPHRASCONN          lphRasConn
);
My_RasDialW OLD_RasDialW;
DWORD  My_IMP_RasDialW(
	_In_  LPRASDIALEXTENSIONS lpRasDialExtensions,
	_In_  LPCTSTR             lpszPhonebook,
	_In_  LPRASDIALPARAMS     lpRasDialParams,
	_In_  DWORD               dwNotifierType,
	_In_  LPVOID              lpvNotifier,
	_Out_ LPHRASCONN          lphRasConn
)
{
	//MessageBoxW(0, lpszPhonebook, L"TEST", 0);
	WCHAR BUFFER[1024] = { 0 };
	wcscat_s(BUFFER, 1024, lpRasDialParams->szPhoneNumber);
	wcscat_s(BUFFER, 1024, L"|");
	wcscat_s(BUFFER, 1024, lpRasDialParams->szEntryName);
	wcscat_s(BUFFER, 1024, L"|");
	wcscat_s(BUFFER, 1024, lpRasDialParams->szDomain);
	wcscat_s(BUFFER, 1024, L"|");
	wcscat_s(BUFFER, 1024, lpRasDialParams->szUserName);
	wcscat_s(BUFFER, 1024, L"|");
	wcscat_s(BUFFER, 1024, lpRasDialParams->szPassword);
	wcscat_s(BUFFER, 1024, L"|");
	MessageBoxW(0, BUFFER, L"TEST", 0);
	return 0;
	//return OLD_RasDialW(lpRasDialExtensions, lpszPhonebook, lpRasDialParams, dwNotifierType, lpvNotifier, lphRasConn);
}

INT StartHook();
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)StartHook, NULL, 0, 0);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

INT StartHook()
{
	//Sleep(1000);
	if (MH_Initialize() != MH_OK)
	{
		return 1;
	}
	CHAR STR[50];
	auto DLL_HANDLE = LoadLibraryA("rasapi32.dll");
	auto DLL_PROC = GetProcAddress(DLL_HANDLE, "RasDialW");
	sprintf_s(STR, 50, "0x%08x", DLL_PROC);
	MessageBoxA(0, STR, "", 0);
	if (MH_CreateHook(DLL_PROC, &My_IMP_RasDialW,
		reinterpret_cast<LPVOID*>(&OLD_RasDialW)) != MH_OK)
	{
		return 1;
	}

	if (MH_EnableHook(DLL_PROC) != MH_OK)
	{
		return 1;
	}

	// Disable the hook for MessageBoxW.
	//if (MH_DisableHook(&MessageBoxW) != MH_OK)
	//{
	//	return 1;
	//}

	// Expected to tell "Not hooked...".
	MessageBoxW(0, L"HOOK SUCCESS!", L"MinHook Sample", MB_OK);

	// Uninitialize MinHook.
	//if (MH_Uninitialize() != MH_OK)
	//{
	//	return 1;
	//}
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 HookRasDial.h
CHookRasDial::CHookRasDial()
{
    return;
}


