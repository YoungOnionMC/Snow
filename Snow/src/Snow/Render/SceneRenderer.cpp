#include <spch.h>
#include "Snow/Render/SceneRenderer.h"

#include "Snow/Render/Renderer.h"
#include "Snow/Render/Renderer2D.h"
#include "Snow/Render/Renderer3D.h"

#include "Snow/ImGui/ImGuiLayer.h"

#include "Snow/Render/Material.h"
#include "Snow/Render/Texture.h"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

namespace Snow {
	namespace Render {
		//struct SceneRendererData {
		//	Ref<Scene> ActiveScene;

		//	struct SceneInfo {
		//		SceneRendererCamera Camera;

		//		Environment SceneEnvironment;
		//		Light ActiveLight;
		//	} SceneData;

		//	struct CompositeInfo {
		//		float Exposure = 1.0f;
		//		int Samples = 4;
		//		bool Bloom = false;
		//		float BloomThreshold = 1.0f;
		//	} CompositeData;

		//	const std::array<const char*, 5> CompositeSampleValues = { "1", "2", "4", "8", "16" };

		//	struct DrawMeshCommand {
		//		Ref<Mesh> Mesh;
		//		//Ref<MaterialInstance> MaterialInstance;
		//		glm::mat4 Transform;
		//	};

		//	std::vector<DrawMeshCommand> MeshDrawList;

		//	struct DrawQuadCommand {
		//		glm::mat4 Transform;
		//		glm::vec4 Color;
		//	};

		//	std::vector<DrawQuadCommand> QuadDrawList;

		//	Ref<Texture2D> BRDFLUT;

		//	Ref<Texture2D> TestTexture;

		//	bool ViewportFocused = false, ViewportHovered = false;
		//	glm::vec2 ViewportSize = { 0.0f, 0.0f };
		//};
		//static SceneRendererData s_SceneRendererData;

		SceneRenderer::SceneRenderer(Ref<Scene> scene, SceneRendererSpecification spec) :
			m_Scene(scene), m_Specification(spec) {
			Init();
		}

