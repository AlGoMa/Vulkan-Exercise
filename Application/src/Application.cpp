#include "Application.h"
using namespace App::Managers;

Application::Application(void)
{
    memset(&m_stAttributes, 0, sizeof(AppAttributes));
}

Application::Application(const char* in_pszAppName,
                         const long in_lPosX,
                         const long in_lPosY,
                         const long in_lWidth,
                         const long in_lHeight,
                         const bool in_bFullScreen,
                         const bool in_bResizable) : m_pInstance(nullptr), m_pParent(nullptr), Common::Resources::IResource<Application>(in_pszAppName)
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

bool Application::UpdateEvent(const t_IntU64 in_eEvent, const EventState in_eNewState)
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

Application::EventState Application::GetEventState(const t_IntU64 in_unEvent, const bool in_bState) noexcept
{ 
    EventState eReturn = m_mEvents.find(in_unEvent)->second;

    UpdateEvent(in_unEvent, in_bState ? EventState::noactive : eReturn);

    return eReturn;
}

void Application::vDeInitialization(void)
{
    memset(&m_stAttributes, 0, sizeof(AppAttributes));

    m_mEvents.clear();

    vDeInitialization();
}