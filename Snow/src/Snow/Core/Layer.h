#pragma once

#include <string>
#include <vector>

namespace Snow {
    namespace Core {
        class Layer {
        public:
            Layer(const std::string& name = "Layer");

            virtual void OnAttach() {};
            virtual void OnDetach() {};
            virtual void OnUpdate() {};
            inline const std::string& GetName() const { return m_Name; }

        protected:
            std::string m_Name;
        };

        class LayerStack {
        public:
            LayerStack() = default;
            ~LayerStack();

            void PushLayer(Layer* layer);
            void PushOverlay(Layer* overlay);
            void PopLayer(Layer* layer);
            void PopOverlay(Layer* overlay);

            std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
            std::vector<Layer*>::const_iterator begin() const { return m_Layers.begin(); }
            std::vector<Layer*>::iterator end() { return m_Layers.end(); }
            std::vector<Layer*>::const_iterator end() const { return m_Layers.end(); }

        private:
            std::vector<Layer*> m_Layers;
            uint32_t m_LayerIndex = 0;
        };
    }
}