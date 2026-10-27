/*
QMM - Q3 MultiMod
Copyright 2025-2026
https://github.com/thecybermind/qmm2/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

#ifndef GOLFHOOK_DLLMAIN_H
#define GOLFHOOK_DLLMAIN_H

#define DLLEXPORT __declspec(dllexport)
#ifdef __cplusplus
#define C_DLLEXPORT extern "C" DLLEXPORT
#else
#define C_DLLEXPORT DLLEXPORT
#endif

extern HMODULE hMe;

void RedirectIOToConsole();

#endif // GOLFHOOK_DLLMAIN_H
