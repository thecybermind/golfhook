/*
GolfHook - Golf With Your Friends Hook
Copyright 2026
https://github.com/thecybermind/golfhook/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "main.h"
#include <Aetherim/src/wrapper.hpp>
#include "log.hpp"

// https://github.com/Toxocious/Aetherim

std::unique_ptr<Wrapper> g_Aetherim;

void Init(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    (void)hInstance; (void)hPrevInstance; (void)lpCmdLine; (void)nCmdShow;

    GHLOG(GH_LOG_DEBUG, GH_TAG) << "Init() called!\n";

    // find a better hook into unity stuff
    // can't initialize Il2cpp before UnityMain, and UnityMain doesn't return

    // Il2cpp::initialize();
    
    // g_Aetherim = std::make_unique<Wrapper>();

    //const auto image = g_Aetherim->get_image("GameAssembly.dll");
    //GHLOG(GH_LOG_DEBUG, GH_TAG) << "Assembly image: " << image;

    //const auto stats = image->get_class("GolfStatsUtility");
    //GHLOG(GH_LOG_DEBUG, GH_TAG) << "GolfStatsUtility class: " << stats;

    GHLOG(GH_LOG_DEBUG, GH_TAG) << "Init() returning!\n";
}

