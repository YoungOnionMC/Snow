#include <spch.h>

#include "Snow/Platform/DirectX11/DirectXPipeline.h"

#include "Snow/Platform/DirectX11/DirectXContext.h"

#include "Snow/Platform/DirectX11/DirectXShader.h"



namespace Snow {
	DirectX11Pipeline::DirectX11Pipeline(const Render::PipelineSpecification& spec) :
		m_Specification(spec) {
		Invalidate();

		Reflect();
	}

	void DirectX11Pipeline::Bind() const {
		auto dxDevice = DirectX11RenderContext::Get()->GetDevice();

		for (auto shader : m_Specification.Shaders) {
			Ref<DirectX11Shader>& dxShader = shader.As<DirectX11Shader>();
			switch (dxShader->GetType()) {
			case Render::ShaderType::Vertex:	dxDevice->GetDeviceContext()->VSSetShader(dxShader->GetShaderStage().vs, NULL, 0); break;
			case Render::ShaderType::Pixel:		dxDevice->GetDeviceContext()->PSSetShader(dxShader->GetShaderStage().ps, NULL, 0); break;
			case Render::ShaderType::Geometry:	dxDevice->GetDeviceContext()->GSSetShader(dxShader->GetShaderStage().gs, NULL, 0); break;
			case Render::ShaderType::Compute:	dxDevice->GetDeviceContext()->CSSetShader(dxShader->GetShaderStage().cs, NULL, 0); break;
			}
		}

		
		dxDevice->GetDeviceContext()->IASetInputLayout(m_InputLayout);
	}

	void DirectX11Pipeline::Invalidate() {
		auto dxDevice = DirectX11RenderContext::Get()->GetDevice();
		auto dxSwapChain = DirectX11RenderContext::Get()->GetSwapChain();

		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = dxSwapChain.GetWidth();
		viewport.Height = dxSwapChain.GetHeight();
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		dxDevice->GetDeviceContext()->RSSetViewports(1, &viewport);

		D3D11_RASTERIZER_DESC rasterDesc;
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_NONE;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = true;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		dxDevice->GetDevice()->CreateRasterizerState(&rasterDesc, &m_RasterizerState);
		dxDevice->GetDeviceContext()->RSSetState(m_RasterizerState);
	}

	static const std::unordered_map<Render::ShaderType, uint32_t> SHADERTYPE = {
		{Render::ShaderType::Vertex, 0},
		{Render::ShaderType::Pixel, 4},
		{Render::ShaderType::Compute, 5},
		{Render::ShaderType::Geometry, 1}
	};

