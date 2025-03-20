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

#if defined(GAME_Q3A)
#include <q3a/game/q_shared.h>
#include <q3a/game/g_local.h>
#elif defined(GAME_RTCWMP)
#include <rtcwmp/game/q_shared.h>
#include <rtcwmp/game/g_local.h>
#elif defined(GAME_RTCWSP)
#include <rtcwsp/game/q_shared.h>
#include <rtcwsp/game/g_local.h>
#elif defined(GAME_JK2MP)
#include <jk2mp/game/q_shared.h>
#include <jk2mp/game/g_local.h>
#elif defined(GAME_JAMP)
#include <jamp/game/q_shared.h>
#include <jamp/game/g_local.h>
#elif defined(GAME_WET)
#include <wet/game/q_shared.h>
#include <wet/game/g_local.h>
#elif defined(GAME_STVOYMP)
#include <stvoymp/game/q_shared.h>
#include <stvoymp/game/g_local.h>
#elif defined(GAME_STVOYSP)
#include <game_stvoysp.h>
#elif defined(GAME_STEF2)
#include <game_stef2.h>
#elif defined(GAME_MOHAA)
#include <game_mohaa.h>
#endif

#endif // __STUB_QMM_GAME_H__
