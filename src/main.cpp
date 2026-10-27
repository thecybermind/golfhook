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
#include <string>

#include <Aetherim/src/wrapper.hpp>
#include "log.hpp"

#include "main.h"


namespace GolfHook {
    Image* Assembly;

    std::map<FARPROC, FARPROC> hook_map_proc;
    std::map<std::string, FARPROC> hook_map_name;

    int64_t il2cpp_init_hook(const char* domain) {
        GHLOG(GH_LOG_TRACE, GH_TAG) << "il2cpp_init_hook(\"" << domain << "\") called.\n";

        decltype(&il2cpp_init_hook) orig = (decltype(orig))hook_map_proc[(FARPROC)&il2cpp_init_hook];

        GHLOG(GH_LOG_TRACE, GH_TAG) << "il2cpp_init() called.\n";
        int64_t ret = orig(domain);
        GHLOG(GH_LOG_TRACE, GH_TAG) << "il2cpp_init() returned " << ret << "\n";

        GHLOG(GH_LOG_INFO, GH_TAG) << "Initializing Aetherim.\n";
        Il2cpp::initialize();
        const auto Aetherim = std::make_unique<Wrapper>();

        GHLOG(GH_LOG_DEBUG, GH_TAG) << "Aetherim image count: " << Aetherim->get_images().size() << "\n";
        for (auto& i : Aetherim->get_images()) {
            GHLOG(GH_LOG_DEBUG, GH_TAG) << "Image: \"" << i->get_name() << "\"\n";
        }

        Assembly = Aetherim->get_image("Assembly-CSharp.dll");
        if (!Assembly) {
            GHLOG(GH_LOG_FATAL, GH_TAG) << "Unable to load Assembly-CSharp image. Shutting down.\n";
            return 0;
        }

        const auto stats = Assembly->get_class("GolfStatsUtility");
        GHLOG(GH_LOG_DEBUG, GH_TAG) << "GolfStatsUtility class: " << stats;

        GHLOG(GH_LOG_TRACE, GH_TAG) << "il2cpp_init_hook(\"" << domain << "\") returning " << ret << ".\n";

        return ret;
    }

    void il2cpp_shutdown_hook() {
        GHLOG(GH_LOG_TRACE, GH_TAG) << "il2cpp_shutdown_hook() called.\n";

        decltype(&il2cpp_shutdown_hook) orig = (decltype(orig))hook_map_proc[(FARPROC)&il2cpp_shutdown_hook];

        GHLOG(GH_LOG_TRACE, GH_TAG) << "il2cpp_shutdown() called.\n";
        orig();
        GHLOG(GH_LOG_TRACE, GH_TAG) << "il2cpp_shutdown() returned.\n";

        GHLOG(GH_LOG_TRACE, GH_TAG) << "il2cpp_shutdown_hook() returning.\n";
    }


} // namespace GolfHook
