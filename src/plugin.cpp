/*
GolfHook - Golf With Your Friends Hook
Copyright 2026
https://github.com/thecybermind/golfhook/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

#define _CRT_SECURE_NO_WARNINGS

#include <cstdarg>
#include <vector>
#include <string>
#include <filesystem>

#include "ghapi.hpp"
#include "format.hpp"
#include "log.hpp"
#include "config.hpp"
#include "main.hpp"     // GolfHook namespace
#include "plugin.hpp"   // g_plugins
#include "util.hpp"


constexpr int ROTATING_BUFFER_NUM = 16;  // must be power of 2
constexpr int ROTATING_BUFFER_MASK = ROTATING_BUFFER_NUM - 1;
constexpr int ROTATING_BUFFER_SIZE = 1024;

static void s_plugin_helper_WriteGHLog(plugin_id plid, int severity, const char* fmt, ...);

// Struct of plugin helper functions
static plugin_funcs s_plugin_funcs = {
    s_plugin_helper_WriteGHLog,
};

// Variables made available to plugins
plugin_vars s_plugin_vars = {
    nullptr, // assembly
};

// List of QMM plugins
std::vector<Plugin> g_plugins;


Plugin::Plugin() : dll(nullptr), GolfHook_Query(nullptr), GolfHook_Attach(nullptr), GolfHook_Detach(nullptr),
    plugininfo(nullptr), pluginfuncs(nullptr), pluginvars(nullptr)
{
}


Plugin::~Plugin() {
    this->Unload();
}


Plugin::Plugin(Plugin&& other) noexcept : Plugin() {
    if (this == &other)
        return;

    *this = std::move(other);
}


Plugin& Plugin::operator=(Plugin&& other) noexcept {
    if (this == &other)
        return *this;

    // QMM_Detach is called for unloading plugins by Unload/destructor, so we should swap as much as possible
    // in case some plugin wants to do stupid stuff in QMM_Detach
    std::swap(this->dll, other.dll);
    std::swap(this->path, other.path);
    std::swap(this->GolfHook_Query, other.GolfHook_Query);
    std::swap(this->GolfHook_Attach, other.GolfHook_Attach);
    std::swap(this->GolfHook_Detach, other.GolfHook_Detach);
    std::swap(this->plugininfo, other.plugininfo);
    std::swap(this->pluginfuncs, other.pluginfuncs);
    std::swap(this->pluginvars, other.pluginvars);

    return *this;
}


bool Plugin::Load(std::string file) {
    // if this plugin somehow already has a dll pointer, wipe it first
    if (this->dll)
        this->Unload();

    // load DLL
    if (!(this->dll = Util::dll_load(file.c_str()))) {
        GHLOG(GH_LOG_ERROR, GH_TAG) << "plugin_load(\"" << file << "\"): DLL load failed for plugin: " << Util::dll_error() << "\n";
        return false;
    }

    // if this DLL is the same as another loaded plugin, cancel
    for (Plugin& t : g_plugins) {
        if (this->dll == t.dll) {
            GHLOG(GH_LOG_ERROR, GH_TAG) << "plugin_load(\"" << Util::path_basename(file) << "\"): DLL is already loaded as plugin\n";
            return false;
        }
    }

    if (!(this->GolfHook_Query = (Plugin::plugin_query)Util::dll_symbol(this->dll, "GolfHook_Query"))) {
        GHLOG(GH_LOG_ERROR, GH_TAG) << "plugin_load(\"" << Util::path_basename(file) << "\"): Unable to find \"GolfHook_Query\" function\n";
        return false;
    }

    // call initial plugin entry point, get interface version and struct pointers
    this->GolfHook_Query(&this->plugininfo, &this->pluginfuncs, &this->pluginvars);
    if (!this->plugininfo) {
        GHLOG(GH_LOG_ERROR, GH_TAG) << "plugin_load(\"" << Util::path_basename(file) << "\"): GolfHook_Query() returned NULL plugininfo\n";
        return false;
    }

    // if the plugin's major interface version is lower, don't load and suggest to upgrade plugin
    if (this->plugininfo->pifv_major < GH_PIFV_MAJOR) {
        GHLOG(GH_LOG_ERROR, GH_TAG) << "plugin_load(\"" << Util::path_basename(file) << "\"): Plugin's interface version (" << this->plugininfo->pifv_major << ":" << this->plugininfo->pifv_minor << ") is less than GolfHook's (" << GH_PIFV_MAJOR << ":" << GH_PIFV_MINOR << "), suggest upgrading plugin.\n";
        return false;
    }
    // if the plugin's interface version is higher, don't load and suggest to upgrade GolfHook
    else if (this->plugininfo->pifv_major > GH_PIFV_MAJOR || this->plugininfo->pifv_minor > GH_PIFV_MINOR) {
        GHLOG(GH_LOG_ERROR, GH_TAG) << "plugin_load(\"" << Util::path_basename(file) << "\"): Plugin's interface version (" << this->plugininfo->pifv_major << ":" << this->plugininfo->pifv_minor << ") is greater than GolfHook's (" << GH_PIFV_MAJOR << ":" << GH_PIFV_MINOR << "), suggest upgrading GolfHook.\n";
        return false;
    }

    // at this point, major versions match and the plugin's minor version is less than or equal to GolfHook's.

    // make sure the plugin gave us the funcs and vars struct pointers
    if (!this->pluginfuncs) {
        GHLOG(GH_LOG_ERROR, GH_TAG) << "plugin_load(\"" << Util::path_basename(file) << "\"): GolfHook_Query() returned NULL pluginfuncs\n";
        return false;
    }
    if (!this->pluginvars) {
        GHLOG(GH_LOG_ERROR, GH_TAG) << "plugin_load(\"" << Util::path_basename(file) << "\"): GolfHook_Query() returned NULL pluginvars\n";
        return false;
    }

    // now fill the plugin's function and variable tables
    s_plugin_vars.assembly = GolfHook::Assembly;
    *this->pluginfuncs = s_plugin_funcs;
    *this->pluginvars = s_plugin_vars;

    // find remaining GolfHook api functions or fail
    if (!(this->GolfHook_Attach = (Plugin::plugin_attach)Util::dll_symbol(this->dll, "GolfHook_Attach"))) {
        GHLOG(GH_LOG_ERROR, GH_TAG) << "plugin_load(\"" << Util::path_basename(file) << "\"): Unable to find \"GolfHook_Attach\" function\n";
        return false;
    }
    if (!(this->GolfHook_Detach = (Plugin::plugin_detach)Util::dll_symbol(this->dll, "GolfHook_Detach"))) {
        GHLOG(GH_LOG_ERROR, GH_TAG) << "plugin_load(\"" << Util::path_basename(file) << "\"): Unable to find \"GolfHook_Detach\" function\n";
        return false;
    }

    // call GolfHook_Attach. if it fails (returns false), destructor will call GolfHook_Detach and unload DLL
    if (!(this->GolfHook_Attach())) {
        GHLOG(GH_LOG_ERROR, GH_TAG) << "plugin_load(\"" << Util::path_basename(file) << "\"): GolfHook_Attach() returned 0\n";
        return false;
    }

    this->path = file;

    return true;
}


void Plugin::Unload() {
    if (this->dll && this->GolfHook_Detach)
        this->GolfHook_Detach();
    Util::dll_close(this->dll);
    this->dll = nullptr;
    this->path.clear();
    this->GolfHook_Query = nullptr;
    this->GolfHook_Attach = nullptr;
    this->GolfHook_Detach = nullptr;
    this->plugininfo = nullptr;
    this->pluginfuncs = nullptr;
    this->pluginvars = nullptr;
}


void Plugin::UpdateVars() {
    return;
}


/**
* @brief Write to the GolfHook log.
*
* @param plid Plugin ID of the calling plugin
* @param severity Log message severity
* @param fmt Format string
* @param ... Format arguments
*/
static void s_plugin_helper_WriteGHLog(plugin_id plid, int severity, const char* fmt, ...) {
    if (!fmt) {
        GHLOG(GH_LOG_TRACE, GH_TAG) << "Plugin \"" << ((plugin_info*)plid)->name << "\" called WriteGHLog() with null fmt\n";
        return;
    }

    if (severity < GH_LOG_TRACE || severity > GH_LOG_FATAL)
        severity = GH_LOG_INFO;

    // if log severity is below thresholds, don't log
    if (!Log::log_level_match(severity))
        return;

    // get log tag from plugin
    plugin_info* plinfo = (plugin_info*)plid;
    const char* logtag = plinfo->logtag;
    if (!logtag || !*logtag)
        logtag = plinfo->name;

    va_list argptr;
    static char buf[1024];

    va_start(argptr, fmt);
    vsnprintf(buf, sizeof(buf), fmt, argptr);
    va_end(argptr);

    // not GHLOG since we already checked log_level_match()
    LOG(severity, Util::str_toupper(logtag)) << buf << std::flush;
}
