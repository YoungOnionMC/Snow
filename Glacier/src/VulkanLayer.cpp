#include "VulkanLayer.h"

void VulkanLayer::OnAttach() {

	Snow::Render::FramebufferSpecification fbSpec;
	fbSpec.Width = 1080;
	fbSpec.Height = 720;
	fbSpec.AttachmentList = { Snow::Render::ImageFormat::RGBA };
	fbSpec.ClearColor = { 1.0f, 0.3f, 1.0f, 1.0f };
	fbSpec.SwapChainTarget = true;
	Snow::Ref<Snow::Render::Framebuffer> fb = Snow::Render::Framebuffer::Create(fbSpec);

	Snow::Render::RenderPassSpecification renderPassSpec;
	renderPassSpec.TargetFramebuffer = fb;

	m_RenderPass = Snow::Render::RenderPass::Create(renderPassSpec);
}

void VulkanLayer::OnDetach() {
}

void VulkanLayer::OnUpdate(Snow::Timestep ts) {
	//Snow::Render::Renderer::BeginRenderPass(m_RenderPass);

	//Snow::Render::Renderer::EndRenderPass();
}

void VulkanLayer::OnEvent(Snow::Core::Event::Event& e) {
}
