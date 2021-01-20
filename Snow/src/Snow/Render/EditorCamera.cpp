#include <spch.h>

#include "Snow/Render/EditorCamera.h"
#include "Snow/Core/Input.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace Snow {
	namespace Render {
		EditorCamera::EditorCamera(float fov, float aspectRatio, float nearC, float farC) :
			m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearC), m_FarClip(farC), Camera(glm::perspective(glm::radians(fov), aspectRatio, nearC, farC)) {
			UpdateViewMatrix();
		}

		void EditorCamera::UpdateProjMatrix() {
			m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
			m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
		}

		void EditorCamera::UpdateViewMatrix() {
			m_Position = CalculatePosition();

			glm::quat orientation = GetOrientation();
			m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
			m_ViewMatrix = glm::inverse(m_ViewMatrix);
		}

		std::pair<float, float> EditorCamera::PanSpeed() const {
			float x = std::min(m_ViewportWidth / 1000.0f, 2.4f);
			float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

			float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
			float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

			return { xFactor, yFactor };
		}

		float EditorCamera::RotationSpeed() const {
			return 0.8f;
		}

		float EditorCamera::ZoomSpeed() const {
			float distance = m_Distance * 0.2f;
			distance = std::max(distance, 0.0f);
			float speed = distance * distance;
			speed = std::min(speed, 100.0f);
			return speed;
		}

		static int test = 0;

		void EditorCamera::OnUpdate(Timestep ts) {
			if (Core::Input::IsKeyPressed(KeyCode::LeftControl)) {
				const glm::vec2& mouse = Core::Input::GetMousePos();
				glm::vec2 delta = (mouse - m_InitialMousePos) * 0.003f;
				m_InitialMousePos = mouse;

				if (Core::Input::IsMouseButtonPressed(MouseCode::ButtonLeft))
					MousePan(delta);
				if (Core::Input::IsMouseButtonPressed(MouseCode::ButtonMiddle))
					MouseRotate(delta);
				if (Core::Input::IsMouseButtonPressed(MouseCode::ButtonRight))
					MouseZoom(delta.y);
			}

			UpdateViewMatrix();
		}

		void EditorCamera::MousePan(const glm::vec2& delta) {
			auto [xSpeed, ySpeed] = PanSpeed();
			m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
			m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
		}

		void EditorCamera::MouseRotate(const glm::vec2& delta) {
			float yawSign = GetUpDirection().y < 0.0f ? -1.0f : 1.0f;
			m_Yaw += yawSign * delta.x * RotationSpeed();
			m_Pitch += delta.y * RotationSpeed();
		}

		void EditorCamera::MouseZoom(float delta) {
			m_Distance -= delta * ZoomSpeed();
			if (m_Distance < 1.0f) {
				m_FocalPoint += GetForwardDirection();
				m_Distance = 1.0f;
			}
		}

		glm::vec3 EditorCamera::GetUpDirection() const {
			return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
		}

		glm::vec3 EditorCamera::GetRightDirection() const {
			return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
		}

		glm::vec3 EditorCamera::GetForwardDirection() const {
			return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
		}

		glm::vec3 EditorCamera::CalculatePosition() const {
			return m_FocalPoint - GetForwardDirection() * m_Distance;
		}

		glm::quat EditorCamera::GetOrientation() const {
			return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
		}
	}
}