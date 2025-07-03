#ifndef LAYER_H
#define LAYER_H

#include <Core/Base.h>
#include <Core/TimeStep.h>
#include <Events/Event.h>

namespace fts
{

    class Application;
    class Event;
    class Layer
    {
        friend class Application;

    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        virtual void OnAttach()
        {
        }
        virtual void OnDetach()
        {
        }
        virtual void OnUpdate(Timestep ts)
        {
        }

        virtual void OnPreRender() {};
        virtual void OnRenderer(const Timestep& ts) {};
        virtual void OnPostRender() {};

        virtual void OnImGuiRender()
        {
        }

        virtual void OnEvent(const Event& event)
        {
        }
        virtual void OnWindowResize(uint32_t width, uint32_t height)
        {
        }

        const std::string& GetName() const
        {
            return m_DebugName;
        }

    protected:
        std::string m_DebugName;
        fts::Application& mApp;
        bool IsEnabled;
    };

} // namespace fts

#endif // LAYER_H
