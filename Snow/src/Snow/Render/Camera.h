#pragma once

#include <glm/glm.hpp>

namespace Snow {
    namespace Render {
        class Camera {
        public:
            Camera() = default;
            Camera(const glm::mat4& projection) :
                m_Projection(projection) {}

            virtual ~Camera() = default;

            const glm::mat4& GetProjection() const { return m_Projection; }
            void SetProjectionMatrix(const glm::mat4& projMatrix) { m_Projection = projMatrix; }

        protected:
            glm::mat4 m_Projection = glm::mat4(1.0f); 
        };
    }
}