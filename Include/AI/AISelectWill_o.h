#pragma once
#include "AISelect.h"
class CAISelectWill_o :
    public CAISelect
{
public:
	CAISelectWill_o();
	virtual ~CAISelectWill_o();

public:
	virtual bool Run(class CAIController* pController, class CAICharacter* pOwner, class CScene* pScene, float DeltaTime);
};

