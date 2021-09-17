#pragma once

#include <Snow.h>


class VulkanLayer : public Snow::Core::Layer {
public:
    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(Snow::Timestep ts) override;

    void OnEvent(Snow::Core::Event::Event& e) override;

private:
    Snow::Ref<Snow::Render::RenderPass> m_RenderPass;
};