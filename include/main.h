/*
GolfHook - Golf With Your Friends Hook
Copyright 2026
https://github.com/thecybermind/golfhook/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

#ifndef GOLFHOOK_MAIN_H
#define GOLFHOOK_MAIN_H

#include <windef.h>
#include <Aetherim/src/wrapper.hpp>


namespace GolfHook {
    extern std::unique_ptr<Wrapper> g_Aetherim;

    int UnityMain_Pre(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow);
} // namespace GolfHook


#endif // GOLFHOOK_MAIN_H
