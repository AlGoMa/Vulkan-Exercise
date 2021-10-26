#pragma once
#include <vector>
#include <memory>
#include <Resource.h>
#include <type_traits>
#include <map>

const     long width = (800);
const     long height = (600);
constexpr long x = width / 2;
constexpr long y = height / 2;

namespace App
{
    namespace Managers
    {
        class Application : 
            public Resources::IResource<Application>
        {
        public:
            typedef enum class AppBehavior : bool { Asynchronus = 0, Synchronus } AppBehavior;
            typedef enum class AppStatus : uint8_t { Running = 0, Exit, Halted } AppStatus;
            typedef enum class EventState : uint8_t { noactive, active, unknown } EventState;

            /* Application Size:
            X Position,
            Y Position,
            Width,
            Height
            */
            typedef struct stAppDimentions
            {
                long m_lPosX;
                long m_lPosY;
                long m_lWidth;
                long m_lHeight;
            } AppDimentions;

            typedef struct stAppAttributes
            {
                AppDimentions m_stSize;
                bool          m_bFullScreen;
                bool          m_bResizable;
                bool          m_bMouseSupportEvents;
                bool          m_bMouseVisibility;
                bool          m_bKeyboardEvents;
                bool          m_bVSync;
            } AppAttributes;

            typedef struct stAppDescriptor
            {
                AppAttributes                mAttributes;
                std::string                  mAppName;
                std::shared_ptr<App::Managers::Application> mParent;
            } AppDescriptor;
             
        protected:
            Application(_In_ _Const_ const char* in_szAppName, _In_ _Const_ const long in_lPosX = x, _In_ _Const_ const long in_lPosY = y, _In_ _Const_ const long in_lWidth = width, _In_ _Const_ const long in_lHeight = height, _In_opt_ _Const_ const bool in_bFullScreen = false, _In_opt_ _Const_ const bool in_bResizable = false);
            Application(const AppDescriptor& in_stDescriptor);
            Application(void);

        public:
            virtual ~Application(void) = default;

            void Run (void);
            void Run (const AppBehavior in_eBehavior);

        protected:
            virtual void                vInitialization       (void);
            virtual void                vDeInitialization     (void);
            virtual void                Draw                  (void) = 0;
            virtual void                LoadContent           (void) = 0;
            virtual void                ProcessInput          (void) = 0;
            virtual const AppStatus     Update                (void) = 0;
                                                              
            bool                        UpdateEvent           (uint64_t in_eEvent, const EventState in_eNewState);
            void                        ResetEvents           (void);
            EventState                  GetEventStateAndReset (uint16_t in_Event) noexcept;

            virtual inline void         SetParent             (const Application* in_pParent = nullptr) { m_pParent = const_cast<Application*>(in_pParent); }
            inline void                 AttachEvent           (uint64_t in_eEvent) noexcept { m_mEvents.insert ( std::make_pair(in_eEvent, EventState::noactive)); }                       
                                                              
            inline Application*         GetParent             (void) const { return m_pParent; }
            inline const char*          GetAppName            (void) const { return GetName(); }
            inline const t_IntU64&      GetID                 (void) const { return GetHashID(); }
            inline const bool&          ResizableState        (void) const { return m_stAttributes.mAttributes.m_bResizable; }
            inline const bool&          FullScreenState       (void) const { return m_stAttributes.mAttributes.m_bFullScreen; }
            inline       AppDimentions& GetAppDimentions      (void)       { return m_stAttributes.mAttributes.m_stSize; }
            inline const void*          GetWndInstance        (void) const { return m_pInstance; }
            inline const EventState     GetEventState         (uint16_t in_Event) noexcept { return m_mEvents.find(in_Event)->second; }
            
            void*                          m_pInstance;
        private:
            Application*                   m_pParent;
            stAppDescriptor                m_stAttributes;
            std::map<uint64_t, EventState> m_mEvents;
        };
    }
}