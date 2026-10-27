/*
GolfHook - Golf With Your Friends Hook
Copyright 2026
https://github.com/thecybermind/golfhook/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

#include <MinHook/MinHook.h>

#include "dllmain.h"
#include "iat_hook.h"
#include "main.h"
#include "config.hpp"
#include "log.hpp"
#include "format.hpp"
#include "version.h"


#pragma comment(lib, "..\\lib\\MinHook.x64.lib")

HMODULE hMe = nullptr;      // this DLL handle


static struct {
    const char* name;
    FARPROC proc;
} procmap[] = {
    { "il2cpp_init", (FARPROC)GolfHook::il2cpp_init_hook },
    { "il2cpp_shutdown", (FARPROC)GolfHook::il2cpp_shutdown_hook },
};


FARPROC WINAPI UnityPlayer_GetProcAddress_hook(HMODULE hModule, LPCSTR lpProcName) {
    static HMODULE hGameAssembly = nullptr;

    FARPROC ret = GetProcAddress(hModule, lpProcName);

    if (!hGameAssembly)
        hGameAssembly = GetModuleHandleA("GameAssembly.dll");

    if (hModule != hGameAssembly)
        return ret;

    // hook functions from GameAssembly
    for (auto& i : procmap) {
        if (strcmp(lpProcName, i.name) == 0) {
            GHLOG(GH_LOG_DEBUG, GH_TAG) << "GetProcAddress(\"GameAssembly.dll\", \"" << lpProcName << "\") called.\n";

            // store original pointer in hook maps for lookup
            GolfHook::hook_map_proc[i.proc] = ret;
            GolfHook::hook_map_name[i.name] = ret;
            return i.proc;
        }
    }

    return ret;
}


/**
 * This is the entry point called by "Golf With Your Friends.exe".
 * 
 * We primarily chose this because it guarantees we get loaded before UnityPlayer[_orig].dll. Since we load that DLL
 * and call UnityMain ourselves, we are able to hook that DLL's IAT for GetProcAddress to hook functions in GameAssembly.dll.
 * 
 */
C_DLLEXPORT int UnityMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    GHLOG(GH_LOG_TRACE, GH_TAG) << "UnityMain() called\n";

    // load original UnityPlayer.dll
    static HMODULE hUnityPlayer = LoadLibraryA("UnityPlayer_orig.dll");
    static int(*UnityMain_orig)(HINSTANCE, HINSTANCE, LPWSTR, int) = nullptr;

    if (!hUnityPlayer) {
        GHLOG(GH_LOG_FATAL, GH_TAG) << "Unable to load UnityPlayer_orig.dll! Shutting down.\n";
        return -1;
    }

    if (!UnityMain_orig)
        UnityMain_orig = (decltype(UnityMain_orig))GetProcAddress(hUnityPlayer, "UnityMain");

    if (!UnityMain_orig) {
        GHLOG(GH_LOG_FATAL, GH_TAG) << "Unable to load UnityMain from UnityPlayer_orig.dll! Shutting down.\n";
        return -1;
    }

    // hook GetProcAddress called from UnityPlayer_orig.dll
    if (!install_IAT_hook(hUnityPlayer, "kernel32.dll", "GetProcAddress", (FARPROC)UnityPlayer_GetProcAddress_hook)) {
        GHLOG(GH_LOG_FATAL, GH_TAG) << "Unable to hook UnityPlayer_orig.dll's GetProcAddress! Shutting down.\n";
        return -1;
    }

    int ret = UnityMain_orig(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

    GHLOG(GH_LOG_TRACE, GH_TAG) << "UnityMain() returning " << ret << "\n";

    return ret;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        hMe = hModule;

        RedirectIOToConsole();

        Log::log_init("golfhook.log", Log::DEFAULT_SEVERITY, Log::DEFAULT_APPEND);

        GHLOG(GH_LOG_NOTICE, GH_TAG) << "GolfHook v" GH_VERSION " (" GH_BUILD ") loaded!\n";
        GHLOG(GH_LOG_INFO, GH_TAG) << "Built: " GH_COMPILE " by " GH_BUILDER "\n";
        GHLOG(GH_LOG_INFO, GH_TAG) << "URL: " GH_URL "\n";
        GHLOG(GH_LOG_INFO, GH_TAG) << "\n";

        g_cfg = Config::cfg_load("golfhook.json");
        if (!g_cfg.empty()) {
            GHLOG(GH_LOG_INFO, GH_TAG) << "Config file found: \"golfhook.json\"\n";
        }
        else {
            GHLOG(GH_LOG_INFO, GH_TAG) << "Config file not found.\n";
        }

        // update log level from config file
        std::string cfg_loglevel = Config::cfg_get_string(g_cfg, "loglevel", "");
        if (!cfg_loglevel.empty())
            Log::log_set_severity(Log::log_severity_from_name(cfg_loglevel));

        if (MH_Initialize() != MH_OK) {
            GHLOG(GH_LOG_FATAL, GH_TAG) << "MinHook failed to initialize.\n";
            return FALSE;
        }
    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
        MH_Uninitialize();

        GHLOG(GH_LOG_NOTICE, GH_TAG) << "GolfHook shutting down.\n";
    }

    return TRUE;
}


/**
* Generate a new console window for standard in/out/err.
* 
* From https://stackoverflow.com/a/191880/809900
*/
static constexpr WORD MAX_CONSOLE_LINES = 500;  // maximum mumber of lines the output console should have
void RedirectIOToConsole() {
    int hConHandle;
    HANDLE lStdHandle;
    CONSOLE_SCREEN_BUFFER_INFO coninfo;
    FILE* fp;

    // allocate a console for this app
    AllocConsole();

    // set the screen buffer to be big enough to let us scroll text
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
    coninfo.dwSize.Y = MAX_CONSOLE_LINES;
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

    // redirect unbuffered STDOUT to the console
    lStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    hConHandle = _open_osfhandle((intptr_t)lStdHandle, _O_TEXT);
    freopen_s(&fp, "CONOUT$", "w", stdout);
    setvbuf(stdout, NULL, _IONBF, 0);

    // redirect unbuffered STDIN to the console
    lStdHandle = GetStdHandle(STD_INPUT_HANDLE);
    hConHandle = _open_osfhandle((intptr_t)lStdHandle, _O_TEXT);
    freopen_s(&fp, "CONIN$", "r", stdin);
    setvbuf(stdin, NULL, _IONBF, 0);

    // redirect unbuffered STDERR to the console
    lStdHandle = GetStdHandle(STD_ERROR_HANDLE);
    hConHandle = _open_osfhandle((intptr_t)lStdHandle, _O_TEXT);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    setvbuf(stderr, NULL, _IONBF, 0);

    // make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
    // point to console as well
    std::ios::sync_with_stdio();
}
