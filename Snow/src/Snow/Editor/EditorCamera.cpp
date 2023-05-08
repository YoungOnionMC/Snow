#include <spch.h>

#include "Snow/Editor/EditorCamera.h"
#include "Snow/Core/Input.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define M_PI 3.141592f

namespace Snow {
	namespace Editor {
		EditorCamera::EditorCamera(float fov, float width, float height, float nearC, float farC) :
			m_FOV(fov), m_AspectRatio(width / height), m_NearClip(nearC), m_FarClip(farC) {
			EditorCamera(glm::perspectiveFov(fov, width, height, nearC, farC));
		}

		EditorCamera::EditorCamera(const glm::mat4& projMatrix) :
			Camera(projMatrix) {
			m_FocalPoint = glm::vec3(0.0f);

			glm::vec3 position = { 0, 0, -10 };
			m_Distance = glm::distance(position, m_FocalPoint);

			m_Yaw = 3.0f * (float)M_PI / 4.0f;
			m_Pitch = M_PI / 4.0f;

			m_Position = CalculatePosition();
			const glm::quat orientation = GetOrientation();
			m_WorldRotation = glm::eulerAngles(orientation) * (180.0f / (float)M_PI);
			m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
			m_ViewMatrix = glm::inverse(m_ViewMatrix);
		}

		void EditorCamera::UpdateProjMatrix() {
			m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
			m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
		}

		void EditorCamera::UpdateViewMatrix() {
			const float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;

			const float cosAngle = glm::dot(GetForwardDirection(), GetUpDirection());
			if (cosAngle * yawSign > 0.99f)
				m_PitchDelta = 0.f;

			const glm::vec3 lookAt = m_Position + GetForwardDirection();
			m_WorldRotation = glm::normalize(m_FocalPoint - m_Position);
			m_FocalPoint = m_Position + GetForwardDirection() * m_Distance;
			m_Distance = glm::distance(m_Position, m_FocalPoint);
			m_ViewMatrix = glm::lookAt(m_Position, lookAt, glm::vec3{ 0.f, yawSign, 0.f });

			m_YawDelta *= 0.6f;
			m_PitchDelta *= 0.6f;
			m_PositionDelta *= 0.8f;
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

		static void DisableMouse() {
			Core::Input::SetCursorMode(Core::CursorMode::Locked);
		}

		static void EnableMouse() {
			Core::Input::SetCursorMode(Core::CursorMode::Normal);
		}


		void EditorCamera::OnUpdate(Timestep ts) {
			const glm::vec2& mouse = Core::Input::GetMousePos();
			glm::vec2 delta = (mouse - m_InitialMousePos) * 0.002f;

			if (m_IsActive) {
				if (Core::Input::IsMouseButtonPressed(MouseCode::ButtonRight) && !Core::Input::IsKeyPressed(KeyCode::LeftAlt)) {
					m_CameraMode = CameraMode::FLYCAM;
					DisableMouse();
					const float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;

					if (Core::Input::IsKeyPressed(KeyCode::LeftShift))
						m_Speed = 0.02f;
					else
						m_Speed = 0.002f;

					if (Core::Input::IsKeyPressed(KeyCode::LeftControl))
						m_Speed = 0.0002f;
					else
						m_Speed = 0.002f;

					if(Core::Input::IsKeyPressed(KeyCode::Q))
						m_PositionDelta -= ts.GetMilliseconds() * m_Speed * glm::vec3{ 0.0f, yawSign, 0.0f };
					if (Core::Input::IsKeyPressed(KeyCode::E))
						m_PositionDelta += ts.GetMilliseconds() * m_Speed * glm::vec3{ 0.0f, yawSign, 0.0f };
					if (Core::Input::IsKeyPressed(KeyCode::S))
						m_PositionDelta -= ts.GetMilliseconds() * m_Speed * m_WorldRotation;
					if (Core::Input::IsKeyPressed(KeyCode::W))
						m_PositionDelta += ts.GetMilliseconds() * m_Speed * m_WorldRotation;
					if (Core::Input::IsKeyPressed(KeyCode::A))
						m_PositionDelta -= ts.GetMilliseconds() * m_Speed * m_RightDirection;
					if (Core::Input::IsKeyPressed(KeyCode::D))
						m_PositionDelta += ts.GetMilliseconds() * m_Speed * m_RightDirection;

					constexpr float maxRate{ 0.12f };
					m_YawDelta += glm::clamp(yawSign * delta.x * RotationSpeed(), -maxRate, maxRate);
					m_PitchDelta += glm::clamp(yawSign * delta.y * RotationSpeed(), -maxRate, maxRate);

					m_RightDirection = glm::cross(m_WorldRotation, glm::vec3(0.0f, yawSign, 0.0f));

					m_WorldRotation = glm::rotate(glm::normalize(glm::cross(glm::angleAxis(-m_PitchDelta, m_RightDirection),
						glm::angleAxis(-m_YawDelta, glm::vec3{ 0.0f, yawSign, 0.0f }))), m_WorldRotation);
				}
				else if (Core::Input::IsKeyPressed(KeyCode::LeftAlt)) {
					m_CameraMode = CameraMode::ARCBALL;

					if (Core::Input::IsMouseButtonPressed(MouseCode::ButtonLeft)) {
						DisableMouse();
						MousePan(delta);
					}
					else if (Core::Input::IsMouseButtonPressed(MouseCode::ButtonMiddle)) {
						DisableMouse();
						MouseRotate(delta);
					}
					else if (Core::Input::IsMouseButtonPressed(MouseCode::ButtonRight)) {
						DisableMouse();
						MouseZoom(delta.y);
					}
					else
						EnableMouse();
				}
				else
					EnableMouse();
			}
			m_InitialMousePos = mouse;

			m_Position += m_PositionDelta;
			m_Yaw += m_YawDelta;
			m_Pitch += m_PitchDelta;

			if (m_CameraMode == CameraMode::ARCBALL)
				m_Position = CalculatePosition();

			UpdateViewMatrix();
		}

		void EditorCamera::MousePan(const glm::vec2& delta) {
			auto [xSpeed, ySpeed] = PanSpeed();
			m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
			m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
		}

		void EditorCamera::MouseRotate(const glm::vec2& delta) {
			float yawSign = GetUpDirection().y < 0.0f ? -1.0f : 1.0f;
			m_YawDelta += yawSign * delta.x * RotationSpeed();
			m_PitchDelta += delta.y * RotationSpeed();
		}

		void EditorCamera::MouseZoom(float delta) {
			m_Distance -= delta * ZoomSpeed();
			m_Position = m_FocalPoint - GetForwardDirection() * m_Distance;
			const glm::vec3 forwardDir = GetForwardDirection();
			if (m_Distance < 1.0f) {
				m_FocalPoint += forwardDir;
				m_Distance = 1.0f;
			}
			m_PositionDelta += delta * ZoomSpeed() * forwardDir;
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
			return m_FocalPoint - GetForwardDirection() * m_Distance + m_PositionDelta;
		}

		glm::quat EditorCamera::GetOrientation() const {
			return glm::quat(glm::vec3(-m_Pitch - m_PitchDelta, -m_Yaw - m_YawDelta, 0.0f));
		}
	}
}