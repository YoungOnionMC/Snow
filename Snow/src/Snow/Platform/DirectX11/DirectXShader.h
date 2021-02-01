#pragma once

#include "Snow/Render/Shader/Shader.h"

#include "Snow/Platform/DirectX11/DirectXCommon.h"

#include <d3dcompiler.h>

namespace Snow {
	class DirectX11Shader : public Render::Shader {

		union ShaderBlobs {
			struct {
				ID3D10Blob* vsBlob;
				ID3D10Blob* gsBlob;
				ID3D10Blob* dsBlob;
				ID3D10Blob* hsBlob;
				ID3D10Blob* psBlob;
				ID3D10Blob* csBlob;
			};
			ID3D10Blob* of[6] = { nullptr };
		};

		struct ShaderStages {
			ID3D11VertexShader* vs = nullptr;
			ID3D11PixelShader* ps = nullptr;
			ID3D11GeometryShader* gs = nullptr;
			ID3D11HullShader* hs = nullptr;
			ID3D11DomainShader* ds = nullptr;
			ID3D11ComputeShader* cs = nullptr;

		};

	public:
		DirectX11Shader(Render::ShaderType type, const std::string& path);

		void Reload() override {}

		//const Render::ShaderType GetType() const override { return m_Type; }

		const std::string& GetPath() const override { return m_Path; }
		const std::string& GetName() const override { return m_Name; }

		
		const ShaderStages& GetShaderStage() const { return m_Stages; }
		ID3D10Blob* GetBlob(Render::ShaderType type);

	private:

		void CreateSPIRVBinaryCache();
		void CreateHLSLBinaryCache();

		void CreateDirectXShaderModule(ID3D10Blob*& blob, bool spirvmodule);

		std::string ReadShaderFromFile(const std::string& path);

		std::string m_Path, m_Name;
		std::string m_Source;

		Render::ShaderType m_Type;

		ShaderBlobs m_Blob;
		ShaderStages m_Stages;
		

		ID3D10Blob* m_D3D10Blob;
	};
}