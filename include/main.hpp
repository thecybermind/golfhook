/*
GolfHook - Golf With Your Friends Hook
Copyright 2026
https://github.com/thecybermind/golfhook/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

#ifndef GOLFHOOK_MAIN_HPP
#define GOLFHOOK_MAIN_HPP

#include <windef.h>
#include <map>
#include <string>
#include <memory>

#include "il2cpp.hpp"

namespace GolfHook {
    extern std::unique_ptr<IL2CPP::Manager> Manager;
    extern IL2CPP::IImage* Assembly;

    extern std::map<FARPROC, FARPROC> hook_map_proc;
    extern std::map<std::string, FARPROC> hook_map_name;

    int64_t il2cpp_init_hook(const char* domain);
    void il2cpp_shutdown_hook();
} // namespace GolfHook


#endif // GOLFHOOK_MAIN_HPP
