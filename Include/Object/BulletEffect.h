#pragma once

#include "MoveableObject.h"

class CBulletEffect :
    public CMoveableObject
{
	friend class CScene;

protected:
	CBulletEffect();
	CBulletEffect(const CBulletEffect& obj);
	virtual ~CBulletEffect();

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual CBulletEffect* Clone()	const;

public:
	void EffectEnd();
};

