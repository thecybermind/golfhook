/*
GolfHook - Golf With Your Friends Hook
Copyright 2026
https://github.com/thecybermind/golfhook/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

#ifndef GOLFHOOK_PLUGIN_HPP
#define GOLFHOOK_PLUGIN_HPP

#include <vector>
#include <map>
#include <string>
#include "ghapi.hpp"

// A GolfHook plugin
struct Plugin {
    // GolfHook_Query signature
    using plugin_query = void (*)(plugin_info** pinfo, plugin_funcs** pluginfuncs, plugin_vars** pluginvars);
    // GolfHook_Attach signature
    using plugin_attach = bool (*)();
    // GolfHook_Detach signature
    using plugin_detach = void (*)();

    void* dll = nullptr;                                // Plugin DLL handle
    std::string path;                                   // Plugin path
    plugin_query GolfHook_Query = nullptr;              // GolfHook_Query function pointer
    plugin_attach GolfHook_Attach = nullptr;            // GolfHook_Attach function pointer
    plugin_detach GolfHook_Detach = nullptr;            // GolfHook_Detach function pointer
    plugin_info* plugininfo = nullptr;                  // Plugin-provided info
    plugin_funcs* pluginfuncs = nullptr;                // Plugin-provided function structure
    plugin_vars* pluginvars = nullptr;                  // Plugin-provided var structure

    Plugin();
    ~Plugin();
    // delete copy constructor/assignment since a Plugin object owns the DLL pointer
    Plugin(const Plugin& other) = delete;
    Plugin& operator=(const Plugin& other) = delete;
    Plugin(Plugin&& other) noexcept;
    Plugin& operator=(Plugin&& other) noexcept;

    /**
    * @brief Load a GolfHook plugin
    *
    * @param file Path to Plugin DLL
    * @return true if load successful, false otherwise
    */
    bool Load(std::string file);

    /**
    * @brief Unload a GolfHook plugin
    *
    */
    void Unload();

    /**
    * @brief Update a plugin's pluginvars
    */
    void UpdateVars();
};

// List of GolfHook plugins
extern std::vector<Plugin> g_plugins;

#endif // GOLFHOOK_PLUGIN_HPP
