#include <spch.h>

#include "Snow/Render/Renderer3D.h"

#include <glm/gtc/type_ptr.hpp>

#include "Snow/Render/RenderCommand.h"

namespace Snow {
	namespace Render {
		void Renderer3D::DrawMesh(Ref<Render::Mesh> mesh, const glm::mat4& transform, Ref<MaterialInstance>& materialInstance) {
			Ref<MaterialInstance> matInstance = mesh->GetMaterialInstance();
			Ref<Material> material = matInstance->GetMaterial();
			
			mesh->GetVertexBuffer()->Bind();
			material->GetShader()->Bind();

			auto tf = transform;

			material->GetShader()->SetUniformBufferData("ObjectTransform", &tf, sizeof(glm::mat4));

			if(materialInstance)
				materialInstance->Bind();
			
			mesh->GetIndexBuffer()->Bind();
			RenderCommand::SetDepthTesting(true);
			RenderCommand::DrawIndexed(mesh->GetIndexBuffer()->GetCount(), PrimitiveType::Triangle);
			RenderCommand::SetDepthTesting(false);
		}
	}
}