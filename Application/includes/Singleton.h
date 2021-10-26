#pragma once
#include <App_Types.h>
#include <Resource.h>
#include <memory>

namespace App::SingletonPattern
{
    template <class TClass>
    class Singleton : 
        public App::Resources::IResource<Singleton<TClass>>
    {
    protected:
        Singleton   (void) {}
        ~Singleton  (void) {}

    public:
        static TClass*  GetInstance (void) { return nullptr == sm_pInstance ? sm_pInstance = new TClass() : sm_pInstance; }
        static void     Release     (void) { delete sm_pInstance; }

    private:
        static TClass* sm_pInstance;
    };

    template <class TClass>
    TClass* Singleton<TClass>::sm_pInstance(nullptr);
}