#pragma once
#include "AIBase.h"
class CAIWolfAttack :
    public CAIBase
{
public:
    CAIWolfAttack();
    virtual ~CAIWolfAttack();

public:
    virtual void Run(class CAIController* pController, class CAICharacter* pOwner, class CScene* pScene, float DeltaTime);
};

