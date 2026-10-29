# GolfHook
GolfHook - Golf With Your Friends Hook  
Copyright 2026  
https://github.com/thecybermind/golfhook/  
3-clause BSD license: https://opensource.org/license/bsd-3-clause  
  
Created By: Kevin Masterson < k.m.masterson@gmail.com >  
  
---

_Work in progress._

GolfHook is a proxy DLL for [Golf With Your Friends](https://store.steampowered.com/app/431240/Golf_With_Your_Friends/) that waits for the Unity engine to start and then uses [Aetherim](https://github.com/Toxocious/Aetherim/) to get access to IL2CPP methods. GolfHook then provides the loaded Aetherim assembly image to individual plugins for them to hook things as needed.

## Thanks

Designed by:
  - Kevin Masterson

GolfHook uses the following libraries:  
  - [nlohmann/json](https://github.com/nlohmann/json)
  - [aixlog](https://github.com/badaix/aixlog)
  - [fmtlib](https://github.com/fmtlib/fmt)
  - [MinHook](https://github.com/TsudaKageyu/minhook/)
  - [Aetherim](https://github.com/Toxocious/Aetherim/)

See the LIBLICENSES file for each of these libraries' licenses.