#include <spch.h>
#include "Snow/Render/SceneRenderer.h"

#include "Snow/Render/Renderer.h"
#include "Snow/Render/Renderer3D.h"

namespace Snow {
	namespace Render {
		struct SceneRendererData {
			Ref<Scene> ActiveScene;

			struct SceneInfo {
				SceneRendererCamera Camera;

				Light ActiveLight;
			} SceneData;

			struct DrawCommand {
				Ref<Mesh> Mesh;
				glm::mat4 Transform;
			};
			std::vector<DrawCommand> DrawList;

			Ref<Pipeline> CompositePipeline;

			Ref<RenderPass> GeometryPass;
			Ref<RenderPass> CompPass;
		} s_Data;

		void SceneRenderer::Init() {
			FramebufferSpecification geoFramebufferSpec;
			geoFramebufferSpec.Attachments = { FramebufferTextureFormat::RGBA16F, FramebufferTextureFormat::RGBA16F, FramebufferTextureFormat::Depth };
			geoFramebufferSpec.Samples = 4;
			geoFramebufferSpec.ClearColor = { 0.2f, 0.2f, 0.2f, 1.0f };

			RenderPassSpecification geoRenderPassSpec;
			geoRenderPassSpec.TargetFramebuffer = Framebuffer::Create(geoFramebufferSpec);
			s_Data.GeometryPass = RenderPass::Create(geoRenderPassSpec);

			FramebufferSpecification compFramebufferSpec;
			compFramebufferSpec.Attachments = { FramebufferTextureFormat::RGBA8 };
			compFramebufferSpec.ClearColor = { 0.2f, 0.2f, 0.2f, 1.0f };

			RenderPassSpecification compRenderPassSpec;
			compRenderPassSpec.TargetFramebuffer = Framebuffer::Create(compFramebufferSpec);
			s_Data.CompPass = RenderPass::Create(compRenderPassSpec);


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

		void SceneRenderer::EndScene() {
			s_Data.ActiveScene = nullptr;

			FlushDrawList();
		}

		void SceneRenderer::SubmitMesh(Ref<Mesh> mesh, const glm::mat4& transform) {
			s_Data.DrawList.push_back({ mesh, transform });
		}

		void SceneRenderer::GeometryPass() {
			//Renderer::BeginRenderPass(s_Data.GeometryPass);

			auto& sceneCamera = s_Data.SceneData.Camera;

			auto viewProjection = sceneCamera.Camera.GetProjection() * sceneCamera.ViewMatrix;
			glm::vec3 cameraPosition = glm::inverse(sceneCamera.ViewMatrix)[3];

			for (auto& dc : s_Data.DrawList) {
				Ref<MaterialInstance> mi = dc.Mesh->GetMaterialInstance();
				Ref<Pipeline> pl = mi->GetMaterial()->GetPipeline();

				

				pl->SetUniformBufferData(2, &s_Data.SceneData.ActiveLight, sizeof(Light));

				Renderer3D::DrawMesh(dc.Mesh, dc.Transform);

				//pl->SetUniformBufferData(2, )
			}

			//Renderer::EndRenderPass();
		}

		void SceneRenderer::FlushDrawList() {
			GeometryPass();

			s_Data.DrawList.clear();
			s_Data.SceneData = {};
		}
	}
}