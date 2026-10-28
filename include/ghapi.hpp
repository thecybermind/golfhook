/*
GolfHook - Golf With Your Friends Hook
Copyright 2026
https://github.com/thecybermind/golfhook/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

#ifndef GOLFHOOK_GHAPI_HPP
#define GOLFHOOK_GHAPI_HPP

#include "aetherim.hpp"

// plugins and internal use
#if defined(_WIN32)
 #define DLLEXPORT __declspec(dllexport)
#elif defined(__linux__)
 #define DLLEXPORT __attribute__((visibility("default")))
#else
 #error Unknown OS?
#endif
#ifdef __cplusplus
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
    GHLOG_TRACE,    // Lowest level, only for detailed debugging
    GHLOG_DEBUG,    // Debugging information
    GHLOG_INFO,     // Normal information, displayed to game console
    GHLOG_NOTICE,   // Something of note
    GHLOG_WARNING,  // Warning, possible problems
    GHLOG_ERROR,    // Error, maybe recoverable
    GHLOG_FATAL,    // Fatal, unrecoverable
};

/* Prototype struct for GolfHook plugin utility functions */
typedef struct {
    void (*pfnWriteGHLog)(plugin_id plid, int severity, const char* fmt, ...);                          // Write to the GolfHook log
} plugin_funcs;

#define GH_WRITEGHLOG(sev, fmt, ...)    (g_pluginfuncs.pfnWriteGHLog)(PLID, sev, fmt, ## __VA_ARGS__)   // Write to the GolfHook log

/* Struct of vars for GolfHook plugins */
typedef struct {
    Aetherim::Image* assembly;
} plugin_vars;

/* Plugin use only */

// Set '*plugininfo' to &g_plugininfo in GolfHook_Query (or just use GH_GIVE_PTRS())
extern plugin_info g_plugininfo;
// Set '*pluginfuncs` to &g_pluginfuncs in GolfHook_Query (or just use GH_GIVE_PTRS())
extern plugin_funcs g_pluginfuncs;
// Set '*pluginvars` to &g_pluginvars in GolfHook_Query (or just use GH_GIVE_PTRS())
extern plugin_vars g_pluginvars;

// Pass pointers to QMM in QMM_Query
#define GH_GIVE_PTRS() do { \
            *plugininfo = &g_plugininfo; \
            *pluginfuncs = &g_pluginfuncs; \
            *pluginvars = &g_pluginvars; \
        } while (0)

/* Prototypes for entry points in the plugin */
C_DLLEXPORT void GolfHook_Query(plugin_info** plugininfo, plugin_funcs** pluginfuncs, plugin_vars** pluginvars);
C_DLLEXPORT bool GolfHook_Attach();
C_DLLEXPORT void GolfHook_Detach();

#endif // GOLFHOOK_GHAPI_HPP
