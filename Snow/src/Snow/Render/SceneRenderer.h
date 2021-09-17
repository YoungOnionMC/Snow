#pragma once

#include "Snow/Scene/Scene.h"

#include "Snow/Render/Mesh.h"

#include "Snow/Scene/SceneCamera.h"

#include "Snow/Render/ComputePipeline.h"
#include "Snow/Render/RenderPass.h"
#include "Snow/Render/RenderCommandBuffer.h"
#include "Snow/Render/StorageBufferSet.h"
#include "Snow/Render/UniformBufferSet.h"

namespace Snow {
	namespace Render {
		struct SceneRendererCamera {
			Camera Camera;
			glm::mat4 ViewMatrix;
			float Near, Far;
			float FOV;
		};

		struct SceneRendererSpecification {
			bool SwapChainTarget = false;
		};

		class SceneRenderer : public RefCounted {
		public:
			SceneRenderer(Ref<Scene> scene, SceneRendererSpecification specification = SceneRendererSpecification());

			void Init();

			void SetScene(Ref<Scene> scene);

			void OnViewportResize(uint32_t width, uint32_t height);

			void BeginScene(const SceneRendererCamera& camera);
			void BeginScene(const Editor::EditorCamera& camera);
			void EndScene();

			void SubmitMesh(Ref<Mesh> mesh, Ref<MaterialTable> materialTable, const glm::mat4& transform = glm::mat4(1.0f), Ref<Material> overrideMaterial = nullptr);//, const Ref<MaterialInstance> overrideMaterial = nullptr);
			void Submit2DQuad(const glm::mat4& transform, const glm::vec4& color);

			static std::pair<Ref<TextureCube>, Ref<TextureCube>> CreateEnvironmentMap(const std::string& filepath);

			Ref<Image2D> GetFinalPassImage();

			Ref<RenderPass> GetCompositeRenderPass() { return m_CompositePipeline->GetSpecification().BindedRenderPass; }

			void OnImGuiRender();
		private:
			void FlushDrawList();

			void ClearPass();
			void GeometryPass();


			void CompositePass();


			Ref<Scene> m_Scene;
			SceneRendererSpecification m_Specification;
			Ref<RenderCommandBuffer> m_CommandBuffer;

			Ref<UniformBufferSet> m_UniformBufferSet;
			Ref<StorageBufferSet> m_StorageBufferSet;

			struct SceneInfo {
				SceneRendererCamera SceneCamera;
			} m_SceneData;

			struct UBCamera {
				glm::mat4 ViewProjection;
				glm::mat4 InvViewProjection;
				glm::mat4 View;
				glm::mat4 Projection;
			} CameraDataUB;

			struct DrawCommand {
				Ref<Mesh> Mesh;
				Ref<MaterialTable> MaterialTable;
				glm::mat4 Transform;
				Ref<Material> OverrideMaterial;
			};
			std::vector<DrawCommand> m_DrawList;


			Ref<Shader> m_CompositeShader;
			Ref<Material> m_CompositeMaterial;

			Ref<Shader> m_GridShader;
			Ref<Material> m_GridMaterial;

			Ref<RenderPass> m_ExternalCompositeRenderPass;

			Ref<Pipeline> m_GridPipeline;

			Ref<Pipeline> m_GeometryPipeline;
			Ref<Pipeline> m_CompositePipeline;

			bool m_ResourcesCreated = false;
		};
	}
}
