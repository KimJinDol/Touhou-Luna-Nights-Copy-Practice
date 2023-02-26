#pragma once
#include "AISelect.h"
class CAISelectWolf:
	public CAISelect
{
public:
	CAISelectWolf();
	virtual ~CAISelectWolf();

public:
	virtual bool Run(class CAIController* pController, class CAICharacter* pOwner, class CScene* pScene, float DeltaTime);
};

