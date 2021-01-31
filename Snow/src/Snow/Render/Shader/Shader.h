#pragma once

#include "Snow/Core/Ref.h"
#include <string>
//#include "Snow/Render/Shader/ShaderUniform.h"

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
            virtual const std::string& GetName() const = 0;

            virtual void Reload() = 0;

            static Ref<Shader> Create(ShaderType type, const std::string& path);

            
            
        };
        
    }
}