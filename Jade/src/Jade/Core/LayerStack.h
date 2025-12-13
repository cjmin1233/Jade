#pragma once
#include <vector>

#include "Jade/Core/Base.h"
#include "Jade/Core/Layer.h"

namespace Jade
{
    class LayerStack
    {
    public:
        LayerStack();
        ~LayerStack();

        void PushLayer(const Ref<Layer>& layer);
        void PushOverlay(const Ref<Layer>& overlay);
        void PopLayer(const Ref<Layer>& layer);
        void PopOverlay(const Ref<Layer>& overlay);

        std::vector<Ref<Layer>>::iterator begin() { return m_Layers.begin(); }
        std::vector<Ref<Layer>>::iterator end() { return m_Layers.end(); }
        std::vector<Ref<Layer>>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
        std::vector<Ref<Layer>>::reverse_iterator rend() { return m_Layers.rend(); }

        std::vector<Ref<Layer>>::const_iterator cbegin() const { return m_Layers.cbegin(); }
        std::vector<Ref<Layer>>::const_iterator cend() const { return m_Layers.cend(); }
        std::vector<Ref<Layer>>::const_reverse_iterator crbegin() const { return m_Layers.crbegin(); }
        std::vector<Ref<Layer>>::const_reverse_iterator crend() const { return m_Layers.crend(); }

    private:
        std::vector<Ref<Layer>> m_Layers;

        unsigned int m_LayerInsertIndex;
    };
}
