#include <spch.h>

#include "Snow/Render/Renderer3D.h"

#include <glm/gtc/type_ptr.hpp>

#include "Snow/Render/RenderCommand.h"

namespace Snow {
	namespace Render {
		void Renderer3D::DrawMesh(Ref<Render::Mesh> mesh, const glm::mat4& transform) {
			Ref<MaterialInstance> matInstance = mesh->GetMaterialInstance();
			Ref<Material> material = matInstance->GetMaterial();
			
			mesh->GetVertexBuffer()->Bind();
			material->GetPipeline()->Bind();

			auto tf = transform;

			material->GetPipeline()->SetUniformBufferData(1, &tf, sizeof(glm::mat4));
			mesh->GetIndexBuffer()->Bind();

			RenderCommand::DrawIndexed(mesh->GetIndexBuffer()->GetCount(), material->GetPipeline()->GetSpecification().Type);
		}
	}
}