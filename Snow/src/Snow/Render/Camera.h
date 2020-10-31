#pragma once

#include "Snow/Math/Math.h"

namespace Snow {
    namespace Render {
        class Camera {
        public:
            Camera() = default;
            Camera(const Math::Matrix4x4f& projection) :
                m_Projection(projection) {}

            virtual ~Camera() = default;

            const Math::Matrix4x4f& GetProjection() const { return m_Projection; }

        protected:
            Math::Matrix4x4f m_Projection = Math::Matrix4x4f(1.0f); 
        };
    }
}