#pragma once
#include "AIBase.h"
class CAITrace :
    public CAIBase
{
public:
    CAITrace();
    virtual ~CAITrace();

public:
    virtual void Run(class CAIController* pController, class CAICharacter* pOwner, class CScene* pScene, float DeltaTime);
};

