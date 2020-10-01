#pragma once

#include "Snow/Core/Ref.h"
#include <string>

namespace Snow {
    namespace Render {
        enum class ShaderType {
            None = 0,
            Vertex, Pixel, Geometry, Compute, Tessalation, 
        };

        class Shader : public RefCounted {
        public:
            virtual const ShaderType GetType() const = 0;
            virtual const std::string& GetPath() const = 0;

            static Ref<Shader> Create(ShaderType type, const std::string& path);

            
            
        };
        
    }
}