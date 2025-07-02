#ifndef LAYER_H
#define LAYER_H

#include <Core/Base.h>
#include <Core/TimeStep.h>
#include <Events/Event.h>

namespace fts
{

    class Event;
    class Layer
    {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer() = default;

        virtual void OnAttach()
        {
        }
        virtual void OnDetach()
        {
        }
        virtual void OnUpdate(Timestep ts)
        {
        }
        virtual void OnImGuiRender()
        {
        }
        virtual void OnEvent(Event& event)
        {
        }

        const std::string& GetName() const
        {
            return m_DebugName;
        }

    protected:
        std::string m_DebugName;
    };

} // namespace fts

#endif // LAYER_H
