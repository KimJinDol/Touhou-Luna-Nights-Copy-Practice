#pragma once

#include "../GameInfo.h"

class CAIBase
{
public:
    CAIBase();
    virtual ~CAIBase() = 0;

protected:
    std::string m_Name;
    //bool        m_isComplete; // �ӹ��� ��� �����ߴ���

public:
    //void SetComplete(bool Value)
    //{
    //    m_isComplete = Value;
    //}

    void SetName(const std::string& Name)
    {
        m_Name = Name;
    }

public:
	/* bool IsComplete()    const
	 {
		 return m_isComplete;
	 }*/

    const std::string& GetName()  const
    {
        return m_Name;
    }

public:
    virtual bool Init();
    virtual void Run(class CAIController* pController, class CAICharacter* pOwner, class CScene* pScene, float DeltaTime) = 0;
};

