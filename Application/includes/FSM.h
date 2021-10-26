#pragma once
#include <map>
#include <vector>
#include <memory>
#include <App_Types.h>
#include <string.h>
#include <Resource.h>

namespace App
{
    namespace FiniteStateMachine
    {
        class State;
        class Event;

        class FSM final:
            Resources::IResource<FSM>
        {
        public:
            FSM(void);
            
            ~FSM(void);

            enum class eSeachType : unsigned short { ById, ByName };

            void Dispose(void) override;

            void AddState(_In_ _Const_ _Notnull_ const State* in_pNewState, _In_ _Const_ _Notnull_ const State* in_obSubstate);

            void LinkToSuperState(_In_ _Const_ const t_IntU64 in_obSubState, _In_ _Const_ const t_IntU64 in_obSuperState);

            void UnlinkToSuperState(_In_ _Const_ const t_IntU64& in_unSubState);

            bool RemoveState(_In_ _Const_ const t_IntU64& in_u32StateID);

            const State* GetState(_In_ _Const_ const t_IntU64& in_unState);

            const State* GetState(_In_ _Const_ const std::string in_strStateName);

            const State* QuerySuperState(_In_ _Const_ const t_IntU64& in_objSubState);

            void DispatchEvent(_In_ _Const_ _Notnull_ const Event* in_pEvent);

            void ChangeState(_In_ _Const_ const t_IntU64& in_obState);

            void InitState(_In_ _Const_ const t_IntU64& in_unStateID);

            void AddEvent(_In_ _Notnull_ Event* in_pNewEvent);

            void DispatchAll(void);
            
            void Reset(void);

            const std::string CurrentState(void);

        private:
            void InitState(_In_ _Const_ const t_IntU64& in_unStateID, bool in_bInitSuperState);
            std::pair<std::vector<State*>::iterator, State*> GetStatePair(const t_IntU64& in_unStateID, eSeachType in_eSearchType = eSeachType::ById);

            inline std::map<t_IntU64, t_IntU64> SuperStates(void) const { return m_dicSuperStates; }
            inline std::vector<State*> States(void) const { return m_vecStates; }
            inline std::vector<Event*> Events(void) const { return m_vecEvents; }

        /**
        * Members
        */
            std::map<t_IntU64, t_IntU64>    m_dicSuperStates;
            std::vector<State*>             m_vecStates;
            std::vector<Event*>             m_vecEvents;
            t_IntU64                        m_unCurrentState;
        };


        class Event :
            public Resources::IResource<Event>
        {
        public:
            Event(_In_ _Notnull_ std::string in_strIdentifier) :
                Resources::IResource<Event>(in_strIdentifier) {}
            Event(void) :
                Resources::IResource<Event>(App::Resources::ClassName(typeid(Event).name())) {}
            ~Event(void) { }
        };

        class State : 
            public Resources::IResource<State>
        {
        public:
            State(_In_ _Notnull_ std::string in_strIdentifier) : Resources::IResource<State>(in_strIdentifier), 
                                                                 m_pOwner(nullptr),
                                                                 m_unSubState(0) {}
            State(void) : Resources::IResource<State>(App::Resources::ClassName(typeid(State).name())), 
                          m_unSubState(0), 
                          m_pOwner(nullptr)
            {
            }
        public:
            typedef enum class eResponseStates : bool { _Received, _NoReceived } Response;

            void SetOwner(const FSM* in_pOwner)
            {
                m_pOwner = const_cast<FSM*>(in_pOwner);
            }

            ~State(void)
            {
                Dispose();
            }

            void Dispose(void) override
            {
                if (0 < m_unSubState && NULL != m_pOwner)
                {
                    m_pOwner->UnlinkToSuperState(m_unSubState);
                    m_unSubState = 0;
                }
            }

            virtual Response  OnUpdate(_In_ _Const_ _Notnull_ const Event* in_pMessage);
            virtual void      OnInit(void) = 0;
            virtual void      OnExit(void) = 0;

            const t_IntU64 GetSuperState(void)
            {
                State* out_pSSID = (nullptr != m_pOwner) ? const_cast<State*>(m_pOwner->QuerySuperState(GetHashID())) : nullptr;

                return NULL != out_pSSID ? out_pSSID->GetHashID() : 0;
            }

            inline t_IntU64& GetSubState(void) { return m_unSubState; }

        private:
            t_IntU64                        m_unSubState;
            FSM*                            m_pOwner;
        };
    }
}