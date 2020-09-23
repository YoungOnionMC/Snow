#include <Snow.h>

class EditorLayer : public Snow::Core::Layer {
public:
    void OnAttach();
    void OnDetach();
    void OnUpdate();

    void OnImGuiRender();


private:

};