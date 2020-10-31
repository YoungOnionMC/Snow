#pragma once

#include <Snow.h>
#include "Panels/SceneHierarchyPanel.h"

class EditorLayer : public Snow::Core::Layer {
public:
    void OnAttach();
    void OnDetach();
    void OnUpdate();

    void OnImGuiRender();


private:

    Snow::Ref<Snow::Scene> m_ActiveScene;

    Snow::SceneHierarchyPanel m_SceneHierarchyPanel;

};