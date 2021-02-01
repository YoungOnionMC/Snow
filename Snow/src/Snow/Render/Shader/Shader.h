#pragma once

#include "Snow/Core/Ref.h"
#include <string>
#include <initializer_list>
//#include "Snow/Render/Shader/ShaderUniform.h"

namespace Snow {
    namespace Render {
        struct ShaderUniformBuffer;
        struct ShaderResource;

        enum class ShaderType {
            None = 0,
            Vertex, Pixel, Geometry, Compute, Tessalation, 
        };

        using ShaderModule = std::pair<ShaderType, std::string>;

        class Shader : public RefCounted {
        public:
            //virtual const ShaderType GetType() const = 0;
            virtual const std::string& GetPath() const = 0;
            virtual const std::string& GetName() const = 0;

            virtual void Bind() const = 0;
            virtual void Reload() = 0;

            virtual const ShaderUniformBuffer& GetUniformBuffer(const std::string& name) const = 0;
            virtual const std::unordered_map<std::string, ShaderResource>& GetResources() const = 0;

            virtual void SetUniformBufferData(const std::string& uniformBufferName, void* data = nullptr, uint32_t size = 0) = 0;
            virtual void SetUniformBufferData(uint32_t bindingPoint, void* data = nullptr, uint32_t size = 0) = 0;

            static Ref<Shader> Create(const std::initializer_list<ShaderModule>& shaderModules);// ShaderType type, const std::string& path);

            
            
        };
        
    }
}