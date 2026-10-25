/*
GolfHook - Golf With Your Friends Hook
Copyright 2026
https://github.com/thecybermind/golfhook/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

#include "main.h"
#include <Aetherim/src/wrapper.hpp>

// https://github.com/Toxocious/Aetherim

void PreInit() {
}

void PostInit() {
    Il2cpp::initialize();
    const auto Aetherim = std::make_unique<Wrapper>();
    const auto image = Aetherim->get_image("GameAssembly.dll");

    const auto player = image->get_class("?");
}

