/*
GolfHook - Golf With Your Friends Hook
Copyright 2026
https://github.com/thecybermind/golfhook/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

#pragma once

#ifndef GOLFHOOK_HOOK_IAT_H
#define GOLFHOOK_HOOK_IAT_H

void* install_IAT_hook(void* target_module, const char* dll_name, const char* function_name, void* function_hook);

#endif // GOLFHOOK_HOOK_IAT_H
