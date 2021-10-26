#pragma once
#include <App_Types.h>
#include <iostream>
#include <functional>
#include <regex>
#include <typeinfo>

namespace App
{
    namespace Resources
    {
        static std::string ClassName(const char* in_pszName)
        {
            std::smatch t_matches;
            std::string strInput(in_pszName);
            std::regex_search(strInput, t_matches, std::regex("(?!.*::)(?!=[\\s]?)([\\w]+)"));
            
            return t_matches[1];
        }

        template <class TClass>
        class IResource
        {
        protected:
            typedef struct RESOURCEDATA
            {
                t_IntU64     m_ulHashID;
                std::string  m_strRscName;
            } stResourceID;

            IResource(void) 
            { 
                m_stResourceIn = stResourceID{ std::hash<std::string>()(typeid(IResource).name()), ClassName(typeid(this).name()) };
            }

            IResource(_In_ _Notnull_ std::string in_strIdentifier) 
            {
                m_stResourceIn.m_strRscName = in_strIdentifier;
                m_stResourceIn.m_ulHashID = std::hash<std::string>()(in_strIdentifier);
            }

            virtual ~IResource(void) = default;
            virtual void                  Dispose(void) { memset(&m_stResourceIn, 0, sizeof(stResourceID)); }
            virtual bool                  Equals(IResource& object) { return this->GetName() == object.GetName() && this->GetHashID() == object.GetHashID(); }
            virtual const std::type_info& GetType(void) { return typeid(this); }

        public:
            inline const t_IntU64 GetHashID (void) const { return m_stResourceIn.m_ulHashID; }
            inline const char*    GetName(void) const { return m_stResourceIn.m_strRscName.c_str(); }

        private:
            stResourceID m_stResourceIn;
        };
    }
}
