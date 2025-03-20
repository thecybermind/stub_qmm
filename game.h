/*
STUB_QMM - Example QMM Plugin
Copyright 2004-2025
https://github.com/thecybermind/stub_qmm/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < cybermind@gmail.com >

*/

#ifndef __STUB_QMM_GAME_H__
#define __STUB_QMM_GAME_H__

#ifdef _WIN32
// 'typedef ': ignored on left of '<unnamed-enum>' when no variable is declared
#pragma warning(disable:4091)
#endif

#if defined(GAME_Q3A)
#include <q3a/game/g_local.h>
#elif defined(GAME_RTCWMP)
#include <rtcwmp/game/g_local.h>
#elif defined(GAME_RTCWSP)
#include <rtcwsp/game/g_local.h>
#elif defined(GAME_JK2MP)
#include <jk2mp/game/g_local.h>
#elif defined(GAME_JAMP)
#include <jamp/game/g_local.h>
#elif defined(GAME_WET)
#include <wet/game/g_local.h>
#elif defined(GAME_STVOYMP)
#include <stvoymp/game/g_local.h>
#elif defined(GAME_STVOYSP)
#define GAME_INCLUDE
#include <stvoysp/game/q_shared.h>
#include <stvoysp/game/g_local.h>
#include <game_stvoysp.h>
#elif defined(GAME_STEF2)
#include <stef2/game/g_local.h>
#include <game_stef2.h>
#elif defined(GAME_MOHAA)
#include <mohaa/fgame/g_local.h>
#include <game_mohaa.h>
#endif

#endif // __STUB_QMM_GAME_H__
