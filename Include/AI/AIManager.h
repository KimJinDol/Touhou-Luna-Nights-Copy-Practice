#pragma once

#include "../GameInfo.h"

class CAIManager
{
private:
    std::unordered_map<std::string, class CAIBase*> m_mapAI;
    std::unordered_map<std::string, class CAISelect*> m_mapAISelect;

public:
    class CAIBase* FindAI(const std::string& Name);
    class CAISelect* FindAISelect(const std::string& Name);

public:
    template <typename T> 
    T* CreateAI(const std::string& Name)
    {
        T* pAI = (T*)FindAI(Name);

        if (pAI)
            return pAI;

        pAI = new T;

        pAI->SetName(Name);

        m_mapAI.insert(std::make_pair(Name, (class CAIBase*)pAI));

        return pAI;
    }

    template <typename T>
    T* CreateAISelect(const std::string& Name)
    {
        T* pAISelect = (T*)FindAISelect(Name);

        if (pAISelect)
            return pAISelect;

        pAISelect = new T;

        pAISelect->SetName(Name);

        m_mapAISelect.insert(std::make_pair(Name, (class CAISelect*)pAISelect));

        return pAISelect;
    }

    DECLARE_SINGLE(CAIManager)
};

