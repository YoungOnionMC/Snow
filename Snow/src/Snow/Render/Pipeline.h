#pragma once

#include "Snow/Core/Ref.h"

#include "Snow/Render/Shader/Shader.h"
#include "Snow/Render/RenderPass.h"

#include <string>
#include <vector>

namespace Snow {
    namespace Render {

        enum class AttribType {
            None = 0,
            Float, Float2, Float3, Float4,
            Mat3x3, Mat4x4,
            Int, Int2, Int3, Int4, 
        };

        struct AttribElement {
            std::string Name = "";
            AttribType Type = AttribType::None;
            uint32_t Size = 0;
            uint32_t Offset = 0;
            uint32_t Index = 0;
            bool Normalized = false;

            

            AttribElement() = default;
            AttribElement(AttribType type, const std::string& name, uint32_t index = 0, bool normalized = false) :
                Name(name), Type(type), Size(GetTypeSize(type)), Offset(0), Normalized(normalized), Index(index) {}

            static uint32_t GetTypeSize(AttribType type) {
                switch(type) {
                case AttribType::Float: return sizeof(float);
                case AttribType::Float2: return sizeof(float) * 2;
                case AttribType::Float3: return sizeof(float) * 3;
                case AttribType::Float4: return sizeof(float) * 4;
                case AttribType::Mat3x3: return sizeof(float) * 3 * 3;
                case AttribType::Mat4x4: return sizeof(float) * 4 * 4;
                case AttribType::Int: return sizeof(int);
                case AttribType::Int2: return sizeof(int) * 2;
                case AttribType::Int3: return sizeof(int) * 3;
                case AttribType::Int4: return sizeof(int) * 4;
                }

                return 0;
            }

            uint32_t GetComponentCount() const {
                switch (Type) {
				case AttribType::Float:   return 1;
				case AttribType::Float2:  return 2;
				case AttribType::Float3:  return 3;
				case AttribType::Float4:  return 4;
				case AttribType::Mat3x3:    return 3 * 3;
				case AttribType::Mat4x4:    return 4 * 4;
				case AttribType::Int:     return 1;
				case AttribType::Int2:    return 2;
				case AttribType::Int3:    return 3;
				case AttribType::Int4:    return 4;
				}
                return 0;
            }
        };

        struct VertexBufferLayout {
        public:
            VertexBufferLayout() {}

            VertexBufferLayout(const std::initializer_list<AttribElement>& elements) :
                m_Elements(elements) {
                CalculateStride();
            }

            inline uint32_t GetStride() const { return m_Stride; }
            inline const std::vector<AttribElement>& GetElements() const { return m_Elements; }

            std::vector<AttribElement>::iterator begin() { return m_Elements.begin(); }
			std::vector<AttribElement>::iterator end() { return m_Elements.end(); }
			std::vector<AttribElement>::const_iterator begin() const { return m_Elements.begin(); }
			std::vector<AttribElement>::const_iterator end() const { return m_Elements.end(); }

        private:
            void CalculateStride() {
                uint32_t offset = 0;
                m_Stride = 0;
                for(auto& element : m_Elements) {
                    element.Offset = offset;
                    offset += element.Size;
                    m_Stride +=element.Size;
                }
            }

            std::vector<AttribElement> m_Elements;
            uint32_t m_Stride = 0;
        };

        struct PipelineSpecification {
            std::vector<Ref<Shader>> Shaders;
            VertexBufferLayout Layout;
            Ref<RenderPass> RenderPass;
        };

        class Pipeline : public RefCounted {
        public:
            ~Pipeline() = default;

            virtual void Bind() const = 0; 

            virtual const PipelineSpecification& GetSpecification() const = 0;

            static Ref<Pipeline> Create(const PipelineSpecification& spec);
        private:

        };
    }
}