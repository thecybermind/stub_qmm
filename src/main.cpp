/*
STUB_QMM - Example QMM Plugin
Copyright 2004-2025
https://github.com/thecybermind/stub_qmm/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < cybermind@gmail.com >

*/

#define _CRT_SECURE_NO_WARNINGS 1

#include <qmmapi.h>

#include "version.h"
#include "game.h"

#include <string.h>

pluginres_t* g_result = nullptr;
plugininfo_t g_plugininfo = {
	"STUB_QMM",									// name of plugin
	STUB_QMM_VERSION,							// version of plugin
	"Stub/test plugin",							// description of plugin
	STUB_QMM_BUILDER,							// author of plugin
	"https://github.com/thecybermind/stub_qmm",	// website of plugin
	0,											// reserved
	0,											// reserved
	0,											// reserved
	QMM_PIFV_MAJOR,								// plugin interface version major
	QMM_PIFV_MINOR								// plugin interface version minor
};
eng_syscall_t g_syscall = NULL;
mod_vmMain_t g_vmMain = NULL;
pluginfuncs_t* g_pluginfuncs = NULL;
intptr_t g_vmbase = 0;

// store the game's entity and client info
gentity_t* g_gents = NULL;
int g_numgents = 0;
int g_gentsize = sizeof(gentity_t);
gclient_t* g_clients = NULL;
int g_clientsize = sizeof(gclient_t);

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
    - engfunc  = pointer to the engine's syscall function
    - modfunc  = pointer to the mod's vmMain function
    - presult  = pointer to plugin result variable
	- vmbase   = value to add to pointers passed to the engine from a QVM mod (0 if DLL mod)
    - reserved = reserved for future use
   
   return:
    - 0 = failure, QMM_Detach will be called and plugin will be unloaded
    - 1 = succeed, plugin will be loaded
*/
C_DLLEXPORT int QMM_Attach(eng_syscall_t engfunc, mod_vmMain_t modfunc, pluginres_t* presult, pluginfuncs_t* pluginfuncs, intptr_t vmbase, intptr_t reserved) {
	QMM_SAVE_VARS();

	// ignore 'reserved' but satisfy unused warnings
	reserved = 0;

	return 1;
}

/* QMM_Detach
   This is the last function called by QMM. This is called after returning false from QMM_Attach(), or during final shutdown.
   Keep in mind, the shutdown process occurs in QMM's vmMain(GAME_SHUTDOWN) *after* it has been routed to the mod, so the mod
   is completely shutdown at this point, and is generally unsafe to call into. You can, however, still use some engine functions
   through syscall.
    - reserved = reserved for future use
*/
C_DLLEXPORT void QMM_Detach(intptr_t reserved) {
	// ignore 'reserved' but satisfy unused warnings
	reserved = 0;
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
	switch (cmd) {
		case GAME_INIT:
			// example showing writing to QMM log
			QMM_WRITEQMMLOG(QMM_VARARGS("Stub_QMM loaded! Game engine: %s", QMM_GETGAMEENGINE()), QMMLOG_INFO, "STUB_QMM");
			break;
		case GAME_CLIENT_COMMAND:
			char buf[16] = "";
			intptr_t clientnum = args[0];

			// some engines use this arg/buf/buflen syntax for getting ARGV
			// others return the char*, so we use QMM_ARGV to handle both methods automatically
			QMM_ARGV(0, buf, sizeof(buf));
			if (!strcmp(buf, "myinfo")) {
				char userinfo[MAX_INFO_STRING];
				g_syscall(G_GET_USERINFO, clientnum, userinfo, sizeof(userinfo));
				g_syscall(G_SEND_SERVER_COMMAND, clientnum, QMM_VARARGS("print \"[STUB_QMM] Your infostring is: '%s'\"\n", userinfo));
				QMM_RET_SUPERCEDE(1);
			}
			// purely an example to show entity/client access
			else if (!strcmp(buf, "myweapon")) {
				gclient_t* client = CLIENT_FROM_NUM(clientnum);
#if defined(GAME_STEF2)
				int left = client->ps.activeItems[ITEM_NAME_WEAPON_LEFT];
				int right = client->ps.activeItems[ITEM_NAME_WEAPON_RIGHT];
				g_syscall(G_SEND_SERVER_COMMAND, clientnum, QMM_VARARGS("print \"[STUB_QMM] Your weapons are: %d %d\"\n", left, right));
#else
 #if defined(GAME_MOHAA) || defined(GAME_MOHSH) || defined(GAME_MOHBT)
				int item = client->ps.activeItems[ITEM_WEAPON];
 #else
				int item = client->ps.weapon;
 #endif
				g_syscall(G_SEND_SERVER_COMMAND, clientnum, QMM_VARARGS("print \"[STUB_QMM] Your weapon is: %d\"\n", item));
#endif
				QMM_RET_SUPERCEDE(1);
			}
	}

	QMM_RET_IGNORED(1);
}

/* QMM_syscall
   This is called BEFORE the engine's syscall function is called (by mod).
   You would typically store entity information here in a cmd==G_LOCATE_GAME_DATA check
    - cmd = command like G_PRINT, G_LOCATE_GAME_DATA, etc. (game-specific)
	- args = arguments to cmd
*/
C_DLLEXPORT intptr_t QMM_syscall(intptr_t cmd, intptr_t* args) {
	// this is fairly common to store entity/client data for later
	if (cmd == G_LOCATE_GAME_DATA) {
		g_gents = (gentity_t*)(args[0]);
		g_numgents = args[1];
		g_gentsize = args[2];
		g_clients = (gclient_t*)(args[3]);
		g_clientsize = args[4];

		g_syscall(G_PRINT, "(STUB_QMM) Entity data stored!\n");
	}

	QMM_RET_IGNORED(1);
}

/* QMM_vmMain_Post
   This is called AFTER the mod's vmMain function is called (by engine)
   Keep in mind, if cmd==GAME_SHUTDOWN, this function is called *after* it has been routed to the mod, so the mod is completely
   shutdown at this point, and is generally unsafe to call into. You can, however, use some engine functions through syscall.
    - cmd = command like GAME_INIT, GAME_CLIENT_COMMAND, etc. (game-specific)
	- args = arguments to cmd
*/
C_DLLEXPORT intptr_t QMM_vmMain_Post(intptr_t cmd, intptr_t* args) {

	QMM_RET_IGNORED(1);
}

/* QMM_syscall_Post
   This is called AFTER the engine's syscall function is called (by mod)
    - cmd = command like G_PRINT, G_LOCATE_GAME_DATA, etc. (game-specific)
	- args = arguments to cmd
*/
C_DLLEXPORT intptr_t QMM_syscall_Post(intptr_t cmd, intptr_t* args) {

	QMM_RET_IGNORED(1);
}
