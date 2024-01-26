#include <spch.h>
#include "Snow/Platform/Vulkan/VulkanRenderer.h"

#include <vulkan/vulkan.h>

#include "Snow/Render/Renderer.h"
#include "Snow/Render/SceneRenderer.h"

#include "Snow/Platform/Vulkan/VulkanBuffer.h"
#include "Snow/Platform/Vulkan/VulkanComputePipeline.h"
#include "Snow/Platform/Vulkan/VulkanContext.h"
#include "Snow/Platform/Vulkan/VulkanFramebuffer.h"
#include "Snow/Platform/Vulkan/VulkanMaterial.h"
#include "Snow/Platform/Vulkan/VulkanPipeline.h"
#include "Snow/Platform/Vulkan/VulkanRenderCommandBuffer.h"
#include "Snow/Platform/Vulkan/VulkanStorageBuffer.h"
#include "Snow/Platform/Vulkan/VulkanSwapChain.h"
#include "Snow/Platform/Vulkan/VulkanTexture.h"
#include "Snow/Platform/Vulkan/VulkanUniformBuffer.h"



namespace Snow {
	
	using namespace Render;
	struct VulkanRendererData {
		RenderCapabilities RenderCaps;

		Ref<Texture2D> BRDFLut;

		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<IndexBuffer> QuadIndexBuffer;
		VulkanShader::ShaderMaterialDescriptorSet QuadDescriptorSet;

		std::unordered_map<SceneRenderer*, std::vector<VulkanShader::ShaderMaterialDescriptorSet>> RendererDescriptorSet;
		VkDescriptorSet ActiveRendererDescriptorSet = nullptr;
		std::vector<VkDescriptorPool> DescriptorPools;
		std::vector<uint32_t> DescriptorPoolAllocationCount;

		using WriteDescriptors = std::vector<std::vector<VkWriteDescriptorSet>>;

		std::unordered_map<UniformBufferSet*, std::unordered_map<uint64_t, WriteDescriptors>> UniformBufferWriteDescriptorCache;
		std::unordered_map<StorageBufferSet*, std::unordered_map<uint64_t, WriteDescriptors>> StorageBufferWriteDescriptorCache;

		uint32_t DrawCallCount = 0;
		int32_t SelectedDrawCall = -1;
	};

	static VulkanRendererData* s_Data = nullptr;

