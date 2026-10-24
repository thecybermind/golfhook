/*
GolfHook - Golf With Your Friends Hook
Copyright 2026
https://github.com/thecybermind/golfhook/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

/**
 * This file handles initial mod loading.
 * 
 * It uses IAT patching to hook GetProcAddress requests towards this DLL (version.dll) and to point them at the real version.dll.
 * 
 * It also uses IAT patching to hook the initial call to UnityPlayer.dll!UnityMain to begin the Unity engine.
 */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "hook.h"
#include "main.h"

#define DLLEXPORT __declspec(dllexport)
#ifdef __cplusplus
#define C_DLLEXPORT extern "C" DLLEXPORT
#else
#define C_DLLEXPORT DLLEXPORT
#endif

static HMODULE hMe = nullptr;                   // this DLL handle
static HMODULE hVersion = nullptr;              // real version.dll handle

typedef FARPROC(*pfnGPA)(HMODULE, LPCSTR);
static pfnGPA GetProcAddress_orig = nullptr;    // original GetProcAddress function pointer from binary's IAT

typedef int (*pfnUnityMain)(HINSTANCE, HINSTANCE, LPWSTR, int);
static pfnUnityMain UnityMain_orig = nullptr;   // original UnityMain function pointer from binary's IAT

// This hook is just to point any requests directed towards this DLL to the real version.dll instead.
static FARPROC WINAPI GetProcAddress_hook(HMODULE hModule, LPCSTR lpProcName) {
    // if trying to load a function in this DLL, redirect to the real version.dll
    if (hModule == hMe) {
        if (!hVersion)
            hVersion = LoadLibraryA("c:\\windows\\system32\\version.dll");
        // fail? who knows
        if (!hVersion)
            return nullptr;

        return GetProcAddress_orig(hVersion, lpProcName);
    }
    
    // do the original call
    return GetProcAddress_orig(hModule, lpProcName);
}


// This hook is to actually get notified when UnityMain is called in UnityPlayer.dll.
static int UnityMain_hook(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    PreInit();
    int ret = UnityMain_orig(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    PostInit();
    return ret;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        hMe = hModule;

        // install IAT hook for binary->kernel32.dll!GetProcAddress
        GetProcAddress_orig = (pfnGPA)install_IAT_hook(GetModuleHandle(NULL), "kernel32.dll", "GetProcAddress", (FARPROC)GetProcAddress_hook);

        // install IAT hook for binary->UnityPlayer.dll!UnityMain
        UnityMain_orig = (pfnUnityMain)install_IAT_hook(GetModuleHandle(NULL), "UnityPlayer.dll", "UnityMain", (FARPROC)UnityMain_hook);
    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
        // unregister hooks
        (void)install_IAT_hook(GetModuleHandle(NULL), "kernel32.dll", "GetProcAddress", (FARPROC)GetProcAddress_orig);
        (void)install_IAT_hook(GetModuleHandle(NULL), "UnityPlayer.dll", "UnityMain", (FARPROC)UnityMain_orig);
    }
    return TRUE;
}


C_DLLEXPORT void WINAPI _stub_func() {
    // do nothing.
    // this is just used to fill out the required DLL exports and they should never actually get called
}
#pragma comment(linker,"/export:GetFileVersionInfoA=_stub_func,@1")
#pragma comment(linker,"/export:GetFileVersionInfoByHandle=_stub_func,@2")
#pragma comment(linker,"/export:GetFileVersionInfoExA=_stub_func,@3")
#pragma comment(linker,"/export:GetFileVersionInfoExW=_stub_func,@4")
#pragma comment(linker,"/export:GetFileVersionInfoSizeA=_stub_func,@5")//_GetFileVersionInfoSizeA,@5")
#pragma comment(linker,"/export:GetFileVersionInfoSizeExA=_stub_func,@6")
#pragma comment(linker,"/export:GetFileVersionInfoSizeExW=_stub_func,@7")
#pragma comment(linker,"/export:GetFileVersionInfoSizeW=_stub_func,@8")
#pragma comment(linker,"/export:GetFileVersionInfoW=_stub_func,@9")
#pragma comment(linker,"/export:VerFindFileA=_stub_func,@10")
#pragma comment(linker,"/export:VerFindFileW=_stub_func,@11")
#pragma comment(linker,"/export:VerInstallFileA=_stub_func,@12")
#pragma comment(linker,"/export:VerInstallFileW=_stub_func,@13")
#pragma comment(linker,"/export:VerLanguageNameA=_stub_func,@14")
#pragma comment(linker,"/export:VerLanguageNameW=_stub_func,@15")
#pragma comment(linker,"/export:VerQueryValueA=_stub_func,@16")
#pragma comment(linker,"/export:VerQueryValueW=_stub_func,@17")
