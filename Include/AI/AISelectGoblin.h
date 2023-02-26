#pragma once
#include "AISelect.h"
class CAISelectGoblin :
    public CAISelect
{
public:
    CAISelectGoblin();
    virtual ~CAISelectGoblin();

public:
    virtual bool Run(class CAIController* pController, class CAICharacter* pOwner, class CScene* pScene, float DeltaTime);
};

