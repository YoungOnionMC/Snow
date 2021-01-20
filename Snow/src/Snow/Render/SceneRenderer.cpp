#include <spch.h>
#include "Snow/Render/SceneRenderer.h"

#include "Snow/Render/Renderer.h"
#include "Snow/Render/Renderer2D.h"
#include "Snow/Render/Renderer3D.h"

#include "Snow/ImGui/ImGuiLayer.h"

#include <imgui.h>

namespace Snow {
	namespace Render {
		struct SceneRendererData {
			Ref<Scene> ActiveScene;

			struct SceneInfo {
				SceneRendererCamera Camera;

				Light ActiveLight;
			} SceneData;

			struct CompositeInfo {
				float Exposure = 1.0f;
				int Samples = 4;
				bool Bloom = true;
				float BloomThreshold = 1.0f;
			} CompositeData;

			const std::array<const char*, 5> CompositeSampleValues = { "1", "2", "4", "8", "16" };

			struct DrawMeshCommand {
				Ref<Mesh> Mesh;
				Ref<MaterialInstance> MaterialInstance;
				glm::mat4 Transform;
			};

			std::vector<DrawMeshCommand> MeshDrawList;

			struct DrawQuadCommand {
				glm::mat4 Transform;
				glm::vec4 Color;
			};

			std::vector<DrawQuadCommand> QuadDrawList;

			Ref<Pipeline> CompositePipeline;

			Ref<RenderPass> GeometryPass;
			Ref<RenderPass> CompPass;

			bool ViewportFocused = false, ViewportHovered = false;
			glm::vec2 ViewportSize = { 0.0f, 0.0f };
		} s_Data;

		void SceneRenderer::Init() {
			FramebufferSpecification geoFramebufferSpec;
			geoFramebufferSpec.AttachmentList = { FramebufferTextureFormat::RGBA16F, FramebufferTextureFormat::RGBA16F, FramebufferTextureFormat::Depth };
			geoFramebufferSpec.Samples = 4;
			geoFramebufferSpec.ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
			

			RenderPassSpecification geoRenderPassSpec;
			geoRenderPassSpec.TargetFramebuffer = Framebuffer::Create(geoFramebufferSpec);
			s_Data.GeometryPass = RenderPass::Create(geoRenderPassSpec);

			FramebufferSpecification compFramebufferSpec;
			compFramebufferSpec.AttachmentList = { FramebufferTextureFormat::RGBA8 };
			compFramebufferSpec.ClearColor = { 1.0f, 0.2f, 0.2f, 1.0f };
			compFramebufferSpec.SwapChainTarget = false;

			RenderPassSpecification compRenderPassSpec;
			compRenderPassSpec.TargetFramebuffer = Framebuffer::Create(compFramebufferSpec);
			s_Data.CompPass = RenderPass::Create(compRenderPassSpec);

			PipelineSpecification compPipelineSpec;
			compPipelineSpec.Type = PrimitiveType::Triangle;
			compPipelineSpec.Layout = {
				{ AttribType::Float3, "a_Position" },
				{ AttribType::Float2, "a_TexCoord" }
			};
			compPipelineSpec.Shaders = { Renderer::GetShaderLibrary()->Get("SceneCompositeVert"), Renderer::GetShaderLibrary()->Get("SceneCompositeFrag") };
			//compPipelineSpec.Shaders = { Shader::Create(ShaderType::Vertex, "assets/shaders/glsl/PBRVert.glsl"), Shader::Create(ShaderType::Pixel, "assets/shaders/glsl/PBRFrag.glsl") };
			
			s_Data.CompositePipeline = Pipeline::Create(compPipelineSpec);


		}

		void SceneRenderer::OnViewportResize(uint32_t width, uint32_t height) {
			s_Data.GeometryPass->GetSpecification().TargetFramebuffer->Resize(width, height);
			s_Data.CompPass->GetSpecification().TargetFramebuffer->Resize(width, height);
		}

		void SceneRenderer::BeginScene(const Ref<Scene> scene, const SceneRendererCamera& camera) {
			s_Data.ActiveScene = scene;

			s_Data.SceneData.Camera = camera;
			s_Data.SceneData.ActiveLight = scene->GetLight();


		}

		void SceneRenderer::BeginScene(const Ref<Scene> scene, const EditorCamera& camera) {
			s_Data.ActiveScene = scene;

			s_Data.SceneData.Camera = { camera, camera.GetViewMatrix() };
			s_Data.SceneData.ActiveLight = scene->GetLight();
		}

