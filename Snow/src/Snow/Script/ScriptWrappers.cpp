#include <spch.h>
#include "Snow/Script/ScriptWrappers.h"

#include "Snow/Scene/Entity.h"
#include "Snow/Core/Input.h"

#include <mono/jit/jit.h>

#include <glm/gtc/type_ptr.hpp>


namespace Snow {
	namespace Script {

		extern std::unordered_map<MonoType*, std::function<bool(Entity&)>> s_HasComponentFunctions;
		extern std::unordered_map<MonoType*, std::function<void(Entity&)>> s_CreateComponentFunctions;

		enum class ComponentID {
			None = 0,
			Transform = 1,
			Mesh = 2,
			Script = 3,
			spriteRenderer = 4,
		};

		void Snow_Entity_GetTransform(uint64_t entityID, glm::mat4* outTransform) {
			Ref<Scene> scene = ScriptEngine::GetSceneContext();
			SNOW_CORE_ASSERT(scene, "No active Scene");
			const auto& entityMap = scene->GetEntityMap();
			SNOW_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene");

			Entity entity = entityMap.at(entityID);
			auto& transformComponent = entity.GetComponent<TransformComponent>();
			memcpy(outTransform, glm::value_ptr(transformComponent.GetTransform()), sizeof(glm::mat4));
		}

		void Snow_Entity_SetTransform(uint64_t entityID, glm::mat4* inTransform) {
			Ref<Scene> scene = ScriptEngine::GetSceneContext();
			SNOW_CORE_ASSERT(scene, "No active Scene");
			const auto& entityMap = scene->GetEntityMap();
			SNOW_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene");

			Entity entity = entityMap.at(entityID);
			auto& transformComp = entity.GetComponent<TransformComponent>();
			transformComp.SetTransform(*inTransform);
		}

		void Snow_Entity_CreateComponent(uint64_t entityID, void* type) {
			Ref<Scene> scene = ScriptEngine::GetSceneContext();
			SNOW_CORE_ASSERT(scene, "No active Scene");
			const auto& entityMap = scene->GetEntityMap();
			SNOW_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene");

			Entity entity = entityMap.at(entityID);
			MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
			s_CreateComponentFunctions[monoType](entity);
		}

		bool Snow_Entity_HasComponent(uint64_t entityID, void* type) {
			Ref<Scene> scene = ScriptEngine::GetSceneContext();
			SNOW_CORE_ASSERT(scene, "No active Scene");
			const auto& entityMap = scene->GetEntityMap();
			SNOW_CORE_ASSERT(entityMap.find(entityID) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene");

			Entity entity = entityMap.at(entityID);
			MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
			bool result = s_HasComponentFunctions[monoType](entity);
			return result;
		}

		uint64_t Snow_Entity_FindEntityByTag(MonoString* tag) {
			Ref<Scene> scene = ScriptEngine::GetSceneContext();
			SNOW_CORE_ASSERT(scene, "No active Scene");

			Entity entity = scene->FindEntityByTag(mono_string_to_utf8(tag));
			if (entity)
				return entity.GetComponent<IDComponent>().ID;

			return 0;
		}

		bool Snow_Input_IsKeyPressed(KeyCode keycode) {
			return Core::Input::IsKeyPressed(keycode);
		}

		bool Snow_Input_IsMouseButtonPressed(MouseCode mousecode) {
			return Core::Input::IsMouseButtonPressed(mousecode);
		}

		void Snow_Input_GetMousePosition(glm::vec2* mousePosition) {
			memcpy(mousePosition, glm::value_ptr(Core::Input::GetMousePos()), sizeof(glm::vec2));
		}
	}
}