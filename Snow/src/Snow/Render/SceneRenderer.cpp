#include <spch.h>
#include "Snow/Render/SceneRenderer.h"

#include "Snow/Render/Renderer.h"
#include "Snow/Render/Renderer2D.h"
#include "Snow/Render/Renderer3D.h"

#include "Snow/ImGui/ImGuiLayer.h"

#include "Snow/Render/Material.h"
#include "Snow/Render/Texture.h"

#include "Snow/ImGui/ImGui.h"

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
			m_UniformBufferSet->Create(sizeof(UBShadow), 1);
			m_UniformBufferSet->Create(sizeof(UBScene), 2);
			m_UniformBufferSet->Create(sizeof(UBRenderer), 3);
			m_UniformBufferSet->Create(sizeof(UBPointLights), 4);


			Ref<Shader> compositeShader = Renderer::GetShaderLibrary()->Get("SceneComposite");
			
			{
				ImageSpecification spec;
				spec.Format = ImageFormat::Depth32F;
				spec.Usage = ImageUsage::Attachment;
				spec.Width = 4096;
				spec.Height = 4096;
				spec.Layers = 4;
				Ref<Image2D> depthImage = Image2D::Create(spec);
				depthImage->Invalidate();
				depthImage->CreatePerLayerImageViews();

				FramebufferSpecification fbSpec;
				fbSpec.Width = 4096;
				fbSpec.Height = 4096;
				fbSpec.AttachmentList = { ImageFormat::Depth32F };
				fbSpec.ClearColor = { 0.0f, 0.0f, 0.0f, 0.0f };
				fbSpec.ClearDepthValue = 1.0f;
				fbSpec.NoResize = true;
				fbSpec.ExistingImage = depthImage;
				fbSpec.DebugName = "Shadow Map";

				Ref<Shader> shadowMapShader = Renderer::GetShaderLibrary()->Get("ShadowMap");

				PipelineSpecification pipelineSpec;
				pipelineSpec.DebugName = "ShadowPass";
				pipelineSpec.Shader = shadowMapShader;
				pipelineSpec.Layout = {
					{AttribType::Float3, "a_Position"},
					{AttribType::Float3, "a_Normal"},
					{AttribType::Float3, "a_Tangent"},
					{AttribType::Float3, "a_Binormal"},
					{AttribType::Float2, "a_TexCoord"},
				};

				m_ShadowPassPipelines.resize(4);
				for (uint32_t i = 0; i < 4; i++) {
					fbSpec.ExistingImageLayers.clear();
					fbSpec.ExistingImageLayers.emplace_back(i);

					RenderPassSpecification rpSpec;
					rpSpec.TargetFramebuffer = Framebuffer::Create(fbSpec);
					rpSpec.DebugName = "ShadowMap" + std::to_string(i);
					pipelineSpec.BindedRenderPass = RenderPass::Create(rpSpec);
					m_ShadowPassPipelines[i] = Pipeline::Create(pipelineSpec);
				}

				m_ShadowPassMaterial = Material::Create(shadowMapShader, "ShadowPass");
			}

			//PreDepth Pass
			{
				FramebufferSpecification fbSpec;
				fbSpec.AttachmentList = { /*ImageFormat::Red,*/ ImageFormat::Depth24Stencil8};
				fbSpec.ClearColor = { 0.0f, 0.0f, 0.0f, 0.0f };
				fbSpec.ClearDepthValue = 0.0f;
				fbSpec.DebugName = "PreDepth";

				RenderPassSpecification rpSpec;
				rpSpec.TargetFramebuffer = Render::Framebuffer::Create(fbSpec);
				rpSpec.DebugName = fbSpec.DebugName;

				auto shader = Renderer::GetShaderLibrary()->Get("PreDepth");
				PipelineSpecification pipelineSpec;
				pipelineSpec.DebugName = fbSpec.DebugName;
				pipelineSpec.Shader = shader;
				pipelineSpec.Layout = {
					{AttribType::Float3, "a_Position"},
					{AttribType::Float3, "a_Normal"},
					{AttribType::Float3, "a_Tangent"},
					{AttribType::Float3, "a_Binormal"},
					{AttribType::Float2, "a_TexCoord"},
				};
				pipelineSpec.BindedRenderPass = RenderPass::Create(rpSpec);
				m_PreDepthPipeline = Pipeline::Create(pipelineSpec);
				m_PreDepthMaterial = Material::Create(shader, pipelineSpec.DebugName);
			}

			// Geometry Pipeline
			{
				FramebufferSpecification framebufferSpec;
				framebufferSpec.AttachmentList = { ImageFormat::RGBA32F, ImageFormat::RGBA32F, ImageFormat::RGBA16F, ImageFormat::DepthStencil };
				framebufferSpec.ExistingImages[3] = m_PreDepthPipeline->GetSpecification().BindedRenderPass->GetSpecification().TargetFramebuffer->GetDepthImage();

				framebufferSpec.Samples = 1;
				framebufferSpec.ClearColor = { 0.3f, 0.1f, 0.1f, 1.0f };
				framebufferSpec.ClearDepthOnLoad = false;
				framebufferSpec.DebugName = "Geometry";

				Ref<Framebuffer> framebuffer = Framebuffer::Create(framebufferSpec);


				RenderPassSpecification renderPassSpec;
				renderPassSpec.TargetFramebuffer = framebuffer;
				renderPassSpec.DebugName = "Geometry";
				
				PipelineSpecification pipelineSpec;
				pipelineSpec.LineWidth = 1.0f;
				pipelineSpec.DepthTest = true;
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

				//s_SceneRendererData.GeometryPass = RenderPass::Create(geoRenderPassSpec);
			}

			{
				m_LightCullingWorkGroups = { (m_ViewportWidth + m_ViewportWidth % 16) / 16, (m_ViewportHeight + m_ViewportHeight % 16) / 16, 1 };

				m_StorageBufferSet = StorageBufferSet::Create(framesInFlight);
				m_StorageBufferSet->Create(1, 14);
			}

			// Grid Pipeline
			{
				Ref<Shader> gridShader = Renderer::GetShaderLibrary()->Get("Grid");

				const float gridScale = 160.0f;
				const float gridSize = 0.025f;
				m_GridMaterial = Render::Material::Create(gridShader);
				m_GridMaterial->Set("u_Settings.Scale", gridScale);
				m_GridMaterial->Set("u_Settings.Size", gridSize);

				PipelineSpecification pipelineSpec;
				pipelineSpec.DebugName = "Grid";
				pipelineSpec.Shader = gridShader;
				pipelineSpec.BackfaceCulling = false;
				pipelineSpec.Layout = {
					{AttribType::Float3, "a_Position"},
					{AttribType::Float2, "a_TexCoord"},
				};
				pipelineSpec.BindedRenderPass = m_GeometryPipeline->GetSpecification().BindedRenderPass;
				m_GridPipeline = Render::Pipeline::Create(pipelineSpec);
			}

			{
				auto skyboxShader = Renderer::GetShaderLibrary()->Get("Skybox");

				PipelineSpecification pipelineSpec;
				pipelineSpec.DebugName = "Skybox";
				pipelineSpec.Shader = skyboxShader;
				pipelineSpec.BackfaceCulling = false;
				pipelineSpec.Layout = {
					{AttribType::Float3, "a_Position"},
					{AttribType::Float2, "a_TexCoord"}
				};
				pipelineSpec.BindedRenderPass = m_GeometryPipeline->GetSpecification().BindedRenderPass;
				m_SkyboxPipeline = Pipeline::Create(pipelineSpec);

				m_SkyboxMaterial = Material::Create(skyboxShader);
				m_SkyboxMaterial->SetFlag(MaterialFlag::DepthTest, false);
			}

			// Composite Pipeline
			{
				FramebufferSpecification compFramebufferSpec;

				compFramebufferSpec.DebugName = "SceneComposite";
				compFramebufferSpec.ClearColor = { 0.35f, 0.8f, 0.15f, 1.0f };
				compFramebufferSpec.Transfer = true;
				//compFramebufferSpec.SwapChainTarget = m_Specification.SwapChainTarget;
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
				pipelineSpec.Shader = compositeShader;
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

				m_CompositeMaterial = Material::Create(compositeShader);
			}

			if(!m_Specification.SwapChainTarget) {
				FramebufferSpecification framebufferSpec;
				framebufferSpec.AttachmentList = { ImageFormat::RGBA, ImageFormat::DepthStencil };
				framebufferSpec.ClearColor = { 0.6f, 0.7f, 0.9f, 1.0f };
				framebufferSpec.ClearColorOnLoad = false;
				framebufferSpec.ClearDepthOnLoad = false;
				framebufferSpec.DebugName = "External Composite";

				framebufferSpec.ExistingImages[0] = m_CompositePipeline->GetSpecification().BindedRenderPass->GetSpecification().TargetFramebuffer->GetImage();
				framebufferSpec.ExistingImages[1] = m_PreDepthPipeline->GetSpecification().BindedRenderPass->GetSpecification().TargetFramebuffer->GetDepthImage();

				Ref<Framebuffer> framebuffer = Framebuffer::Create(framebufferSpec);

				RenderPassSpecification renderPassSpec;
				renderPassSpec.TargetFramebuffer = framebuffer;
				renderPassSpec.DebugName = framebufferSpec.DebugName;
				m_ExternalCompositeRenderPass = RenderPass::Create(renderPassSpec);
			}

			
			Ref<SceneRenderer> instance = this;
			Renderer::Submit([instance]() mutable {
				instance->m_ResourcesCreated = true;
			});
		}

		void SceneRenderer::SetScene(Ref<Scene> scene) {
			m_Scene = scene;
		}

		void SceneRenderer::OnViewportResize(uint32_t width, uint32_t height) {
			if (m_ViewportWidth != width || m_ViewportHeight != height) {
				m_ViewportWidth = width;
				m_ViewportHeight = height;
				m_NeedsResize = true;
			}
		}

		void SceneRenderer::BeginScene(const SceneRendererCamera& camera) {
			SNOW_CORE_ASSERT(m_Scene);

			if (!m_ResourcesCreated)
				return;

			m_SceneData.SceneCamera = camera;
			m_SceneData.SceneEnvironment = m_Scene->m_Environment;
			m_SceneData.SceneEnvironmentIntensity = m_Scene->m_EnvironmentIntensity;
			m_SceneData.ActiveLight = m_Scene->m_Light;
			m_SceneData.SkyboxLod = m_Scene->m_SkyboxLod;
			m_SceneData.SceneLightEnvironment = m_Scene->m_LightEnvironment;

			if (m_NeedsResize) {
				m_NeedsResize = false;

				m_GeometryPipeline->GetSpecification().BindedRenderPass->GetSpecification().TargetFramebuffer->Resize(m_ViewportWidth, m_ViewportHeight);

				m_CompositePipeline->GetSpecification().BindedRenderPass->GetSpecification().TargetFramebuffer->Resize(m_ViewportWidth, m_ViewportHeight);

				if(m_ExternalCompositeRenderPass)
					m_ExternalCompositeRenderPass->GetSpecification().TargetFramebuffer->Resize(m_ViewportWidth, m_ViewportHeight);

				if (m_Specification.SwapChainTarget)
					m_CommandBuffer = RenderCommandBuffer::CreateFromSwapChain("SceneRenderer");

				m_LightCullingWorkGroups = { (m_ViewportWidth + m_ViewportWidth % 16) / 16, (m_ViewportHeight + m_ViewportHeight % 16) / 16, 1 };
				RendererDataUB.tilesCountX = m_LightCullingWorkGroups.x;

				m_StorageBufferSet->Resize(14, 0, m_LightCullingWorkGroups.x * m_LightCullingWorkGroups.y * 4096);
			}

			UBCamera& cameraData = CameraDataUB;
			UBScene& sceneData = SceneDataUB;
			UBShadow& shadowData = ShadowDataUB;
			UBRenderer& rendererData = RendererDataUB;
			UBPointLights& pointLightData = PointLightsUB;

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

			const std::vector<PointLight>& pointLightVec = m_SceneData.SceneLightEnvironment.PointLights;
			pointLightData.Count = pointLightVec.size();
			std::memcpy(pointLightData.PointLights, pointLightVec.data(), sizeof(PointLight) * pointLightVec.size());
			Renderer::Submit([instance, &pointLightData]() mutable {
				uint32_t bufferIndex = Render::Renderer::GetCurrentFrameIndex();
				instance->m_UniformBufferSet->Get(4, 0, bufferIndex)->RTSetData(&pointLightData, 16ull + sizeof PointLight * pointLightData.Count);
			});

			auto& directionalLight = m_SceneData.SceneLightEnvironment.DirectionalLights[0];
			sceneData.Light.Direction = directionalLight.Direction;
			sceneData.Light.Radiance = directionalLight.Radiance;
			sceneData.Light.Multiplier = directionalLight.Multiplier;
			sceneData.u_CameraPosition = cameraPosition;
			sceneData.EnvironmentMapIntensity = m_SceneData.SceneEnvironmentIntensity;
			Render::Renderer::Submit([instance, sceneData]() mutable {
				uint32_t bufferIndex = Render::Renderer::GetCurrentFrameIndex();
				instance->m_UniformBufferSet->Get(2, 0, bufferIndex)->RTSetData(&sceneData, sizeof(sceneData));
			});

			CascadeData cascades[4];
			CalculateCascades(cascades, sceneCamera, directionalLight.Direction);

			for (uint32_t i = 0; i < 4; i++) {
				CascadeSplits[i] = cascades[i].SplitDepth;
				shadowData.ViewProjection[i] = cascades[i].ViewProj;
			}
			Render::Renderer::Submit([instance, shadowData]() mutable {
				uint32_t bufferIndex = Render::Renderer::GetCurrentFrameIndex();
				instance->m_UniformBufferSet->Get(1, 0, bufferIndex)->RTSetData(&shadowData, sizeof(shadowData));
				});

			rendererData.CascadeSplits = CascadeSplits;
			Render::Renderer::Submit([instance, rendererData]() mutable {
				uint32_t bufferIndex = Render::Renderer::GetCurrentFrameIndex();
				instance->m_UniformBufferSet->Get(3, 0, bufferIndex)->RTSetData(&rendererData, sizeof(rendererData));
			});

			Renderer::SetSceneEnvironment(this, m_SceneData.SceneEnvironment, m_ShadowPassPipelines[2]->GetSpecification().BindedRenderPass->GetSpecification().TargetFramebuffer->GetDepthImage());
		}

		void SceneRenderer::BeginScene(const Editor::EditorCamera& camera) {
		}

		void SceneRenderer::EndScene() {
			//s_SceneRendererData.ActiveScene = nullptr;
			FlushDrawList();

			
		}

		void SceneRenderer::SubmitMesh(Ref<Mesh> mesh, Ref<MaterialTable> materialTable, const glm::mat4& transform, Ref<Material> overrideMaterial) {//, const Ref<MaterialInstance> overrideMaterial) {
			m_DrawList.push_back({ mesh, materialTable, transform, overrideMaterial });
			m_ShadowDrawList.push_back({ mesh, materialTable, transform, overrideMaterial });
		}

		void SceneRenderer::Submit2DQuad(const glm::mat4& transform, const glm::vec4& color) {
			//s_SceneRendererData.QuadDrawList.push_back({ transform, color });
		}

		void SceneRenderer::PreDepthPass() {
			Renderer::BeginRenderPass(m_CommandBuffer, m_PreDepthPipeline->GetSpecification().BindedRenderPass);
			for (auto& dc : m_DrawList) 
				Renderer::RenderMeshWithMaterial(m_CommandBuffer, m_PreDepthPipeline, m_UniformBufferSet, nullptr, dc.Mesh, dc.Transform, m_PreDepthMaterial);
			
			Renderer::EndRenderPass(m_CommandBuffer);
		}

		void SceneRenderer::ShadowMapPass() {

			auto& directionalLights = m_SceneData.SceneLightEnvironment.DirectionalLights;
			if (directionalLights[0].Multiplier = 0.0f || !directionalLights[0].CastShadows) {
				for (uint32_t i = 0; i < 4; i++) {
					ClearPass(m_ShadowPassPipelines[i]->GetSpecification().BindedRenderPass);
				}
				return;
			}

			for (uint32_t i = 0; i < 4; i++) {
				Renderer::BeginRenderPass(m_CommandBuffer, m_ShadowPassPipelines[i]->GetSpecification().BindedRenderPass);

				const Buffer cascade(&i, sizeof(uint32_t));
				for (auto& dc : m_ShadowDrawList) {
					Renderer::RenderMeshWithMaterial(m_CommandBuffer, m_ShadowPassPipelines[i], m_UniformBufferSet, nullptr, dc.Mesh, dc.Transform, m_ShadowPassMaterial, cascade);
				}
				Renderer::EndRenderPass(m_CommandBuffer);
			}
		}

		void SceneRenderer::GeometryPass() {

			//auto [rad, irr] = CreateEnvironmentMap("assets/env/birchwood_4k.hdr");
			//s_SceneRendererData.SceneData.SceneEnvironment.RadianceMap = rad;

			//Renderer::ClearImage(m_CommandBuffer, m_GeometryPipeline->GetSpecification().BindedRenderPass->GetSpecification().TargetFramebuffer->GetImage());
			Renderer::BeginRenderPass(m_CommandBuffer, m_GeometryPipeline->GetSpecification().BindedRenderPass, false);

			m_SkyboxMaterial->Set("u_Uniforms.TextureLod", m_SceneData.SkyboxLod);
			m_SkyboxMaterial->Set("u_Uniforms.Intensity", m_SceneData.SceneEnvironmentIntensity);
			const Ref<TextureCube> radianceMap = m_SceneData.SceneEnvironment ? m_SceneData.SceneEnvironment->RadianceMap : Renderer::GetBlackCubeTexture();
			m_SkyboxMaterial->Set("u_Texture", radianceMap);
			Renderer::SubmitFullscreenQuad(m_CommandBuffer, m_SkyboxPipeline, m_UniformBufferSet, nullptr, m_SkyboxMaterial);

			for (auto& dc : m_DrawList) {
				Renderer::RenderMesh(m_CommandBuffer, m_GeometryPipeline, m_UniformBufferSet, m_StorageBufferSet, dc.Mesh, dc.MaterialTable ? dc.MaterialTable : dc.Mesh->GetMaterials(), dc.Transform);
			}

			if (GetOptions().ShowGrid) {
				glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(8.0f));
				Renderer::RenderQuad(m_CommandBuffer, m_GridPipeline, m_UniformBufferSet, nullptr, m_GridMaterial, transform);
			}


			Renderer::EndRenderPass(m_CommandBuffer);
			
		}

		void SceneRenderer::CompositePass() {
			Renderer::BeginRenderPass(m_CommandBuffer, m_CompositePipeline->GetSpecification().BindedRenderPass, true);

			auto framebuffer = m_GeometryPipeline->GetSpecification().BindedRenderPass->GetSpecification().TargetFramebuffer;
			int textureSamples = framebuffer->GetSpecification().Samples;

			float exposure = 1.0f;

			//m_CompositeMaterial->Set("u_Uniforms.TextureSamples", textureSamples);
			m_CompositeMaterial->Set("u_Uniforms.Exposure", exposure);
			m_CompositeMaterial->Set("u_Texture", framebuffer->GetImage());

			Renderer::SubmitFullscreenQuad(m_CommandBuffer, m_CompositePipeline, nullptr, m_CompositeMaterial);

			Renderer::EndRenderPass(m_CommandBuffer);
		}

		

		Ref<Image2D> SceneRenderer::GetFinalPassImage() {
			if (!m_ResourcesCreated)
				return nullptr;

			return m_CompositePipeline->GetSpecification().BindedRenderPass->GetSpecification().TargetFramebuffer->GetImage();
		}

		void SceneRenderer::FlushDrawList() {
			if (m_ResourcesCreated) {


				m_CommandBuffer->Begin();

				//ShadowMapPass();
				PreDepthPass();
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

			m_DrawList.clear();
			m_ShadowDrawList.clear();
		}

		void SceneRenderer::ClearPass(Ref<RenderPass> renderPass, bool clear) {
			Renderer::BeginRenderPass(m_CommandBuffer, renderPass, clear);
			Renderer::EndRenderPass(m_CommandBuffer);
		}

		void SceneRenderer::ClearPass() {
			Renderer::BeginRenderPass(m_CommandBuffer, m_CompositePipeline->GetSpecification().BindedRenderPass, true);
			Renderer::EndRenderPass(m_CommandBuffer);
		}

		void SceneRenderer::CalculateCascades(CascadeData* cascades, const SceneRendererCamera& sceneCamera, const glm::vec3& lightDirection) const {
			struct FrustumBounds
			{
				float r, l, b, t, f, n;
			};

			//FrustumBounds frustumBounds[3];

			auto viewProjection = sceneCamera.Camera.GetProjection() * sceneCamera.ViewMatrix;

			const int SHADOW_MAP_CASCADE_COUNT = 4;
			float cascadeSplits[SHADOW_MAP_CASCADE_COUNT];

			// TODO: less hard-coding!
			float nearClip = 0.1f;
			float farClip = 1000.0f;
			float clipRange = farClip - nearClip;

			float minZ = nearClip;
			float maxZ = nearClip + clipRange;

			float range = maxZ - minZ;
			float ratio = maxZ / minZ;

			// Calculate split depths based on view camera frustum
			// Based on method presented in https://developer.nvidia.com/gpugems/GPUGems3/gpugems3_ch10.html
			for (uint32_t i = 0; i < SHADOW_MAP_CASCADE_COUNT; i++)
			{
				float p = (i + 1) / static_cast<float>(SHADOW_MAP_CASCADE_COUNT);
				float log = minZ * std::pow(ratio, p);
				float uniform = minZ + range * p;
				float d = CascadeSplitLambda * (log - uniform) + uniform;
				cascadeSplits[i] = (d - nearClip) / clipRange;
			}

			cascadeSplits[3] = 0.3f;

			// Manually set cascades here
			// cascadeSplits[0] = 0.05f;
			// cascadeSplits[1] = 0.15f;
			// cascadeSplits[2] = 0.3f;
			// cascadeSplits[3] = 1.0f;

			// Calculate orthographic projection matrix for each cascade
			float lastSplitDist = 0.0;
			for (uint32_t i = 0; i < SHADOW_MAP_CASCADE_COUNT; i++)
			{
				float splitDist = cascadeSplits[i];

				glm::vec3 frustumCorners[8] =
				{
					glm::vec3(-1.0f,  1.0f, -1.0f),
					glm::vec3(1.0f,  1.0f, -1.0f),
					glm::vec3(1.0f, -1.0f, -1.0f),
					glm::vec3(-1.0f, -1.0f, -1.0f),
					glm::vec3(-1.0f,  1.0f,  1.0f),
					glm::vec3(1.0f,  1.0f,  1.0f),
					glm::vec3(1.0f, -1.0f,  1.0f),
					glm::vec3(-1.0f, -1.0f,  1.0f),
				};

				// Project frustum corners into world space
				glm::mat4 invCam = glm::inverse(viewProjection);
				for (uint32_t i = 0; i < 8; i++)
				{
					glm::vec4 invCorner = invCam * glm::vec4(frustumCorners[i], 1.0f);
					frustumCorners[i] = invCorner / invCorner.w;
				}

				for (uint32_t i = 0; i < 4; i++)
				{
					glm::vec3 dist = frustumCorners[i + 4] - frustumCorners[i];
					frustumCorners[i + 4] = frustumCorners[i] + (dist * splitDist);
					frustumCorners[i] = frustumCorners[i] + (dist * lastSplitDist);
				}

				// Get frustum center
				glm::vec3 frustumCenter = glm::vec3(0.0f);
				for (uint32_t i = 0; i < 8; i++)
					frustumCenter += frustumCorners[i];

				frustumCenter /= 8.0f;

				//frustumCenter *= 0.01f;

				float radius = 0.0f;
				for (uint32_t i = 0; i < 8; i++)
				{
					float distance = glm::length(frustumCorners[i] - frustumCenter);
					radius = glm::max(radius, distance);
				}
				radius = std::ceil(radius * 16.0f) / 16.0f;

				glm::vec3 maxExtents = glm::vec3(radius);
				glm::vec3 minExtents = -maxExtents;

				glm::vec3 lightDir = -lightDirection;
				glm::mat4 lightViewMatrix = glm::lookAt(frustumCenter - lightDir * -minExtents.z, frustumCenter, glm::vec3(0.0f, 0.0f, 1.0f));
				glm::mat4 lightOrthoMatrix = glm::ortho(minExtents.x, maxExtents.x, minExtents.y, maxExtents.y, 0.0f + CascadeNearPlaneOffset, maxExtents.z - minExtents.z + CascadeFarPlaneOffset);

				// Offset to texel space to avoid shimmering (from https://stackoverflow.com/questions/33499053/cascaded-shadow-map-shimmering)
				glm::mat4 shadowMatrix = lightOrthoMatrix * lightViewMatrix;
				const float ShadowMapResolution = 4096.0f;
				glm::vec4 shadowOrigin = (shadowMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) * ShadowMapResolution / 2.0f;
				glm::vec4 roundedOrigin = glm::round(shadowOrigin);
				glm::vec4 roundOffset = roundedOrigin - shadowOrigin;
				roundOffset = roundOffset * 2.0f / ShadowMapResolution;
				roundOffset.z = 0.0f;
				roundOffset.w = 0.0f;

				lightOrthoMatrix[3] += roundOffset;

				// Store split distance and matrix in cascade
				cascades[i].SplitDepth = (nearClip + splitDist * clipRange) * -1.0f;
				cascades[i].ViewProj = lightOrthoMatrix * lightViewMatrix;
				cascades[i].View = lightViewMatrix;

				lastSplitDist = cascadeSplits[i];
			}

			/*
			struct FrustumBounds {
				float r, l, b, t, f, n;
			};

			auto viewProj = sceneCamera.Camera.GetProjection() * sceneCamera.ViewMatrix;

			const int ShadowMapCascadeCount = 4;
			float cascadeSplits[ShadowMapCascadeCount];

			float nearClip = 0.1f;
			float farClip = 1000.0f;
			float clipRange = farClip - nearClip;

			float minZ = nearClip;
			float maxZ = nearClip + clipRange;

			float range = maxZ - minZ;
			float ratio = maxZ / minZ;

			for (uint32_t i = 0; i < ShadowMapCascadeCount; i++) {
				float p = (i + 1) / static_cast<float>(ShadowMapCascadeCount);
				float log = minZ * std::pow(ratio, p);
				float uniform = minZ + range * p;
				float d = CascadeSplitLambda * (log - uniform) + uniform;
				cascadeSplits[i] = (d - nearClip) / clipRange;
			}

			cascadeSplits[3] = 0.3f;

			float lastSplitDist = 0.0f;
			for (uint32_t i = 0; i < ShadowMapCascadeCount; i++) {
				float splitDist = cascadeSplits[i];

				glm::vec3 frustumCorners[8] = {
					glm::vec3(-1.0f,  1.0f, -1.0f),
					glm::vec3( 1.0f,  1.0f, -1.0f),
					glm::vec3( 1.0f, -1.0f, -1.0f),
					glm::vec3(-1.0f, -1.0f, -1.0f),
					glm::vec3(-1.0f,  1.0f,  1.0f),
					glm::vec3( 1.0f,  1.0f,  1.0f),
					glm::vec3( 1.0f, -1.0f,  1.0f),
					glm::vec3(-1.0f, -1.0f,  1.0f)
				};

				glm::mat4 invCamera = glm::inverse(viewProj);
				for (uint32_t i = 0; i < 8; i++) {
					glm::vec4 invCorner = invCamera * glm::vec4(frustumCorners[i], 1.0f);
					frustumCorners[i] = invCorner / invCorner.w;
				}

				for (uint32_t i = 0; i < 4; i++) {
					glm::vec3 dist = frustumCorners[i + 4] - frustumCorners[i];
					frustumCorners[i + 4] = frustumCorners[i] + (dist * splitDist);
					frustumCorners[i] = frustumCorners[i] + (dist * lastSplitDist);
				}

				glm::vec3 frustumCenter = glm::vec3(0.0f);
				for (uint32_t i = 0; i < 8; i++)
					frustumCenter += frustumCorners[i];
				frustumCenter /= 8.0f;

				float radius = 0.0f;
				for (uint32_t i = 0; i < 8; i++) {
					float dist = glm::length(frustumCorners[i] - frustumCenter);
					radius = glm::max(radius, dist);
				}
				radius = std::ceil(radius * 16.0f) / 16.0f;

				glm::vec3 maxExtents = glm::vec3(radius);
				glm::vec3 minExtents = -maxExtents;

				glm::vec3 lightDir = -lightDirection;
				glm::mat4 lightViewMat = glm::lookAt(frustumCenter - lightDir * -minExtents.z, frustumCenter, glm::vec3(0.0f, 0.0f, 1.0f));
				glm::mat4 lightOrthoMat = glm::ortho(minExtents.x, maxExtents.x, minExtents.y, maxExtents.y, 0.0f + CascadeNearPlaneOffset, maxExtents.z - minExtents.z + CascadeFarPlaneOffset);

				glm::mat4 shadowMatrix = lightOrthoMat * lightViewMat;
				const float ShadowMapResolution = 4096.0f;
				glm::vec4 shadowOrigin = (shadowMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) * ShadowMapResolution / 2.0f;
				glm::vec4 roundedOrigin = glm::round(shadowOrigin);
				glm::vec4 roundOffset = roundedOrigin - shadowOrigin;
				roundOffset = roundOffset * 2.0f / ShadowMapResolution;
				roundOffset.z = 0.0f;
				roundOffset.w = 0.0f;

				lightOrthoMat[3] += roundOffset;

				cascades[i].SplitDepth = (nearClip + splitDist * clipRange) * -1.0f;
				cascades[i].ViewProj = lightOrthoMat * lightViewMat;
				cascades[i].View = lightViewMat;

				lastSplitDist = cascadeSplits[i];
			}

			*/
		}

		SceneRendererOptions& SceneRenderer::GetOptions() {
			return m_Options;
		}

		void SceneRenderer::OnImGuiRender() {
			ImGui::Begin("SceneOptions");

			if(UI::PropertyGridHeader("Options")) {
				UI::BeginPropertyGrid();
				UI::Property("Show Grid", m_Options.ShowGrid);
				UI::Property("Show Bounding Boxes", m_Options.ShowBoundingBoxes);
				UI::Property("Show Shadow Cascades", RendererDataUB.ShowCascades);
				UI::EndPropertyGrid();

				UI::EndTreeNode();
			}

			if (UI::PropertyGridHeader("Shadows")) {
				UI::BeginPropertyGrid();
				UI::Property("Soft Shadows", RendererDataUB.SoftShadows);
				UI::Property("Directional Light Size", RendererDataUB.LightSize, 0.01f);
				UI::Property("Max Shadow Distance", RendererDataUB.MaxShadowDistance, 1.0f);
				UI::Property("Shadow Fade", RendererDataUB.ShadowFade, 5.0f);
				UI::EndPropertyGrid();
				if (UI::BeginTreeNode("Cascade Settings")) {
					UI::BeginPropertyGrid();
					UI::Property("Cascade Fading", RendererDataUB.CascadeFading);
					UI::Property("Cascade Transition Fade", RendererDataUB.CascadeTransitionFade, 0.05f, 0.0f, FLT_MAX);
					UI::Property("Cascade Split", CascadeSplitLambda, 0.01f);
					UI::Property("Cascade Near Plane Offset", CascadeNearPlaneOffset);
					UI::Property("Cascade Far Plane Offset", CascadeFarPlaneOffset);
					UI::EndPropertyGrid();
					UI::EndTreeNode();
				}
				if (UI::BeginTreeNode("Shadow Map", false)) {
					static int cascadeIndex = 0;
					auto fb = m_ShadowPassPipelines[cascadeIndex]->GetSpecification().BindedRenderPass->GetSpecification().TargetFramebuffer;
					auto image = fb->GetDepthImage();

					float size = ImGui::GetContentRegionAvailWidth();
					UI::BeginPropertyGrid();
					UI::PropertySlider("Cascade Index", cascadeIndex, 0, 3);
					UI::EndPropertyGrid();
					if (m_ResourcesCreated)
						UI::Image(image, (uint32_t)cascadeIndex, { size, size }, { 0, 1 }, { 1, 0 });
					UI::EndTreeNode();
				}

				if (UI::BeginTreeNode("DebugShadowMap", false)) {
					
					auto fb = m_ShadowPassPipelines[0]->GetSpecification().BindedRenderPass->GetSpecification().TargetFramebuffer;
					auto image = fb->GetDepthImage();

					float size = ImGui::GetContentRegionAvailWidth();

					if (m_ResourcesCreated) {
						UI::Image(image, (uint32_t)0, { size, size }, { 0, 1 }, { 1, 0 });
						UI::Image(image, (uint32_t)1, { size, size }, { 0, 1 }, { 1, 0 });
						UI::Image(image, (uint32_t)2, { size, size }, { 0, 1 }, { 1, 0 });
						UI::Image(image, (uint32_t)3, { size, size }, { 0, 1 }, { 1, 0 });
					}
					UI::EndTreeNode();
				}

				UI::EndTreeNode();
			}

			if (UI::PropertyGridHeader("Render Statistics")) {
				uint32_t frameIndex = Renderer::GetCurrentFrameIndex();

				if (UI::BeginTreeNode("Pipeline Stats")) {
					const PipelineStatistics& pipelineStats = m_CommandBuffer->GetPipelineStatistics(frameIndex);
					ImGui::Text("Input Assembly Vertices: %llu", pipelineStats.InputAssemblyVertices);
					ImGui::Text("Input Assembly Primitives: %llu", pipelineStats.InputAssemblyPrimitives);
					ImGui::Text("Clipping Invocations: %llu", pipelineStats.ClippingInvocations);
					ImGui::Text("Clipping Primitives: %llu", pipelineStats.ClippingPrimitives);
					ImGui::Text("Vertex Shader Invocations: %llu", pipelineStats.VertexShaderInvocations);
					ImGui::Text("Fragment Shader Invocations: %llu", pipelineStats.FragmentShaderInvocations);
					ImGui::Text("Compute Shader Invocations: %llu", pipelineStats.ComputeShaderInvocations);
					UI::EndTreeNode();
				}
				UI::EndTreeNode();
			}
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