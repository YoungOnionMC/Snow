#pragma once

#include <string>
#include <vector>

namespace Snow {
    namespace Render {

        enum class UniformType {
            None = -1,
            Bool,
            Int, Int2, Int3, Int4,
            Float, Float2, Float3, Float4,
            Mat3x3, Mat4x4
        };

        class ShaderUniform {
        public:
            ShaderUniform() = default;
            ShaderUniform(const std::string& name, UniformType type, uint32_t size, uint32_t offset) :
                Name(name), Type(type), Size(size), Offset(offset) {}

            const std::string& GetName() const { return Name; }
            UniformType GetType() const { return Type; }
            uint32_t GetSize() const { return Size; }
            uint32_t GetOffset() const { return Offset; }

        private:
            std::string Name;
            UniformType Type = UniformType::None;
            uint32_t Size;
            uint32_t Offset;
        };

        class ShaderUniformBuffer {
        public:
            std::string Name;
            uint32_t Index;
            uint32_t BindingPoint;
            uint32_t Size;
            uint32_t RendererID;
            std::vector<ShaderUniform> Uniforms;
        };

        class ShaderBuffer {
        public:
            std::string Name;
            uint32_t Size = 0;
            std::unordered_map<std::string, ShaderUniform> Uniforms;
        };

        enum class ResourceType {
            None = -1,
            Sampler, Texture2D, TextureCube
        };

        class ShaderResource {
        public:
            ShaderResource() {}
            ShaderResource(const std::string& name, uint32_t resourceRegister, uint32_t count) :
                m_Name(name), m_Register(resourceRegister), m_Count(count) {}

            const std::string& GetName() const { return m_Name; }
            uint32_t GetRegister() const { return m_Register; }
            uint32_t GetCount() const { return m_Count; }
        private:
            std::string m_Name;
            uint32_t m_Register = 0;
            uint32_t m_Count = 0;
        };

        typedef std::vector<ShaderResource*> ShaderResourceList; 
    }
}