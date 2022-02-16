#include "Application.h"
using namespace App::Managers;

Application::Application(void)
{
    memset(&m_stAttributes, 0, sizeof(AppAttributes));
}

Application::Application(const char* in_pszAppName,
    long in_lPosX,
    long in_lPosY,
    long in_lWidth,
    long in_lHeight,
    bool in_bFullScreen,
    bool in_bResizable) : m_pInstance(nullptr), m_pParent(nullptr), Common::Resources::IResource<Application>(in_pszAppName)
{
    m_stAttributes.mAppName = in_pszAppName;
    m_stAttributes.mAttributes.m_stSize.m_lPosX = in_lPosX;
    m_stAttributes.mAttributes.m_stSize.m_lPosY = in_lPosY;
    m_stAttributes.mAttributes.m_stSize.m_lWidth = in_lWidth;
    m_stAttributes.mAttributes.m_stSize.m_lHeight = in_lHeight;
    m_stAttributes.mAttributes.m_bFullScreen = in_bFullScreen;
    m_stAttributes.mAttributes.m_bResizable = in_bResizable;
}

Application::Application(const AppDescriptor& in_stDescriptor) : m_pInstance(nullptr), m_pParent(nullptr)
{
    m_stAttributes = in_stDescriptor;
}

void Application::vInitialization(void)
{
}

void Application::Run(void)
{
    Run(AppBehavior::Asynchronus);
}

void Application::Run(const AppBehavior in_eBehavior)
{
    LoadContent();
    
    vInitialization();

    while ( AppStatus::Running == Update() )
    {
        // Dispatch inputs gathered.
        ProcessInput();
        // Frame Update
        // Draw content.
        Draw();
    }

    vDeInitialization();
}

bool Application::UpdateEvent(uint64_t in_eEvent, const EventState in_eNewState)
{
    bool bResult = false;

    if (bResult = (std::end(m_mEvents) != m_mEvents.find(in_eEvent)))
    {
        m_mEvents[in_eEvent] = in_eNewState;
    }

    return bResult;
}

void Application::ResetEvents(void) 
{
    std::for_each(m_mEvents.rbegin(), m_mEvents.rend(),
        [](auto& rEvent) {
            rEvent.second = EventState::noactive;
        });
}

Application::EventState Application::GetEventStateAndReset(uint16_t in_Event) noexcept
{ 
    EventState eReturn = GetEventState(in_Event);

    UpdateEvent(in_Event, EventState::noactive);

    return eReturn;
}

void Application::vDeInitialization(void)
{
    memset(&m_stAttributes, 0, sizeof(AppAttributes));

    m_mEvents.clear();

    vDeInitialization();
}