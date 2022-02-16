#pragma once
#include <Resource.h>
#include <atomic>

#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
#include <unistd.h>
#if defined(_POSIX_VERSION) || defined(__linux__)
    #include <pthread.h>
    #include <mutex>
    #define PTHREAD_H
#endif
#else
#include <thread>
#include <Windows.h>
#endif

namespace Common::Concurrency
{
    class Semaphore final : 
        public Common::Resources::IResource<Semaphore>
    {
    public:
        Semaphore(const t_IntU32 in_unCont);
        virtual ~Semaphore(void);

    private:
        t_IntU32 m_unCount;
    #ifdef PTHREAD_H
        pthread_mutex_t m_Mutex;
        pthread_cond_t m_ConditionVariable;
    #else
        CRITICAL_SECTION m_hCriticalSection;
        HANDLE m_rEvent;
    #endif

    public:
        void Take(void);
        void Give(void);
        void Wait(void) { Take(); }
        void Post(void) { Give(); }
        void Signal(void) { Give(); }
        void Up(void) { Give(); }
        void Test(void) { Take(); }
        void Increment(void) { Give(); }
    };

    class ReentrantLock final :
        public Common::Resources::IResource<ReentrantLock>
    {
    public:
        ReentrantLock(void) : m_unRefCounter(0U), m_aRefCounter(0)
        {

        }
        ~ReentrantLock(void) = default;

        void Adquire(void);
        void Release(void);
        bool TryAdquire(void);

    private:
        std::atomic<t_IntU64> m_aRefCounter;
        t_IntU64 m_unRefCounter;
    };
}