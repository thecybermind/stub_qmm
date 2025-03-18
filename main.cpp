/*
STUB_QMM - Example QMM Plugin
Copyright 2004-2025
https://github.com/thecybermind/stub_qmm/
3-clause BSD license: https://opensource.org/license/bsd-3-clause

Created By:
    Kevin Masterson < cybermind@gmail.com >

*/

#include "version.h"
#include <q_shared.h>
#include <g_local.h>
#include <qmmapi.h>

pluginres_t* g_result = NULL;
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
   This is the second function called by QMM
   Do game-independent startup routines here, and their correspnoding shutdown routines in QMM_Detach
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
   This is the last function called by QMM
   Do game-independent shutdown routines here
    - reserved = reserved for future use
*/
C_DLLEXPORT void QMM_Detach(int reserved) {
	//ignore 'reserved' but satisfy unused warnings
	reserved = 0;
}

/* QMM_vmMain
   This is called BEFORE the mod's vmMain function is called (by engine)
   Do engine-dependent startup routines here, in a cmd==GAME_INIT check
   Do mod-dependent shutdown routines here, in a cmd==GAME_SHUTDOWN check
    - cmd = command like GAME_INIT, GAME_CLIENT_COMMAND, etc. (game-specific)
	- argX = argument to cmd
*/
C_DLLEXPORT int QMM_vmMain(int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11) {
	if (cmd == GAME_CLIENT_COMMAND) {
		char buf[16];
		g_syscall(G_ARGV, 0, buf, sizeof(buf));
		if (!strcmp(buf, "myinfo")) {
			char userinfo[MAX_INFO_STRING];
			g_syscall(G_GET_USERINFO, arg0, userinfo, sizeof(userinfo));
			g_syscall(G_SEND_SERVER_COMMAND, arg0, QMM_VARARGS("print \"[STUB_QMM] Your infostring is: '%s'\"", userinfo));
			QMM_RET_SUPERCEDE(1);
		}
	}

	QMM_RET_IGNORED(1);
}

/* QMM_syscall
   This is called BEFORE the engine's syscall function is called (by mod)
   Store entity information here, in a cmd==G_LOCATE_GAME_DATA check
    - cmd = command like G_PRINT, G_LOCATE_GAME_DATA, etc. (game-specific)
	- argX = argument to cmd
*/
C_DLLEXPORT int QMM_syscall(int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11, int arg12) {
	if (cmd == G_LOCATE_GAME_DATA) {
		g_gents = (gentity_t*)arg0;
		//g_maxgents = ENTITYNUM_MAX_NORMAL;
		g_gentsize = arg2;
		g_clients = (gclient_t*)arg3;
		g_clientsize = arg4;

		g_syscall(G_PRINT, "(STUB_QMM) Entity data stored!\n");
		
		QMM_RET_SUPERCEDE(1);
	}

	QMM_RET_IGNORED(1);
}

/* QMM_vmMain_Post
   This is called AFTER the mod's vmMain function is called (by engine)
   Do engine-dependent shutdown routines here, in a cmd==GAME_SHUTDOWN check
   Do mod-dependent startup routines here, in a cmd==GAME_INIT check
    - cmd = command like GAME_INIT, GAME_CLIENT_COMMAND, etc. (game-specific)
	- argX = argument to cmd
*/
C_DLLEXPORT int QMM_vmMain_Post(int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11) {
	
	QMM_RET_IGNORED(1);
}

/* QMM_syscall_Post
   This is called AFTER the engine's syscall function is called (by mod)
    - cmd = command like G_PRINT, G_LOCATE_GAME_DATA, etc. (game-specific)
	- argX = argument to cmd
*/
C_DLLEXPORT int QMM_syscall_Post(int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11, int arg12) {
	//g_syscall(G_PRINT, QMM_VARARGS("[STUB_QMM] QMM_syscall_Post(%s): Hi!\n", QMM_ENGMSGNAME(cmd)));

	QMM_RET_IGNORED(1);
}
