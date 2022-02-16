#include <FSM.h>
#include <Utilities.h>

App::FiniteStateMachine::FSM::FSM(void) : 
    Common::Resources::IResource<App::FiniteStateMachine::FSM>(Common::Resources::ClassName(typeid(App::FiniteStateMachine::FSM).name()))
{
}

App::FiniteStateMachine::FSM::~FSM(void)
{
    Dispose();
}

void App::FiniteStateMachine::FSM::Dispose(void)
{
    __super::Dispose();
    Reset();
    m_dicSuperStates.clear();

    for (auto oState : m_vecStates)
    {
        SAFE_RELEASE(oState);
    }
    
    m_vecEvents.clear();
    m_vecStates.clear();
}

const std::string  App::FiniteStateMachine::FSM::CurrentState(void) 
{ 
    return GetState(m_unCurrentState)->GetName(); 
}

void App::FiniteStateMachine::FSM::AddState(const State* in_pNewState, const State* in_obSubstate)
{
    if ((nullptr != in_pNewState) &&
        (nullptr == GetState(in_pNewState->GetHashID())))
    {
        m_vecStates.push_back(const_cast<State*>(in_pNewState));
        m_vecStates.back()->SetOwner(this);
        m_vecStates.back()->GetSubState() = in_obSubstate ? in_obSubstate->GetHashID() : 0;
    }
}

void App::FiniteStateMachine::FSM::LinkToSuperState(t_IntU64 in_obSubState, t_IntU64 in_obSuperState)
{
    if ((m_dicSuperStates.find(in_obSubState) != m_dicSuperStates.end()) &&
        (nullptr != GetState(in_obSubState)) &&
        (nullptr != GetState(in_obSuperState)))
    {
        m_dicSuperStates.insert(std::make_pair(in_obSubState, in_obSuperState));
    }
}

void App::FiniteStateMachine::FSM::UnlinkToSuperState(const t_IntU64& in_unSubState)
{
    if (m_dicSuperStates.find(in_unSubState) != m_dicSuperStates.end())
    {
        const_cast<State*>(QuerySuperState(in_unSubState))->GetSubState() = 0;
        m_dicSuperStates.erase(in_unSubState);
    }
}

bool App::FiniteStateMachine::FSM::RemoveState(const t_IntU64& in_u32StateID)
{
    State* pRqState = nullptr;

    if (nullptr != (pRqState = const_cast<State*>(GetState(in_u32StateID))))
    {
        m_unCurrentState = m_unCurrentState != pRqState->GetHashID() ?
            m_unCurrentState : 0;

        pRqState->OnExit();

        UnlinkToSuperState(pRqState->GetHashID());

        m_vecStates.erase(GetStatePair(pRqState->GetHashID()).first);
    }

    return nullptr != pRqState;
}

void App::FiniteStateMachine::FSM::InitState(const t_IntU64& in_unStateID, bool in_bInitSuperState)
{
    State* pCurrentState = nullptr;

    if (nullptr != (pCurrentState = const_cast<State*>(GetState(in_unStateID))) &&
        m_unCurrentState != pCurrentState->GetHashID()
        )
    {
        t_IntU64 unCurrent = 0;

        if (in_bInitSuperState &&
            0 < (unCurrent = pCurrentState->GetSuperState()) &&
            nullptr != (pCurrentState = const_cast<State*>(GetState(unCurrent)))
            )
        {
            pCurrentState->OnInit();
        }

        unCurrent = pCurrentState->GetHashID();

        while (nullptr != (pCurrentState = const_cast<State*>(GetState(unCurrent))))
        {
            pCurrentState->OnInit();
            m_unCurrentState = pCurrentState->GetHashID();
            unCurrent = pCurrentState->GetSubState();
        }
    }
}

void App::FiniteStateMachine::FSM::InitState(const t_IntU64& in_unStateID)
{
    InitState(in_unStateID, true);
}

