/*
GolfHook - Golf With Your Friends Hook
Copyright 2026
https://github.com/thecybermind/golfhook/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

#if defined(_WIN32)
 #include <MinHook/MinHook.h>
 // #pragma comment(lib, "..\\lib\\MinHook.x64.lib")
 #pragma comment(lib, "..\\lib\\libMinHook.x64.lib")
 #define WINAPI __stdcall
#elif defined(__linux__)
 #error Add linux headers
 #define WINAPI
#endif

#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

#include "dllmain.h"
#include "iat_hook.h"
#include "main.hpp"
#include "util.hpp"
#include "config.hpp"
#include "log.hpp"
#include "format.hpp"
#include "version.h"
#include "ghapi.hpp"


void* hMe = nullptr;      // this DLL handle


static struct {
    const char* name;
    void* proc;
} hooks[] = {
    { "il2cpp_init", GolfHook::il2cpp_init_hook },
    { "il2cpp_shutdown", GolfHook::il2cpp_shutdown_hook },
};


void* WINAPI UnityPlayer_GetProcAddress_hook(void* hModule, const char* lpProcName) {
    static void* hGameAssembly = nullptr;

    void* ret = Util::dll_symbol(hModule, lpProcName);

    if (!hGameAssembly)
        hGameAssembly = Util::dll_handle("GameAssembly.dll");

    if (hModule != hGameAssembly)
        return ret;

    // hook functions from GameAssembly
    for (auto& i : hooks) {
        if (strcmp(lpProcName, i.name) == 0) {
            GHLOG(GH_LOG_DEBUG, GH_TAG) << "GetProcAddress(\"GameAssembly.dll\", \"" << lpProcName << "\") called.\n";

            // store original pointer in hook maps for lookup in hook functions
            GolfHook::hook_map_proc[i.proc] = ret;
            GolfHook::hook_map_name[i.name] = ret;

            // return our hook function address instead
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
C_DLLEXPORT int UnityMain(void* hInstance, void* hPrevInstance, wchar_t* lpCmdLine, int nCmdShow) {
    GHLOG(GH_LOG_TRACE, GH_TAG) << "UnityMain() called\n";

    // load original UnityPlayer.dll
    static void* hUnityPlayer = Util::dll_load("UnityPlayer_orig.dll");
    static decltype(&UnityMain) UnityMain_orig = nullptr;

    if (!hUnityPlayer) {
        GHLOG(GH_LOG_FATAL, GH_TAG) << "Unable to load UnityPlayer_orig.dll! Shutting down.\n";
        return -1;
    }

    if (!UnityMain_orig)
        UnityMain_orig = (decltype(UnityMain_orig))Util::dll_symbol(hUnityPlayer, "UnityMain");

    if (!UnityMain_orig) {
        GHLOG(GH_LOG_FATAL, GH_TAG) << "Unable to load UnityMain from UnityPlayer_orig.dll! Shutting down.\n";
        return -1;
    }

#if defined(_WIN32)
    const char* func = "GetProcAddress";
#elif defined(__linux__)
    const char* func = "dlsym";
#endif
    // hook GetProcAddress called from UnityPlayer_orig.dll
    if (!install_IAT_hook(hUnityPlayer, "kernel32.dll", func, &UnityPlayer_GetProcAddress_hook)) {
        GHLOG(GH_LOG_FATAL, GH_TAG) << "Unable to hook UnityPlayer_orig.dll's " << func << "! Shutting down.\n";
        return -1;
    }

    int ret = UnityMain_orig(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

    GHLOG(GH_LOG_TRACE, GH_TAG) << "UnityMain() returning " << ret << "\n";

    return ret;
}


/**
* Generate a new console window for standard in/out/err.
*
* From https://stackoverflow.com/a/191880/809900
*/
static void RedirectIOToConsole() {
#if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO coninfo;
    FILE* fp;

    // allocate a console for this app
    AllocConsole();

    // set the screen buffer to be big enough to let us scroll text
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
    coninfo.dwSize.Y = 500; // maximum mumber of lines the output console should have
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

    // redirect unbuffered STDOUT to the console
    freopen_s(&fp, "CONOUT$", "w", stdout);
    setvbuf(stdout, NULL, _IONBF, 0);

    // redirect unbuffered STDIN to the console
    freopen_s(&fp, "CONIN$", "r", stdin);
    setvbuf(stdin, NULL, _IONBF, 0);

    // redirect unbuffered STDERR to the console
    freopen_s(&fp, "CONOUT$", "w", stderr);
    setvbuf(stderr, NULL, _IONBF, 0);

    // make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
    // point to console as well
    std::ios::sync_with_stdio();
#endif  // _WIN32
}


// Initialize console, logging, and config
int __stdcall DllMain(void* hModule, unsigned long ul_reason_for_call, void*) {
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

        /*
        if (MH_Initialize() != MH_OK) {
            GHLOG(GH_LOG_FATAL, GH_TAG) << "MinHook failed to initialize.\n";
            return FALSE;
        }
        */
    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
        // MH_Uninitialize();

        GHLOG(GH_LOG_NOTICE, GH_TAG) << "GolfHook shutting down.\n";
    }

    return TRUE;
}

#if defined(__linux__)
// GCC constructor to route to DllMain
C_DLLEXPORT void __attribute__((constructor)) dll_init() {
    Dl_info dli;
    memset(&dli, 0, sizeof(dli));

    if (!dladdr((void*)dll_init, &dli))
        return nullptr;

    (void)DllMain(dli.dli_fbase, 1 /* DLL_PROCESS_ATTACH */, nullptr);
}

// GCC destructor to route to DllMain
C_DLLEXPORT void __attribute__((destructor)) dll_fini() {
    (void)DllMain(hMe, 0 /* DLL_PROCESS_DETACH */, nullptr);
}
#endif // __linux__

