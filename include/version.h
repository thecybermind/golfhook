/*
GolfHook - Golf With Your Friends Hook
Copyright 2026
https://github.com/thecybermind/golfhook/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

#ifndef GOLFHOOK_VERSION_H
#define GOLFHOOK_VERSION_H

// Evaluate and stringify a macro
#define STRINGIFY(x) STRINGIFY2(x)
#define STRINGIFY2(x) #x

// Major semver component
#define GH_VERSION_MAJOR    0
// Minor semver component
#define GH_VERSION_MINOR    0
// Revision semver component
#define GH_VERSION_REV      1
#define GH_VERSION_REV2     0

// String of dotted version number
#define GH_VERSION          STRINGIFY(GH_VERSION_MAJOR) "." STRINGIFY(GH_VERSION_MINOR) "." STRINGIFY(GH_VERSION_REV) "." STRINGIFY(GH_VERSION_REV2)

// When was the DLL built? Uses __TIME__ and __DATE__
#define GH_COMPILE          __TIME__ " " __DATE__
// Name of the builder
#define GH_BUILDER          "Kevin Masterson"
// GH URL
#define GH_URL              "https://github.com/thecybermind/golfhook/"

#if defined(_DEBUG)
 #define GH_BUILD "Debug"
#elif defined(NDEBUG)
 #define GH_BUILD "Release"
#else
 #define GH_BUILD "Unknown"
#endif

// Comma-separated DWORD form of version for golfhook.rc
#define GH_VERSION_DWORD    GH_VERSION_MAJOR , GH_VERSION_MINOR , GH_VERSION_REV , GH_VERSION_REV2

#endif // GOLFHOOK_VERSION_H
