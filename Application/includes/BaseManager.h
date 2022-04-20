#pragma once
#include <Resource.h>
#include <unordered_map>
#include <unordered_set>

namespace App::Managers
{
    template<typename T>
    concept Hashable = requires(T a)
    {
        { std::hash<T>{}(a) } -> std::convertible_to<t_IntU32>;
    };

    template <typename TManager>
    requires (std::is_class<TManager>::value &&  std::is_default_constructible<TManager>::value)
    class BaseManager :
        public Common::Resources::IResource<BaseManager<TManager>>
    {
    public:
        BaseManager(void) = default;
        virtual ~BaseManager(void) {  };

        virtual void Start(void);
        virtual void Shutdown(void);

    };
}