		void SceneRenderer::Init() {

			if (m_Specification.SwapChainTarget)
				m_CommandBuffer = Render::RenderCommandBuffer::CreateFromSwapChain("SceneRenderer");
			else
				m_CommandBuffer = RenderCommandBuffer::Create(0, "SceneRenderer");

			uint32_t framesInFlight = Renderer::GetConfig().FramesInFlight;
			m_UniformBufferSet = UniformBufferSet::Create(framesInFlight);
			m_UniformBufferSet->Create(sizeof(UBCamera), 0);

			m_CompositeShader = Renderer::GetShaderLibrary()->Get("SceneComposite");
			m_CompositeMaterial = Material::Create(m_CompositeShader);

			// Geometry Pipeline
			
			{
#define pbr 0
#if pbr
				FramebufferSpecification framebufferSpec;
				framebufferSpec.AttachmentList = { ImageFormat::RGBA32F, ImageFormat::RGBA32F, ImageFormat::RGBA16F, ImageFormat::DepthStencil };
				framebufferSpec.Samples = 1;
				framebufferSpec.ClearColor = { 0.1f, 0.6f, 0.1f, 1.0f };
				framebufferSpec.DebugName = "Geometry";

				Ref<Framebuffer> framebuffer = Framebuffer::Create(framebufferSpec);


				RenderPassSpecification renderPassSpec;
				renderPassSpec.TargetFramebuffer = framebuffer;
				renderPassSpec.DebugName = "Geometry";

				PipelineSpecification pipelineSpec;
				pipelineSpec.LineWidth = 1.0f;
				pipelineSpec.Layout = {
					{AttribType::Float3, "a_Position"},
					{AttribType::Float3, "a_Normal"},
					{AttribType::Float3, "a_Tangent"},
					{AttribType::Float3, "a_Binormal"},
					{AttribType::Float2, "a_TexCoord"},
				};
				pipelineSpec.Shader = Renderer::GetShaderLibrary()->Get("PBR");
				pipelineSpec.BindedRenderPass = RenderPass::Create(renderPassSpec);
				pipelineSpec.DebugName = "PBR";
				m_GeometryPipeline = Pipeline::Create(pipelineSpec);
#else
				FramebufferSpecification framebufferSpec;
				framebufferSpec.AttachmentList = { ImageFormat::RGBA32F, ImageFormat::Depth32F };
				framebufferSpec.Samples = 1;
				framebufferSpec.ClearColor = { 0.1f, 0.6f, 0.1f, 1.0f };
				framebufferSpec.DebugName = "Geometry";

				Ref<Framebuffer> framebuffer = Framebuffer::Create(framebufferSpec);


				RenderPassSpecification renderPassSpec;
				renderPassSpec.TargetFramebuffer = framebuffer;
				renderPassSpec.DebugName = "Geometry";

				PipelineSpecification pipelineSpec;
				pipelineSpec.LineWidth = 1.0f;
				pipelineSpec.Layout = {
					{AttribType::Float3, "a_Position"},
					{AttribType::Float2, "a_TexCoord"},
					{AttribType::Float, "a_TexID"},
					{AttribType::Float4, "a_Color"},
				};
				pipelineSpec.Shader = Renderer::GetShaderLibrary()->Get("Texture2D");
				pipelineSpec.BindedRenderPass = RenderPass::Create(renderPassSpec);
				pipelineSpec.DebugName = "Texture2D";
				m_GeometryPipeline = Pipeline::Create(pipelineSpec);
#endif
				//s_SceneRendererData.GeometryPass = RenderPass::Create(geoRenderPassSpec);
			}

			// Grid Pipeline
			{
				m_GridShader = Renderer::GetShaderLibrary()->Get("Grid");
				const float gridScale = 16.0f;
				const float gridSize = 0.025f;
				m_GridMaterial = Render::Material::Create(m_GridShader);
				m_GridMaterial->Set("u_Settings.Scale", gridScale);
				m_GridMaterial->Set("u_Settings.Size", gridSize);

				PipelineSpecification pipelineSpec;
				pipelineSpec.DebugName = "Grid";
				pipelineSpec.Shader = m_GridShader;
				pipelineSpec.BackfaceCulling = false;
				pipelineSpec.Layout = {
					{AttribType::Float3, "a_Position"},
					{AttribType::Float2, "a_TexCoord"},
				};
				pipelineSpec.BindedRenderPass = m_GeometryPipeline->GetSpecification().BindedRenderPass;
				m_GridPipeline = Render::Pipeline::Create(pipelineSpec);
			}

			// Composite Pipeline
			{
				FramebufferSpecification compFramebufferSpec;

				compFramebufferSpec.DebugName = "SceneComposite";
				compFramebufferSpec.ClearColor = { 0.2f, 0.2f, 0.2f, 1.0f };
				compFramebufferSpec.SwapChainTarget = m_Specification.SwapChainTarget;
				if(m_Specification.SwapChainTarget)
					compFramebufferSpec.AttachmentList = { ImageFormat::RGBA };
				else
					compFramebufferSpec.AttachmentList = { ImageFormat::RGBA, ImageFormat::Depth32F };

				Ref<Framebuffer> framebuffer = Framebuffer::Create(compFramebufferSpec);

				RenderPassSpecification compRenderPassSpec;
				compRenderPassSpec.TargetFramebuffer = Framebuffer::Create(compFramebufferSpec);
				compRenderPassSpec.DebugName = "SceneComposite";

				PipelineSpecification pipelineSpec;
				pipelineSpec.Layout = {
					{AttribType::Float3, "a_Position"},
					{AttribType::Float2, "a_TexCoord"},
				};
				pipelineSpec.BackfaceCulling = false;
				pipelineSpec.Shader = Renderer::GetShaderLibrary()->Get("SceneComposite");
				pipelineSpec.BindedRenderPass = RenderPass::Create(compRenderPassSpec);
				pipelineSpec.DebugName = "SceneComposite";
				pipelineSpec.DepthWrite = false;
				m_CompositePipeline = Pipeline::Create(pipelineSpec);
				

				PipelineSpecification compPipelineSpec;
				compPipelineSpec.Topology = PrimitiveTopology::Triangle;
				compPipelineSpec.Layout = {
					{ AttribType::Float3, "a_Position" },
					{ AttribType::Float2, "a_TexCoord" }
				};
			}

			if(!m_Specification.SwapChainTarget) {
				FramebufferSpecification framebufferSpec;
				framebufferSpec.AttachmentList = { ImageFormat::RGBA, ImageFormat::Depth32F };
				framebufferSpec.ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
				framebufferSpec.ClearOnLoad = false;
				framebufferSpec.DebugName = "External Composite";

				framebufferSpec.ExistingImages[0] = m_CompositePipeline->GetSpecification().BindedRenderPass->GetSpecification().TargetFramebuffer->GetImage();
				framebufferSpec.ExistingImages[1] = m_GeometryPipeline->GetSpecification().BindedRenderPass->GetSpecification().TargetFramebuffer->GetDepthImage();

				Ref<Framebuffer> framebuffer = Framebuffer::Create(framebufferSpec);

				RenderPassSpecification renderPassSpec;
				renderPassSpec.TargetFramebuffer = framebuffer;
				renderPassSpec.DebugName = "External Composite";
				m_ExternalCompositeRenderPass = RenderPass::Create(renderPassSpec);
			}

			
			Ref<SceneRenderer> instance = this;
			Renderer::Submit([instance]() mutable {
				instance->m_ResourcesCreated = true;
			});
			//s_SceneRendererData.SceneData.SceneEnvironment = {};
			//auto [rad, irr] = CreateEnvironmentMap("assets/env/birchwood_4k.hdr");
			//s_SceneRendererData.SceneData.SceneEnvironment.RadianceMap = rad;
			//s_SceneRendererData.SceneData.SceneEnvironment.IrradianceMap = irr;
		}

