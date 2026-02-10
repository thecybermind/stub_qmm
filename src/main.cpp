/*
STUB_QMM - Example QMM Plugin
Copyright 2004-2026
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


// log level to use for all trace logging
const int loglevel = QMMLOG_INFO;

// return a string for each pplugin result value
const char* plugin_result_to_str(pluginres_t res) {
	switch (res) {
		case QMM_UNUSED:
			return "QMM_UNUSED";
		case QMM_ERROR:
			return "QMM_ERROR";
		case QMM_IGNORED:
			return "QMM_IGNORED";
		case QMM_OVERRIDE:
			return "QMM_OVERRIDE";
		case QMM_SUPERCEDE:
			return "QMM_SUPERCEDE";
	default:
		return "unknown";
	};
}


// "safe" strncpy that always null-terminates
char* strncpyz(char* dest, const char* src, size_t count) {
	char* ret = strncpy(dest, src, count);
	dest[count - 1] = '\0';
	return ret;
}


// "safe" strncat where count is the total size of dest, and always null-terminates WITHIN the buffer
char* strncatz(char* dest, const char* src, size_t count) {
	size_t len = strlen(dest);
	if (len >= count)
		return dest;
	// free space available in dest
	size_t avail = count - len;
	// strncat may null terminate at count+1 which may be outside the buffer if count is the full buffer size
	if (avail)
		avail--;
	char* ret = strncat(dest, src, avail);
	dest[count - 1] = '\0';
	return ret;
}


// return all args including arg 0 (G_ARGS doesn't include arg 0)
char* get_args() {
	static char arg[MAX_STRING_CHARS];
	static char buf[MAX_STRING_CHARS];
	QMM_ARGV(PLID, 0, buf, sizeof(buf));
	intptr_t argc = g_syscall(G_ARGC);
	for (intptr_t argnum = 1; argnum < argc; argnum++) {
		QMM_ARGV(PLID, argnum, arg, sizeof(arg));
		strncatz(buf, " ", sizeof(buf));
		strncatz(buf, arg, sizeof(buf));
	}

	return buf;
}


char* str_escape(const char* str) {
	static char buf[MAX_STRING_CHARS];
	strncpyz(buf, str, sizeof(buf));

	char f;

	char* find = strpbrk(buf, "\n\r%");
	while (find) {
		if (*find == '%') {
			*find = '@';
		}
		else {
			f = (*find == '\n' ? 'n' : 'r');
			*find = '\0';
			strncpyz(buf, QMM_VARARGS(PLID, "%s\\%c%s", buf, f, find + 1), sizeof(buf));
		}
		find = strpbrk(find, "\n\r");
	}

	return buf;
}


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

	// make sure this DLL is loaded only in the right engine
	if (strcmp(QMM_GETGAMEENGINE(PLID), GAME_STR) != 0)
		return 0;
	
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
static intptr_t last_cmd = -1;
static bool is_ignoring_syscalls = false;
C_DLLEXPORT intptr_t QMM_vmMain(intptr_t cmd, intptr_t* args) {
	if (cmd == GAME_INIT) {
		// example showing writing to QMM log on initialization
		QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "Stub_QMM loaded! Game engine: %s\n", QMM_GETGAMEENGINE(PLID)), QMMLOG_INFO);
	}

	// ignore repeated GAME_RUN_FRAME calls
	if (last_cmd == cmd && cmd == GAME_RUN_FRAME) {
		is_ignoring_syscalls = true;
		last_cmd = cmd;
		QMM_RET_IGNORED(0);
	}
	last_cmd = cmd;

	const char* msgname = QMM_MODMSGNAME(PLID, cmd);
	intptr_t current_return_value = QMM_VAR_RETURN(intptr_t);
	const char* highest_result = plugin_result_to_str(QMM_VAR_HIGH_RES());

	if (cmd == GAME_CONSOLE_COMMAND) {
		QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "QMM_vmMain(%s) :: args: \"%s\" :: current return value: %d :: highest result: %s\n", msgname, str_escape(get_args()), current_return_value, highest_result), loglevel);
	}
	else if (cmd == GAME_CLIENT_CONNECT || cmd == GAME_CLIENT_COMMAND || cmd == GAME_CLIENT_BEGIN ||
		cmd == GAME_CLIENT_USERINFO_CHANGED || cmd == GAME_CLIENT_DISCONNECT || cmd == GAME_CLIENT_THINK) {
		intptr_t clientNum = args[0];
// some games pass entity pointers to GAME_CLIENT_ messages instead of ints
#if defined(GAME_CLIENT_COMMAND_HAS_ENT)
		if (clientNum)
			clientNum = NUM_FROM_ENT(clientNum) + 1;
#endif
		if (cmd == GAME_CLIENT_CONNECT) {
// these games return a bool rather than a string
#if defined(GAME_CLIENT_CONNECT_RETURNS_BOOL)
			QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "QMM_vmMain(%s, %d) :: current return value: %d :: highest result: %s\n", msgname, clientNum, current_return_value, highest_result), loglevel);
#else
			QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "QMM_vmMain(%s, %d) :: current return value: \"%s\" :: highest result: %s\n", msgname, clientNum, (char*)current_return_value, highest_result), loglevel);
#endif
		}
		else if (cmd == GAME_CLIENT_COMMAND) {
			QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "QMM_vmMain(%s, %d) :: args: \"%s\" :: current return value: %d :: highest result: %s\n", msgname, clientNum, str_escape(get_args()), current_return_value, highest_result), loglevel);
		}
		else {
			QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "QMM_vmMain(%s, %d) :: highest result: %s\n", msgname, clientNum, highest_result), loglevel);
		}
	}
// mapname is the first argument to SpawnEntities or Init in all games with this feature flag
// MOH?? have a SpawnEntities function but no mapname
#if defined(GAME_HAS_SPAWN_ENTITIES_MAPNAME)
	else if (cmd == GAME_SPAWN_ENTITIES) {
		QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "QMM_vmMain(%s, \"%s\") :: highest result: %s\n", msgname, (char*)args[0], highest_result), loglevel);
	}
#endif
	else {
		QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "QMM_vmMain(%s) :: current return value: %d :: highest result: %s\n", msgname, current_return_value, highest_result), loglevel);
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
	if (is_ignoring_syscalls) {
		// end ignoring syscalls
		is_ignoring_syscalls = false;
		QMM_RET_IGNORED(0);
	}

	const char* msgname = QMM_MODMSGNAME(PLID, cmd);
	intptr_t current_return_value = QMM_VAR_RETURN(intptr_t);
	intptr_t original_return_value = QMM_VAR_ORIG_RETURN(intptr_t);
	const char* highest_result = plugin_result_to_str(QMM_VAR_HIGH_RES());

	if (cmd == GAME_CONSOLE_COMMAND) {
		QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "QMM_vmMain_Post(%s) :: args: \"%s\" :: current return value: %d :: original return value: %d :: highest result: %s\n", msgname, get_args(), current_return_value, original_return_value, highest_result), loglevel);
	}
	else if (cmd == GAME_CLIENT_CONNECT || cmd == GAME_CLIENT_COMMAND || cmd == GAME_CLIENT_BEGIN ||
		cmd == GAME_CLIENT_USERINFO_CHANGED || cmd == GAME_CLIENT_DISCONNECT || cmd == GAME_CLIENT_THINK) {
		intptr_t clientNum = args[0];
		// some games pass entity pointers to GAME_CLIENT_ messages instead of ints
#if defined(GAME_CLIENT_COMMAND_HAS_ENT)
		if (clientNum)
			clientNum = NUM_FROM_ENT(clientNum) + 1;
#endif
		if (cmd == GAME_CLIENT_CONNECT) {
			// some games return a bool rather than a string
#if defined(GAME_CLIENT_CONNECT_RETURNS_BOOL)
			QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "QMM_vmMain_Post(%s, %d) :: current return value: %d :: original return value: %d :: highest result: %s\n", msgname, clientNum, current_return_value, original_return_value, highest_result), loglevel);
#else
			QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "QMM_vmMain_Post(%s, %d) :: current return value: \"%s\" :: original return value: \"%s\" :: highest result: %s\n", msgname, clientNum, (const char*)current_return_value, (const char*)original_return_value, highest_result), loglevel);
#endif
		}
		else if (cmd == GAME_CLIENT_COMMAND) {
			QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "QMM_vmMain_Post(%s, %d) :: args: \"%s\" :: current return value: %d :: original return value: %d :: highest result: %s\n", msgname, clientNum, get_args(), current_return_value, original_return_value, highest_result), loglevel);
		}
		else {
			QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "QMM_vmMain_Post(%s, %d) :: highest result: %s\n", msgname, clientNum, highest_result), loglevel);
		}
	}
	// mapname is the first argument to SpawnEntities or Init in all games with this feature flag
	// MOH?? have a SpawnEntities function but no mapname
#if defined(GAME_HAS_SPAWN_ENTITIES_MAPNAME)
	else if (cmd == GAME_SPAWN_ENTITIES) {
		QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "QMM_vmMain_Post(%s, \"%s\") :: highest result: %s\n", msgname, (char*)args[0], highest_result), loglevel);
	}
#endif
	else {
		QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "QMM_vmMain_Post(%s) :: current return value: %d :: original return value: %d :: highest result: %s\n", msgname, current_return_value, original_return_value, highest_result), loglevel);
	}

	// end ignoring syscalls
	is_ignoring_syscalls = false;

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

	if (is_ignoring_syscalls)
		QMM_RET_IGNORED(0);

	const char* msgname = QMM_ENGMSGNAME(PLID, cmd);
	intptr_t current_return_value = QMM_VAR_RETURN(intptr_t);
	const char* highest_result = plugin_result_to_str(QMM_VAR_HIGH_RES());

	if (cmd == G_PRINT || cmd == G_ERROR) {
		QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "QMM_syscall(%s, \"%s\") :: highest result: %s\n", msgname, str_escape((char*)args[0]), highest_result), loglevel);
	}
	else if (cmd == G_CVAR_VARIABLE_STRING_BUFFER) {
		QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "QMM_syscall(%s, \"%s\") :: highest result: %s\n", msgname, (char*)args[0], highest_result), loglevel);
	}
	else if (cmd == G_CVAR_VARIABLE_INTEGER_VALUE || cmd == G_FS_FOPEN_FILE) {
		QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "QMM_syscall(%s, \"%s\") :: current return value: %d :: highest result: %s\n", msgname, (char*)args[0], current_return_value, highest_result), loglevel);
	}
	else if (cmd == G_CVAR_SET) {
		QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "QMM_syscall(%s, \"%s\", \"%s\") :: highest result: %s\n", msgname, (char*)args[0], (char*)args[1], highest_result), loglevel);
	}
	else if (cmd == G_CVAR_REGISTER) {
		QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "QMM_syscall(%s, \"%s\", \"%s\") :: highest result: %s\n", msgname, (char*)args[1], (char*)args[2], highest_result), loglevel);
	}
	else if (cmd == G_LOCATE_GAME_DATA) {
		QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "QMM_syscall(%s, %p, %d, %d, %p, %d) :: highest result: %s\n", msgname, (void*)args[0], args[1], args[2], (void*)args[3], args[4], highest_result), loglevel);
	}
	else {
		QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "QMM_syscall(%s) :: current return value: %d :: highest result: %s\n", msgname, current_return_value, highest_result), loglevel);
	}

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
	if (is_ignoring_syscalls)
		QMM_RET_IGNORED(0);

	const char* msgname = QMM_ENGMSGNAME(PLID, cmd);
	intptr_t current_return_value = QMM_VAR_RETURN(intptr_t);
	intptr_t original_return_value = QMM_VAR_ORIG_RETURN(intptr_t);
	const char* highest_result = plugin_result_to_str(QMM_VAR_HIGH_RES());

	if (cmd == G_ARGV) {
#if defined(GAME_ARGV_RETURN)
		QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "QMM_syscall_Post(%s, %d) :: current return value: \"%s\" :: original return value: \"%s\" :: highest result: %s\n", msgname, args[0], (char*)current_return_value, (char*)original_return_value, highest_result), loglevel);
#else
		QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "QMM_syscall_Post(%s, %d, \"%s\") :: highest result: %s\n", msgname, args[0], (char*)args[1], highest_result), loglevel);
#endif
	}
	else if (cmd == G_ARGS) {
		QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "QMM_syscall_Post(%s) :: current return value: \"%s\" :: original return value: \"%s\" :: highest result: %s\n", msgname, (char*)current_return_value, (char*)original_return_value, highest_result), loglevel);
	}
	else if (cmd == G_GET_ENTITY_TOKEN || cmd == G_CVAR_VARIABLE_INTEGER_VALUE || cmd == G_FS_FOPEN_FILE) {
		QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "QMM_syscall_Post(%s, \"%s\") :: current return value: %d :: original return value: %d :: highest result: %s\n", msgname, (const char*)args[0], current_return_value, original_return_value, highest_result), loglevel);
	}
	else if (cmd == G_PRINT || cmd == G_ERROR) {
		QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "QMM_syscall_Post(%s, \"%s\") :: highest result: %s\n", msgname, str_escape((char*)args[0]), highest_result), loglevel);
	}
	else if (cmd == G_CVAR_VARIABLE_STRING_BUFFER || cmd == G_CVAR_SET) {
		QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "QMM_syscall_Post(%s, \"%s\", \"%s\") :: highest result: %s\n", msgname, (char*)args[0], (char*)args[1], highest_result), loglevel);
	}
	else if (cmd == G_CVAR_REGISTER) {
		QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "QMM_syscall_Post(%s, \"%s\", \"%s\") :: highest result: %s\n", msgname, (char*)args[1], (char*)args[2], highest_result), loglevel);
	}
	else if (cmd == G_LOCATE_GAME_DATA) {
		QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "QMM_syscall_Post(%s, %p, %d, %d, %p, %d) :: highest result: %s\n", msgname, (void*)args[0], args[1], args[2], (void*)args[3], args[4], highest_result), loglevel);
	}
	else {
		QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "QMM_syscall_Post(%s) :: current return value: %d :: original return value: %d :: highest result: %s\n", msgname, current_return_value, original_return_value, highest_result), loglevel);
	}

	QMM_RET_IGNORED(0);
}


/* QMM_PluginMessage
   This is called by other plugins using the QMM_PLUGIN_BROADCAST helper
*/
C_DLLEXPORT void QMM_PluginMessage(plid_t from_plid, const char* message, void* buf, intptr_t buflen, int is_broadcast) {
	QMM_WRITEQMMLOG(PLID, QMM_VARARGS(PLID, "QMM_PluginMessage(\"%s\", %p, %d, %d)", message, buf, buflen, is_broadcast), loglevel);
}
