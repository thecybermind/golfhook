/*
GolfHook - Golf With Your Friends Hook
Copyright 2026
https://github.com/thecybermind/golfhook/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/


#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cctype>
#include <cstring>
#include <cstddef>      // size_t
#include <vector>
#include <string>
#include <chrono>
#include <filesystem>

#include "util.hpp"
#include "format.hpp"


namespace Util {

    std::string path_normalize(const std::string& path) {
        auto fspath = std::filesystem::path(path).lexically_normal();
        // check for empty or starting with ".." just in case
        if (fspath.empty() || *fspath.begin() == "..")
            return "";
        return fspath.generic_u8string();
    }


    std::string path_dirname(const std::string& path) {
        return std::filesystem::path(path).parent_path().lexically_normal().generic_u8string();
    }


    std::string path_basename(const std::string& path) {
        return std::filesystem::path(path).filename().u8string();
    }


    std::string path_baseext(const std::string& path) {
        return std::filesystem::path(path).extension().u8string();
    }


    bool path_is_absolute(const std::string& path) {
        if (path.empty())
            return false;
        return std::filesystem::path(path).is_absolute();
    }


    bool path_is_relative(const std::string& path) {
        if (path.empty())
            return false;
        return std::filesystem::path(path).is_relative();
    }


    void path_mkdir(const std::string& path) {
        if (path.empty())
            return;

        std::filesystem::create_directories(path);
    }


    void* dll_load(const char* filename) {
        return (void*)LoadLibraryA(filename);
    }


    void* dll_symbol(void* dll, const char* symbol) {
        if (!dll || !symbol)
            return nullptr;
        // symbol could technically be an ordinal and not a string, but the ordinal can't be 0
        // so a null check works. null dll handle seems to be a no-op.
        return (void*)GetProcAddress((HMODULE)dll, symbol);
    }


    bool dll_close(void* dll) {
        if (!dll)
            return false;
        return (bool)FreeLibrary((HMODULE)dll);
    }


    const char* dll_error() {
        // this will return the last error from any win32 function, not just library functions
        static std::string str;
        char* buf = nullptr;
        str = "";

        DWORD err = GetLastError();

        DWORD ret = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&buf, 0, nullptr);

        if (ret && buf)
            str = buf;
        // generic message in case FormatMessageA doesn't know about error code
        else
            str = fmt::format("Unknown error #{}", err);

        LocalFree(buf); // no-op to pass NULL

        return str.c_str();
    }


    std::string str_tolower(std::string str) {
        for (char& c : str)
            c = (char)std::tolower((unsigned char)c);

        return str;
    }


    std::string str_toupper(std::string str) {
        for (char& c : str)
            c = (char)std::toupper((unsigned char)c);

        return str;
    }


    bool str_stristr(const std::string& haystack, const std::string& needle) {
        return str_tolower(haystack).find(str_tolower(needle)) != std::string::npos;
    }


    int str_stricmp(const std::string& s1, const std::string& s2) {
        return str_tolower(s1).compare(str_tolower(s2));
    }


    bool str_striequal(const std::string& s1, const std::string& s2) {
        return str_stricmp(s1, s2) == 0;
    }


    // "safe" strncpy that always null-terminates
    char* strncpyz(char* dest, const char* src, size_t count) {
        if (!dest || !src || !count)
            return dest;
        // strncpy may not null-terminate if it writes a full buffer
        char* ret = strncpy(dest, src, count);
        // so we null-terminate manually
        if (count)
            dest[count - 1] = '\0';
        else
            dest[0] = '\0';
        return ret;
    }

}   // namespace Util
