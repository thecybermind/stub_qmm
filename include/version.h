/*
STUB_QMM - Example QMM Plugin
Copyright 2004-2025
https://github.com/thecybermind/stub_qmm/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

#ifndef __STUB_QMM_VERSION_H__
#define __STUB_QMM_VERSION_H__

#define STRINGIFY(x) STRINGIFY2(x)
#define STRINGIFY2(x) #x

#define STUB_QMM_VERSION_MAJOR	2
#define STUB_QMM_VERSION_MINOR	4
#define STUB_QMM_VERSION_REV	1

#define STUB_QMM_VERSION		STRINGIFY(STUB_QMM_VERSION_MAJOR) "." STRINGIFY(STUB_QMM_VERSION_MINOR) "." STRINGIFY(STUB_QMM_VERSION_REV)

#if defined(_WIN32)
#define STUB_QMM_OS             "Windows"
#ifdef _WIN64
#define STUB_QMM_ARCH           "x86_64"
#else
#define STUB_QMM_ARCH           "x86"
#endif
#elif defined(__linux__)
#define STUB_QMM_OS             "Linux"
#ifdef __LP64__
#define STUB_QMM_ARCH           "x86_64"
#else
#define STUB_QMM_ARCH           "x86"
#endif
#endif

#define STUB_QMM_VERSION_DWORD	STUB_QMM_VERSION_MAJOR , STUB_QMM_VERSION_MINOR , STUB_QMM_VERSION_REV , 0
#define STUB_QMM_COMPILE		__TIME__ " " __DATE__
#define STUB_QMM_BUILDER		"cybermind"

#endif // __STUB_QMM_VERSION_H__
