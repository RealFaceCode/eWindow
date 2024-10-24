#pragma once

#define EWIN_VERSION_MAJOR 0
#define EWIN_VERSION_MINOR 1
#define EWIN_VERSION_PATCH 0

#ifdef _WIN32
        #define DLL_EXPORT __declspec(dllexport)
        #define DLL_IMPORT __declspec(dllimport)
#else
        #define DLL_EXPORT __attribute__((visibility("default")))
        #define DLL_IMPORT __attribute__((visibility("default")))
#endif

#ifdef EWIN_EXPORTS
    #define EWIN_API DLL_EXPORT
#else
    #define EWIN_API DLL_IMPORT
#endif