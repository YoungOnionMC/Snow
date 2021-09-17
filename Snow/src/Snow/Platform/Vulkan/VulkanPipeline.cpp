#include <spch.h>
#include "Snow/Platform/Vulkan/VulkanPipeline.h"

#include "Snow/Platform/Vulkan/VulkanContext.h"
#include "Snow/Platform/Vulkan/VulkanFramebuffer.h"

#include "Snow/Platform/Vulkan/VulkanShader.h"

#include <spirv_cross.hpp>

namespace Snow {
	using namespace Render;

	static VkPrimitiveTopology GetVulkanTopology(PrimitiveTopology topology) {
		switch (topology) {
		case PrimitiveTopology::Points:	return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
		case PrimitiveTopology::Line:	return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
		case PrimitiveTopology::LineStrip:	return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
		case PrimitiveTopology::Triangle:	return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		case PrimitiveTopology::TriangleStrip:	return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
		case PrimitiveTopology::TriangleFan:	return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
		}
		SNOW_CORE_ASSERT(false, "Unknown topology");
		return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
	}

	VulkanPipeline::VulkanPipeline(const Render::PipelineSpecification& spec) :
		m_Specification(spec) {
		SNOW_CORE_ASSERT(spec.Shader);
		SNOW_CORE_ASSERT(spec.BindedRenderPass);
		Invalidate();
		Renderer::RegisterShaderDependency(spec.Shader, this);

	}

	static VkShaderStageFlagBits ShaderTypeToVKShaderStage(Render::ShaderType type) {
		switch (type) {
		case Render::ShaderType::Vertex:	return VK_SHADER_STAGE_VERTEX_BIT;
		case Render::ShaderType::Pixel:		return VK_SHADER_STAGE_FRAGMENT_BIT;
		case Render::ShaderType::Geometry:	return VK_SHADER_STAGE_GEOMETRY_BIT;
		}
	}

