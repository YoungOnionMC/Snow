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

		struct SceneRendererOptions {
			bool ShowGrid = false;
			bool ShowBoundingBoxes = false;
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

			SceneRendererOptions& GetOptions();

			static std::pair<Ref<TextureCube>, Ref<TextureCube>> CreateEnvironmentMap(const std::string& filepath);

			Ref<Image2D> GetFinalPassImage();

			Ref<RenderPass> GetCompositeRenderPass() { return m_ExternalCompositeRenderPass; }

			void OnImGuiRender();
		private:
			void FlushDrawList();

			void ClearPass();
			void PreDepthPass();
			void ShadowMapPass();
			void GeometryPass();


			void CompositePass();

			void ClearPass(Ref<RenderPass> renderPass, bool clear = false);

			struct CascadeData {
				glm::mat4 ViewProj;
				glm::mat4 View;
				float SplitDepth;
			};
			void CalculateCascades(CascadeData* cascades, const SceneRendererCamera& camera, const glm::vec3& lightDirection) const;

			Ref<Scene> m_Scene;
			SceneRendererSpecification m_Specification;

			SceneRendererOptions m_Options;

			Ref<RenderCommandBuffer> m_CommandBuffer;

			Ref<UniformBufferSet> m_UniformBufferSet;
			Ref<StorageBufferSet> m_StorageBufferSet;

			uint32_t m_ViewportWidth, m_ViewportHeight;
			bool m_NeedsResize = false;

			struct SceneInfo {
				SceneRendererCamera SceneCamera;

				Ref<Environment> SceneEnvironment;
				float SkyboxLod = 0.0f;
				float SceneEnvironmentIntensity;
				LightEnvironment SceneLightEnvironment;
				DirectionalLight ActiveLight;

			} m_SceneData;

			struct UBCamera {
				glm::mat4 ViewProjection;
				glm::mat4 InvViewProjection;
				glm::mat4 View;
				glm::mat4 Projection;
			} CameraDataUB;

			struct UBRenderer {
				glm::vec4 CascadeSplits;
				uint32_t tilesCountX{ 0 };
				bool ShowCascades = false;
				char Padding[3] = { 0,0,0 };
				bool SoftShadows = true;
				char Padding1[3] = { 0,0,0 };
				float LightSize = 0.5f;
				float MaxShadowDistance = 200.0f;
				float ShadowFade = 1.0f;
				bool CascadeFading = true;
				char Padding2[3] = { 0,0,0 };
				float CascadeTransitionFade = 1.0f;
				bool ShowLightComplexity = false;
				char Padding3[3] = { 0,0,0 };

			} RendererDataUB;

			struct UBShadow {
				glm::mat4 ViewProjection[4];
			} ShadowDataUB;

			struct DirLight {
				glm::vec3 Direction = {0.0f, 0.0f, 0.0f};
				float Padding = 0.0f;
				glm::vec3 Radiance = {1.0f, 1.0f, 1.0f};
				float Multiplier = 1.0f;
			};

			struct UBPointLights {
				uint32_t Count{ 0 };
				glm::vec3 Padding{};
				PointLight PointLights[1024]{};
			} PointLightsUB;

			struct UBScene {
				DirLight Light;
				glm::vec3 u_CameraPosition;
				float EnvironmentMapIntensity = 1.0f;
			} SceneDataUB;

			struct DrawCommand {
				Ref<Mesh> Mesh;
				Ref<MaterialTable> MaterialTable;
				glm::mat4 Transform;
				Ref<Material> OverrideMaterial;
			};

			std::vector<DrawCommand> m_DrawList;
			std::vector<DrawCommand> m_ShadowDrawList;

			


			Ref<Material> m_CompositeMaterial;

			Ref<Material> m_GridMaterial;
			Ref<Pipeline> m_GridPipeline;

			Ref<Material> m_SkyboxMaterial;
			Ref<Pipeline> m_SkyboxPipeline;

			Ref<Material> m_PreDepthMaterial;
			Ref<Pipeline> m_PreDepthPipeline;

			Ref<RenderPass> m_ExternalCompositeRenderPass;


			Ref<Material> m_ShadowPassMaterial;
			std::vector<Ref<Pipeline>> m_ShadowPassPipelines;

			Ref<Pipeline> m_GeometryPipeline;
			Ref<Pipeline> m_CompositePipeline;

			glm::vec3 m_LightCullingWorkGroups;

			float LightDistance = 0.1f;
			float CascadeSplitLambda = 0.92f;
			glm::vec4 CascadeSplits;
			float CascadeFarPlaneOffset = 50.0f, CascadeNearPlaneOffset = -50.0f;

			bool m_ResourcesCreated = false;
		};
	}
}
