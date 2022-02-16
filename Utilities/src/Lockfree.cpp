#include <Lockfree.h>
#include <Utilities.h>

Common::Concurrency::Semaphore::Semaphore(t_IntU32 in_unCount) 
    : m_unCount(in_unCount)
{
#ifdef PTHREAD_H
    pthread_mutex_init(&m_Mutex, nullptr);
    pthread_cond_init(&m_ConditionVariable, nullptr)
#else
    InitializeCriticalSection(&m_hCriticalSection);
    m_rEvent = CreateEvent(nullptr, false, false, nullptr);
#endif
}

Common::Concurrency::Semaphore::~Semaphore(void)
{
#ifdef PTHREAD_H
    pthread_mutex_destroy(&m_Mutex);
    pthread_cond_destroy(&m_ConditionVariable)
    #else
    DeleteCriticalSection(&m_hCriticalSection);
#endif
}

void Common::Concurrency::Semaphore::Take(void)
{
#ifdef PTHREAD_H
    pthread_mutex_lock(&m_Mutex);
#else
    EnterCriticalSection(&m_hCriticalSection);
#endif

    while (0 == m_unCount)
    {
#ifdef PTHREAD_H
        pthread_cond_wait(&m_ConditionVariable, &m_Mutex);
#else
        WaitForSingleObject(&m_rEvent, INFINITE);
        ResetEvent(&m_rEvent);
#endif
    }

    --m_unCount;
#ifdef PTHREAD_H
    pthread_mutex_unlock(&m_Mutex);
#else
    LeaveCriticalSection(&m_hCriticalSection);
#endif
}

void Common::Concurrency::Semaphore::Give(void)
{
#ifdef PTHREAD_H
    pthread_mutex_lock(&m_Mutex);
#else
    EnterCriticalSection(&m_hCriticalSection);
#endif
    ++m_unCount;
    #ifdef PTHREAD_H
        pthread_cond_signal(&m_ConditionVariable);
    #else
        if ( 1 == m_unCount )
            SetEvent(&m_rEvent);
    #endif
    #ifdef PTHREAD_H
        pthread_mutex_unlock(&m_Mutex);
    #else
        LeaveCriticalSection(&m_hCriticalSection);
    #endif
}

void Common::Concurrency::ReentrantLock::Adquire(void)
{
    t_IntU64 unId = std::hash<std::thread::id>()(std::this_thread::get_id());

    if (unId != m_aRefCounter.load(std::memory_order_relaxed))
    {
        t_IntU64 unExpected = 0;
        while (m_aRefCounter.compare_exchange_weak(unExpected, unId, std::memory_order_relaxed, std::memory_order_relaxed))
        {
            unExpected = 0;
            std::this_thread::yield();
        }
    }

    m_unRefCounter++;
    std::atomic_thread_fence(std::memory_order_acquire);
}

void Common::Concurrency::ReentrantLock::Release(void)
{
    std::atomic_thread_fence(std::memory_order_release);

    t_IntU64 unId = std::hash<std::thread::id>()(std::this_thread::get_id());

    STATIC_ASSERT(unId == m_aRefCounter.load(std::memory_order_relaxed));

    if ((m_unRefCounter--) == 0)
    {
        m_aRefCounter.store(0, std::memory_order_relaxed);
    }
}

bool Common::Concurrency::ReentrantLock::TryAdquire(void)
{
    t_IntU64 unId = std::hash<std::thread::id>()(std::this_thread::get_id());
    bool out_bResult = true;

    if (unId != m_aRefCounter.load(std::memory_order_relaxed))
    {
        t_IntU64 unExpected = 0;
        out_bResult = m_aRefCounter.compare_exchange_weak(unExpected, unId, std::memory_order_relaxed, std::memory_order_relaxed);
    }
    else
    {
        m_unRefCounter++;
        std::atomic_thread_fence(std::memory_order_acquire);
    }
    
    return out_bResult;
}