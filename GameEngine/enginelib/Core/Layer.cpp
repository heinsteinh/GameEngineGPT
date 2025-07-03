#include <Core/Application.h>
#include <Core/Layer.h>
#include <ftspch.h>

namespace fts
{

    Layer::Layer(const std::string& debugName)
        : m_DebugName(debugName)
        , IsEnabled(true)
        , mApp(Application::Get())
    {
        FTS_CORE_INFO("Initializing layer: {}", m_DebugName);
    }

    Layer::~Layer()
    {
        FTS_CORE_INFO("Deleting layer: {}", m_DebugName);
    }
} // namespace fts
