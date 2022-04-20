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
            public Common::Resources::IResource<Application>
        {
        public:
            typedef enum class AppBehavior : bool { Asynchronus = 0, Synchronus } AppBehavior;
            typedef enum class AppStatus : t_IntU8 { Running = 0, Exit, Halted } AppStatus;
            typedef enum class EventState : t_IntU8 { noactive, active, unknown } EventState;

            typedef struct stPoint
            {
                t_IntU64 _x;
                t_IntU64 _y;
            } Point;

            /* Application Size:
            X Position,
            Y Position,
            Width,
            Height
            */
            typedef struct stAppDimentions
            {
                union
                {
                    struct
                    {
                        t_IntS64 m_lPosX;
                        t_IntS64 m_lPosY;
                        t_IntS64 m_lWidth;
                        t_IntS64 m_lHeight;
                    };
                    struct 
                    {
                        Point m_stPosition;
                        Point m_stSize;
                    };
                };

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
                                                              
            bool                        UpdateEvent           (const t_IntU64 in_eEvent, const EventState in_eNewState);
            void                        ResetEvents           (void);
            EventState                  GetEventState         (const t_IntU64 in_Event, const bool in_bReset = true) noexcept;

            virtual inline void         SetParent             (const Application* in_pParent = nullptr) { m_pParent = const_cast<Application*>(in_pParent); }
            inline void                 AttachEvent           (const t_IntU64 in_unEvent) noexcept { m_mEvents.insert ( std::make_pair(in_unEvent, EventState::noactive)); }
            inline       Application*   GetParent             (void) const                   { return m_pParent; }
            inline const char*          GetAppName            (void) const                   { return GetName(); }
            inline const t_IntU64&      GetID                 (void) const                   { return GetHashID(); }
            inline const bool&          ResizableState        (void) const                   { return m_stAttributes.mAttributes.m_bResizable; }
            inline const bool&          FullScreenState       (void) const                   { return m_stAttributes.mAttributes.m_bFullScreen; }
            inline       AppDimentions& GetAppDimentions      (void)                         { return m_stAttributes.mAttributes.m_stSize; }
            template<typename tclass>
            inline       tclass*        GetWndInstance        (void) const { return reinterpret_cast<tclass*>(m_pInstance); }
            
            void*                          m_pInstance;
        private:
            Application*                   m_pParent;
            stAppDescriptor                m_stAttributes;
            std::map<t_IntU64, EventState> m_mEvents;
        };
    }
}