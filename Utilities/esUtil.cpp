#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL_opengles2_gl2.h>
#include <SDL/SDL_egl.h>
#include <SDL/SDL_video.h>
#include <stdarg.h>

////////////////////////////////////// WIN32 ////////////////////////////////////////
#ifdef _WIN32

#include "esUtil_win.h"

//  esMainLoop()
//
//    Start the main loop for the OpenGL ES application
void ESUTIL_API esMainLoop(ESContext *esContext)
{
	WinLoop(esContext);
}

#endif
////////////////////////////////////// WIN32 ////////////////////////////////////////


//  esInitContext()
//
//      Initialize ES utility context.  This must be called before calling any other
//      functions.
void ESUTIL_API esInitContext(ESContext *esContext)
{
	if (esContext != NULL)
	{
		memset(esContext, 0, sizeof(ESContext));
	}
}

// CreateEGLContext()
//Creates an EGL rendering context and all associated elements
EGLBoolean CreateEGLContext ( EGLNativeWindowType hWnd, EGLDisplay* eglDisplay,
                              EGLContext* eglContext, EGLSurface* eglSurface,
                              EGLint attribList[])
{
   EGLint numConfigs;
   EGLint majorVersion;
   EGLint minorVersion;
   EGLDisplay display;
   EGLContext context;
   EGLSurface surface;
   EGLConfig config;
   EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };

   // Get Display

#ifdef _WIN32
   display = eglGetDisplay(GetDC(hWnd));
#elif defined __ANDROID__
   display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
#endif
   
   if ( display == EGL_NO_DISPLAY )
   {
	  __android_log_print(ANDROID_LOG_WARN, "esUtil", "eglGetDisplay");
      return EGL_FALSE;
   }
   

   // Initialize EGL
   if (!eglInitialize(display, &majorVersion, &minorVersion))
   //if ( !eglInitialize(display, &majorVersion, &minorVersion) )
   {
	   __android_log_print(ANDROID_LOG_WARN, "esUtil", "eglInit");
      return EGL_FALSE;
   }

   // Get configs
   if ( !eglGetConfigs(display, NULL, 0, &numConfigs) )
   {
	   __android_log_print(ANDROID_LOG_WARN, "esUtil", "eglGetConfig");
      return EGL_FALSE;
   }
   
   // Choose config
   if ( !eglChooseConfig(display, attribList, &config, 1, &numConfigs) )
   {
	   __android_log_print(ANDROID_LOG_WARN, "esUtil", "eglChooseConfig");
      return EGL_FALSE;
   }

   //////////////////////////// ANDROID ////////////////////////////////////////////
   // set native window size and format
#ifdef __ANDROID__
   EGLint format;
   if (!eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format)) {
		__android_log_print(ANDROID_LOG_WARN, "esUtil", "eglGetConfigAttrib");
		return EGL_FALSE;
   }
   
   ANativeWindow_setBuffersGeometry(hWnd, 0, 0, format);
#endif
   // Create a surface
   surface = eglCreateWindowSurface(display, config, hWnd, NULL);
   if ( surface == EGL_NO_SURFACE )
   {
	   __android_log_print(ANDROID_LOG_WARN, "esUtil", "eglCreateWindowSurface");
      return EGL_FALSE;
   }
   
   // Create a GL context
   //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
   context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs );
   if ( context == EGL_NO_CONTEXT )
   {
	   __android_log_print(ANDROID_LOG_WARN, "esUtil", "eglCreateContext");
      return EGL_FALSE;
   }   
   
// on windows rendering is done in the same thread
#ifdef _WIN32
   // Make the context current
   if ( !eglMakeCurrent(display, surface, surface, context) )
   {
	   __android_log_print(ANDROID_LOG_WARN, "esUtil", "eglMakeCurrent");
      return EGL_FALSE;
   }
#endif

   *eglDisplay = display;
   *eglSurface = surface;
   *eglContext = context;

   return EGL_TRUE;
} 


//  esCreateWindow()
//
//      title - name for title bar of window
//      width - width of window to create
//      height - height of window to create
//      flags  - bitwise or of window creation flags 
//          ES_WINDOW_ALPHA       - specifies that the framebuffer should have alpha
//          ES_WINDOW_DEPTH       - specifies that a depth buffer should be created
//          ES_WINDOW_STENCIL     - specifies that a stencil buffer should be created
//          ES_WINDOW_MULTISAMPLE - specifies that a multi-sample buffer should be created
GLboolean ESUTIL_API esCreateWindow ( ESContext *esContext, const char* title, GLint width, GLint height, GLuint flags )
{
   EGLint attribList[] =
   {
       EGL_RED_SIZE,       5,
       EGL_GREEN_SIZE,     6,
       EGL_BLUE_SIZE,      5,
       EGL_ALPHA_SIZE,     (flags & ES_WINDOW_ALPHA) ? 8 : EGL_DONT_CARE,
       EGL_DEPTH_SIZE,     (flags & ES_WINDOW_DEPTH) ? 24 : EGL_DONT_CARE,
       EGL_STENCIL_SIZE,   (flags & ES_WINDOW_STENCIL) ? 8 : EGL_DONT_CARE,
       EGL_SAMPLE_BUFFERS, (flags & ES_WINDOW_MULTISAMPLE) ? 1 : 0,
       EGL_NONE
   };
   
   if ( esContext == NULL )
   {
      return GL_FALSE;
   }

// create window (only for windows)
#ifdef _WIN32
   esContext->width = width;
   esContext->height = height;

   if ( !WinCreate ( esContext, title) )
   {
      return GL_FALSE;
   }
#endif

   if ( !CreateEGLContext ( esContext->hWnd,
                            &esContext->eglDisplay,
                            &esContext->eglContext,
                            &esContext->eglSurface,
                            attribList) )
   {
      return GL_FALSE;
   }

#ifdef __ANDROID__
	// get size
	eglQuerySurface(esContext->eglDisplay, esContext->eglSurface, EGL_WIDTH, &esContext->width);
	eglQuerySurface(esContext->eglDisplay, esContext->eglSurface, EGL_HEIGHT, & esContext->height);
#endif

   return GL_TRUE;
}

//  esRegisterDrawFunc()
void ESUTIL_API esRegisterDrawFunc(ESContext *esContext, void (ESCALLBACK *drawFunc) (ESContext*))
{
	esContext->drawFunc = drawFunc;
}

//  esRegisterUpdateFunc()
void ESUTIL_API esRegisterUpdateFunc(ESContext *esContext, void (ESCALLBACK *updateFunc) (ESContext*, float))
{
	esContext->updateFunc = updateFunc;
}


//  esRegisterKeyFunc()
void ESUTIL_API esRegisterKeyFunc(ESContext *esContext, void (ESCALLBACK *keyFunc) (ESContext*, unsigned char, bool))
{
	esContext->keyFunc = keyFunc;
}

// esLogMessage()
//    Log an error message to the debug output for the platform
void ESUTIL_API esLogMessage(const char *formatStr, ...)
{
	va_list params;
	char buf[2048];

	va_start(params, formatStr);
	sprintf(buf, formatStr, params);

	printf("%s", buf);
	fprintf(stderr, "%s", buf);

	va_end(params);
}