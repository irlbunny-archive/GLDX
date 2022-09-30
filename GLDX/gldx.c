#include "gl.h"
#include "wgl.h"
#include <stdio.h>
#include <d3d9.h>
#pragma comment (lib, "d3d9.lib")

// FIXME(kaitlyn~): Deal with this in HGLRC handles.
LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3d_device;

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
    return 0;
}

GLDXAPI BOOL  WINAPI _wglDeleteContext(HGLRC hGLRC) {
    IDirect3DDevice9_Release(d3d_device);
    IDirect3D9_Release(d3d);
    return TRUE; // HACK! could not always be success.
}
