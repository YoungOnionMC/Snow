#include <spch.h>
#include "Snow/Platform/Vulkan/VulkanPipeline.h"

#include "Snow/Platform/Vulkan/VulkanContext.h"

#include "Snow/Platform/Vulkan/VulkanShader.h"

#include <spirv_cross.hpp>

namespace Snow {
	namespace Render {
		VulkanPipeline::VulkanPipeline(const PipelineSpecification& spec) :
			m_Specification(spec) {

			Invalidate();

		}

		void VulkanPipeline::Invalidate() {
			Ref<VulkanPipeline> instance = this;

			Ref<VulkanDevice> vkDevice = VulkanContext::GetCurrentDevice();
			VulkanSwapChain vkSwapChain = VulkanContext::Get()->GetSwapChain();

			
			m_GraphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			m_GraphicsPipelineCreateInfo.layout = nullptr;
			

			VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
			inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

			VkPipelineRasterizationStateCreateInfo rasterizationState = {};
			rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
			rasterizationState.cullMode = VK_CULL_MODE_NONE;
			rasterizationState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
			rasterizationState.depthClampEnable = VK_FALSE;
			rasterizationState.rasterizerDiscardEnable = VK_FALSE;
			rasterizationState.depthBiasEnable = VK_FALSE;
			rasterizationState.depthBiasConstantFactor = 0.0f;
			rasterizationState.depthBiasClamp = 0.0f;
			rasterizationState.depthBiasSlopeFactor = 0.0f;
			rasterizationState.lineWidth = 1.0f;

			VkPipelineColorBlendAttachmentState blendAttachmentState[1] = {};
			blendAttachmentState[0].colorWriteMask = 0xf;
			blendAttachmentState[0].blendEnable = VK_FALSE;

			VkPipelineColorBlendStateCreateInfo colorBlendState = {};
			colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			colorBlendState.attachmentCount = 1;
			colorBlendState.pAttachments = blendAttachmentState;

			VkViewport viewport = {};
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = vkSwapChain.GetWidth();
			viewport.height = vkSwapChain.GetHeight();
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;

			VkRect2D scissor = {};
			scissor.offset = { 0, 0 };
			scissor.extent = { vkSwapChain.GetWidth(), vkSwapChain.GetHeight() };


			VkPipelineViewportStateCreateInfo viewportState = {};
			viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewportState.viewportCount = 1;
			viewportState.pViewports = &viewport;
			viewportState.scissorCount = 1;
			viewportState.pScissors = &scissor;

			VkPipelineMultisampleStateCreateInfo multisampleState = {};
			multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
			multisampleState.pSampleMask = nullptr;



			Reflect(m_GraphicsPipelineCreateInfo);
			m_GraphicsPipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
			m_GraphicsPipelineCreateInfo.pRasterizationState = &rasterizationState;
			m_GraphicsPipelineCreateInfo.pColorBlendState = &colorBlendState;
			m_GraphicsPipelineCreateInfo.pViewportState = &viewportState;
			m_GraphicsPipelineCreateInfo.pMultisampleState = &multisampleState;
			//m_GraphicsPipelineCreateInfo.pDepthStencilState = &depthStencilState;
			m_GraphicsPipelineCreateInfo.renderPass = vkSwapChain.GetRenderPass();

			VkPipelineCacheCreateInfo pipelineCacheInfo = {};
			pipelineCacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
			VkPipelineCache pipelineCache;
			vkCreatePipelineCache(vkDevice->GetVulkanDevice(), &pipelineCacheInfo, nullptr, &pipelineCache);

			vkCreateGraphicsPipelines(vkDevice->GetVulkanDevice(), pipelineCache, 1, &m_GraphicsPipelineCreateInfo, nullptr, &m_VulkanPipeline);
			
		}

		void VulkanPipeline::Bind() const {
			VulkanContext* vkContext = VulkanContext::Get();
			VulkanSwapChain vkSwapChain = vkContext->GetSwapChain();
			vkCmdBindPipeline(vkSwapChain.GetCurrentDrawCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_VulkanPipeline);
		}

