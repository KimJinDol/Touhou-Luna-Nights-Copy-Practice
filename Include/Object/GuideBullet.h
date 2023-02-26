#pragma once

#include "MoveableObject.h"

class CGuideBullet :
    public CMoveableObject
{
	friend class CScene;

protected:
	CGuideBullet();
	CGuideBullet(const CGuideBullet& obj);
	virtual ~CGuideBullet();

protected:
	float		m_SmokeTime;
	float		m_SmokeTimeMax;
	float		m_DelayTime;
	float		m_DelayTimeMax;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual CGuideBullet* Clone()	const;

public:
	void BeginCollision(class CCollider* pSrc, class CCollider* pDest, float DeltaTime);
};

