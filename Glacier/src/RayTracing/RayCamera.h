#pragma once

#include <Snow.h>

class RayCamera : public Snow::Render::Camera {
public:
	RayCamera(float fov, float nearP, float farP);

	void OnUpdate(Snow::Timestep ts);

	void OnResize(uint32_t width, uint32_t height);

	const glm::mat4& GetInverseProjection() const { return m_InverseProjection; }
	const glm::mat4& GetView() const { return m_View; }
	const glm::mat4& GetInverseView() const { return m_InverseView; }

	const glm::vec3& GetPosition() const { return m_Pos; }
	const glm::vec3& GetDirection() const { return m_ForwardDirection; }

	const std::vector<glm::vec3>& getRayDirections() const { return m_RayDirections; }

	float GetRotSpeed();

private:
	void RecalculateProjection();
	void RecalculateView();
	void RecalculateRayDirections();

	glm::mat4 m_View;
	glm::mat4 m_InverseView;
	glm::mat4 m_InverseProjection;

	float m_FOV;
	float m_Near, m_Far;

	glm::vec3 m_Pos{ 0.0f, 0.0f, 0.0f };
	glm::vec3 m_ForwardDirection{ 0.0f, 0.0f, 0.0f };

	std::vector<glm::vec3> m_RayDirections;

	glm::vec2 m_LastMousePos{ 0.0f, 0.0f };

	uint32_t m_ViewportWidth, m_ViewportHeight;
};