	void VulkanPipeline::Invalidate() {
		Ref<VulkanPipeline> instance = this;
		Renderer::Submit([instance]() mutable {

			SNOW_CORE_WARN("[Vulkan Pipeline] Creating Pipeline {0}", instance->m_Specification.DebugName);

			Ref<VulkanDevice> vkDevice = VulkanContext::GetCurrentDevice();
			VulkanSwapChain vkSwapChain = VulkanContext::Get()->GetSwapChain();

			Ref<VulkanShader> vkShader = instance->m_Specification.Shader.As<VulkanShader>();

			auto descriptorSetLayouts = vkShader->GetAllDescriptorSetLayouts();
			auto pushConstantRanges = vkShader->GetPushConstantRanges();

			std::vector<VkPushConstantRange> vkpushConstantRanges;
			for (auto pcr : pushConstantRanges) {
				VkPushConstantRange pushconstantrange;
				pushconstantrange.stageFlags = pcr.ShaderStage;
				pushconstantrange.size = pcr.Size;
				pushconstantrange.offset = pcr.Offset;
				vkpushConstantRanges.push_back(pushconstantrange);
			}

			VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
			pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipelineLayoutInfo.pNext = nullptr;
			pipelineLayoutInfo.setLayoutCount = descriptorSetLayouts.size();
			pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
			pipelineLayoutInfo.pushConstantRangeCount = vkpushConstantRanges.size();
			pipelineLayoutInfo.pPushConstantRanges = vkpushConstantRanges.data();

			VKCheckError(vkCreatePipelineLayout(vkDevice->GetVulkanDevice(), &pipelineLayoutInfo, nullptr, &instance->m_PipelineLayout));

			// Input Assembly
			VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
			inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			inputAssemblyState.topology = GetVulkanTopology(instance->m_Specification.Topology);

			// Rasterization Type
			VkPipelineRasterizationStateCreateInfo rasterizationState = {};
			rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rasterizationState.polygonMode = instance->m_Specification.Wireframe ? VK_POLYGON_MODE_LINE : VK_POLYGON_MODE_FILL;
			rasterizationState.cullMode = instance->m_Specification.BackfaceCulling ? VK_CULL_MODE_BACK_BIT : VK_CULL_MODE_NONE;
			rasterizationState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
			rasterizationState.depthClampEnable = VK_FALSE;
			rasterizationState.rasterizerDiscardEnable = VK_FALSE;
			rasterizationState.depthBiasEnable = VK_FALSE;
			rasterizationState.depthBiasConstantFactor = 0.0f;
			rasterizationState.depthBiasClamp = 0.0f;
			rasterizationState.depthBiasSlopeFactor = 0.0f;
			rasterizationState.lineWidth = instance->m_Specification.LineWidth;

			// Color Blending
			VkPipelineColorBlendAttachmentState blendAttachmentState[1] = {};
			blendAttachmentState[0].colorWriteMask = 0xf;
			blendAttachmentState[0].blendEnable = VK_FALSE;

			VkPipelineColorBlendStateCreateInfo colorBlendState = {};
			colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			colorBlendState.attachmentCount = 1;
			colorBlendState.pAttachments = blendAttachmentState;

			// Viewport Creation
			VkViewport viewport = {};
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = vkSwapChain.GetWidth();
			viewport.height = vkSwapChain.GetHeight();
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;

			// Scissor Creation
			VkRect2D scissor = {};
			scissor.offset = { 0, 0 };
			scissor.extent = { vkSwapChain.GetWidth(), vkSwapChain.GetHeight() };


			VkPipelineViewportStateCreateInfo viewportState = {};
			viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewportState.viewportCount = 1;
			viewportState.pViewports = &viewport;
			viewportState.scissorCount = 1;
			viewportState.pScissors = &scissor;

			// Multisample Creation
			VkPipelineMultisampleStateCreateInfo multisampleState = {};
			multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
			multisampleState.pSampleMask = nullptr;

			// Depth Stencil state
			VkPipelineDepthStencilStateCreateInfo depthStencilStateInfo = {};
			depthStencilStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
			depthStencilStateInfo.depthTestEnable = instance->m_Specification.DepthTest ? VK_TRUE : VK_FALSE;
			depthStencilStateInfo.depthWriteEnable = instance->m_Specification.DepthWrite ? VK_TRUE : VK_FALSE;
			depthStencilStateInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
			depthStencilStateInfo.depthBoundsTestEnable = VK_FALSE;
			depthStencilStateInfo.back.failOp = VK_STENCIL_OP_KEEP;
			depthStencilStateInfo.back.passOp = VK_STENCIL_OP_KEEP;
			depthStencilStateInfo.back.compareOp = VK_COMPARE_OP_ALWAYS;
			depthStencilStateInfo.stencilTestEnable = VK_FALSE;
			depthStencilStateInfo.front = depthStencilStateInfo.back;

			// Dynamic States
			std::vector<VkDynamicState> dynamicStateEnables;
			dynamicStateEnables.push_back(VK_DYNAMIC_STATE_VIEWPORT);
			dynamicStateEnables.push_back(VK_DYNAMIC_STATE_SCISSOR);
			if (instance->m_Specification.Topology == PrimitiveTopology::Line || instance->m_Specification.Topology == PrimitiveTopology::LineStrip || instance->m_Specification.Wireframe)
				dynamicStateEnables.push_back(VK_DYNAMIC_STATE_LINE_WIDTH);

			VkPipelineDynamicStateCreateInfo dynamicState = {};
			dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			dynamicState.pNext = nullptr;
			dynamicState.flags = 0;
			dynamicState.dynamicStateCount = dynamicStateEnables.size();
			dynamicState.pDynamicStates = dynamicStateEnables.data();

			// Vertex Input Bindings
			VkVertexInputBindingDescription vertexInputBinding = {};
			vertexInputBinding.binding = 0;
			vertexInputBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			vertexInputBinding.stride = instance->m_Specification.Layout.GetStride();

			VkPipelineVertexInputStateCreateInfo vertexInputState = {};
			vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			vertexInputState.vertexBindingDescriptionCount = 1;
			vertexInputState.pVertexBindingDescriptions = &vertexInputBinding;
			const auto vertinputAttribs = vkShader->GetVertexInputAttributes();
			vertexInputState.vertexAttributeDescriptionCount = vertinputAttribs.size();
			vertexInputState.pVertexAttributeDescriptions = vertinputAttribs.data();

			// Shader Attachments
			std::vector<VkPipelineShaderStageCreateInfo> pipelineShaderStages;
			for (uint32_t i = 0; i < instance->m_Specification.Shader->GetModuleCount(); i++) {
				VkPipelineShaderStageCreateInfo shaderStage = {};
				shaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				shaderStage.stage = ShaderTypeToVKShaderStage(vkShader->GetShaderModuleType(i));
				SNOW_CORE_ASSERT(vkShader->GetShaderModuleType(i) != Render::ShaderType::Compute, "Graphics pipeline is not compatible with compute shaders!");
				shaderStage.module = vkShader->GetVulkanShaderModule(i);
				shaderStage.pName = "main";

				pipelineShaderStages.push_back(shaderStage);
			}

			Ref<VulkanFramebuffer> vkFramebuffer = instance->m_Specification.BindedRenderPass->GetSpecification().TargetFramebuffer.As<VulkanFramebuffer>();

			VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
			pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

			// Pipeline Layout
			pipelineCreateInfo.layout = instance->m_PipelineLayout;
			pipelineCreateInfo.basePipelineIndex = -1;
			pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
			pipelineCreateInfo.flags = 0;

			// Applying all previous pipeline states
			pipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
			pipelineCreateInfo.pRasterizationState = &rasterizationState;
			pipelineCreateInfo.pColorBlendState = &colorBlendState;
			pipelineCreateInfo.pViewportState = &viewportState;
			pipelineCreateInfo.pMultisampleState = &multisampleState;
			pipelineCreateInfo.pDepthStencilState = &depthStencilStateInfo;
			pipelineCreateInfo.pDynamicState = &dynamicState;
			pipelineCreateInfo.renderPass = vkFramebuffer->GetRenderPass();//vkSwapChain.GetRenderPass();

			pipelineCreateInfo.stageCount = pipelineShaderStages.size();
			pipelineCreateInfo.pStages = pipelineShaderStages.data();
			pipelineCreateInfo.pVertexInputState = &vertexInputState;

			VkPipelineCacheCreateInfo pipelineCacheInfo = {};
			pipelineCacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
			VkPipelineCache pipelineCache;
			vkCreatePipelineCache(vkDevice->GetVulkanDevice(), &pipelineCacheInfo, nullptr, &pipelineCache);

			vkCreateGraphicsPipelines(vkDevice->GetVulkanDevice(), pipelineCache, 1, &pipelineCreateInfo, nullptr, &instance->m_VulkanPipeline);
		});
	}

	void VulkanPipeline::Bind() const {
		Ref<VulkanContext> vkContext = VulkanContext::Get();
		VulkanSwapChain vkSwapChain = vkContext->GetSwapChain();
		vkCmdBindPipeline(vkSwapChain.GetCurrentDrawCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_VulkanPipeline);
	}

	void VulkanPipeline::Reflect(VkGraphicsPipelineCreateInfo& pipelineCreateInfo) {
		

			
				

		
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

	

}