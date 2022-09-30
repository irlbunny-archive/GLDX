#include "gl.h"
#include "wgl.h"
#include <stdio.h>
#include <d3d9.h>
#pragma comment (lib, "d3d9.lib")

// FIXME(kaitlyn~): Deal with this in HGLRC handles.
LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3d_device;
D3DCOLOR clear_color;
D3DCOLOR current_color;

struct CUSTOMVERTEX { float x, y, z, rhw; D3DCOLOR color; } vertices[3]; //hardcoded triangle vertex
int vertices_idx = 0;

GLDXAPI HGLRC WINAPI _wglCreateContext(HDC hDC) {
    HWND hWnd = WindowFromDC(hDC);

    d3d = Direct3DCreate9(D3D_SDK_VERSION);

    D3DPRESENT_PARAMETERS d3d_pp;
    ZeroMemory(&d3d_pp, sizeof(d3d_pp));

    d3d_pp.Windowed = TRUE;
    d3d_pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3d_pp.hDeviceWindow = hWnd;

    HRESULT hResult = IDirect3D9_CreateDevice(d3d,
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3d_pp,
        &d3d_device);

    printf("[GLDX] Initialized Direct3D9 device. Result: %ld", hResult);
    return 1;
}

GLDXAPI BOOL WINAPI _wglDeleteContext(HGLRC hGLRC) {
    IDirect3DDevice9_Release(d3d_device);
    IDirect3D9_Release(d3d);
    return TRUE; // HACK! could not always be success.
}

GLDXAPI BOOL WINAPI _wglMakeCurrent(HDC hDC, HGLRC hGLRC) {
    // TODO IMPLEMENT
    return TRUE;
}

GLDXAPI void APIENTRY glBegin(GLenum mode) {
    // ...
    vertices_idx = 0;
}

// FIXME: handle mask
GLDXAPI void APIENTRY glClear(GLbitfield mask) {
    IDirect3DDevice9_Clear(d3d_device, 0, NULL, D3DCLEAR_TARGET, clear_color, 1.0f, 0);
}

GLDXAPI void APIENTRY glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) {
    clear_color = D3DCOLOR_COLORVALUE(red, green, blue, alpha);
}

GLDXAPI void APIENTRY glColor3f(GLfloat red, GLfloat green, GLfloat blue) {
    current_color = D3DCOLOR_COLORVALUE(red, green, blue, 1.0f);
}

GLDXAPI void APIENTRY glEnd(void) {
    LPDIRECT3DVERTEXBUFFER9 vertex_object = NULL;
    void* vertex_buffer = NULL;

    IDirect3DDevice9_CreateVertexBuffer(d3d_device, 3 * sizeof(struct CUSTOMVERTEX), 0, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &vertex_object, NULL);
    IDirect3DVertexBuffer9_Lock(vertex_object, 0, 3 * sizeof(struct CUSTOMVERTEX), &vertex_buffer, 0);
    memcpy(vertex_buffer, vertices, 3 * sizeof(struct CUSTOMVERTEX));
    IDirect3DVertexBuffer9_Unlock(vertex_object);

    IDirect3DDevice9_BeginScene(d3d_device);
    IDirect3DDevice9_SetStreamSource(d3d_device, 0, vertex_object, 0, sizeof(struct CUSTOMVERTEX));
    IDirect3DDevice9_SetFVF(d3d_device, D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    IDirect3DDevice9_DrawPrimitive(d3d_device, D3DPT_TRIANGLELIST, 0, 1);
    IDirect3DDevice9_EndScene(d3d_device);

    IDirect3DDevice9_Present(d3d_device, NULL, NULL, NULL, NULL); // HACK!! We should not present here, look into hooking SwapBuffers.
    IDirect3DVertexBuffer9_Release(vertex_object);
}

GLDXAPI const GLubyte* APIENTRY glGetString(GLenum name) {
    switch (name) {
    case GL_VERSION:
        return "1.1 GLDX"; // FIXME hardcoded
    }
    return NULL;
}

GLDXAPI void APIENTRY glVertex2f(GLfloat x, GLfloat y) {
    vertices[vertices_idx].x = (x * 500) + 500;
    vertices[vertices_idx].y = (y * 300) + 300;
    vertices[vertices_idx].z = 0;
    vertices[vertices_idx].rhw = 1.0f;
    vertices[vertices_idx].color = current_color;
    vertices_idx++;
}
