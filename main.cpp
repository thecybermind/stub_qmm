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
	"STUB_QMM",									//name of plugin
	STUB_QMM_VERSION,							//version of plugin
	"Stub/test plugin",							//description of plugin
	STUB_QMM_BUILDER,							//author of plugin
	"https://github.com/thecybermind/stub_qmm",	//website of plugin
	0,											//reserved
	0,											//reserved
	0,											//reserved
	QMM_PIFV_MAJOR,								//plugin interface version major
	QMM_PIFV_MINOR								//plugin interface version minor
};
eng_syscall_t g_syscall = NULL;
mod_vmMain_t g_vmMain = NULL;
pluginfuncs_t* g_pluginfuncs = NULL;
int g_vmbase = 0;

gentity_t* g_gents = NULL;
int g_gentsize = sizeof(gentity_t);
int g_maxgents = ENTITYNUM_MAX_NORMAL;
gclient_t* g_clients = NULL;
int g_clientsize = sizeof(gclient_t);

/* QMM_Query
   This is the first function called by QMM
   Do not do anything here that requires shutdown routines, as there is no equivalent called at shutdown
    - pinfo = address of a pointer in QMM to this plugin's plugininfo struct
*/
C_DLLEXPORT void QMM_Query(plugininfo_t** pinfo) {
	//give QMM our plugin info struct
	QMM_GIVE_PINFO();
}

/* QMM_Attach
   This is the second function called by QMM.
   Keep in mind, this is called during QMM's handling of vmMain(GAME_INIT) *before* it has been routed to the mod,
   so the mod is completely uninitialized at this point, and is generally unsafe to call into. You can, however, use
   some engine functions through syscall.
    - engfunc = pointer to the engine's syscall function
    - modfunc = pointer to the mod's vmMain function
    - presult = pointer to plugin result variable
	- vmbase = value to add to pointers passed to the engine from a QVM mod (0 if DLL mod)
    - reserved = reserved for future use
   return 0 = failure, QMM_Detach will be called and plugin will be unloaded
   return 1 = succeed, plugin will be loaded
*/
C_DLLEXPORT int QMM_Attach(eng_syscall_t engfunc, mod_vmMain_t modfunc, pluginres_t* presult, pluginfuncs_t* pluginfuncs, int vmbase, int reserved) {
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
C_DLLEXPORT void QMM_Detach(int reserved) {
	//ignore 'reserved' but satisfy unused warnings
	reserved = 0;
}

/* QMM_vmMain
   This is called BEFORE the mod's vmMain function is called (by engine).
   Keep in mind, if cmd==GAME_INIT, this function is called *before* it has been routed to the mod, so the mod is completely
   uninitialized at this point, and is generally unsafe to call into. You can, however, use some engine functions through
   syscall.
    - cmd = command like GAME_INIT, GAME_CLIENT_COMMAND, etc. (game-specific)
	- varargs = arguments to cmd
*/
C_DLLEXPORT int QMM_vmMain(int cmd, ...) {
	QMM_GET_VMMAIN_ARGS();

	if (cmd == GAME_CLIENT_COMMAND) {
		char buf[16] = { 0 };
		g_syscall(G_ARGV, 0, buf, sizeof(buf));
		if (!strcmp(buf, "myinfo")) {
			char userinfo[MAX_INFO_STRING];
			g_syscall(G_GET_USERINFO, args[0], userinfo, sizeof(userinfo));
			g_syscall(G_SEND_SERVER_COMMAND, args[0], QMM_VARARGS("print \"[STUB_QMM] Your infostring is: '%s'\"", userinfo));
			QMM_RET_SUPERCEDE(1);
		}
	}

	QMM_RET_IGNORED(1);
}

/* QMM_syscall
   This is called BEFORE the engine's syscall function is called (by mod).
   You would typically store entity information here, in a cmd==G_LOCATE_GAME_DATA check
    - cmd = command like G_PRINT, G_LOCATE_GAME_DATA, etc. (game-specific)
	- varargs = arguments to cmd
*/
C_DLLEXPORT int QMM_syscall(int cmd, ...) {
	QMM_GET_SYSCALL_ARGS();

#if !defined(GAME_STVOYSP)
	if (cmd == G_LOCATE_GAME_DATA) {
		g_gents = (gentity_t*)(args[0]);
		//g_maxgents = ENTITYNUM_MAX_NORMAL;
		g_gentsize = args[2];
		g_clients = (gclient_t*)(args[3]);
		g_clientsize = args[4];

		g_syscall(G_PRINT, "(STUB_QMM) Entity data stored!\n");
		
		QMM_RET_SUPERCEDE(1);
	}
#endif

	QMM_RET_IGNORED(1);
}

/* QMM_vmMain_Post
   This is called AFTER the mod's vmMain function is called (by engine)
   Keep in mind, if cmd==GAME_SHUTDOWN, this function is called *after* it has been routed to the mod, so the mod is completely
   shutdown at this point, and is generally unsafe to call into. You can, however, use some engine functions through syscall.
    - cmd = command like GAME_INIT, GAME_CLIENT_COMMAND, etc. (game-specific)
	- varargs = arguments to cmd
*/
C_DLLEXPORT int QMM_vmMain_Post(int cmd, ...) {
	QMM_GET_VMMAIN_ARGS();

#if defined(GAME_STVOYSP)
	// we can check for an entity's client pointer after a 2nd client connects, and determine the actual gclient_t size
	if (cmd == GAME_CLIENT_CONNECT) {
		int clientnum = args[0];
		gentity_t* ent = ENT_FROM_NUM(clientnum);	// this is safe since we already know gentity_t size
		gclient_t* client = ent->client;

		// first client is 0, and that doesn't help us (also, can't divide by 0)
		if (clientnum)
			g_clientsize = ((unsigned char*)client - (unsigned char*)g_clients) / clientnum;
	}
#endif

	QMM_RET_IGNORED(1);
}

/* QMM_syscall_Post
   This is called AFTER the engine's syscall function is called (by mod)
    - cmd = command like G_PRINT, G_LOCATE_GAME_DATA, etc. (game-specific)
	- varargs = arguments to cmd
*/
C_DLLEXPORT int QMM_syscall_Post(int cmd, ...) {
	QMM_GET_SYSCALL_ARGS();

#if defined(GAME_STVOYSP)
	if (cmd == GAME_INIT) {
		// stvoy needs to call vmMain(GAMEVP_GENTITIES) and vmMain(GAMEV_GENTITYSIZE) to get the values out of the export struct
		g_gents = (gentity_t*)g_vmMain(GAMEVP_GENTITIES);
		g_gentsize = g_vmMain(GAMEV_GENTITYSIZE);

		// for clients, the mod stores the first client pointer in the first entity:
		// g_entities[0].client = level.clients;
		g_clients = (gclient_t*)(g_gents[0].client);
	}
#endif

	QMM_RET_IGNORED(1);
}
