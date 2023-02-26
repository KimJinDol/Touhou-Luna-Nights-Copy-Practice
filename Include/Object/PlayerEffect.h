#pragma once
#include "Effect.h"
#include "../Interface.h"

class CPlayerEffect :
    public CEffect, public ITimeStopInterface, public ISlowTimeInterface
{
	friend class CScene;

protected:
	CPlayerEffect();
	CPlayerEffect(const CPlayerEffect& obj);
	virtual ~CPlayerEffect();


public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void Update(float DeltaTime, TimeMode eMode);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual CPlayerEffect* Clone()	const;

public:
	virtual void SetTimeMode(TimeMode eMode);

};

