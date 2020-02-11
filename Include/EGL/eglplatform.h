/* -*- mode: c; tab-width: 8; -*- */
/* vi: set sw=4 ts=8: */
/* Platform-specific types and definitions for egl.h */

#ifndef __eglplatform_h_
#define __eglplatform_h_

/* Windows calling convention boilerplate */
#if defined(_WIN32) && !defined(APIENTRY) && !defined(__CYGWIN__) && !defined(__SCITECH_SNAP__)
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif

#include <sys/types.h>

/* Macros used in EGL function prototype declarations.
 *
 * EGLAPI return-type EGLAPIENTRY eglFunction(arguments);
 *
 * On Windows, EGLAPIENTRY can be defined like APIENTRY.
 * On most other platforms, it should be empty.
 */

#if defined(_WIN32) && !defined(EGLAPIENTRY)
	#define EGLAPIENTRY __stdcall
#else
	#define EGLAPIENTRY
#endif


#if defined(_WIN32) && !defined(EGLAPI)
	#ifdef __EGL_EXPORTS
		#define EGLAPI __declspec(dllexport)
	#else
		#define EGLAPI __declspec(dllimport)
	#endif
#else
	#define EGLAPI
#endif

/* The types NativeDisplayType, NativeWindowType, and NativePixmapType
 * are aliases of window-system-dependent types, such as X Display * or
 * Windows Device Context. They must be defined in platform-specific
 * code below. The EGL-prefixed versions of Native*Type are the same
 * types, renamed in EGL 1.3 so all types in the API start with "EGL".
 */

#ifdef _WIN32
	#define NativeDisplayType HDC
	#define NativeWindowType  HWND
	#define NativePixmapType  HBITMAP
#elif defined __ANDROID__
	#define NativeDisplayType void*
	#define NativeWindowType  void*
	#define NativePixmapType  void*
#endif


/* EGL 1.2 types, renamed for consistency in EGL 1.3 */
typedef NativeDisplayType 	EGLNativeDisplayType;
typedef NativePixmapType 	EGLNativePixmapType;
typedef NativeWindowType 	EGLNativeWindowType;

#endif /* __eglplatform_h */
