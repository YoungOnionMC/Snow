#pragma once

#include "Snow/Core/Ref.h"
#include <string>
#include <initializer_list>
//#include "Snow/Render/ShaderUniform.h"

namespace Snow {
    namespace Render {
        class ShaderUniformBuffer;
        class ShaderBuffer;
        class ShaderResource;

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

            //virtual void Bind() const = 0;
            virtual void Reload(bool forceCompile = false) = 0;

            virtual size_t GetHash() const = 0;

            virtual const uint32_t GetModuleCount() const = 0;

            //virtual const ShaderUniformBuffer& GetUniformBuffer(const std::string& name) const = 0;

            virtual const std::unordered_map<std::string, ShaderBuffer>& GetShaderBuffers() const = 0;
            virtual const std::unordered_map<std::string, ShaderResource>& GetResources() const = 0;

            virtual std::vector<ShaderType> GetModuleTypes() = 0;

            //virtual void SetUniformBufferData(const std::string& uniformBufferName, void* data = nullptr, uint32_t size = 0) = 0;
            //virtual void SetUniformBufferData(uint32_t bindingPoint, void* data = nullptr, uint32_t size = 0) = 0;

            //virtual void SetUniform(const std::string& name, int value) = 0;
            //virtual void SetUniform(const std::string& name, float value) = 0;

            static Ref<Shader> Create(const std::initializer_list<ShaderModule>& shaderModules);// ShaderType type, const std::string& path);

            static ShaderType ShaderTypeFromString(const std::string& type);
            static std::string ShaderTypeToString(ShaderType type);
        };
        
    }
}