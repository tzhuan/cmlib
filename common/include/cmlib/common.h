#ifndef CMLIB_COMMON_H
#define CMLIB_COMMON_H

/** @file cmlib/common.h
 *  @brief common header for all cmlib components.
 *  @author Tz-Huan Huang
 */

// set DLLAPI if we're using VC
#ifdef _MSC_VER
    #pragma warning(disable: 4996)

    #ifdef CMLIB_BUILD_DLL
        #define DLLAPI __declspec(dllexport)
    #elif defined(CMLIB_LINK_DLL)
        #define DLLAPI __declspec(dllimport)
    #else
        #define DLLAPI
    #endif // CMLIB_BUILD_DLL
#else
    #define DLLAPI
#endif // _MSC_VER

#endif // CMLIB_COMMON_H
