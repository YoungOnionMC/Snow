#pragma once

#include "Snow/Scene/Scene.h"

#include "Snow/Render/Mesh.h"

#include "Snow/Scene/SceneCamera.h"

namespace Snow {
	namespace Render {
		struct SceneRendererCamera {
			Camera Camera;
			glm::mat4 ViewMatrix;
		};

		class SceneRenderer {
		public:
			static void Init();

			static void OnViewportResize(uint32_t width, uint32_t height);

			static void BeginScene(const Ref<Scene> scene, const SceneRendererCamera& camera);
			static void BeginScene(const Ref<Scene> scene, const EditorCamera& camera);
			static void EndScene();

			static void SubmitMesh(Ref<Mesh> mesh, const glm::mat4& transform = glm::mat4(1.0f), const Ref<MaterialInstance> overrideMaterial = nullptr);
			static void Submit2DQuad(const glm::mat4& transform, const glm::vec4& color);

			static void* GetFinalColorAttachment();
			static void OnImGuiRender();
		private:
			static void FlushDrawList();

			static void GeometryPass();
			static void CompositePass();


			
		};
	}
}
