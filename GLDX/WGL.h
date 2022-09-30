#ifndef __wgl_h_
#ifndef __WGL_H__

#define __wgl_h_
#define __WGL_H__

#include <windows.h>

#ifdef GLDX_EXPORTS
#define GLDXAPI __declspec(dllexport)
#else
#define GLDXAPI __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

GLDXAPI HGLRC WINAPI _wglCreateContext(HDC);
GLDXAPI BOOL  WINAPI _wglDeleteContext(HGLRC);
GLDXAPI BOOL  WINAPI _wglMakeCurrent(HDC, HGLRC);

#ifdef __cplusplus
}
#endif

#endif /* __WGL_H__ */
#endif /* __wgl_h_ */
