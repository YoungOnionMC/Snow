#include "RayCamera.h"

#include "Snow/Core/Input.h"

#include <glm/glm.hpp>

RayCamera::RayCamera(float fov, float nearP, float farP) :
	m_FOV(fov), m_Near(nearP), m_Far(farP) {

	m_ForwardDirection = glm::vec3(0, 0, -1);
	m_Pos = glm::vec3(0, 0, 3);
}

using namespace Snow;

void RayCamera::OnUpdate(Timestep ts) {
	glm::vec2 mouse = Core::Input::GetMousePos();

	glm::vec2 d = (mouse - m_LastMousePos) * 0.002f;
	m_LastMousePos = mouse;

	if (!Snow::Core::Input::IsMouseButtonPressed(MouseCode::ButtonRight)) {
		Core::Input::SetCursorMode(Core::CursorMode::Normal);
		return;
	}

	Core::Input::SetCursorMode(Core::CursorMode::Locked);

	bool moved = false;

	constexpr glm::vec3 upDir(0.0f, 1.0f, 0.0f);
	glm::vec3 rightDir = glm::cross(m_ForwardDirection, upDir);

	float speed = 4.0f;

	if (Core::Input::IsKeyPressed(KeyCode::W)) {
		m_Pos += m_ForwardDirection * (float)ts * speed;
		moved = true;
	}
	else if (Core::Input::IsKeyPressed(KeyCode::S)) {
		m_Pos -= m_ForwardDirection * (float)ts * speed;
		moved = true;
	}

	if (Core::Input::IsKeyPressed(KeyCode::A)) {
		m_Pos -= rightDir * (float)ts * speed;
		moved = true;
	}
	else if (Core::Input::IsKeyPressed(KeyCode::D)) {
		m_Pos += rightDir * (float)ts * speed;
		moved = true;
	}

	if (Core::Input::IsKeyPressed(KeyCode::Q)) {
		m_Pos += upDir * (float)ts * speed;
		moved = true;
	}
	else if (Core::Input::IsKeyPressed(KeyCode::E)) {
		m_Pos -= upDir * (float)ts * speed;
		moved = true;
	}

	if (d.x != 0.0f || d.y != 0.0f) {
		float pitch = d.y * GetRotSpeed();
		float yaw = d.x * GetRotSpeed();

		glm::quat q = glm::normalize(glm::cross(glm::angleAxis(-pitch, rightDir), glm::angleAxis(-yaw, glm::vec3(0.0f, 1.0f, 0.0f))));
		m_ForwardDirection = glm::rotate(q, m_ForwardDirection);

		moved = true;
	}

	if (moved) {
		RecalculateView();
		RecalculateRayDirections();
	}
}

void RayCamera::OnResize(uint32_t width, uint32_t height) {
	if (width == m_ViewportWidth && height == m_ViewportHeight)
		return;

	m_ViewportWidth = width;
	m_ViewportHeight = height;

	RecalculateProjection();
	RecalculateRayDirections();
}

float RayCamera::GetRotSpeed() {
	return 0.3f;
}

void RayCamera::RecalculateProjection() {
	m_Projection = glm::perspectiveFov(glm::radians(m_FOV), (float)m_ViewportWidth, (float)m_ViewportHeight, m_Near, m_Far);
	m_InverseProjection = glm::inverse(m_Projection);
}

void RayCamera::RecalculateView() {
	m_View = glm::lookAt(m_Pos, m_Pos + m_ForwardDirection, glm::vec3(0, 1, 0));
	m_InverseView = glm::inverse(m_View);
}

void RayCamera::RecalculateRayDirections() {
	m_RayDirections.resize(m_ViewportWidth * m_ViewportHeight);

	for (uint32_t y = 0; y < m_ViewportHeight; y++) {
		for (uint32_t x = 0; x < m_ViewportWidth; x++) {
			glm::vec2 coord = { (float)x / (float)m_ViewportWidth, (float)y / (float)m_ViewportHeight };
			coord = coord * 2.0f - 1.0f;

			glm::vec4 target = m_InverseProjection * glm::vec4(coord.x, coord.y, 1, 1);
			glm::vec3 rayDir = glm::vec3(m_InverseView * glm::vec4(glm::normalize(glm::vec3(target) / target.w), 0));
			m_RayDirections[x + y * m_ViewportWidth] = rayDir;
		}
	}
}