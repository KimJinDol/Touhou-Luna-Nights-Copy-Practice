#pragma once
#include "AIAttack.h"
class CAIGoblinAttack :
    public CAIAttack
{
public:
    CAIGoblinAttack();
    virtual ~CAIGoblinAttack();

public:
    virtual void Run(class CAIController* pController, class CAICharacter* pOwner, class CScene* pScene, float DeltaTime);
};

