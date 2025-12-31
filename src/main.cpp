/*
STUB_QMM - Example QMM Plugin
Copyright 2004-2025
https://github.com/thecybermind/stub_qmm/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < k.m.masterson@gmail.com >

*/

#define _CRT_SECURE_NO_WARNINGS 1

#include <qmmapi.h>

#include "version.h"
#include "game.h"

#include <string.h>

pluginres_t* g_result = nullptr;
plugininfo_t g_plugininfo = {
	QMM_PIFV_MAJOR,								// plugin interface version major
	QMM_PIFV_MINOR,								// plugin interface version minor
	"Stub_QMM",									// name of plugin
	STUB_QMM_VERSION,							// version of plugin
	"Stub/test plugin",							// description of plugin
	STUB_QMM_BUILDER,							// author of plugin
	"https://github.com/thecybermind/stub_qmm",	// website of plugin
	"STUB",										// log tag
};
eng_syscall_t g_syscall = nullptr;
mod_vmMain_t g_vmMain = nullptr;
pluginfuncs_t* g_pluginfuncs = nullptr;
pluginvars_t* g_pluginvars = nullptr;

// store the game's entity and client info
gentity_t* g_gents = nullptr;
intptr_t g_numgents = 0;
intptr_t g_gentsize = 0;
gclient_t* g_clients = nullptr;
intptr_t g_clientsize = 0;


/* QMM_Query
   This is the first function called by QMM
   Do not do anything here that requires shutdown routines, as there is no equivalent called at shutdown
    - pinfo = address of a pointer in QMM to this plugin's plugininfo struct
*/
C_DLLEXPORT void QMM_Query(plugininfo_t** pinfo) {
	// give QMM our plugin info struct
	QMM_GIVE_PINFO();
}


/* QMM_Attach
   This is the second function called by QMM.
   Keep in mind, this is called during QMM's handling of vmMain(GAME_INIT) *before* it has been routed to the mod,
   so the mod is completely uninitialized at this point, and is generally unsafe to call into. You can, however, use
   some engine functions through syscall.
    - engfunc     = pointer to the engine's syscall function
    - modfunc     = pointer to the mod's vmMain function
    - presult     = pointer to plugin result variable
	- pluginfuncs = pointer to table of plugin helper function pointers
    - pluginvars  = pointer to table of plugin helper variables
   
   return:
    - 0 = failure, QMM_Detach will be called and plugin will be unloaded
    - 1 = succeed, plugin will be loaded
*/
C_DLLEXPORT int QMM_Attach(eng_syscall_t engfunc, mod_vmMain_t modfunc, pluginres_t* presult, pluginfuncs_t* pluginfuncs, pluginvars_t* pluginvars) {
	QMM_SAVE_VARS();

	return 1;
}


/* QMM_Detach
   This is the last function called by QMM. This is called after returning false from QMM_Attach(), or during final shutdown.
   Keep in mind, the shutdown process occurs in QMM's vmMain(GAME_SHUTDOWN) *after* it has been routed to the mod, so the mod
   is completely shutdown at this point, and is generally unsafe to call into. You can, however, still use some engine functions
   through syscall.
    - reserved = reserved for future use
*/
C_DLLEXPORT void QMM_Detach() {
}


/* QMM_vmMain
   This is called BEFORE the mod's vmMain function is called (by engine).
   Keep in mind, if cmd==GAME_INIT, this function is called *before* it has been routed to the mod, so the mod is completely
   uninitialized at this point, and is generally unsafe to call into. You can, however, use some engine functions through
   syscall.
    - cmd  = command like GAME_INIT, GAME_CLIENT_COMMAND, etc. (game-specific)
	- args = arguments to cmd
*/
C_DLLEXPORT intptr_t QMM_vmMain(intptr_t cmd, intptr_t* args) {
	if (cmd == GAME_INIT) {
		// example showing writing to QMM log on initialization
		QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "Stub_QMM loaded! Game engine: %s\n", QMM_GETGAMEENGINE(PLID)), QMMLOG_INFO);
	}

	QMM_RET_IGNORED(0);
}


/* QMM_syscall
   This is called BEFORE the engine's syscall function is called (by mod).
   You would typically store entity information here in a cmd==G_LOCATE_GAME_DATA check
    - cmd = command like G_PRINT, G_LOCATE_GAME_DATA, etc. (game-specific)
	- args = arguments to cmd
*/
C_DLLEXPORT intptr_t QMM_syscall(intptr_t cmd, intptr_t* args) {
	// this is fairly common to store entity/client data. the second argument (num gentities) changes every time
	// a new entity is spawned, so this gets called a lot. no other args should change after the first call.
	// for QUAKE2, Q2R, SIN, STVOYSP, JASP, and JK2SP engines, this is a QMM polyfill call when it detects changes
	if (cmd == G_LOCATE_GAME_DATA) {
		g_gents = (gentity_t*)(args[0]);
		g_numgents = args[1];
		g_gentsize = args[2];
		g_clients = (gclient_t*)(args[3]);
		g_clientsize = args[4];
	}

	QMM_RET_IGNORED(0);
}


/* QMM_vmMain_Post
   This is called AFTER the mod's vmMain function is called (by engine)
   Keep in mind, if cmd==GAME_SHUTDOWN, this function is called *after* it has been routed to the mod, so the mod is completely
   shutdown at this point, and is generally unsafe to call into. You can, however, use some engine functions through syscall.
    - cmd = command like GAME_INIT, GAME_CLIENT_COMMAND, etc. (game-specific)
	- args = arguments to cmd

   In QMM_vmMain_Post functions, you can access *g_pluginvars->preturn to get the return value of the vmMain call that will be returned back to the engine
*/
C_DLLEXPORT intptr_t QMM_vmMain_Post(intptr_t cmd, intptr_t* args) {

	QMM_RET_IGNORED(0);
}


/* QMM_syscall_Post
   This is called AFTER the engine's syscall function is called (by mod)
    - cmd = command like G_PRINT, G_LOCATE_GAME_DATA, etc. (game-specific)
	- args = arguments to cmd

   In QMM_syscall_Post functions, you can access *g_pluginvars->preturn / QMM_VAR_RETURN to get the return value of the syscall call that will be returned
   back to the mod
*/
C_DLLEXPORT intptr_t QMM_syscall_Post(intptr_t cmd, intptr_t* args) {

	QMM_RET_IGNORED(0);
}


/* QMM_PluginMessage
   This is called by other plugins using the QMM_PLUGIN_BROADCAST helper
*/
C_DLLEXPORT void QMM_PluginMessage(plid_t from_plid, const char* message, void* buf, intptr_t buflen) {
}
