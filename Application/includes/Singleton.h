#pragma once
#include <App_Types.h>
#include <Resource.h>
#include <memory>

namespace App::SingletonPattern
{
    template <class TClass>
    requires std::is_class<TClass>::value
    class Singleton : 
        public Common::Resources::IResource<TClass>
    {
    protected:
        Singleton(void) = default;
        virtual ~Singleton(void) {};

    public:
        static TClass*  GetInstance (void) { return nullptr == sm_pInstance ? sm_pInstance = new TClass() : sm_pInstance; }
        static void     Release     (void) { delete sm_pInstance; }

    private:
        static TClass* sm_pInstance;
    };

    template <class TClass>
    requires std::is_class<TClass>::value
    TClass* Singleton<TClass>::sm_pInstance(nullptr);
}