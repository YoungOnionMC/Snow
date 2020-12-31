#pragma once

#include <Windows.h>
#include <dxgicommon.h>
#include <dxgi.h>
#include <dxgi1_6.h>

#include <d3dcommon.h>

#include <d3d11_4.h>

#include <d3dcompiler.h>

static void DirectXErrorCheck(HRESULT result) {
    if (result != S_OK)
        SNOW_CORE_ERROR("HResult is '{0}' in {1}:{2}", result, __LINE__, __FILE__);
}

#define DXCheckError(fn) { DirectXErrorCheck((fn)); }

#include <wrl/client.h>


