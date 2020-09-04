#include "Snow/Render/RenderContext.h"

//#include <vulkan.h>

namespace Snow {
    namespace Render {
        class VulkanContext : public Context {
        public:
            VulkanContext(const ContextSpecification& spec);

            virtual const ContextSpecification& GetSpecification() const override { return m_Specification; }
        private:
            //void CreateInstance();



           // VkInstance m_VulkanInstance;

            //VkResult m_Result;


            ContextSpecification m_Specification;

            uint32_t m_ExtensionCount = 0;

        };
    }
}