		void SceneRenderer::EndScene() {
			s_Data.ActiveScene = nullptr;

			FlushDrawList();

			
		}

		void SceneRenderer::SubmitMesh(Ref<Mesh> mesh, const glm::mat4& transform, const Ref<MaterialInstance> overrideMaterial) {
			s_Data.MeshDrawList.push_back({ mesh, overrideMaterial, transform});
		}

		void SceneRenderer::Submit2DQuad(const glm::mat4& transform, const glm::vec4& color) {
			s_Data.QuadDrawList.push_back({ transform, color });
		}

		void SceneRenderer::GeometryPass() {
			Renderer::BeginRenderPass(s_Data.GeometryPass);
			auto& sceneCamera = s_Data.SceneData.Camera;
			
			auto viewProjection = sceneCamera.Camera.GetProjection() * sceneCamera.ViewMatrix;
			//auto viewProjection = sceneCamera.GetViewProjectionMatrix();
			glm::vec3 cameraPosition = glm::inverse(sceneCamera.ViewMatrix)[3];

			struct EnvironmentUB {
				Light lights;
				glm::vec3 u_CameraPosition;
			};

			EnvironmentUB enviroUB;
			enviroUB.lights = s_Data.SceneData.ActiveLight;
			enviroUB.u_CameraPosition = cameraPosition;
			s_Data.CompositePipeline->SetUniformBufferData("Environment", &enviroUB, sizeof(EnvironmentUB));

			s_Data.CompositePipeline->SetUniformBufferData("Camera", &viewProjection, sizeof(glm::mat4));

			for (auto& dc : s_Data.MeshDrawList) {
				Ref<MaterialInstance> mi = dc.Mesh->GetMaterialInstance();
				Ref<Pipeline> pl = mi->GetMaterial()->GetPipeline();

				

				//pl->SetUniformBufferData(2, &s_Data.SceneData.ActiveLight, sizeof(Light));

				Renderer3D::DrawMesh(dc.Mesh, dc.Transform, dc.MaterialInstance);

				//pl->SetUniformBufferData(2, )
			}

			Render::Renderer2D::BeginScene(s_Data.SceneData.Camera.Camera, s_Data.SceneData.Camera.ViewMatrix);
			for (auto& dc : s_Data.QuadDrawList) {
				Renderer2D::DrawQuad(dc.Transform, dc.Color);
			}
			Render::Renderer2D::EndScene();

			Renderer::EndRenderPass();
		}

		void SceneRenderer::CompositePass() {
			Renderer::BeginRenderPass(s_Data.CompPass);

			
			s_Data.CompositeData.Samples = s_Data.GeometryPass->GetSpecification().TargetFramebuffer->GetSpecification().Samples;

			s_Data.CompositePipeline->Bind();
			s_Data.CompositePipeline->SetUniformBufferData("CompositeBuffer", &s_Data.CompositeData, sizeof(SceneRendererData::CompositeInfo));
			s_Data.GeometryPass->GetSpecification().TargetFramebuffer->BindTexture();
			Renderer::SubmitFullscreenQuad(nullptr);

			Renderer::EndRenderPass();
		}

		void SceneRenderer::OnImGuiRender() {
			ImGui::Begin("Composite Pass");
			ImGui::DragFloat("Exposure", &s_Data.CompositeData.Exposure, 0.01f, 0.0f, 10.0f);
			ImGui::Combo("Samples", &s_Data.CompositeData.Samples, s_Data.CompositeSampleValues.data(), s_Data.CompositeSampleValues.size());
			//ImGui::InputInt("Samples", &s_Data.CompositeData.Samples, )
			ImGui::Checkbox("Bloom Enable", &s_Data.CompositeData.Bloom);
			if (s_Data.CompositeData.Bloom)
				ImGui::DragFloat("Bloom Threshold", &s_Data.CompositeData.BloomThreshold, 0.01f, 0.0f, 1.0f);
			ImGui::End();
		}

		void* SceneRenderer::GetFinalColorAttachment() {
			return s_Data.CompPass->GetSpecification().TargetFramebuffer->GetColorAttachmentTexture(0);
		}

		void SceneRenderer::FlushDrawList() {
			GeometryPass();
			CompositePass();

			s_Data.QuadDrawList.clear();
			s_Data.MeshDrawList.clear();
			s_Data.SceneData = {};
		}
	}
}