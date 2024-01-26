#pragma once

#include "Snow/Render/Camera.h"

#include "Snow/Core/Timestep.h"

#include <glm/glm.hpp>

#include <tuple>

namespace Snow {
	namespace Editor {
		enum class CameraMode {
			NONE, FLYCAM, ARCBALL
		};

		class EditorCamera : public Render::Camera {
		public:
			EditorCamera() = default;
			EditorCamera(float fov, float width, float height, float nearC, float farC);
			EditorCamera(const glm::mat4& projMatrix);

			void OnUpdate(Timestep ts);
			//void OnEvent(Core::Event::Event& e);

			bool IsActive() const { return m_IsActive; }
			void SetActive(bool active) { m_IsActive = active; }

			inline float GetDistance() const { return m_Distance; }
			inline void SetDistance(float distance) { m_Distance = distance; }

			inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjMatrix(); }

			const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
			glm::mat4 GetViewProjectionMatrix() const { return m_Projection * m_ViewMatrix; }

			glm::vec3 GetUpDirection() const;
			glm::vec3 GetRightDirection() const;
			glm::vec3 GetForwardDirection() const;
			const glm::vec3& GetPosition() const { return m_Position; }
			glm::quat GetOrientation() const;

			float GetPitch() const { return m_Pitch; }
			float GetYaw() const { return m_Yaw; }

		private:
			void UpdateProjMatrix();
			void UpdateViewMatrix();

			void MousePan(const glm::vec2& delta);
			void MouseRotate(const glm::vec2& delta);
			void MouseZoom(float delta);

			glm::vec3 CalculatePosition() const;

			std::pair<float, float> PanSpeed() const;
			float RotationSpeed() const;
			float ZoomSpeed() const;

			float m_FOV = 45.0f, m_AspectRatio = (16.0f/9.0f), m_NearClip = 0.1f, m_FarClip = 1000.0f;

			glm::mat4 m_ViewMatrix;
			glm::vec3 m_Position;
			glm::vec3 m_WorldRotation;
			glm::vec3 m_FocalPoint;

			bool m_IsActive = true;

			glm::vec2 m_InitialMousePos = { 0.0f, 0.0f };

			float m_Distance = 10.0f;
			float m_Speed{ 0.002f };
			float m_LastSpeed = 0.f;

			float m_Pitch = 0.0f, m_Yaw = 0.0f;
			float m_PitchDelta{}, m_YawDelta{};
			glm::vec3 m_PositionDelta{};
			glm::vec3 m_RightDirection{};

			CameraMode m_CameraMode{ CameraMode::ARCBALL };

			float m_MinFocusDistance = 100.f;

			float m_ViewportWidth = 1280.0f, m_ViewportHeight = 720.0f;
		};
	}
}
