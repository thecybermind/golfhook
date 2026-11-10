/*
GolfHook - Golf With Your Friends Hook
Copyright 2026
https://github.com/thecybermind/golfhook/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

#pragma once

#ifndef GOLFHOOK_MAIN_HPP
#define GOLFHOOK_MAIN_HPP

#include <map>
#include <string>
#include <memory>

#define IL2CPP_INTERNAL     // use internal location of DLL pointer struct
#include "il2cpp.hpp"
#undef IL2CPP_INTERNAL
#include "hook_iat.h"


namespace GolfHook {
    extern void* hModule;                               // GolfHook's DLL handle

    extern const IL2CPP::Manager Manager;               // IL2CPP Manager (loads Assemblies)
    extern const IL2CPP::Image* Assembly;               // Assembly-CSharp.dll assembly

    extern std::map<void*, void*> hook_map_proc;        // Map of hook procedure -> original procedure
    extern std::map<std::string, void*> hook_map_name;  // Map of original procedure name -> original procedure

    int64_t il2cpp_init_hook(const char* domain);
    void il2cpp_shutdown_hook();
} // namespace GolfHook


#endif // GOLFHOOK_MAIN_HPP
