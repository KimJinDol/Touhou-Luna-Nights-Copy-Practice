#pragma once
#include "AIBase.h"
class CAIAttack :
    public CAIBase
{
public:
    CAIAttack();
    virtual ~CAIAttack();

public:
    virtual void Run(class CAIController* pController, class CAICharacter* pOwner, class CScene* pScene, float DeltaTime) = 0;
};

