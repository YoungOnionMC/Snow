#include <spch.h>
#include "Snow/Render/SceneRenderer.h"

#include "Snow/Render/Renderer.h"
#include "Snow/Render/Renderer3D.h"

#include "Snow/ImGui/ImGuiLayer.h"

#include <imgui.h>

namespace Snow {
	namespace Render {
		struct SceneRendererData {
			Ref<Scene> ActiveScene;

			struct SceneInfo {
				EditorCamera Camera;

				Light ActiveLight;
			} SceneData;

			struct DrawCommand {
				Ref<Mesh> Mesh;
				Ref<MaterialInstance> MaterialInstance;
				glm::mat4 Transform;
			};
			std::vector<DrawCommand> DrawList;

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
			geoFramebufferSpec.ClearColor = { 0.2f, 0.2f, 0.2f, 1.0f };
			

			RenderPassSpecification geoRenderPassSpec;
			geoRenderPassSpec.TargetFramebuffer = Framebuffer::Create(geoFramebufferSpec);
			s_Data.GeometryPass = RenderPass::Create(geoRenderPassSpec);

			FramebufferSpecification compFramebufferSpec;
			compFramebufferSpec.AttachmentList = { FramebufferTextureFormat::RGBA8 };
			compFramebufferSpec.ClearColor = { 1.0f, 0.2f, 0.2f, 1.0f };
			compFramebufferSpec.SwapChainTarget = true;

			RenderPassSpecification compRenderPassSpec;
			compRenderPassSpec.TargetFramebuffer = Framebuffer::Create(compFramebufferSpec);
			s_Data.CompPass = RenderPass::Create(compRenderPassSpec);

			PipelineSpecification compPipelineSpec;
			compPipelineSpec.Type = PrimitiveType::Triangle;
			compPipelineSpec.Layout = {
				{ AttribType::Float3, "a_Position" },
				{ AttribType::Float3, "a_Normal" },
				{ AttribType::Float3, "a_Tangent" },
				{ AttribType::Float3, "a_Bitangent" },
				{ AttribType::Float2, "a_TexCoord" }
			};
			//compPipelineSpec.Shaders = { Renderer::GetShaderLibrary()->Get("PBRVert") };
			//compPipelineSpec.Shaders = { Shader::Create(ShaderType::Vertex, "assets/shaders/glsl/PBRVert.glsl"), Shader::Create(ShaderType::Pixel, "assets/shaders/glsl/PBRFrag.glsl") };
			
			//s_Data.CompositePipeline = Pipeline::Create(compPipelineSpec);


		}

		void SceneRenderer::OnViewportResize(uint32_t width, uint32_t height) {
			s_Data.GeometryPass->GetSpecification().TargetFramebuffer->Resize(width, height);
			s_Data.CompPass->GetSpecification().TargetFramebuffer->Resize(width, height);
		}

		void SceneRenderer::BeginScene(const Ref<Scene> scene, const SceneRendererCamera& camera) {
			s_Data.ActiveScene = scene;

			//s_Data.SceneData.Camera = camera;
			s_Data.SceneData.ActiveLight = scene->GetLight();


		}

		void SceneRenderer::BeginScene(const Ref<Scene> scene, const EditorCamera& camera) {
			s_Data.ActiveScene = scene;

			s_Data.SceneData.Camera = camera;
			s_Data.SceneData.ActiveLight = scene->GetLight();
		}

		void SceneRenderer::EndScene() {
			s_Data.ActiveScene = nullptr;

			FlushDrawList();

			
		}

		void SceneRenderer::SubmitMesh(Ref<Mesh> mesh, const glm::mat4& transform, const Ref<MaterialInstance> overrideMaterial) {
			s_Data.DrawList.push_back({ mesh, overrideMaterial, transform});
		}

		void SceneRenderer::GeometryPass() {
			Renderer::BeginRenderPass(s_Data.CompPass);
#if 0
			auto& sceneCamera = s_Data.SceneData.Camera;

			auto viewProjection = sceneCamera.GetViewProjectionMatrix();
			glm::vec3 cameraPosition = glm::inverse(sceneCamera.GetViewMatrix())[3];

			struct EnvironmentUB {
				Light lights;
				glm::vec3 u_CameraPosition;
			};

			EnvironmentUB enviroUB;
			enviroUB.lights = s_Data.SceneData.ActiveLight;
			enviroUB.u_CameraPosition = cameraPosition;
			s_Data.CompositePipeline->SetUniformBufferData("Environment", &enviroUB, sizeof(EnvironmentUB));

			s_Data.CompositePipeline->SetUniformBufferData("Camera", &viewProjection, sizeof(glm::mat4));

			for (auto& dc : s_Data.DrawList) {
				Ref<MaterialInstance> mi = dc.Mesh->GetMaterialInstance();
				Ref<Pipeline> pl = mi->GetMaterial()->GetPipeline();

				

				//pl->SetUniformBufferData(2, &s_Data.SceneData.ActiveLight, sizeof(Light));

				Renderer3D::DrawMesh(dc.Mesh, dc.Transform, dc.MaterialInstance);

				//pl->SetUniformBufferData(2, )
			}
#endif
			Renderer::EndRenderPass();
		}

		void* SceneRenderer::GetFinalColorAttachment() {
			return s_Data.GeometryPass->GetSpecification().TargetFramebuffer->GetColorAttachmentTexture(0);
		}

		void SceneRenderer::FlushDrawList() {
			GeometryPass();

			s_Data.DrawList.clear();
			s_Data.SceneData = {};
		}
	}
}