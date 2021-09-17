#pragma once

#include "Snow/Render/Pipeline.h"

#include "Snow/Platform/DirectX11/DirectXCommon.h"

#include <d3d11shader.h>

#include "Snow/Render/ShaderUniform.h"

namespace Snow {


	union ShaderReflections {
		struct {
			ID3D11ShaderReflection* vsRefl;
			ID3D11ShaderReflection* gsRefl;
			ID3D11ShaderReflection* dsRefl;
			ID3D11ShaderReflection* hsRefl;
			ID3D11ShaderReflection* psRefl;
			ID3D11ShaderReflection* csRefl;
		};
		ID3D11ShaderReflection* of[6] = { nullptr };
	};

	struct ConstantBuffer {
		Render::ShaderUniformBuffer UniformBuffer;
		ID3D11Buffer* Data;
	};

	class DirectX11Pipeline : public Render::Pipeline {
	public:
		DirectX11Pipeline(const Render::PipelineSpecification& spec);

		void Bind() const override;

		virtual const Render::ShaderUniformBuffer& GetUniformBuffer(const std::string& name) const;
		virtual const std::unordered_map<std::string, Render::ShaderResource>& GetResources() const { return m_Resources; };

		virtual void SetUniformBufferData(const std::string& uniformBufferName, void* data, uint32_t size);
		virtual void SetUniformBufferData(uint32_t bindingPoint, void* data = nullptr, uint32_t size = 0) {}

		const Render::PipelineSpecification& GetSpecification() const override { return m_Specification; }
	private:
		void Invalidate();
		void Reflect();

		void ReflectConstantBuffersLayouts(ID3D11ShaderReflection*& shaderReflection, Render::ShaderType type);
		void ReflectResources(ID3D11ShaderReflection*& shaderReflection, Render::ShaderType type);

		Render::PipelineSpecification m_Specification;

		ShaderReflections m_Reflections;

		std::vector<ConstantBuffer> m_UniformBuffers;
		std::unordered_map<std::string, Render::ShaderResource> m_Resources;

		ID3D11InputLayout* m_InputLayout;
		ID3D11RasterizerState* m_RasterizerState;
	};
}