		void VulkanPipeline::Reflect(VkGraphicsPipelineCreateInfo& pipelineCreateInfo) {
			VkVertexInputBindingDescription vertexInputBinding = {};
			vertexInputBinding.binding = 0;
			vertexInputBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			VkPipelineVertexInputStateCreateInfo vertexInputState = {};
			vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			vertexInputState.vertexBindingDescriptionCount = 1;

			std::vector<VkPipelineShaderStageCreateInfo> pipelineShaderStages;


			for (auto& shader : m_Specification.Shaders) {
				Ref<VulkanShader> vkShader = shader.As<VulkanShader>();
				spirv_cross::Compiler compiler(vkShader->GetSPIRVBinaryData());

				spirv_cross::ShaderResources res = compiler.get_shader_resources();

				if (shader->GetType() == ShaderType::Vertex) {
					std::vector<VkVertexInputAttributeDescription> vertexAttributes;

					uint32_t attribElementIndex = 0;
					uint32_t offset = 0;
					uint32_t size = 0;
					for (spirv_cross::Resource& resource : res.stage_inputs) {
						offset += size;
						auto& element = m_Specification.Layout.GetElements()[attribElementIndex];
						auto type = compiler.get_type(resource.type_id);

						if (type.basetype == 13 && element.GetComponentCount() != type.vecsize) {
							SNOW_CORE_WARN("Attrib {0} from the pipeline does not match the bound shaders", attribElementIndex);
						}

						VkVertexInputAttributeDescription vertexInputAttributeDesc = {};
						vertexInputAttributeDesc.binding = 0;
						vertexInputAttributeDesc.location = compiler.get_decoration(resource.id, spv::DecorationLocation);
						vertexInputAttributeDesc.format = SPIRVTypeToVKFormat(type);
						vertexInputAttributeDesc.offset = offset;

						vertexAttributes.push_back(vertexInputAttributeDesc);

						attribElementIndex++;
						size += AttribElement::GetTypeSize(element.Type);
					}
					vertexInputBinding.stride = size;

					vertexInputState.pVertexBindingDescriptions = &vertexInputBinding;
					vertexInputState.vertexAttributeDescriptionCount = attribElementIndex;
					vertexInputState.pVertexAttributeDescriptions = vertexAttributes.data();

				}

				VkPipelineShaderStageCreateInfo shaderStage = {};
				shaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				shaderStage.stage = SnowShaderTypeToVKShaderStage(shader->GetType());
				shaderStage.module = vkShader->GetVulkanShaderModule();
				shaderStage.pName = "main";

				pipelineShaderStages.push_back(shaderStage);

				std::vector<VkDescriptorSetLayoutBinding> descriptorSetBindings = {};
				for (spirv_cross::Resource& resource : res.uniform_buffers) {
					auto& type = compiler.get_type(resource.id);

					const auto& name = resource.name;
					auto& bufferType = compiler.get_type(resource.base_type_id);
					int memberCount = bufferType.member_types.size();
					uint32_t bindingPoint = compiler.get_decoration(resource.id, spv::DecorationBinding);
					uint32_t size = compiler.get_declared_struct_size(bufferType);

					if (m_UniformBuffers.find(bindingPoint) == m_UniformBuffers.end()) {
						VulkanUniformBuffer& buffer = m_UniformBuffers[bindingPoint];
						buffer.BindingPoint = bindingPoint;
						buffer.Size = size;
						buffer.Name = name;
						buffer.ShaderStage = SnowShaderTypeToVKShaderStage(shader->GetType());
					}



					VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
					descriptorSetLayoutBinding.binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
					descriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					descriptorSetLayoutBinding.stageFlags = SnowShaderTypeToVKShaderStage(shader->GetType());
					descriptorSetLayoutBinding.descriptorCount = 1;

					descriptorSetBindings.push_back(descriptorSetLayoutBinding);
				}
				

				for (spirv_cross::Resource& resource : res.push_constant_buffers) {

				}

			}
			
			pipelineCreateInfo.stageCount = pipelineShaderStages.size();
			pipelineCreateInfo.pStages = pipelineShaderStages.data();

			pipelineCreateInfo.pVertexInputState = &vertexInputState;


			
		}

		VkFormat VulkanPipeline::SPIRVTypeToVKFormat(spirv_cross::SPIRType type) {
			switch (type.basetype) {
			case spirv_cross::SPIRType::Boolean:	return VK_FORMAT_R16_SINT;
			case spirv_cross::SPIRType::Int: {
				if (type.vecsize == 1)	return VK_FORMAT_R32_UINT;
				if (type.vecsize == 2)	return VK_FORMAT_R32G32_UINT;
				if (type.vecsize == 3)	return VK_FORMAT_R32G32B32_UINT;
				if (type.vecsize == 4)	return VK_FORMAT_R32G32B32A32_UINT;
			}
			case spirv_cross::SPIRType::Float: {
				if (type.vecsize == 1)	return VK_FORMAT_R32_SFLOAT;
				if (type.vecsize == 2)	return VK_FORMAT_R32G32_SFLOAT;
				if (type.vecsize == 3)	return VK_FORMAT_R32G32B32_SFLOAT;
				if (type.vecsize == 4)	return VK_FORMAT_R32G32B32A32_SFLOAT;
			}
			}
		}

		VkShaderStageFlagBits VulkanPipeline::SnowShaderTypeToVKShaderStage(ShaderType type) {
			switch (type) {
			case ShaderType::Vertex:	return VK_SHADER_STAGE_VERTEX_BIT;
			case ShaderType::Pixel:		return VK_SHADER_STAGE_FRAGMENT_BIT;
			case ShaderType::Compute:	return VK_SHADER_STAGE_COMPUTE_BIT;
			case ShaderType::Geometry:	return VK_SHADER_STAGE_GEOMETRY_BIT;
			}
		}

		void VulkanPipeline::AllocateUniformBuffer(VulkanUniformBuffer& buffer) {
			Ref<VulkanDevice> vkDevice = VulkanContext::GetCurrentDevice();

			VkMemoryAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.pNext = nullptr;
			allocInfo.allocationSize = 0;
			allocInfo.memoryTypeIndex = 0;

			VkBufferCreateInfo bufferCreateInfo = {};
			bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferCreateInfo.size = buffer.Size;
			bufferCreateInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

			VulkanAllocator allocator("UniformBuffer");

			vkCreateBuffer(vkDevice->GetVulkanDevice(), &bufferCreateInfo, nullptr, &buffer.Buffer);

			VkMemoryRequirements memReqs;
			vkGetBufferMemoryRequirements(vkDevice->GetVulkanDevice(), buffer.Buffer, &memReqs);
			allocInfo.allocationSize = memReqs.size;

			allocator.Allocate(memReqs, &buffer.Memory, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
			vkBindBufferMemory(vkDevice->GetVulkanDevice(), buffer.Buffer, buffer.Memory, 0);

			buffer.DescriptorInfo.buffer = buffer.Buffer;
			buffer.DescriptorInfo.offset = 0;
			buffer.DescriptorInfo.range = buffer.Size;
		}
	}
}