	void Snow::VulkanRenderer::Init() {
		using namespace Render;

		s_Data = new VulkanRendererData();
		const auto& config = Renderer::GetConfig();
		s_Data->DescriptorPools.resize(config.FramesInFlight);
		s_Data->DescriptorPoolAllocationCount.resize(config.FramesInFlight);

		auto& caps = s_Data->RenderCaps;
		auto& properties = VulkanContext::GetCurrentDevice()->GetProperties();
		caps.Vendor = properties.vendorID;
		caps.Device = properties.deviceName;
		caps.Version = std::to_string(properties.driverVersion);

		Renderer::Submit([]() {
			VkDescriptorPoolSize poolSizes[] = {
				{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
				{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
				{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
				{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
			};

			VkDescriptorPoolCreateInfo poolInfo = {};
			poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
			poolInfo.maxSets = 100000;
			poolInfo.poolSizeCount = (uint32_t)IM_ARRAYSIZE(poolSizes);
			poolInfo.pPoolSizes = poolSizes;
			VkDevice device = VulkanContext::GetCurrentDevice()->GetVulkanDevice();
			uint32_t framesInFlight = Renderer::GetConfig().FramesInFlight;
			for (uint32_t i = 0; i < framesInFlight; i++) {
				VKCheckError(vkCreateDescriptorPool(device, &poolInfo, nullptr, &s_Data->DescriptorPools[i]));
				s_Data->DescriptorPoolAllocationCount[i] = 0;
			}
			});

		// Create fullscreen quad
		{
			float x = -1;
			float y = -1;
			float width = 2, height = 2;
			struct QuadVertex {
				glm::vec3 Position;
				glm::vec2 TexCoord;
			};

			QuadVertex* data = new QuadVertex[4];

			data[0].Position = glm::vec3(x, y, 0.0f);
			data[0].TexCoord = glm::vec2(0, 0);

			data[1].Position = glm::vec3(x + width, y, 0.0f);
			data[1].TexCoord = glm::vec2(1, 0);

			data[2].Position = glm::vec3(x + width, y + height, 0.0f);
			data[2].TexCoord = glm::vec2(1, 1);

			data[3].Position = glm::vec3(x, y + height, 0.0f);
			data[3].TexCoord = glm::vec2(0, 1);

			s_Data->QuadVertexBuffer = VertexBuffer::Create(data, 4 * sizeof(QuadVertex));
			uint32_t indices[6] = { 0, 1, 2, 2, 3, 0, };
			s_Data->QuadIndexBuffer = IndexBuffer::Create(indices, 6 * sizeof(uint32_t));
		}

		s_Data->BRDFLut = Render::Renderer::GetBRDFLutTexture();

	}

	void Snow::VulkanRenderer::Shutdown() {
		VulkanShader::ClearUniformBuffers();
		delete s_Data;
	}

	
	void VulkanRenderer::BeginFrame() {
		Renderer::Submit([]() {
			Ref<VulkanSwapChain> swapChain = Core::Application::Get().GetWindow()->GetSwapChain().As<VulkanSwapChain>();

			auto vkDevice = VulkanContext::GetCurrentDevice();
			uint32_t bufferIndex = swapChain->GetCurrentBufferIndex();
			VKCheckError(vkResetDescriptorPool(vkDevice->GetVulkanDevice(), s_Data->DescriptorPools[bufferIndex], 0));
			memset(s_Data->DescriptorPoolAllocationCount.data(), 0, s_Data->DescriptorPoolAllocationCount.size() * sizeof(uint32_t));

			s_Data->DrawCallCount = 0;
		});

	}

	void VulkanRenderer::EndFrame() {
	}

	void VulkanRenderer::BeginRenderPass(Ref<RenderCommandBuffer> commandBuffer, const Ref<RenderPass>& renderPass, bool explicitClear) {
		Renderer::Submit([commandBuffer, renderPass, explicitClear]() {
			uint32_t frameIndex = Renderer::GetCurrentFrameIndex();
			VkCommandBuffer renderCommandBuffer = commandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(frameIndex);

			auto fb = renderPass->GetSpecification().TargetFramebuffer;
			Ref<VulkanFramebuffer> framebuffer = fb.As<VulkanFramebuffer>();
			const auto& fbSpec = framebuffer->GetSpecification();

			uint32_t width = framebuffer->GetWidth();
			uint32_t height = framebuffer->GetHeight();

			VkViewport viewport = {};
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;

			VkRenderPassBeginInfo renderPassBeginInfo = {};
			renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassBeginInfo.pNext = nullptr;
			renderPassBeginInfo.renderPass = framebuffer->GetRenderPass();
			renderPassBeginInfo.renderArea.offset.x = 0;
			renderPassBeginInfo.renderArea.offset.y = 0;
			renderPassBeginInfo.renderArea.extent.width = width;
			renderPassBeginInfo.renderArea.extent.height = height;

			if (framebuffer->GetSpecification().SwapChainTarget) {
				Ref<VulkanSwapChain> swapChain = Core::Application::Get().GetWindow()->GetSwapChain().As<VulkanSwapChain>();
				width = swapChain->GetWidth();
				height = swapChain->GetHeight();
				renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				renderPassBeginInfo.pNext = nullptr;
				renderPassBeginInfo.renderPass = framebuffer->GetRenderPass();
				renderPassBeginInfo.renderArea.offset.x = 0;
				renderPassBeginInfo.renderArea.offset.y = 0;
				renderPassBeginInfo.renderArea.extent.width = width;
				renderPassBeginInfo.renderArea.extent.height = height;
				renderPassBeginInfo.framebuffer = swapChain->GetCurrentFramebuffer();

				viewport.x = 0.0f;
				viewport.y = (float)height;
				viewport.width = (float)width;
				viewport.height = -(float)height;
			}
			else {
				width = framebuffer->GetWidth();
				height = framebuffer->GetHeight();
				renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				renderPassBeginInfo.pNext = nullptr;
				renderPassBeginInfo.renderPass = framebuffer->GetRenderPass();
				renderPassBeginInfo.renderArea.offset.x = 0;
				renderPassBeginInfo.renderArea.offset.y = 0;
				renderPassBeginInfo.renderArea.extent.width = width;
				renderPassBeginInfo.renderArea.extent.height = height;
				renderPassBeginInfo.framebuffer = framebuffer->GetFramebufferHandle();

				viewport.x = 0.0f;
				viewport.y = 0.0f;
				viewport.width = (float)width;
				viewport.height = (float)height;
			}

			// TODO: Does our framebuffer have a depth attachment?
			const auto& clearValues = framebuffer->GetVkClearValues();
			renderPassBeginInfo.clearValueCount = clearValues.size();
			renderPassBeginInfo.pClearValues = clearValues.data();
			

			vkCmdBeginRenderPass(renderCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

			if (explicitClear) {
				const uint32_t colorAttachmentCount = (uint32_t)framebuffer->GetColorAttachmentCount();
				bool HasDepth = framebuffer->HasDepthAttachment();
				const uint32_t totalAttachmentCount = colorAttachmentCount + (HasDepth ? 1 : 0);
				SNOW_CORE_ASSERT(clearValues.size() == totalAttachmentCount);

				std::vector<VkClearAttachment> attachments(totalAttachmentCount);
				std::vector<VkClearRect> clearRects(totalAttachmentCount);
				for (uint32_t i = 0; i < colorAttachmentCount; i++) {
					attachments[i].aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					attachments[i].colorAttachment = i;
					attachments[i].clearValue = clearValues[i];

					clearRects[i].rect.offset = { (int32_t)0, (int32_t)0 };
					clearRects[i].rect.extent = { width, height };
					clearRects[i].baseArrayLayer = 0;
					clearRects[i].layerCount = 1;
				}

				if (framebuffer->HasDepthAttachment()) {
					ImageFormat format = framebuffer->GetDepthImage()->GetSpecification().Format;
					attachments[colorAttachmentCount].aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | (format == ImageFormat::Depth24Stencil8 ? VK_IMAGE_ASPECT_STENCIL_BIT : 0);
					attachments[colorAttachmentCount].clearValue = clearValues[colorAttachmentCount];

					clearRects[colorAttachmentCount].rect.offset = { (int32_t)0, (int32_t)0 };
					clearRects[colorAttachmentCount].rect.extent = { width, height };
					clearRects[colorAttachmentCount].baseArrayLayer = 0;
					clearRects[colorAttachmentCount].layerCount = 1;
				}
				
				vkCmdClearAttachments(renderCommandBuffer, totalAttachmentCount, attachments.data(), totalAttachmentCount, clearRects.data());
			}

			// Update dynamic viewport state
			vkCmdSetViewport(renderCommandBuffer, 0, 1, &viewport);

			// Update dynamic scissor state
			VkRect2D scissor = {};
			scissor.extent.width = width;
			scissor.extent.height = height;
			scissor.offset.x = 0;
			scissor.offset.y = 0;
			vkCmdSetScissor(renderCommandBuffer, 0, 1, &scissor);
		});
	}

	void VulkanRenderer::EndRenderPass(Ref<RenderCommandBuffer> commandBuffer) {
		Renderer::Submit([commandBuffer]() {
			uint32_t frameIndex = Renderer::GetCurrentFrameIndex();
			VkCommandBuffer renderCommandBuffer = commandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(frameIndex);

			vkCmdEndRenderPass(renderCommandBuffer);
		});
	}

	void VulkanRenderer::ClearImage(Ref<RenderCommandBuffer> commandBuffer, Ref<Image2D> image) {
		Renderer::Submit([commandBuffer, image = image.As<VulkanImage2D>()]{
			const auto vkCommandBuffer = commandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(Renderer::GetCurrentFrameIndex());
			VkImageSubresourceRange subresourceRange = {};
			subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			subresourceRange.baseMipLevel = 0;
			subresourceRange.levelCount = image->GetSpecification().Mips;
			subresourceRange.layerCount = image->GetSpecification().Layers;

			VkClearColorValue clearColor{ 0.0f, 0.0f, 0.0f, 0.0f };
			vkCmdClearColorImage(vkCommandBuffer, image->GetImageInfo().Image,
				image->GetSpecification().Usage == ImageUsage::Storage ? VK_IMAGE_LAYOUT_GENERAL : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
				&clearColor, 1, &subresourceRange);

		});
	}

	VkDescriptorSet VulkanRenderer::RTAllocateDescriptorSet(VkDescriptorSetAllocateInfo& allocInfo) {
		uint32_t bufferIndex = Renderer::GetCurrentFrameIndex();
		allocInfo.descriptorPool = s_Data->DescriptorPools[bufferIndex];
		auto& device = VulkanContext::GetCurrentDevice();
		VkDescriptorSet result;
		VKCheckError(vkAllocateDescriptorSets(device->GetVulkanDevice(), &allocInfo, &result));
		s_Data->DescriptorPoolAllocationCount[bufferIndex] += allocInfo.descriptorSetCount;

		//SNOW_CORE_TRACE("DescriptorPoolAllocations for buffer {0} : ({1})", bufferIndex, s_Data->DescriptorPoolAllocationCount[bufferIndex]);

		return result;
	}

	void VulkanRenderer::SubmitFullscreenQuad(Ref<RenderCommandBuffer> commandBuffer, Ref<Pipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<Material> material) {
		SubmitFullscreenQuad(commandBuffer, pipeline, uniformBufferSet, nullptr, material);
	}

	void VulkanRenderer::SubmitFullscreenQuad(Ref<RenderCommandBuffer> commandBuffer, Ref<Pipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Material> material) {

		Ref<VulkanMaterial> vkMaterial = material.As<VulkanMaterial>();

		Renderer::Submit([commandBuffer, pipeline, uniformBufferSet, storageBufferSet, vkMaterial]() mutable {

			uint32_t frameIndex = Renderer::GetCurrentFrameIndex();
			VkCommandBuffer renderCommandBuffer = commandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(frameIndex);

			Ref<VulkanPipeline> vkPipeline = pipeline.As<VulkanPipeline>();
			VkPipelineLayout layout = vkPipeline->GetVulkanPipelineLayout();

			auto vkVertexBuffer = s_Data->QuadVertexBuffer.As<VulkanVertexBuffer>();
			VkBuffer vkvBuffer = vkVertexBuffer->GetBuffer();
			VkDeviceSize offsets[1] = { 0 };
			vkCmdBindVertexBuffers(renderCommandBuffer, 0, 1, &vkvBuffer, offsets);

			auto vkIndexBuffer = s_Data->QuadIndexBuffer.As<VulkanIndexBuffer>();
			VkBuffer ibBuffer = vkIndexBuffer->GetBuffer();
			vkCmdBindIndexBuffer(renderCommandBuffer, ibBuffer, 0, VK_INDEX_TYPE_UINT32);

			VkPipeline vulkanPipeline = vkPipeline->GetVulkanPipeline();
			vkCmdBindPipeline(renderCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline);

			//Update Material
			RTUpdateMaterialForRendering(vkMaterial, uniformBufferSet, storageBufferSet);

			uint32_t bufferIndex = Renderer::GetCurrentFrameIndex();
			VkDescriptorSet descriptorSet = vkMaterial->GetDescriptorSet(bufferIndex);
			if (descriptorSet)
				vkCmdBindDescriptorSets(renderCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1, &descriptorSet, 0, nullptr);

			Buffer uniformStorageBuffer = vkMaterial->GetUniformStorageBuffer();
			if (uniformStorageBuffer.Size)
				vkCmdPushConstants(renderCommandBuffer, layout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, uniformStorageBuffer.Size, uniformStorageBuffer.Data);
			vkCmdDrawIndexed(renderCommandBuffer, s_Data->QuadIndexBuffer->GetCount(), 1, 0, 0, 0);
			});
	}

	void VulkanRenderer::RenderMesh(Ref<RenderCommandBuffer> commandBuffer, Ref<Pipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Mesh> mesh, Ref<MaterialTable> materialTable, const glm::mat4& transform) {


		Renderer::Submit([commandBuffer, pipeline, uniformBufferSet, storageBufferSet, mesh, materialTable, transform]() mutable {

			if (s_Data->SelectedDrawCall != -1 && s_Data->DrawCallCount > s_Data->SelectedDrawCall)
				return;

			uint32_t frameIndex = Renderer::GetCurrentFrameIndex();
			VkCommandBuffer renderCommandBuffer = commandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(frameIndex);

			Ref<MeshSource> meshAsset = mesh->GetMeshAsset();
			Ref<VulkanVertexBuffer> vkVertexBuffer = meshAsset->GetVertexBuffer().As<VulkanVertexBuffer>();
			VkBuffer vkBuffer = vkVertexBuffer->GetBuffer();
			VkDeviceSize offsets[1] = { 0 };
			vkCmdBindVertexBuffers(renderCommandBuffer, 0, 1, &vkBuffer, offsets);

			auto vkIndexBuffer = meshAsset->GetIndexBuffer().As<VulkanIndexBuffer>();
			VkBuffer ibBuffer = vkIndexBuffer->GetBuffer();
			vkCmdBindIndexBuffer(renderCommandBuffer, ibBuffer, 0, VK_INDEX_TYPE_UINT32);

			Ref<VulkanPipeline> vkPipeline = pipeline.As<VulkanPipeline>();
			VkPipeline vulkanPipeline = vkPipeline->GetVulkanPipeline();
			vkCmdBindPipeline(renderCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline);

			std::vector<std::vector<VkWriteDescriptorSet>> writeDescriptors;

			auto& meshMaterialTable = mesh->GetMaterials();
			uint32_t materialCount = meshMaterialTable->GetMaterialCount() > 0 ? meshMaterialTable->GetMaterialCount() : materialTable->GetMaterialCount();
			std::vector<Ref<MaterialAsset>> renderMaterials(materialCount);
			for (uint32_t i = 0; i < materialCount; i++) {
				if (materialTable->HasMaterial(i))
					renderMaterials[i] = materialTable->GetMaterial(i);
				else
					renderMaterials[i] = meshMaterialTable->GetMaterial(i);

				Ref<VulkanMaterial> vulkanMaterial = renderMaterials[i]->GetMaterial().As<VulkanMaterial>();
				RTUpdateMaterialForRendering(vulkanMaterial, uniformBufferSet, storageBufferSet);
			}

			const auto& meshAssetSubmeshes = meshAsset->GetSubmeshes();
			auto& submeshes = mesh->GetSubmeshes();
			for (uint32_t submeshIndex : submeshes) {

				const Submesh& submesh = meshAssetSubmeshes[submeshIndex];
				auto& material = renderMaterials[submesh.MaterialIndex]->GetMaterial().As<VulkanMaterial>();
				VkPipelineLayout layout = vkPipeline->GetVulkanPipelineLayout();
				VkDescriptorSet descriptorSet = material->GetDescriptorSet(frameIndex);

				std::array<VkDescriptorSet, 2> descriptorSets = {
					descriptorSet,
					s_Data->ActiveRendererDescriptorSet
				};

				vkCmdBindDescriptorSets(renderCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, (uint32_t)descriptorSets.size(), descriptorSets.data(), 0, nullptr);

				glm::mat4 worldTransform = transform * submesh.Transform;

				Buffer uniformStorageBuffer = material->GetUniformStorageBuffer();
				vkCmdPushConstants(renderCommandBuffer, layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &worldTransform);
				vkCmdPushConstants(renderCommandBuffer, layout, VK_SHADER_STAGE_FRAGMENT_BIT, sizeof(glm::mat4), uniformStorageBuffer.Size, uniformStorageBuffer.Data);
				vkCmdDrawIndexed(renderCommandBuffer, submesh.IndexCount, 1, submesh.BaseIndex, submesh.BaseVertex, 0);
				s_Data->DrawCallCount++;
			}
			});
	}

	void VulkanRenderer::RenderMeshWithMaterial(Ref<RenderCommandBuffer> commandBuffer, Ref<Pipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Mesh> mesh, const glm::mat4& transform, Ref<Material> material, Buffer additionalUniforms) {
		Buffer pushConstantBuffer;
		pushConstantBuffer.Allocate(sizeof(glm::mat4) + additionalUniforms.Size);
		if (additionalUniforms.Size)
			pushConstantBuffer.Write(additionalUniforms.Data, additionalUniforms.Size, sizeof(glm::mat4));
		
		Renderer::Submit([commandBuffer, pipeline, uniformBufferSet, storageBufferSet, mesh, material, transform, pushConstantBuffer]() mutable {


			uint32_t frameIndex = Renderer::GetCurrentFrameIndex();
			VkCommandBuffer renderCommandBuffer = commandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(frameIndex);

			Ref<MeshSource> meshAsset = mesh->GetMeshAsset();
			Ref<VulkanVertexBuffer> vkVertexBuffer = meshAsset->GetVertexBuffer().As<VulkanVertexBuffer>();
			VkBuffer vkBuffer = vkVertexBuffer->GetBuffer();
			VkDeviceSize offsets[1] = { 0 };
			vkCmdBindVertexBuffers(renderCommandBuffer, 0, 1, &vkBuffer, offsets);

			auto vkIndexBuffer = meshAsset->GetIndexBuffer().As<VulkanIndexBuffer>();
			VkBuffer ibBuffer = vkIndexBuffer->GetBuffer();
			vkCmdBindIndexBuffer(renderCommandBuffer, ibBuffer, 0, VK_INDEX_TYPE_UINT32);

			Ref<VulkanMaterial> vulkanMaterial = material.As<VulkanMaterial>();
			RTUpdateMaterialForRendering(vulkanMaterial, uniformBufferSet, storageBufferSet);

			Ref<VulkanPipeline> vkPipeline = pipeline.As<VulkanPipeline>();
			VkPipeline vulkanPipeline = vkPipeline->GetVulkanPipeline();
			VkPipelineLayout layout = vkPipeline->GetVulkanPipelineLayout();
			vkCmdBindPipeline(renderCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline);

			std::vector<std::vector<VkWriteDescriptorSet>> writeDescriptors;
			VkDescriptorSet descriptorSet = vulkanMaterial->GetDescriptorSet(frameIndex);
			if(descriptorSet)
				vkCmdBindDescriptorSets(renderCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1, &descriptorSet, 0, nullptr);
			
			Buffer uniformStorageBuffer = vulkanMaterial->GetUniformStorageBuffer();
			if(uniformStorageBuffer)
				vkCmdPushConstants(renderCommandBuffer, layout, VK_SHADER_STAGE_FRAGMENT_BIT, pushConstantBuffer.Size, uniformStorageBuffer.Size, uniformStorageBuffer.Data);

			const auto& meshAssetSubmeshes = meshAsset->GetSubmeshes();
			auto& submeshes = mesh->GetSubmeshes();
			for (uint32_t submeshIndex : submeshes) {

				const Submesh& submesh = meshAssetSubmeshes[submeshIndex];
				
				glm::mat4 worldTransform = transform * submesh.Transform;
				pushConstantBuffer.Write(&worldTransform, sizeof(glm::mat4));
				
				vkCmdPushConstants(renderCommandBuffer, layout, VK_SHADER_STAGE_VERTEX_BIT, 0, pushConstantBuffer.Size, pushConstantBuffer.Data);
				
				vkCmdDrawIndexed(renderCommandBuffer, submesh.IndexCount, 1, submesh.BaseIndex, submesh.BaseVertex, 0);
				//s_Data->DrawCallCount++;
			}
			pushConstantBuffer.Release();
			});
	}

	void VulkanRenderer::RenderQuad(Ref<RenderCommandBuffer> commandBuffer, Ref<Pipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Material> material, const glm::mat4& transform) {
		Ref<VulkanMaterial> vkMaterial = material.As<VulkanMaterial>();

		Renderer::Submit([commandBuffer, pipeline, uniformBufferSet, storageBufferSet, vkMaterial, transform]() mutable {

			uint32_t frameIndex = Renderer::GetCurrentFrameIndex();
			VkCommandBuffer renderCommandBuffer = commandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(frameIndex);

			Ref<VulkanPipeline> vkPipeline = pipeline.As<VulkanPipeline>();
			VkPipelineLayout layout = vkPipeline->GetVulkanPipelineLayout();

			auto vkVertexBuffer = s_Data->QuadVertexBuffer.As<VulkanVertexBuffer>();
			VkBuffer vkBuffer = vkVertexBuffer->GetBuffer();
			VkDeviceSize offsets[1] = { 0 };
			vkCmdBindVertexBuffers(renderCommandBuffer, 0, 1, &vkBuffer, offsets);

			auto vkIndexBuffer = s_Data->QuadIndexBuffer.As<VulkanIndexBuffer>();
			VkBuffer ibBuffer = vkIndexBuffer->GetBuffer();
			vkCmdBindIndexBuffer(renderCommandBuffer, ibBuffer, 0, VK_INDEX_TYPE_UINT32);

			VkPipeline vulkanPipeline = vkPipeline->GetVulkanPipeline();
			vkCmdBindPipeline(renderCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline);

			//Update Material
			//const auto& writeDescriptors = RTRetrieveOrCreateUniformBufferWriteDescriptors(uniformBufferSet, vkMaterial);
			RTUpdateMaterialForRendering(vkMaterial, uniformBufferSet, storageBufferSet);

			uint32_t bufferIndex = Renderer::GetCurrentFrameIndex();
			VkDescriptorSet descriptorSet = vkMaterial->GetDescriptorSet(bufferIndex);
			if (descriptorSet)
				vkCmdBindDescriptorSets(renderCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1, &descriptorSet, 0, nullptr);

			vkCmdPushConstants(renderCommandBuffer, layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &transform);
			Buffer uniformStorageBuffer = vkMaterial->GetUniformStorageBuffer();
			if (uniformStorageBuffer.Size)
				vkCmdPushConstants(renderCommandBuffer, layout, VK_SHADER_STAGE_FRAGMENT_BIT, sizeof(glm::mat4), uniformStorageBuffer.Size, uniformStorageBuffer.Data);
			vkCmdDrawIndexed(renderCommandBuffer, s_Data->QuadIndexBuffer->GetCount(), 1, 0, 0, 0);
			});
	}

	void VulkanRenderer::RenderGeometry(Ref<RenderCommandBuffer> commandBuffer, Ref<Pipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Material> material, Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, const glm::mat4& transform, uint32_t indexCount) {
		Ref<VulkanMaterial> vkMaterial = material.As<VulkanMaterial>();
		if (indexCount == 0)
			indexCount = indexBuffer->GetCount();

		Renderer::Submit([commandBuffer, pipeline, uniformBufferSet, storageBufferSet, vkMaterial, vertexBuffer, indexBuffer, transform, indexCount]() mutable {
			uint32_t frameIndex = Renderer::GetCurrentFrameIndex();
			VkCommandBuffer renderCommandBuffer = commandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(frameIndex);

			Ref<VulkanPipeline> vkPipeline = pipeline.As<VulkanPipeline>();
			VkPipelineLayout layout = vkPipeline->GetVulkanPipelineLayout();

			auto vkVertexBuffer = vertexBuffer.As<VulkanVertexBuffer>();
			VkBuffer vkBuffer = vkVertexBuffer->GetBuffer();
			VkDeviceSize offsets[1] = { 0 };
			vkCmdBindVertexBuffers(renderCommandBuffer, 0, 1, &vkBuffer, offsets);

			auto vkIndexBuffer = indexBuffer.As<VulkanIndexBuffer>();
			VkBuffer ibBuffer = vkIndexBuffer->GetBuffer();
			vkCmdBindIndexBuffer(renderCommandBuffer, ibBuffer, 0, VK_INDEX_TYPE_UINT32);

			VkPipeline vulkanPipeline = vkPipeline->GetVulkanPipeline();
			vkCmdBindPipeline(renderCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline);

			//Update Material
			//const auto& writeDescriptors = RTRetrieveOrCreateUniformBufferWriteDescriptors(uniformBufferSet, vkMaterial);
			RTUpdateMaterialForRendering(vkMaterial, uniformBufferSet, storageBufferSet);

			uint32_t bufferIndex = Renderer::GetCurrentFrameIndex();
			VkDescriptorSet descriptorSet = vkMaterial->GetDescriptorSet(bufferIndex);
			if (descriptorSet)
				vkCmdBindDescriptorSets(renderCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1, &descriptorSet, 0, nullptr);

			vkCmdPushConstants(renderCommandBuffer, layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &transform);
			Buffer uniformStorageBuffer = vkMaterial->GetUniformStorageBuffer();
			if (uniformStorageBuffer.Size)
				vkCmdPushConstants(renderCommandBuffer, layout, VK_SHADER_STAGE_FRAGMENT_BIT, sizeof(glm::mat4), uniformStorageBuffer.Size, uniformStorageBuffer.Data);
			vkCmdDrawIndexed(renderCommandBuffer, indexCount, 1, 0, 0, 0);

			});
	}

	void VulkanRenderer::DispatchComputeShader(Ref<RenderCommandBuffer> commandBuffer, Ref<ComputePipeline> pipeline, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet, Ref<Material> material, const glm::ivec3& workGroups) {
		auto vkMaterial = material.As<VulkanMaterial>();
		auto vkPipeline = pipeline.As<VulkanComputePipeline>();

		Renderer::Submit([commandBuffer, vkPipeline, vkMaterial, uniformBufferSet, storageBufferSet, workGroups]() mutable {
			const uint32_t frameIndex = Renderer::GetCurrentFrameIndex();

			RTUpdateMaterialForRendering(vkMaterial, uniformBufferSet, storageBufferSet);

			const VkDescriptorSet descriptorSet = vkMaterial->GetDescriptorSet(frameIndex);

			vkPipeline->Begin(commandBuffer);
			vkPipeline->Dispatch(descriptorSet, workGroups.x, workGroups.y, workGroups.z);
			vkPipeline->End();
		});
	}

	void VulkanRenderer::SetSceneEnvironment(Ref<SceneRenderer> sceneRenderer, Ref<Environment> environment, Ref<Image2D> shadow) {
		if (!environment)
			environment = Renderer::GetEmptyEnvironment();

		Renderer::Submit([sceneRenderer, environment, shadow]() mutable {
			auto vkShader = Renderer::GetShaderLibrary()->Get("PBR").As<VulkanShader>();
			const uint32_t frameIndex = Renderer::GetCurrentFrameIndex();

			if (s_Data->RendererDescriptorSet.find(sceneRenderer.Raw()) == s_Data->RendererDescriptorSet.end()) {
				const uint32_t framesInFlight = Renderer::GetConfig().FramesInFlight;
				s_Data->RendererDescriptorSet[sceneRenderer.Raw()].resize(framesInFlight);
				for (uint32_t i = 0; i < framesInFlight; i++)
					s_Data->RendererDescriptorSet.at(sceneRenderer.Raw())[i] = vkShader->CreateDescriptorSets(1);

			}

			VkDescriptorSet descriptorSet = s_Data->RendererDescriptorSet.at(sceneRenderer.Raw())[frameIndex].DescriptorSets[0];
			s_Data->ActiveRendererDescriptorSet = descriptorSet;

			std::array<VkWriteDescriptorSet, 4> writeDescriptors;

			Ref<VulkanTextureCube> radianceMap = environment->RadianceMap.As<VulkanTextureCube>();
			Ref<VulkanTextureCube> irradianceMap = environment->IrradianceMap.As<VulkanTextureCube>();

			writeDescriptors[0] = *vkShader->GetDescriptorSet("u_EnvRadianceTex", 1);
			writeDescriptors[0].dstSet = descriptorSet;
			const auto& radianceMapImageInfo = radianceMap->GetVulkanDescriptorInfo();
			writeDescriptors[0].pImageInfo = &radianceMapImageInfo;

			writeDescriptors[1] = *vkShader->GetDescriptorSet("u_EnvIrradianceTex", 1);
			writeDescriptors[1].dstSet = descriptorSet;
			const auto& irradianceMapImageInfo = irradianceMap->GetVulkanDescriptorInfo();
			writeDescriptors[1].pImageInfo = &irradianceMapImageInfo;

			writeDescriptors[2] = *vkShader->GetDescriptorSet("u_BRDFLUTTexture", 1);
			writeDescriptors[2].dstSet = descriptorSet;
			const auto& brdfLUTImageInfo = s_Data->BRDFLut.As<VulkanTexture2D>()->GetVulkanDescriptorInfo();
			writeDescriptors[2].pImageInfo = &brdfLUTImageInfo;

			writeDescriptors[3] = *vkShader->GetDescriptorSet("u_ShadowMapTexture", 1);
			writeDescriptors[3].dstSet = descriptorSet;
			const auto& shadowMapImageInfo = shadow.As<VulkanImage2D>()->GetImageDescriptor();
			writeDescriptors[3].pImageInfo = &shadowMapImageInfo;

			auto vkDevice = VulkanContext::GetCurrentDevice();
			vkUpdateDescriptorSets(vkDevice->GetVulkanDevice(), writeDescriptors.size(), writeDescriptors.data(), 0, nullptr);
		});
	}

	Ref<TextureCube> VulkanRenderer::CreatePreethamSky(float turbidity, float azimuth, float inclination) {
		const uint32_t cubemapSize = 1024;
		const uint32_t irradianceMapSize = 32;

		Ref<TextureCube> environmentMap = TextureCube::Create(ImageFormat::RGBA32F, cubemapSize, cubemapSize);

		Ref<Shader> preethamSkyShader = Renderer::GetShaderLibrary()->Get("PreethamSky");
		Ref<VulkanComputePipeline> preethamSkyComputePipeline = Ref<VulkanComputePipeline>::Create(preethamSkyShader);

		glm::vec3 params = { turbidity, azimuth, inclination };

		Renderer::Submit([preethamSkyComputePipeline, environmentMap, cubemapSize, params]() mutable {
			auto vkDevice = VulkanContext::GetCurrentDevice();
			Ref<VulkanShader> vkShader = preethamSkyComputePipeline->GetShader().As<VulkanShader>();

			std::array<VkWriteDescriptorSet, 1> writeDescriptors;
			auto descriptorSet = vkShader->CreateDescriptorSets();
			Ref<VulkanTextureCube> envUnfilteredCubemap = environmentMap.As<VulkanTextureCube>();
			writeDescriptors[0] = *vkShader->GetDescriptorSet("o_CubeMap");
			writeDescriptors[0].dstSet = descriptorSet.DescriptorSets[0];
			writeDescriptors[0].pImageInfo = &envUnfilteredCubemap->GetVulkanDescriptorInfo();

			vkUpdateDescriptorSets(vkDevice->GetVulkanDevice(), (uint32_t)writeDescriptors.size(), writeDescriptors.data(), 0, nullptr);

			preethamSkyComputePipeline->Begin();
			preethamSkyComputePipeline->SetPushConstants(&params, sizeof(glm::vec3));
			preethamSkyComputePipeline->Dispatch(descriptorSet.DescriptorSets[0], cubemapSize / 32, cubemapSize / 32, 6);
			preethamSkyComputePipeline->End();

			envUnfilteredCubemap->GenerateMips(true);
		});
		return environmentMap;
	}

	static const std::vector<std::vector<VkWriteDescriptorSet>>& RTRetrieveOrCreateUniformBufferWriteDescriptors(Ref<UniformBufferSet>& uniformBufferSet, Ref<VulkanMaterial> vkMaterial) {
		size_t shaderHash = vkMaterial->GetShader()->GetHash();
		if (s_Data->UniformBufferWriteDescriptorCache.find(uniformBufferSet.Raw()) != s_Data->UniformBufferWriteDescriptorCache.end()) {
			const auto& shaderMap = s_Data->UniformBufferWriteDescriptorCache.at(uniformBufferSet.Raw());
			if (shaderMap.find(shaderHash) != shaderMap.end()) {
				const auto& writeDescriptors = shaderMap.at(shaderHash);
				return writeDescriptors;
			}
		}

		uint32_t framesInFlight = Renderer::GetConfig().FramesInFlight;
		Ref<VulkanShader> vkShader = vkMaterial->GetShader().As<VulkanShader>();
		if (vkShader->HasDescriptorSet(0)) {
			const auto& shaderDescriptorSets = vkShader->GetShaderDescriptorSet();
			if (!shaderDescriptorSets.empty()) {
				for (auto&& [binding, ub] : shaderDescriptorSets[0].UniformBuffers) {
					auto& writeDescriptors = s_Data->UniformBufferWriteDescriptorCache[uniformBufferSet.Raw()][shaderHash];
					writeDescriptors.resize(framesInFlight);
					for (uint32_t frame = 0; frame < framesInFlight; frame++) {
						Ref<VulkanUniformBuffer> uniformBuffer = uniformBufferSet->Get(binding, 0, frame);

						VkWriteDescriptorSet writeDescriptorSet = {};
						writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
						writeDescriptorSet.descriptorCount = 1;
						writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
						writeDescriptorSet.pBufferInfo = &uniformBuffer->GetDescriptorBufferInfo();
						writeDescriptorSet.dstBinding = uniformBuffer->GetBinding();
						writeDescriptors[frame].push_back(writeDescriptorSet);
					}
				}
			}
		}
		return s_Data->UniformBufferWriteDescriptorCache[uniformBufferSet.Raw()][shaderHash];
	}

	static const std::vector<std::vector<VkWriteDescriptorSet>>& RTRetrieveOrCreateStorageBufferWriteDescriptors(Ref<StorageBufferSet>& storageBufferSet, Ref<VulkanMaterial> vkMaterial) {
		size_t shaderHash = vkMaterial->GetShader()->GetHash();
		if (s_Data->StorageBufferWriteDescriptorCache.find(storageBufferSet.Raw()) != s_Data->StorageBufferWriteDescriptorCache.end()) {
			const auto& shaderMap = s_Data->StorageBufferWriteDescriptorCache.at(storageBufferSet.Raw());
			if (shaderMap.find(shaderHash) != shaderMap.end()) {
				const auto& writeDescriptors = shaderMap.at(shaderHash);
				return writeDescriptors;
			}
		}

		uint32_t framesInFlight = Renderer::GetConfig().FramesInFlight;
		Ref<VulkanShader> vkShader = vkMaterial->GetShader().As<VulkanShader>();
		if (vkShader->HasDescriptorSet(0)) {
			const auto& shaderDescriptorSets = vkShader->GetShaderDescriptorSet();
			if (!shaderDescriptorSets.empty()) {
				for (auto&& [binding, sb] : shaderDescriptorSets[0].StorageBuffers) {
					auto& writeDescriptors = s_Data->StorageBufferWriteDescriptorCache[storageBufferSet.Raw()][shaderHash];
					writeDescriptors.resize(framesInFlight);
					for (uint32_t frame = 0; frame < framesInFlight; frame++) {
						Ref<VulkanStorageBuffer> storageBuffer = storageBufferSet->Get(binding, 0, frame);

						VkWriteDescriptorSet writeDescriptorSet = {};
						writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
						writeDescriptorSet.descriptorCount = 1;
						writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
						writeDescriptorSet.pBufferInfo = &storageBuffer->GetDescriptorBufferInfo();
						writeDescriptorSet.dstBinding = storageBuffer->GetBinding();
						writeDescriptors[frame].push_back(writeDescriptorSet);
					}
				}
			}
		}
		return s_Data->StorageBufferWriteDescriptorCache[storageBufferSet.Raw()][shaderHash];
	}

	void VulkanRenderer::RTUpdateMaterialForRendering(Ref<VulkanMaterial> material, Ref<UniformBufferSet> uniformBufferSet, Ref<StorageBufferSet> storageBufferSet) {
		if (uniformBufferSet) {
			auto writeDescriptors = RTRetrieveOrCreateUniformBufferWriteDescriptors(uniformBufferSet, material);
			if (storageBufferSet) {
				const auto& storageBufferWriteDescriptors = RTRetrieveOrCreateStorageBufferWriteDescriptors(storageBufferSet, material);

				const uint32_t framesInFlight = Renderer::GetConfig().FramesInFlight;
				for (uint32_t frame = 0; frame < framesInFlight; frame++) {
					writeDescriptors[frame].reserve(writeDescriptors[frame].size() + storageBufferWriteDescriptors[frame].size());
					writeDescriptors[frame].insert(writeDescriptors[frame].end(), storageBufferWriteDescriptors[frame].begin(), storageBufferWriteDescriptors[frame].end());
				}
			}
			material->UpdateForRendering(writeDescriptors);
		}
		else {
			material->UpdateForRendering();
		}
	}

	void Render::Utils::InsertImageMemoryBarrier(VkCommandBuffer cmdBuffer,
		VkImage image,
		VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
		VkImageLayout oldImageLayout, VkImageLayout newImageLayout,
		VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
		VkImageSubresourceRange subresourceRange) {

		VkImageMemoryBarrier imageMemoryBarrier = {};
		imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

		imageMemoryBarrier.image = image;
		imageMemoryBarrier.srcAccessMask = srcAccessMask;
		imageMemoryBarrier.dstAccessMask = dstAccessMask;
		imageMemoryBarrier.oldLayout = oldImageLayout;
		imageMemoryBarrier.newLayout = newImageLayout;
		imageMemoryBarrier.subresourceRange = subresourceRange;

		vkCmdPipelineBarrier(
			cmdBuffer,
			srcStageMask, dstStageMask,
			0,
			0, nullptr,
			0, nullptr,
			1, &imageMemoryBarrier);
	}

	void Render::Utils::SetImageLayout(VkCommandBuffer cmdBuffer, VkImage image,
		VkImageLayout oldImageLayout, VkImageLayout newImageLayout,
		VkImageSubresourceRange subresourceRange,
		VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask) {

		VkImageMemoryBarrier imageMemoryBarrier = {};
		imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.oldLayout = oldImageLayout;
		imageMemoryBarrier.newLayout = newImageLayout;
		imageMemoryBarrier.image = image;
		imageMemoryBarrier.subresourceRange = subresourceRange;


		// source layouts
		// Source access mask controls actions that have to be finished on the old layout
		// before it will be transitionsed to the new layout
		switch (oldImageLayout) {
		case VK_IMAGE_LAYOUT_UNDEFINED:
			// Image layout is undifined
			// only valid as initial layout
			// no flags required
			imageMemoryBarrier.srcAccessMask = 0;
			break;
		case VK_IMAGE_LAYOUT_PREINITIALIZED:
			// Image is preinitialized
			// only valid as initial layout for linear images, preserves memory contents
			// make sure host writes have been finished
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
			// image is a color attachment
			// make sure any writes to the color buffer have been finished
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
			// image is a depth/stencil attachment
			// make sure any writes to the depth/stencil buffer have been finished
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
			// image is a transfer source
			// make sure any reads from the image have been finished
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			break;
		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
			// image is a transfer destination
			// make sure any writes to the image have been finished
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
			// image is read by a shader
			// mak esure any shader reads from the image have been finished
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
			break;
		default:
			break;
		}

		switch (newImageLayout) {
		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			break;
		case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
			imageMemoryBarrier.dstAccessMask = imageMemoryBarrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
			if (imageMemoryBarrier.srcAccessMask = 0) {
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
			}
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			break;
		default:
			break;
		}

		vkCmdPipelineBarrier(cmdBuffer,
			srcStageMask, dstStageMask,
			0,
			0, nullptr,
			0, nullptr,
			1, &imageMemoryBarrier);
	}

	void Render::Utils::SetImageLayout(VkCommandBuffer cmdBuffer, VkImage image,
		VkImageAspectFlags aspectMask,
		VkImageLayout oldImageLayout, VkImageLayout newImageLayout,
		VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask) {

		VkImageSubresourceRange subresourceRange = {};
		subresourceRange.aspectMask = aspectMask;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = 1;
		subresourceRange.layerCount = 1;
		SetImageLayout(cmdBuffer, image, oldImageLayout, newImageLayout, subresourceRange, srcStageMask, dstStageMask);

	}

}