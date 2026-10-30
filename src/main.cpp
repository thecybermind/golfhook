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
#include <map>
#include <memory>

#include "il2cpp.hpp"
#include "log.hpp"
#include "main.hpp"
#include "plugin.hpp"
#include "config.hpp"


namespace GolfHook {
    std::unique_ptr<IL2CPP::Manager> Manager;
    IL2CPP::IImage* Assembly;

    std::map<FARPROC, FARPROC> hook_map_proc;
    std::map<std::string, FARPROC> hook_map_name;

    int64_t il2cpp_init_hook(const char* domain) {
        GHLOG(GH_LOG_TRACE, GH_TAG) << "il2cpp_init_hook(\"" << domain << "\") called.\n";

        // get original function out of hook map
        decltype(&il2cpp_init_hook) orig = (decltype(orig))hook_map_proc[(FARPROC)&il2cpp_init_hook];

        GHLOG(GH_LOG_TRACE, GH_TAG) << "il2cpp_init() called.\n";
        int64_t ret = orig(domain); // call original function
        GHLOG(GH_LOG_TRACE, GH_TAG) << "il2cpp_init() returned " << ret << "\n";

        // at this point, the Unity engine has initialized IL2CPP, so begin looking inside
        GHLOG(GH_LOG_INFO, GH_TAG) << "Initializing IL2CPP.\n";
        IL2CPP::initialize();
        Manager = std::make_unique<IL2CPP::Manager>();

        // output some debug info about the IL2CPP environment
        GHLOG(GH_LOG_DEBUG, GH_TAG) << "IL2CPP debugger attached: " << (Manager->is_debugger_attached() ? "true" : "false") << "\n";
        GHLOG(GH_LOG_DEBUG, GH_TAG) << "IL2CPP image count: " << Manager->get_images().size() << "\n";
        for (auto i : Manager->get_images()) {
            if (!i)
                continue;
            GHLOG(GH_LOG_DEBUG, GH_TAG) << "\tIL2CPP image: \"" << i->get_name() << "\"\n";
        }

        Assembly = Manager->get_image("Assembly-CSharp.dll");
        if (!Assembly) {
            GHLOG(GH_LOG_FATAL, GH_TAG) << "Unable to load Assembly-CSharp IL2CPP image. Shutting down.\n";
            return 0;
        }

        // output some debug info about the assembly
        GHLOG(GH_LOG_DEBUG, GH_TAG) << "Assembly class count: " << Assembly->get_classes().size() << "\n";
        for (auto& i : Assembly->get_classes()) {
            if (!i)
                continue;
            IL2CPP::IClass* c = (IL2CPP::IClass*)i;
            GHLOG(GH_LOG_DEBUG, GH_TAG) << "\tAssembly class: \"" << c->get_name() << "\" (" << c->get_fields().size() << " fields)\n";
        }

        // load plugins
        GHLOG(GH_LOG_NOTICE, GH_TAG) << "Loading plugins...\n";
        for (std::string& plugin_path : Config::cfg_get_array_str(g_cfg, "plugins")) {
            GHLOG(GH_LOG_INFO, GH_TAG) << "Attempting to load plugin \"" << plugin_path << "\"...\n";
            Plugin p;
            if (p.Load(plugin_path)) {
                g_plugins.push_back(std::move(p));
                GHLOG(GH_LOG_INFO, GH_TAG) << "Plugin \"" << plugin_path << "\" loaded.\n";
            }
            else {
                GHLOG(GH_LOG_INFO, GH_TAG) << "Plugin \"" << plugin_path << "\" not loaded.\n";
            }
        }
        GHLOG(GH_LOG_NOTICE, GH_TAG) << g_plugins.size() << " plugin(s) loaded.\n";

        GHLOG(GH_LOG_TRACE, GH_TAG) << "il2cpp_init_hook(\"" << domain << "\") returning " << ret << ".\n";

        return ret;
    }

    void il2cpp_shutdown_hook() {
        GHLOG(GH_LOG_TRACE, GH_TAG) << "il2cpp_shutdown_hook() called.\n";

        // get original function out of hook map
        decltype(&il2cpp_shutdown_hook) orig = (decltype(orig))hook_map_proc[(FARPROC)&il2cpp_shutdown_hook];

        GHLOG(GH_LOG_TRACE, GH_TAG) << "il2cpp_shutdown() called.\n";
        orig(); // call original function
        GHLOG(GH_LOG_TRACE, GH_TAG) << "il2cpp_shutdown() returned.\n";

        GHLOG(GH_LOG_INFO, GH_TAG) << "Shutting down IL2CPP.\n";
        IL2CPP::shutdown();
            
        // unload plugins
        GHLOG(GH_LOG_NOTICE, GH_TAG) << "Unloading plugins...\n";
        for (Plugin& p : g_plugins) {
            p.Unload();
        }
        g_plugins.clear();
        GHLOG(GH_LOG_NOTICE, GH_TAG) << "Plugins unloaded.\n";

        GHLOG(GH_LOG_TRACE, GH_TAG) << "il2cpp_shutdown_hook() returning.\n";
    }


} // namespace GolfHook
