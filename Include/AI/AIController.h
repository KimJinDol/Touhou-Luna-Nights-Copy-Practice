#pragma once

#include "../GameInfo.h"
#include "AIManager.h"

class CAIController
{
public:
    CAIController();
    CAIController(const CAIController& controller);
    virtual ~CAIController();

protected:
    std::unordered_map<std::string, class CAIBase*> m_mapAI;
    class CScene* m_pScene;
    class CAICharacter* m_pOwner;
    class CAIBase* m_pCurrentAI;
    std::unordered_map<std::string, int>    m_mapIntData;
    std::unordered_map<std::string, float>    m_mapFloatData;
    std::unordered_map<std::string, bool>    m_mapBoolData;
    std::unordered_map<std::string, std::string>    m_mapStringData;
    std::unordered_map<std::string, class CGameObject*>    m_mapObjectData;
    class CAISelect* m_pSelectAI;

public:
    void SetScene(class CScene* pScene);
    void SetOwner(class CAICharacter* pOwner);
    void ChangeAI(const std::string& Name);
    class CAIBase* FindAI(const std::string& Name);
    void SetIntData(const std::string& Name, int Data);
    void SetFloatData(const std::string& Name, float Data);
    void SetBoolData(const std::string& Name, bool Data);
    void SetStringData(const std::string& Name, std::string Data);
    void SetGameObjectData(const std::string& Name, class CGameObject* Data);

public:
    class CAIBase* GetCurrentAI()   const
    {
        return m_pCurrentAI;
    }
    int GetIntData(const std::string& Name);
    float GetFloatData(const std::string& Name);
    bool GetBoolData(const std::string& Name);
    std::string GetStringData(const std::string& Name);
    class CGameObject* GetGameObjectData(const std::string& Name);

public:
    void Run(float DeltaTime);
    CAIController* Clone();

public:
    template <typename T>
    T* CreateAI(const std::string& Name)
    {
        // ���� �̸����� ������ AI�� �ִ��� �Ǵ��Ѵ�.
        T* pAI = (T*)CAIManager::GetInst()->FindAI(Name);

        if (pAI)
        {
            if (!FindAI(Name))
                m_mapAI.insert(std::make_pair(Name, pAI));

            if (!m_pCurrentAI)
                m_pCurrentAI = pAI;

            return pAI;
        }

        pAI = CAIManager::GetInst()->CreateAI<T>(Name);

        m_mapAI.insert(std::make_pair(Name, pAI));

        if (!m_pCurrentAI)
            m_pCurrentAI = pAI;

        return pAI;
    }

    template <typename T>
    T* CreateAISelect(const std::string& Name)
    {
        // ���� �̸����� ������ AI�� �ִ��� �Ǵ��Ѵ�.
        T* pAISelect = (T*)CAIManager::GetInst()->FindAISelect(Name);

        if (!pAISelect)
            pAISelect = CAIManager::GetInst()->CreateAI<T>(Name);

        m_pSelectAI = pAISelect;

        return pAISelect;
    }
};

