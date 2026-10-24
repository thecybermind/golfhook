/*
GolfHook - Golf With Your Friends Hook
Copyright 2026
https://github.com/thecybermind/golfhook/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

#ifndef GOLFHOOK_HOOK_H
#define GOLFHOOK_HOOK_H

#define WIN32_LEAN_AND_MEAN
#include <windef.h>

FARPROC install_IAT_hook(HMODULE target_module, const char* dll_name, const char* function_name, FARPROC function_hook);

#endif // GOLFHOOK_HOOK_H
