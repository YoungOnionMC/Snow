#pragma once

#include <string>
#include <vector>

#include "Snow/Render/Shader.h"

namespace Snow {
    namespace Render {

        enum class UniformType {
            None = -1,
            Bool,
            Int, Int2, Int3, Int4,
            UInt, UInt2, UInt3, UInt4,
            Float, Float2, Float3, Float4,
            Double,
            Mat3x3, Mat4x4,
            Struct
        };

        class ShaderUniform {
            friend class ShaderStruct;
        public:
            ShaderUniform() = default;
            ShaderUniform(const std::string& name, UniformType type, uint32_t size, uint32_t offset, uint32_t count = 1) :
                Name(name), Type(type), Struct(nullptr), Size(size), Offset(offset), Count(count) {}
            ShaderUniform(const std::string& name, ShaderStruct* sStruct, uint32_t count);

            const std::string& GetName() const { return Name; }
            UniformType GetType() const { return Type; }
            uint32_t GetSize() const { return Size; }
            uint32_t GetOffset() const { return Offset; }
            uint32_t GetAbsoluteOffset() const;

            const ShaderStruct& GetStruct() { return *Struct; }

        private:
            void SetOffset(uint32_t offset);

            std::string Name;
            UniformType Type = UniformType::None;
            uint32_t Size;
            uint32_t Offset;

            ShaderStruct* Struct;
            uint32_t Count = 0;
        };

        class ShaderUniformBuffer {
        public:
            std::string Name;
            uint32_t Index;
            uint32_t BindingPoint;
            ShaderType Stage = ShaderType::None;
            uint32_t Size;
            uint32_t RendererID;
            std::vector<ShaderUniform> Uniforms;
        };

        class ShaderStruct {
        public:
            ShaderStruct(const std::string& name) :
                Name(name), Size(0), Offset(0) {}

            void AddUniform(ShaderUniform uniform) {
                Size += uniform.GetSize();
                uint32_t offset = 0;
                if (Uniforms.size()) {
                    ShaderUniform& prevUniform = Uniforms.back();
                    offset = prevUniform.GetOffset() + prevUniform.GetSize();
                }
                uniform.SetOffset(offset);
                Uniforms.push_back(uniform);
            }

            const std::string& GetName() const { return Name; }
            uint32_t GetSize() const { return Size; }
            uint32_t GetOffset() const { return Offset; }
            const std::vector<ShaderUniform> GetUniforms() const { return Uniforms; }

            void SetOffset(uint32_t offset) { Offset = offset; }

        private:
            std::string Name;
            uint32_t Size = 0;
            uint32_t Offset = 0;
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
            std::string Name;
            uint32_t Register = 0;
            uint32_t Set = 0;
            uint32_t Count = 0;
            Render::ShaderType Stage = Render::ShaderType::None;
            ResourceType Type = ResourceType::None;

            ShaderResource() {}
            ShaderResource(const std::string& name, Render::ShaderType stage, ResourceType type, uint32_t resourceRegister, uint32_t set = 0, uint32_t count = 1) :
                Name(name), Register(resourceRegister), Set(set), Count(count), Stage(stage), Type(type) {}

            const std::string& GetName() const { return Name; }
            uint32_t GetRegister() const { return Register; }
            uint32_t GetSet() const { return Set; }
            Render::ShaderType GetStage() const { return Stage; }
            ResourceType GetType() const { return Type; }
            uint32_t GetCount() const { return Count; }
        private:
            
        };

        typedef std::vector<ShaderResource*> ShaderResourceList;
    }
}