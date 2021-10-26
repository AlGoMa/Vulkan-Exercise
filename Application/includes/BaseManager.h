#pragma once
#include <Resource.h>
#include <map>

namespace App::Managers
{
    template <class TIdentifier, class TContainer>
    class BaseManager :
        public Resources::IResource<BaseManager<TIdentifier, TContainer>>
    {
    public:
        BaseManager(void) = default;
        virtual ~BaseManager(void) { Dispose(); };

    protected:
        virtual void AddElement(_In_ _Const_ _Notnull_ const std::pair<TIdentifier, TContainer*> in_kpElement)
        {
            m_mpElements.insert(in_kpElement);
        }

        virtual void Dispose(void)
        {
            for (auto oObject : m_mpElements)
            {
                delete oObject.second;
            }

            m_mpElements.clear();
        }

    public:
        virtual void RemoveElement(_In_ _Const_ const TIdentifier& in_oElementID)
        {
            if (m_mpElements.end() != m_mpElements.find(in_oElementID))
            {
                m_mpElements.erase(in_oElementID);
            }
        }

        virtual TContainer* GetElement(_In_ _Notnull_ _Const_ const TIdentifier& in_tIdentifier) noexcept
        {
            return m_mpElements.end() != m_mpElements.find(in_tIdentifier) ? (*this)[in_tIdentifier] : NULL;
        }

        TContainer* operator[](_In_ _Const_ const TIdentifier in_tIdentifier) noexcept
        {
            return m_mpElements.at(in_tIdentifier);
        }

        bool IsEmpty(void) const
        {
            return 0 == m_mpElements.count();
        }

    private:
        std::map<TIdentifier, TContainer*> m_mpElements;
    };
}