		void SceneRenderer::SetScene(Ref<Scene> scene) {
			m_Scene = scene;
		}

		void SceneRenderer::OnViewportResize(uint32_t width, uint32_t height) {
			//s_SceneRendererData.GeometryPass->GetSpecification().TargetFramebuffer->Resize(width, height, true);
			//s_SceneRendererData.CompPass->GetSpecification().TargetFramebuffer->Resize(width, height, true);
		}

		void SceneRenderer::BeginScene(const SceneRendererCamera& camera) {
			SNOW_CORE_ASSERT(m_Scene);

			if (!m_ResourcesCreated)
				return;

			UBCamera& cameraData = CameraDataUB;

			auto& sceneCamera = m_SceneData.SceneCamera;
			const auto viewProj = sceneCamera.Camera.GetProjection() * sceneCamera.ViewMatrix;
			const glm::vec3 cameraPosition = glm::inverse(sceneCamera.ViewMatrix)[3];

			const auto inverseVP = glm::inverse(viewProj);
			cameraData.ViewProjection = viewProj;
			cameraData.InvViewProjection = inverseVP;
			cameraData.Projection = sceneCamera.Camera.GetProjection();
			cameraData.View = sceneCamera.ViewMatrix;
			Ref<SceneRenderer> instance = this;
			Render::Renderer::Submit([instance, cameraData]() mutable {
				uint32_t bufferIndex = Render::Renderer::GetCurrentFrameIndex();
				instance->m_UniformBufferSet->Get(0, 0, bufferIndex)->RTSetData(&cameraData, sizeof(cameraData));
			});
		}

		void SceneRenderer::BeginScene(const Editor::EditorCamera& camera) {
		}

		void SceneRenderer::EndScene() {
			//s_SceneRendererData.ActiveScene = nullptr;
			FlushDrawList();

			
		}

		void SceneRenderer::SubmitMesh(Ref<Mesh> mesh, Ref<MaterialTable> materialTable, const glm::mat4& transform, Ref<Material> overrideMaterial) {//, const Ref<MaterialInstance> overrideMaterial) {
			m_DrawList.push_back({ mesh, materialTable, transform, overrideMaterial });
		}

		void SceneRenderer::Submit2DQuad(const glm::mat4& transform, const glm::vec4& color) {
			//s_SceneRendererData.QuadDrawList.push_back({ transform, color });
		}

