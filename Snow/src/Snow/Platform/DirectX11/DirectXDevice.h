#pragma once

#include "Snow/Platform/DirectX11/DirectXCommon.h"

#include "Snow/Core/Ref.h"

namespace Snow {
	class DirectX11Device : public RefCounted {
	public:
		DirectX11Device();

		IDXGIFactory* GetDXGIFactory() const { return m_DXGIFactory; }

		Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() const { return m_Device; }
		ID3D11DeviceContext* GetDeviceContext() const { return m_DeviceContext; }
	private:

		void FindSuitableAdapter();

		IDXGIFactory* m_DXGIFactory;
		Microsoft::WRL::ComPtr<IDXGIAdapter> m_DXGIAdapter;
		IDXGIOutput* m_AdapterOutput;

		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		ID3D11DeviceContext* m_DeviceContext;

	};
}
