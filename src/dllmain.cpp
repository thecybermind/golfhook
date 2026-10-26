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

    Init(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

    // this function does not appear to actually return, even when exiting the game
    int ret = UnityMain_orig(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

    GHLOG(GH_LOG_TRACE, GH_TAG) << "UnityMain(\"" << lpCmdLineA << "\") returning " << ret << "\n";

    return ret;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        hMe = hModule;

        Log::log_init("golfhook.log", Log::DEFAULT_SEVERITY, Log::DEFAULT_APPEND);

        GHLOG(GH_LOG_NOTICE, GH_TAG) << "GolfHook v" GH_VERSION " (" GH_BUILD ") loaded!\n";

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
