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
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

#include "hook.h"
#include "main.h"
#include "config.hpp"
#include "log.hpp"
#include "format.hpp"
#include "version.h"


#define DLLEXPORT __declspec(dllexport)
#ifdef __cplusplus
#define C_DLLEXPORT extern "C" DLLEXPORT
#else
#define C_DLLEXPORT DLLEXPORT
#endif


static HMODULE hMe = nullptr;                   // this DLL handle
static void RedirectIOToConsole();


C_DLLEXPORT int UnityMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    const char* lpCmdLineA = GetCommandLineA();
    GHLOG(GH_LOG_TRACE, GH_TAG) << "UnityMain(\"" << lpCmdLineA << "\") called\n";

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


    int ret = 0;

    // begin actual Unity hooking
    ret = GolfHook::UnityMain_Pre(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

    // if pre hook returned error, cancel
    if (ret != 0) {
        GHLOG(GH_LOG_FATAL, GH_TAG) << "UnityMain_Pre returned error (" << ret << ")! Shutting down.\n";
        return ret;
    }

    // this function does not appear to actually return, even when exiting the game
    ret = UnityMain_orig(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    // UnityMain_Post(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

    GHLOG(GH_LOG_TRACE, GH_TAG) << "UnityMain(\"" << lpCmdLineA << "\") returning " << ret << "\n";

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
    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
        GHLOG(GH_LOG_NOTICE, GH_TAG) << "GolfHook shutting down.\n";
    }

    return TRUE;
}


/**
* Generate a new console window for standard in/out/err.
* 
* From https://stackoverflow.com/a/191880/809900
*/
static const WORD MAX_CONSOLE_LINES = 500;  // maximum mumber of lines the output console should have
static void RedirectIOToConsole() {
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
