/*
GolfHook - Golf With Your Friends Hook
Copyright 2026
https://github.com/thecybermind/golfhook/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

#if defined(_WIN32)
 // #include <MinHook/MinHook.h>
 // #pragma comment(lib, "..\\lib\\MinHook.x64.lib")
 // #pragma comment(lib, "..\\lib\\libMinHook.x64.lib")
 #define APICONV __stdcall
 #define GPA_FUNC "GetProcAddress"
#elif defined(__linux__)
 #define APICONV
 #define GPA_FUNC "dlsym"
#endif

#include "dllmain.h"
#include "hook_iat.h"
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
    { "il2cpp_init", (void*)GolfHook::il2cpp_init_hook },
    { "il2cpp_shutdown", (void*)GolfHook::il2cpp_shutdown_hook },
};


void* APICONV UnityPlayer_GetProcAddress_hook(void* hModule, const char* lpProcName) {
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

    // hook GetProcAddress called from UnityPlayer_orig.dll
    if (!install_IAT_hook(hUnityPlayer, "kernel32.dll", GPA_FUNC, (void*)&UnityPlayer_GetProcAddress_hook)) {
        GHLOG(GH_LOG_FATAL, GH_TAG) << "Unable to hook UnityPlayer_orig.dll's " GPA_FUNC "! Shutting down.\n";
        return -1;
    }

    int ret = UnityMain_orig(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

    GHLOG(GH_LOG_TRACE, GH_TAG) << "UnityMain() returning " << ret << "\n";

    return ret;
}


// Initialize console, logging, and config
int APICONV DllMain(void* hModule, unsigned long ul_reason_for_call, void*) {
    if (ul_reason_for_call == 1 /* DLL_PROCESS_ATTACH */) {
        hMe = hModule;

        Util::setup_console();

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
    else if (ul_reason_for_call == 0 /* DLL_PROCESS_DETACH */) {
        // MH_Uninitialize();

        GHLOG(GH_LOG_NOTICE, GH_TAG) << "GolfHook shutting down.\n";
    }

    return 1;
}

#if defined(__linux__)
// GCC constructor to route to DllMain
C_DLLEXPORT void __attribute__((constructor)) dll_init() {
    void* handle = Util::dll_handle_ptr((void*)&dll_init);
    if (handle)
        (void)DllMain(handle, 1 /* DLL_PROCESS_ATTACH */, nullptr);
}

// GCC destructor to route to DllMain
C_DLLEXPORT void __attribute__((destructor)) dll_fini() {
    (void)DllMain(hMe, 0 /* DLL_PROCESS_DETACH */, nullptr);
}
#endif // __linux__

