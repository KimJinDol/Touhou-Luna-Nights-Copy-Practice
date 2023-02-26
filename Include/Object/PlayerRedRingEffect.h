#pragma once
#include "PlayerEffect.h"

class CPlayerRedRingEffect :
    public CPlayerEffect
{
	friend class CScene;

protected:
	CPlayerRedRingEffect();
	CPlayerRedRingEffect(const CPlayerRedRingEffect& obj);
	virtual ~CPlayerRedRingEffect();

private:
	Vector2		m_vFixedSize;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void Update(float DeltaTime, TimeMode eMode);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual CPlayerRedRingEffect* Clone()	const;

public:
	virtual void SetTimeMode(TimeMode eMode);
};

