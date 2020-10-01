#pragma once

#include <string>
#include <vector>

namespace Snow {
    namespace Render {
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