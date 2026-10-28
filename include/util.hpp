/*
GolfHook - Golf With Your Friends Hook
Copyright 2026
https://github.com/thecybermind/golfhook/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/


#ifndef GOLFHOOK_UTIL_HPP
#define GOLFHOOK_UTIL_HPP

#include <vector>
#include <string>
#include <cstddef>    // size_t

namespace Util {

    // ---------------------------------
    // ----- Path parsing/handling -----
    // ---------------------------------

    /**
    * @brief Normalizes path.
    *
    * This includes collapsing . & .., using / for path separator, etc.
    *
    * @param path Path
    * @return Normalized path
    */
    std::string path_normalize(const std::string& path);

    /**
    * @brief Gets the directory name component of a path.
    *
    * @param path Path
    * @return Directory name component of path
    */
    std::string path_dirname(const std::string& path);

    /**
    * @brief Gets the filename (or topmost) component of a path.
    *
    * @param path Path
    * @return Filename component of path
    */
    std::string path_basename(const std::string& path);

    /**
    * @brief Gets the file extension component of a path.
    *
    * @param path Path
    * @return File extension component of path
    */
    std::string path_baseext(const std::string& path);

    /**
    * @brief Is the path absolute?
    *
    * @param path Path
    * @return true if path is absolute, false otherwise
    */
    bool path_is_absolute(const std::string& path);

    /**
    * @brief Is the path relative?
    *
    * @param path Path
    * @return true if path is relative, false otherwise
    */
    bool path_is_relative(const std::string& path);

    /**
    * @brief Creates a directory including all missing parent directories.
    *
    * @param path Path
    */
    void path_mkdir(const std::string& path);

    // ---------------------------
    // ----- DLL interaction -----
    // ---------------------------

    /**
    * @brief Load a DLL into the process.
    *
    * This calls LoadLibraryA in Windows, and dlopen in linux.
    *
    * @param filename Path to DLL
    * @return handle for loaded module
    */
    void* dll_load(const char* filename);

    /**
    * @brief Get a named function/variable from a given DLL.
    *
    * This calls GetProcAddress in Windows, and dlsym in linux.
    *
    * @param dll Module handle for DLL
    * @param symbol Name of function/variable to lookup
    * @return pointer to function/variable
    */
    void* dll_symbol(void* dll, const char* symbol);

    /**
    * @brief Unloads a DLL from the process.
    *
    * This calls FreeLibrary in Windows, and dlclose in linux.
    *
    * @param dll Module handle for DLL
    * @return true if unload successful, false otherwise
    */
    bool dll_close(void* dll);

    /**
    * @brief Gets the most recent DLL error.
    *
    * This calls GetLastError in Windows, and dlerror in linux.
    *
    * @return most recent DLL error message
    */
    const char* dll_error();

    // -----------------------------
    // ----- String comparison -----
    // -----------------------------

    /**
    * @brief Convert string to lowercase.
    *
    * @param str String to convert
    * @return converted string
    */
    std::string str_tolower(std::string str);

    /**
    * @brief Convert string to uppercase.
    *
    * @param str String to convert
    * @return converted string
    */
    std::string str_toupper(std::string str);

    /**
    * @brief Case-insensitive search for a string inside a another string.
    *
    * @param haystack String to search in
    * @param needle String to search for
    * @return true if needle is in haystack, false otherwise
    */
    bool str_stristr(const std::string& haystack, const std::string& needle);

    /**
    * @brief Case-insensitive string comparison.
    *
    * @param s1 String to compare
    * @param s2 String to compare
    * @return 0 if s1 and s2 compare equal, less-than-zero if s1 compares less than s2, or greater-than-zero if s1 compares greater than s2
    */
    int str_stricmp(const std::string& s1, const std::string& s2);

    /**
    * @brief Case-insensitive string comparison.
    *
    * @param s1 String to compare
    * @param s2 String to compare
    * @return true if s1 compares equal to s2, false otherwise
    */
    bool str_striequal(const std::string& s1, const std::string& s2);

    /**
    * @brief A "safe" strncpy that always null-terminates.
    *
    * @param dest Buffer to write to
    * @param src Buffer to read from
    * @param count Size of dest buffer
    * @return same pointer as dest
    */
    char* strncpyz(char* dest, const char* src, size_t count);

    // Helper type for horrible_cast
    template <class OutputClass, class InputClass>
    union horrible_union {
        OutputClass out;
        InputClass in;
    };

    /**
    * @brief Casts anything to anything.
    *
    * Used primarily to convert intptr_t to float in ROUTE_IMPORT/ROUTE_EXPORT calls without "*(float*)&arg0".
    *
    * @param InputClass any type
    * @param OutputClass any type
    * @param input an object of type InputClass
    * @return input object re-interpreted as an OutputClass
    */
    template <class OutputClass, class InputClass>
    inline OutputClass horrible_cast(const InputClass input) {
        horrible_union<OutputClass, InputClass> u;
        u.in = input;
        return u.out;
    }

}   // namespace Util

// Allow this to be usable outside of the namespace like a "regular" C++-style cast
using Util::horrible_cast;

#endif // GOLFHOOK_UTIL_HPP
