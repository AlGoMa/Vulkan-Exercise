#pragma once
#include <App_Types.h>
#include <iostream>
#include <functional>
#include <regex>
#include <typeinfo>

namespace Common::Resources
{
    /// <summary>
    /// 
    /// </summary>
    /// <param name="in_pszName"></param>
    /// <returns></returns>
    static std::string ClassName(const char* in_pszName)
    {
        std::smatch out_matches;
        std::string strInput(in_pszName);
        std::regex_search(strInput, out_matches, std::regex("(?!.*::)(?!=[\\s]?)([\\w]+)((?=\\<?)|(?=\\>))"));

        return out_matches[1] ;
    }

    /// <summary>
    /// 
    /// </summary>
    struct stKey
    {
        t_IntU64     m_ulHashID = 0;
        std::string  m_strRscName;

        bool operator==(const stKey& in_refOther) const
        {
            return in_refOther.m_strRscName == m_strRscName && 
                   in_refOther.m_ulHashID == m_ulHashID;
        }
    };
    template<typename t>
    struct Hashable : std::false_type {};

    template<>
    struct Hashable<stKey> : std::true_type
    {
        t_IntU64 operator()(const stKey& in_rKey) const
        {
            return in_rKey.m_ulHashID;
        }
    };

    class IComponent
    {
    public:
        IComponent(void) {}
        virtual ~IComponent(void) {}
    
        bool operator ==(const IComponent& in_other) const { return false; }
    };

    /// <summary>
    /// 
    /// </summary>
    /// <typeparam name="TClass"></typeparam>
    template <typename TClass>
    class IResource
    {
    protected:
        IResource(void)
        {
            m_stResourceIn = stKey{ std::hash<std::string>()(typeid(TClass).name()), ClassName(typeid(this).name()) };
        }
        IResource(_In_ _Notnull_ std::string in_strIdentifier)
        {
            m_stResourceIn.m_strRscName = in_strIdentifier;
            m_stResourceIn.m_ulHashID = std::hash<std::string>()(in_strIdentifier);
        }

        virtual                       ~IResource (void) = default;
        virtual void                  Dispose    (void) {};
        virtual const std::type_info& GetType    (void) { return typeid(this); }

    public:
        inline const t_IntU32 GetHashID  (void)                      const { return m_stResourceIn.m_ulHashID; }
        inline const char*    GetName    (void)                      const { return m_stResourceIn.m_strRscName.c_str(); }
        virtual bool          Equals     (const IResource& in_other) const { return m_stResourceIn == in_other.m_stResourceIn; }
        bool                  operator ==(const IResource& in_other) const { return Equals(in_other); }

    private:
        stKey                 m_stResourceIn;
    };
}
 