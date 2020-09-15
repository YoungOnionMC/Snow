#include "Snow/Render/RenderContext.h"

#include <vulkan/vulkan.h>

#include <vector>


namespace Snow {
    namespace Render {
        class VulkanContext : public Context {
        public:
            VulkanContext(const ContextSpecification& spec);
            ~VulkanContext();

            virtual const ContextSpecification& GetSpecification() const override { return m_Specification; }
        private:
            void CreateInstance();



            VkInstance m_VulkanInstance;

            VkResult m_Result;

            std::vector<const char*> m_InstanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };
            std::vector<VkExtensionProperties> m_Extensions;


            ContextSpecification m_Specification;

        };
    }
}