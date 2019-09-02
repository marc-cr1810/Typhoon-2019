#ifndef Ty_CONFIG_H
#define Ty_CONFIG_H

#ifndef _CRT_SECURE_NO_DEPRECATE
#	define _CRT_SECURE_NO_DEPRECATE 1
#endif

#ifdef _DEBUG
#	define Ty_DEBUG
#elif defined(DEBUG)
#	define Ty_ARDUINO_DEBUG
#endif

#define Ty_EXIT_SUCCESS			0U
#define Ty_EXIT_FAILURE			1U

#define _Ty_STRINGIZE(X) _Ty_STRINGIZE1((X))
#define _Ty_STRINGIZE1(X) _Ty_STRINGIZE2 ## X
#define _Ty_STRINGIZE2(X) #X

#ifdef _MSC_VER
/* Microsoft Visual Studio C++ Compiler Information */

#define MS_WINDOWS
#define MS_WIN32			// Support for win32 and greater

#ifdef _WIN64
#	define MS_WIN64
#endif

#define _Ty_COMPILER_PASTE_VERSION(suffix) ("[MSC v." _Ty_STRINGIZE(_MSC_VER) " " suffix "]")

#ifdef MS_WIN64

#	if defined(_M_IA64)
#		define COMPILER _Ty_COMPILER_PASTE_VERSION("64 bit (Itanium)")
#		define MS_WINI64
#		define TYD_PLATFORM_TAG "win_ia64"
#	elif defined(_M_X64) || defined(_M_AMD64)
#		if defined(__INTEL_COMPILER)
#			define COMPILER ("[ICC v." _Ty_STRINGIZE(__INTEL_COMPILER) " 64 bit (amd64) with MSC v." _Ty_STRINGIZE(_MSC_VER) " CRT]")
#		else
#			define COMPILER _Ty_COMPILER_PASTE_VERSION("64 bit (AMD64)")
#		endif /* __INTEL_COMPILER */
#		define MS_WINX64
#		define TYD_PLATFORM_TAG "win_amd64"
#	else
#		define COMPILER _Ty_COMPILER_PASTE_VERSION("64 bit (Unknown)")
#	endif
#endif

#if defined(MS_WIN32) && !defined(MS_WIN64)
#	if defined(_M_IX86)
#		if defined(__INTEL_COMPILER)
#			define COMPILER ("[ICC v." _Ty_STRINGIZE(__INTEL_COMPILER) " 32 bit (Intel) with MSC v." _Ty_STRINGIZE(_MSC_VER) " CRT]")
#		else
#			define COMPILER _Ty_COMPILER_PASTE_VERSION("32 bit (Intel)")
#		endif /* __INTEL_COMPILER */
#		define TYD_PLATFORM_TAG "win32"
#	elif defined(_M_ARM)
#		define COMPILER _Ty_PASTE_VERSION("32 bit (ARM)")
#		define TYD_PLATFORM_TAG "win_arm"
#	else
#		define COMPILER _Ty_COMPILER_PASTE_VERSION("32 bit (Unknown)")
#	endif
#endif /* MS_WIN32 && !MS_WIN64 */

#endif

enum Opcode
{
	B_NOP,
	B_BREAK,
	B_LDARG_S,
	B_STARG_S,
	B_LDARG,
	B_STARG,
	B_LDARG_L,
	B_STARG_L,
	B_LDLOC_S,
	B_STLOC_S,
	B_LDLOC,
	B_STLOC,
	B_LDLOC_L,
	B_STLOC_L,
	B_LOAD_S,
	B_STORE_S,
	B_LOAD,
	B_STORE,
	B_LOAD_L,
	B_STORE_L,
	B_LDNULL,
	B_LDCONST_S,
	B_LDUCONST_S,
	B_LDCONST,
	B_LDUCONST,
	B_LDCONST_L,
	B_LDUCONST_L,
	B_LDFLOAT_S,
	B_LDFLOAT_D,
	B_LDSTR,
	B_LDTRUE,
	B_LDFALSE,
	B_BR_S,
	B_BRTRUE_S,
	B_BRFALSE_S,
	B_BR,
	B_BRTRUE,
	B_BRFALSE,
	B_BR_L,
	B_BRTRUE_L,
	B_BRFALSE_L,
	B_DUP,
	B_POP,
	B_CALL,
	B_RET,
	B_ADD,
	B_SUB,
	B_MUL,
	B_DIV,
	B_REM,
	B_CEQ,
	B_CNEQ,
	B_CGT,
	B_CLT,
	B_CGTEQ,
	B_CLTEQ,
	B_SYSCALL
};

enum SyscallCode
{
	SYS_NOP,
	SYS_EXIT,
	SYS_KILL,
	SYS_BREAK,
	SYS_FILE_READ,
	SYS_FILE_WRITE,
	SYS_FILE_DELETE,
	SYS_DIR_MAKE,
	SYS_DIR_DELETE,
	SYS_CLOCK_GETTIME,
	SYS_THREAD_SLEEP,
	SYS_THREAD_SLEEPMICRO,
	SYS_THREAD_SLEEPNANO,
	SYS_CONSOLE_READ,
	SYS_CONSOLE_READLINE,
	SYS_CONSOLE_WRITE
};

#endif