#include <spch.h>
#include "Snow/Script/ScriptEngineRegistry.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

#include "Snow/Scene/Entity.h"
#include "Snow/Scene/Components.h"
#include "Snow/Script/ScriptWrappers.h"

namespace Snow {
	namespace Script {
		std::unordered_map<MonoType*, std::function<bool(Entity&)>> s_HasComponentFunctions;
		std::unordered_map<MonoType*, std::function<void(Entity&)>> s_CreateComponentFunctions;

		extern MonoImage* s_CoreAssemblyImage;

#define Component_RegisterType(Type)		\
	{										\
		MonoType* type = mono_reflection_type_from_name("Snow." #Type, s_CoreAssemblyImage);	\
		if(type) {				\
			uint32_t id = mono_type_get_type(type);\
			s_HasComponentFunctions[type] = [](Entity& entity) {return entity.HasComponent<Type>(); };	\
			s_CreateComponentFunctions[type] = [](Entity& entity) { entity.AddComponent<Type>(); };		\
		}			\
		else {		\
			SNOW_CORE_ERROR("No C# component class found for " #Type "!"); \
		}	\
	}

		static void InitComponentTypes() {
			Component_RegisterType(TagComponent);
			Component_RegisterType(TransformComponent);
			Component_RegisterType(SpriteRendererComponent);
			Component_RegisterType(CameraComponent);
			Component_RegisterType(MeshComponent);
			Component_RegisterType(ScriptComponent);
			Component_RegisterType(RigidBody2DComponent);
		}

		void ScriptEngineRegistry::RegisterAll() {
			InitComponentTypes();

			mono_add_internal_call("Snow.Entity::GetTransform_Native", Script::Snow_Entity_GetTransform);
			mono_add_internal_call("Snow.Entity::SetTransform_Native", Script::Snow_Entity_SetTransform);
			mono_add_internal_call("Snow.Entity::CreateComponent_Native", Script::Snow_Entity_CreateComponent);
			mono_add_internal_call("Snow.Entity::HasComponent_Native", Script::Snow_Entity_HasComponent);
			mono_add_internal_call("Snow.Entity::FindEntityByTag_Native", Script::Snow_Entity_FindEntityByTag);

			mono_add_internal_call("Snow.Input::IsKeyPressed_Native", Script::Snow_Input_IsKeyPressed);
			mono_add_internal_call("Snow.Input::IsMouseButtonPressed_Native", Script::Snow_Input_IsMouseButtonPressed);
			mono_add_internal_call("Snow.Input::GetMousePosition_Native", Script::Snow_Input_GetMousePosition);

		}
	}
}