		void SceneRenderer::GeometryPass() {

			//auto [rad, irr] = CreateEnvironmentMap("assets/env/birchwood_4k.hdr");
			//s_SceneRendererData.SceneData.SceneEnvironment.RadianceMap = rad;

			//Renderer::ClearImage(m_CommandBuffer, m_GeometryPipeline->GetSpecification().BindedRenderPass->GetSpecification().TargetFramebuffer->GetImage());
			Renderer::BeginRenderPass(m_CommandBuffer, m_GeometryPipeline->GetSpecification().BindedRenderPass, true);

			for (auto& dc : m_DrawList) {
				Renderer::RenderMesh(m_CommandBuffer, m_GeometryPipeline, m_UniformBufferSet, m_StorageBufferSet, dc.Mesh, dc.MaterialTable ? dc.MaterialTable : dc.Mesh->GetMaterials(), dc.Transform);
			}

			glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 0.0, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(8.0f));
			Renderer::RenderQuad(m_CommandBuffer, m_GridPipeline, m_UniformBufferSet, nullptr, m_GridMaterial, transform);



			Renderer::EndRenderPass(m_CommandBuffer);
			//auto& sceneCamera = s_SceneRendererData.SceneData.Camera;
			
			//auto viewProjection = sceneCamera.Camera.GetProjection() * sceneCamera.ViewMatrix;
			//auto viewProjection = sceneCamera.GetViewProjectionMatrix();
			//glm::vec3 cameraPosition = glm::inverse(sceneCamera.ViewMatrix)[3];


#if 0
			struct EnvironmentUB {
				Light lights;
				glm::vec3 u_CameraPosition;
			};

			EnvironmentUB enviroUB;
			enviroUB.lights = s_SceneRendererData.SceneData.ActiveLight;
			enviroUB.u_CameraPosition = cameraPosition;
			s_SceneRendererData.CompositeShader->SetUniformBufferData("Environment", &enviroUB, sizeof(EnvironmentUB));

			

			s_SceneRendererData.CompositeShader->SetUniformBufferData("Camera", &viewProjection, sizeof(glm::mat4));

			for (auto& dc : s_SceneRendererData.MeshDrawList) {
				//Ref<MaterialInstance> mi = dc.Mesh->GetMaterialInstance();

				//auto baseMaterial = mi->GetMaterial();
				//baseMaterial->Set("u_EnvRadianceTexture", s_SceneRendererData.SceneData.SceneEnvironment.RadianceMap);
				//baseMaterial->Set("u_EnvIrradianceTexture", s_SceneRendererData.SceneData.SceneEnvironment.IrradianceMap);
				//baseMaterial->Set("u_BRDFLUTTexture", s_SceneRendererData.BRDFLUT);

				//pl->SetUniformBufferData(2, &s_Data.SceneData.ActiveLight, sizeof(Light));

				//Renderer3D::DrawMesh(dc.Mesh, dc.Transform, dc.MaterialInstance);

				//pl->SetUniformBufferData(2, )
			}

#endif
			//RenderCommand::SetBlending(true);
			//Render::Renderer2D::BeginScene(s_SceneRendererData.SceneData.Camera.Camera, s_SceneRendererData.SceneData.Camera.ViewMatrix);
			//for (auto& dc : s_SceneRendererData.QuadDrawList) {
				//Renderer2D::DrawQuad(dc.Transform, dc.Color);
			//}
			//Render::Renderer2D::DrawString("This is a wonderful test string for fonts with this font i can write what every i want to", "comic", { -20.0f, 0.0f }, {0.1, 0.9, 0.1});
			
			//Render::Renderer2D::DrawQuad(glm::vec3(10.0f, 10.0f, 0.0f), glm::vec2(1.0), s_SceneRendererData.TestTexture);
			
			//Render::Renderer2D::EndScene();

			//Renderer::EndRenderPass(m_CommandBuffer);
		}

		void SceneRenderer::CompositePass() {
			Renderer::BeginRenderPass(m_CommandBuffer, m_CompositePipeline->GetSpecification().BindedRenderPass, true);

			auto framebuffer = m_GeometryPipeline->GetSpecification().BindedRenderPass->GetSpecification().TargetFramebuffer;
			int textureSamples = framebuffer->GetSpecification().Samples;

			//m_CompositeMaterial->Set("u_Uniforms.TextureSamples", textureSamples);
			m_CompositeMaterial->Set("u_Uniforms.Exposure", 0.05f);
			m_CompositeMaterial->Set("u_Texture", framebuffer->GetImage());

			Renderer::SubmitFullscreenQuad(m_CommandBuffer, m_CompositePipeline, nullptr, m_CompositeMaterial);

			Renderer::EndRenderPass(m_CommandBuffer);
		}

		

