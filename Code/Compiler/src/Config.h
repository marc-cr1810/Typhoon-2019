#ifndef Ty_CONFIG_H
#define Ty_CONFIG_H

#ifndef _CRT_SECURE_NO_DEPRECATE
#	define _CRT_SECURE_NO_DEPRECATE 1
#endif

#define MS_WINDOWS
#define MS_WIN32			// Support for win32 and greater

#ifdef _WIN64
#	define MS_WIN64
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

#endif