void App::FiniteStateMachine::FSM::DispatchAll(void)
{
    while (0 < Events().size())
    {
        Event* pCachedEvent = Events().back();

        Events().erase(Events().begin());

#ifdef BROADCAST
        DispatchEvent(pCachedEvenet);
#elif defined(BYRESGISTRY)
        pCachedEvent->Notify();
#endif
    }
}

void App::FiniteStateMachine::FSM::DispatchEvent(const Event* in_pEvent)
{
    if (nullptr != in_pEvent)
    {
#ifdef BROADCAST
        t_IntU32 unSourceState = m_unCurrentState;

        while (0 != unSourceState)
        {
            State* pCurrentState = const_cast<State*>(GetState(unSourceState));

            if (nullptr != pCurrentState)
            {

                pCurrentState->OnUpdate(const_cast<Event*>(in_pEvent));
                unSourceState = pCurrentState->GetSuperState();
            }
        }
#elif defined(BYRESGISTRY)
        const_cast<Event*>(in_pEvent)->Notify();
#endif         
    }
}

void App::FiniteStateMachine::FSM::ChangeState(const t_IntU64& in_obState)
{
    State* pTargetState = nullptr;

    if (nullptr != (pTargetState = const_cast<State*>(GetState(in_obState))) &&
        (m_unCurrentState != pTargetState->GetHashID())
        )
    {
        t_IntU64 nCurrentState = m_unCurrentState;
        bool     bParentInitialized = false;

        while (0 < nCurrentState)
        {
            State* pCurrentState = const_cast<State*>(GetState(nCurrentState));

            if (nullptr != pCurrentState)
            {
                t_IntU64 unSuperStateID = pTargetState->GetSuperState();

                if ((bParentInitialized = (0 == unSuperStateID)) || (unSuperStateID != pCurrentState->GetHashID()))
                {
                    pCurrentState->OnExit();
                }
                else
                {
                    bParentInitialized = true;
                }

                nCurrentState = pCurrentState->GetSuperState();
            }
        }

        InitState(in_obState, !bParentInitialized);
    }
}

void App::FiniteStateMachine::FSM::AddEvent(Event* in_pNewEvent)
{
    if (nullptr != in_pNewEvent)
    {
        Events().push_back(in_pNewEvent);
    }
}

const App::FiniteStateMachine::State* App::FiniteStateMachine::FSM::GetState(const t_IntU64& in_objState)
{
    State* out_pState = GetStatePair(in_objState).second;

    return out_pState;
}

const App::FiniteStateMachine::State* App::FiniteStateMachine::FSM::GetState(const std::string in_strStateName)
{
    State* out_pState = GetStatePair(std::hash<std::string>()(in_strStateName)).second;

    return out_pState;
}

std::pair<std::vector<App::FiniteStateMachine::State*>::iterator, App::FiniteStateMachine::State*> App::FiniteStateMachine::FSM::GetStatePair(const t_IntU64& in_unStateID, eSeachType in_eSearchType)
{
    std::vector<App::FiniteStateMachine::State*>::iterator out_it = m_vecStates.begin();
    State* out_pState = nullptr;

    for (; out_it != m_vecStates.end(); out_it++)
    {
        if ((*out_it)->GetHashID() == in_unStateID)
        {
            out_pState = *out_it;
            break;
        }
    }

    return std::make_pair(out_it, out_pState);
}

const App::FiniteStateMachine::State* App::FiniteStateMachine::FSM::QuerySuperState(const t_IntU64& in_objSubState)
{
    return m_dicSuperStates.size() && m_dicSuperStates.find(in_objSubState) != m_dicSuperStates.end() ?
        GetState(m_dicSuperStates.at(in_objSubState)) : nullptr;
}

void App::FiniteStateMachine::FSM::Reset()
{
    while (0 != m_unCurrentState)
    {
        State* pState = const_cast<State*>(GetState(m_unCurrentState));
        pState->OnExit();
        m_unCurrentState = pState->GetSuperState();
    }
}
