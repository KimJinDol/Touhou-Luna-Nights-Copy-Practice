#pragma once
#include "MoveableObject.h"
class CSmoke :
    public CMoveableObject
{
	friend class CScene;

protected:
	CSmoke();
	CSmoke(const CSmoke& obj);
	virtual ~CSmoke();

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual CSmoke* Clone()	const;

public:
	void EffectEnd();
};

