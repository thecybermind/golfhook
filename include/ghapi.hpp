/*
GolfHook - Golf With Your Friends Hook
Copyright 2026
https://github.com/thecybermind/golfhook/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

#pragma once

#ifndef GOLFHOOK_GHAPI_HPP
#define GOLFHOOK_GHAPI_HPP

#include "il2cpp.hpp"       // IManager and IImage in plugin_vars
#include "hook_virtual.hpp" // VHook in plugin_funcs

// plugins and internal use
#if defined(_WIN32)
 #define DLLEXPORT __declspec(dllexport)
#elif defined(__linux__)
 #define DLLEXPORT __attribute__((visibility("default")))
#else
 #error Unknown OS?
#endif
#if defined(__cplusplus)
 #define C_DLLEXPORT extern "C" DLLEXPORT
#else
 #define C_DLLEXPORT DLLEXPORT
#endif

// Major interface version increases with breaking changes to signatures, types, etc
#define GH_PIFV_MAJOR  1
// Minor interface version increases with trailing addition to plugin_func or plugin_info structs
#define GH_PIFV_MINOR  0

/* PIFV changes
*/

// Plugin info to pass back to GolfHook
typedef struct {
    int pifv_major;         // Major plugin interface version
    int pifv_minor;         // Minor plugin interface version
    const char* name;       // Name of plugin
    const char* version;    // Version of plugin
    const char* desc;       // Description of plugin
    const char* author;     // Author of plugin
    const char* url;        // Website of plugin
    const char* logtag;     // Log tag (shows in golfhook.log)
} plugin_info;

// "Opaque" plugin info pointer to use as an identifier for plugin funcs.
typedef plugin_info* plugin_id;
// Macro for passing plugin_id to plugin funcs.
#define PLID ((plugin_id)&g_plugininfo)

/* Severity level for GH_WRITEGHLOG */
enum {
    GHLOG_TRACE,    // Lowest level, only for detailed call-trace level debugging
    GHLOG_DEBUG,    // Debugging information
    GHLOG_INFO,     // Normal information, displayed to game console
    GHLOG_NOTICE,   // Something of note
    GHLOG_WARNING,  // Warning, possible problems
    GHLOG_ERROR,    // Error, maybe recoverable
    GHLOG_FATAL,    // Fatal, unrecoverable
};

/* Prototype struct for GolfHook plugin utility functions */
struct plugin_funcs {
    virtual void WriteGHLog(plugin_id plid, int severity, const char* fmt, ...) = 0;    // Write to the GolfHook log with given severity
    virtual const char* VarArgs(plugin_id plid, const char* fmt, ...) = 0;              // Simple vsprintf helper with rotating buffer
    virtual void* HookIAT(plugin_id plid, void* target_module, const char* dll_name,
        const char* function_name, void* function_hook) = 0;                            // Hook IAT/PLT tables for a given function in a target module
    virtual VHook* HookVirtual_New(plugin_id plid) = 0;                                 // Return a pointer to a VHook object to hook a virtual function
    virtual void HookVirtual_Free(plugin_id plid, VHook* vhook) = 0;                    // Frees a VHook object returned by HookVirtual
};

#define GH_WRITEGHLOG(sev, fmt, ...)        (g_pluginfuncs->WriteGHLog)(PLID, sev, fmt, ## __VA_ARGS__) // Write to the GolfHook log with given severity
#define GH_VARARGS(fmt, ...)                (g_pluginfuncs->VarArgs)(PLID, fmt, __VA_ARGS__)            // Simple vsprintf helper with rotating buffer
#define GH_HOOKIAT(target, dll, func, hook) (g_pluginfuncs->HookIAT)(PLID, target, dll, func, hook)     // Hook IAT/PLT tables for a given function in a target module
#define GH_HOOKVIRTUAL_NEW()                    (g_pluginfuncs->HookVirtual_New)(PLID)                  // Return a pointer to a VHook object to hook a virtual function
#define GH_HOOKVIRTUAL_FREE(vhook)          (g_pluginfuncs->HookVirtual_Free)(PLID, vhook)              // Frees a VHook object returned by GH_HOOKVIRTUAL

/* Struct of vars for GolfHook plugins */
typedef struct {
    IL2CPP::IManager* IL2CPP_manager;                   // IManager* to the IL2CPP assembly manager
    IL2CPP::IImage* IL2CPP_assembly;                    // IImage* to the Assembly-CSharp.dll image
} plugin_vars;

#define GH_MANAGER      (g_pluginvars->IL2CPP_manager)  // IManager* to the IL2CPP assembly manager
#define GH_ASSEMBLY     (g_pluginvars->IL2CPP_assembly) // IImage* to the Assembly-CSharp.dll image

/* Plugin use only */

// Plugin's info struct to give to GolfHook (use GH_GIVE_PTRS() in GolfHook_Query)
extern plugin_info g_plugininfo;
// Pointer to GolfHook's pluginfuncs object (use GH_GIVE_PTRS() in GolfHook_Query)
extern plugin_funcs* g_pluginfuncs;
// Pointer to GolfHook's pluginvars object (use GH_GIVE_PTRS() in GolfHook_Query)
extern plugin_vars* g_pluginvars;

// Pass pointers to GolfHook in GolfHook_Query
#define GH_GIVE_PTRS() do { \
            *plugininfo = &g_plugininfo; \
            g_pluginfuncs = pluginfuncs; \
            g_pluginvars = pluginvars; \
        } while (0)

/* Prototypes for entry points in the plugin */
C_DLLEXPORT void GolfHook_Query(plugin_info** plugininfo, plugin_funcs* pluginfuncs, plugin_vars* pluginvars);
C_DLLEXPORT bool GolfHook_Attach();
C_DLLEXPORT void GolfHook_Detach();

#endif // GOLFHOOK_GHAPI_HPP
