#pragma once

#include "Snow/Script/ScriptEngine.h"
#include "Snow/Core/InputCodes.h"

#include <glm/glm.hpp>

extern "C" {
	typedef struct _MonoString MonoString;
	typedef struct _MonoArray MonoArray;
}

namespace Snow {
	namespace Script {
		void Snow_Entity_GetTransform(uint64_t entityID, glm::mat4* transform);
		void Snow_Entity_SetTransform(uint64_t entityID, glm::mat4* transform);
		void Snow_Entity_CreateComponent(uint64_t entityID, void* type);
		bool Snow_Entity_HasComponent(uint64_t entityID, void* type);
		uint64_t Snow_Entity_FindEntityByTag(MonoString* tag);

		bool Snow_Input_IsKeyPressed(KeyCode keycode);
		bool Snow_Input_IsMouseButtonPressed(MouseCode mouseCode);
		void Snow_Input_GetMousePosition(glm::vec2* mousePos);
	}
}