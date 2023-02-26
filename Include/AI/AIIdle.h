#pragma once
#include "AIBase.h"
class CAIIdle :
    public CAIBase
{
public:
    CAIIdle();
    virtual ~CAIIdle();

public:
    virtual void Run(class CAIController* pController, class CAICharacter* pOwner, class CScene* pScene, float DeltaTime);
};