		Ref<Image2D> SceneRenderer::GetFinalPassImage() {
			if (!m_ResourcesCreated)
				return nullptr;

			return m_CompositePipeline->GetSpecification().BindedRenderPass->GetSpecification().TargetFramebuffer->GetImage(0);
		}

		void SceneRenderer::FlushDrawList() {
			if (m_ResourcesCreated) {


				m_CommandBuffer->Begin();

				GeometryPass();
				CompositePass();

				m_CommandBuffer->End();
				m_CommandBuffer->Submit();
			}
			else {
				m_CommandBuffer->Begin();

				ClearPass();

				m_CommandBuffer->End();
				m_CommandBuffer->Submit();
			}


		}

		

		void SceneRenderer::ClearPass() {
			Renderer::BeginRenderPass(m_CommandBuffer, m_CompositePipeline->GetSpecification().BindedRenderPass, true);
			Renderer::EndRenderPass(m_CommandBuffer);
		}

		void SceneRenderer::OnImGuiRender() {
			ImGui::Begin("Composite Pass");
			ImGui::End();
		}

		/*
		static Ref<Shader> equirectangularConversionShader, envFilteringShader, envIrradianceShader;
		std::pair<Ref<TextureCube>, Ref<TextureCube>> SceneRenderer::CreateEnvironmentMap(const std::string& filepath) {
			const uint32_t cubeMapSize = 2048;
			const uint32_t irradianceMapSize = 32;

			
			Ref<TextureCube> envUnfiltered = TextureCube::Create(ImageFormat::Float16, cubeMapSize, cubeMapSize);
			if (!equirectangularConversionShader)
				equirectangularConversionShader = Shader::Create({ {ShaderType::None, "assets/shaders/glsl/EquirectangularToCubeMap.glsl" } });
			
			Ref<Texture2D> envEquirect = Texture2D::Create(filepath);
			SNOW_CORE_ASSERT(envEquirect->GetFormat() == ImageFormat::Float16, "Texture is not HDR");

			equirectangularConversionShader->Bind();
			envEquirect->Bind();

			// Conversion from equirectangular to cube map
			//envUnfiltered->BindImageTexture(0, 0, false, ImageFormat::Float16);
			//RenderCommand::DispatchComputeShader(equirectangularConversionShader, cubeMapSize / 32, cubeMapSize / 32, 6);
			//envUnfiltered->GenerateMips();

			if (!envFilteringShader)
				envFilteringShader = Shader::Create({ {ShaderType::None, "assets/shaders/glsl/EnvironmentMipFilter.glsl"} });

			Ref<TextureCube> envFiltered = TextureCube::Create(ImageFormat::Float16, cubeMapSize, cubeMapSize);

			envUnfiltered->CopyImageData((Ref<Texture>&)envFiltered);

			// Filtering of the environment cubemap
			envFilteringShader->Bind();
			envUnfiltered->Bind();
			
			const float deltaRoughness = 1.0f / glm::max((float)(envFiltered->GetMipLevelCount() - 1.0f), 1.0f);
			for (int level = 1, size = cubeMapSize / 2; level < envFiltered->GetMipLevelCount(); level++, size /= 2) {
				const uint32_t numGroups = glm::max(1, size / 32);
				//envFiltered->BindImageTexture(0, 0, false, ImageFormat::Float16);
				float r = deltaRoughness * level;
				envFilteringShader->SetUniform("u_Roughness", r);
				RenderCommand::DispatchComputeShader(envFilteringShader, numGroups, numGroups, 6);
			}
			

			if (!envIrradianceShader)
				envIrradianceShader = Shader::Create({ {ShaderType::None, "assets/shaders/glsl/EnvironmentIrradiance.glsl"} });
			
			Ref<TextureCube> irradianceMap = TextureCube::Create(ImageFormat::Float16, irradianceMapSize, irradianceMapSize);
			envIrradianceShader->Bind();
			envFiltered->Bind();

			// Generating the irradiance map from the filtered env cubemap
			//irradianceMap->BindImageTexture(0, 0, false, ImageFormat::Float16);
			//RenderCommand::DispatchComputeShader(envIrradianceShader, irradianceMap->GetWidth() / 32, irradianceMap->GetHeight() / 32, 6);
			//irradianceMap->GenerateMips();

			return { envFiltered, irradianceMap};
		}
		*/
	}
}