#include <spch.h>
#include "Snow/Scene/SceneCamera.h"

namespace Snow {
    SceneCamera::SceneCamera() {
        RecalculateProjection();
    }

    void SceneCamera::SetPerspective(float fov, float near, float far) {
        m_ProjectionType = ProjectionType::Perspective;
        m_PerspectiveFOV = fov;
        m_PerspectiveNear = near;
        m_PerspectiveFar = far;
        RecalculateProjection();
    }

    void SceneCamera::SetViewportSize(uint32_t width, uint32_t height) {
        m_AspectRatio = (float)width / (float)height;
        RecalculateProjection();
    }

    void SceneCamera::RecalculateProjection() {
        if(m_ProjectionType == ProjectionType::Perspective) {
            m_Projection = Math::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
        }
        else if(m_ProjectionType == ProjectionType::Orthographic) {
            float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
            float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
            float orthoBottom = -m_OrthographicSize * 0.5f;
            float orthoTop = m_OrthographicSize * 0.5f;

            m_Projection = Math::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
        }
    }
}