	static DXGI_FORMAT GetInputLayoutFormat(D3D11_SIGNATURE_PARAMETER_DESC desc) {
		if (desc.Mask < 2) {
			if (desc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)		return DXGI_FORMAT_R32_UINT;
			else if (desc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)	return DXGI_FORMAT_R32_SINT;
			else if (desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)	return DXGI_FORMAT_R32_FLOAT;
		}
		else if (desc.Mask < 4) {
			if (desc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)		return DXGI_FORMAT_R32G32_UINT;
			else if (desc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)	return DXGI_FORMAT_R32G32_SINT;
			else if (desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)	return DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (desc.Mask < 8) {
			if (desc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)		return DXGI_FORMAT_R32G32B32_UINT;
			else if (desc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)	return DXGI_FORMAT_R32G32B32_SINT;
			else if (desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)	return DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (desc.Mask < 16) {
			if (desc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)		return DXGI_FORMAT_R32G32B32A32_UINT;
			else if (desc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)	return DXGI_FORMAT_R32G32B32A32_SINT;
			else if (desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)	return DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		return DXGI_FORMAT_UNKNOWN;
	}

	const Render::ShaderUniformBuffer& DirectX11Pipeline::GetUniformBuffer(const std::string& name) const {
		for (auto& cbuffer : m_UniformBuffers) {
			if (cbuffer.UniformBuffer.Name == name) {
				return cbuffer.UniformBuffer;
			}
		}
		SNOW_CORE_WARN("Could not find uniform buffer");
		return m_UniformBuffers[0].UniformBuffer;
	}


	void DirectX11Pipeline::SetUniformBufferData(const std::string& uniformBufferName, void* data, uint32_t size) {
		auto dxDevice = DirectX11RenderContext::Get()->GetDevice();

		Render::ShaderUniformBuffer* uniformBuffer = nullptr;
		ID3D11Buffer* d = nullptr;
		
		for (auto& [ub, cbData] : m_UniformBuffers) {
			if (ub.Name == uniformBufferName) {
				uniformBuffer = &ub;
				d = cbData;
				break;
			}
		}
		if (uniformBuffer == nullptr)
			return;

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		memset(&mappedResource, 0, sizeof(D3D11_MAPPED_SUBRESOURCE));
		dxDevice->GetDeviceContext()->Map(d, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy(mappedResource.pData, data, size);
		dxDevice->GetDeviceContext()->Unmap(d, 0);

		switch (uniformBuffer->Stage) {
		case Render::ShaderType::Vertex:	dxDevice->GetDeviceContext()->VSSetConstantBuffers(uniformBuffer->BindingPoint, 1, &d); break;
		case Render::ShaderType::Pixel:		dxDevice->GetDeviceContext()->PSSetConstantBuffers(uniformBuffer->BindingPoint, 1, &d); break;
		case Render::ShaderType::Geometry:	dxDevice->GetDeviceContext()->GSSetConstantBuffers(uniformBuffer->BindingPoint, 1, &d); break;
		case Render::ShaderType::Compute:	dxDevice->GetDeviceContext()->CSSetConstantBuffers(uniformBuffer->BindingPoint, 1, &d); break;
		
		}
		//delete d;
	}

	void DirectX11Pipeline::Reflect() {
		auto dxDevice = DirectX11RenderContext::Get()->GetDevice();

		for (auto shader : m_Specification.Shaders) {
			Ref<DirectX11Shader>& dxShader = shader.As<DirectX11Shader>();
			ID3D10Blob* b = dxShader->GetBlob(dxShader->GetType());
			if (b) {
				void** buffer = reinterpret_cast<void**>(&m_Reflections.of[SHADERTYPE.at(dxShader->GetType())]);
				DXCheckError(D3DReflect(b->GetBufferPointer(), b->GetBufferSize(), IID_ID3D11ShaderReflection, buffer));
			}
		}

		if (m_Reflections.vsRefl) {
			D3D11_SHADER_DESC shaderDesc = {};
			m_Reflections.vsRefl->GetDesc(&shaderDesc);
			std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout(shaderDesc.InputParameters);
			
			D3D_PRIMITIVE primitiveDesc = shaderDesc.InputPrimitive;

			for (uint32_t i = 0; i < shaderDesc.InputParameters; i++) {
				D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
				m_Reflections.vsRefl->GetInputParameterDesc(i, &paramDesc);

				D3D11_INPUT_ELEMENT_DESC elementDesc;
				elementDesc.SemanticName = paramDesc.SemanticName;
				elementDesc.SemanticIndex = paramDesc.SemanticIndex;
				elementDesc.InputSlot = 0;
				elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
				elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				elementDesc.InstanceDataStepRate = 0;

				elementDesc.Format = GetInputLayoutFormat(paramDesc);


				inputLayout[i] = elementDesc;
			}

			const auto* data = inputLayout.data();
			if (data) {
				Ref<DirectX11Shader> dxVSShader;
				for (auto shader : m_Specification.Shaders) {
					Ref<DirectX11Shader> dxShader = shader.As<DirectX11Shader>();
					if (dxShader->GetType() == Render::ShaderType::Vertex)
						dxVSShader = dxShader;
				}
				if (dxVSShader)
					DXCheckError(dxDevice->GetDevice()->CreateInputLayout(data, shaderDesc.InputParameters,
						dxVSShader->GetBlob(Render::ShaderType::Vertex)->GetBufferPointer(),
						dxVSShader->GetBlob(Render::ShaderType::Vertex)->GetBufferSize(),
						&m_InputLayout));

				if (!m_InputLayout)
					SNOW_CORE_WARN("Failed to create InputLayout");
			}
		}

		for (auto shader : m_Specification.Shaders) {
			Ref<DirectX11Shader> dxShader = shader.As<DirectX11Shader>();
			ReflectConstantBuffersLayouts(m_Reflections.of[SHADERTYPE.at(dxShader->GetType())], dxShader->GetType());
		}

		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		for (auto& [buffer, data] : m_UniformBuffers) {
			bufferDesc.ByteWidth = buffer.Size;
			DXCheckError(dxDevice->GetDevice()->CreateBuffer(&bufferDesc, nullptr, &data));
		}

		for (auto shader : m_Specification.Shaders) {
			Ref<DirectX11Shader> dxShader = shader.As<DirectX11Shader>();
			ReflectResources(m_Reflections.of[SHADERTYPE.at(dxShader->GetType())], dxShader->GetType());
		}

		for (auto& shader : m_Specification.Shaders) { // No need for the blobs anymore, reflection is done and shaders have been created
			auto& dxShader = shader.As<DirectX11Shader>();
			dxShader->GetBlob(shader->GetType())->Release();
		}
	}

	Render::UniformType GetTypeFromTypeDesc(const D3D11_SHADER_TYPE_DESC& typeDesc) {
		using namespace Render;
		Render::UniformType retType = Render::UniformType::None;
		switch (typeDesc.Type) {
			case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_BOOL:	retType = UniformType::Bool; break;
			case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_INT: {
				if (typeDesc.Columns == 1) retType = UniformType::Int;	break;
				if (typeDesc.Columns == 2) retType = UniformType::Int2; break;
				if (typeDesc.Columns == 3) retType = UniformType::Int3;	break;
				if (typeDesc.Columns == 4) retType = UniformType::Int4;	break;
			}
			case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_FLOAT: {
				if (typeDesc.Columns == 1) retType = UniformType::Float;	break;
				if (typeDesc.Columns == 2) retType = UniformType::Float2;	break;
				if (typeDesc.Columns == 3) {
					retType = UniformType::Float3;
					if (typeDesc.Rows == 3)	retType = UniformType::Mat3x3;
					break;
				}
				if (typeDesc.Columns == 4) {
					retType = UniformType::Float4;
					if (typeDesc.Rows == 4)	retType = UniformType::Mat4x4;
					break;
				}
			}
		}
		return retType;
	}

	void DirectX11Pipeline::ReflectConstantBuffersLayouts(ID3D11ShaderReflection*& shaderReflection, Render::ShaderType type) {
		auto dxDevice = DirectX11RenderContext::Get()->GetDevice();

		D3D11_SHADER_DESC desc;
		shaderReflection->GetDesc(&desc);

		uint32_t bufferSlot = 0;
		for (uint32_t i = 0; i < desc.ConstantBuffers; i++) {
			//ConstantBufferLayout bufferLayout;
			//bufferLayout.buffSize = 0;
			D3D11_SHADER_BUFFER_DESC desc;
			ID3D11ShaderReflectionConstantBuffer* constBuffer = shaderReflection->GetConstantBufferByIndex(i);
			constBuffer->GetDesc(&desc);

			Render::ShaderUniformBuffer buffer = {};
			buffer.Name = desc.Name;
			buffer.BindingPoint = i;
			buffer.Size = 0;
			uint32_t bufferSize = 0;
			for (uint32_t j = 0; j < desc.Variables; j++) {
				ID3D11ShaderReflectionVariable* var = constBuffer->GetVariableByIndex(j);
				D3D11_SHADER_VARIABLE_DESC variableDesc;
				var->GetDesc(&variableDesc);

				

				//bufferLayout.uniforms.push_back(variableDesc);

				ID3D11ShaderReflectionType* type = var->GetType();
				D3D11_SHADER_TYPE_DESC typeDesc;
				type->GetDesc(&typeDesc);

				buffer.Uniforms.push_back(Render::ShaderUniform(variableDesc.Name, GetTypeFromTypeDesc(typeDesc), variableDesc.Size, variableDesc.StartOffset, typeDesc.Elements));

				//bufferLayout.types.push_back(typeDesc);

				buffer.Size += variableDesc.Size;
			}
			buffer.Stage = type;
			m_UniformBuffers.push_back({ buffer, nullptr });

			bufferSlot++;
		}
	}

	void DirectX11Pipeline::ReflectResources(ID3D11ShaderReflection*& shaderReflection, Render::ShaderType type) {
		uint32_t texSlot = 0, samplerSlot = 0;
		
		D3D11_SHADER_DESC desc = {};
		shaderReflection->GetDesc(&desc);

		for (uint32_t i = 0; i < desc.BoundResources; i++) {
			D3D11_SHADER_INPUT_BIND_DESC ibDesc;
			shaderReflection->GetResourceBindingDesc(i, &ibDesc);
			Render::ShaderResource resource;
			if (ibDesc.Type == D3D_SIT_SAMPLER)
				resource = Render::ShaderResource(ibDesc.Name, type, Render::ResourceType::Sampler, ibDesc.BindPoint, ibDesc.BindCount);
			else if (ibDesc.Type == D3D_SIT_TEXTURE) {
				if (ibDesc.Dimension == D3D_SRV_DIMENSION::D3D10_1_SRV_DIMENSION_TEXTURE2D)
					resource = Render::ShaderResource(ibDesc.Name, type, Render::ResourceType::Texture2D, ibDesc.BindPoint, ibDesc.BindCount);
				else if (ibDesc.Dimension == D3D_SRV_DIMENSION::D3D10_1_SRV_DIMENSION_TEXTURECUBE)
					resource = Render::ShaderResource(ibDesc.Name, type, Render::ResourceType::TextureCube, ibDesc.BindPoint, ibDesc.BindCount);
			}
			else if (ibDesc.Type == D3D_SIT_CBUFFER) // already parsed cbuffers
				return;
			m_Resources[ibDesc.Name] = resource;
		}
	}
}