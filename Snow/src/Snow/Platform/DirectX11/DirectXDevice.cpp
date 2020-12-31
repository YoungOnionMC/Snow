#include <spch.h>
#include "Snow/Platform/DirectX11/DirectXDevice.h"

namespace Snow {
	void DirectX11Device::FindSuitableAdapter() {

		std::string dxgiAdapterDesc;

		Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
		if (m_Device.As<IDXGIDevice>(&dxgiDevice) == S_OK) {
			if (dxgiDevice->GetAdapter(&m_DXGIAdapter) == S_OK) {
				DXGI_ADAPTER_DESC adapterDesc;
				m_DXGIAdapter->GetDesc(&adapterDesc);
				m_DXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_DXGIFactory);
				//dxgiAdapterDesc = adapterDesc.Description;
			}
		}


		DXCheckError(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_DXGIFactory));
		DXCheckError(m_DXGIFactory->EnumAdapters(0, &m_DXGIAdapter));
		DXCheckError(m_DXGIAdapter->EnumOutputs(0, &m_AdapterOutput));
	}

	DirectX11Device::DirectX11Device() {
		const D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};

		UINT deviceFlags = 0;
#ifdef SNOW_DEBUG
		deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		DXCheckError(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, deviceFlags, featureLevels, CountOf(featureLevels), D3D11_SDK_VERSION, &m_Device, NULL, &m_DeviceContext));
	
		FindSuitableAdapter();
	}
}