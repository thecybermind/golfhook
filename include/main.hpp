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

#include "iat_hook.h"
#include "il2cpp.hpp"

namespace GolfHook {
    extern IL2CPP::Manager Manager;
    extern IL2CPP::IImage* Assembly;

    extern std::map<void*, void*> hook_map_proc;
    extern std::map<std::string, void*> hook_map_name;

    int64_t il2cpp_init_hook(const char* domain);
    void il2cpp_shutdown_hook();
} // namespace GolfHook


#endif // GOLFHOOK_MAIN_HPP
