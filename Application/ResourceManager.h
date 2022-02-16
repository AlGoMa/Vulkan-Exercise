#pragma once
#include "Singleton.h"

namespace App::Manager {

    class ResourceManager :
        public SingletonPattern::Singleton<ResourceManager>
    {
    public:
        ResourceManager(void) = default;
        ~ResourceManager(void) = default;
    };
}


