#pragma once
#include "MoveableObject.h"
class CPlayerSubObj :
    public CMoveableObject
{
	friend class CScene;

protected:
	CPlayerSubObj();
	CPlayerSubObj(const CPlayerSubObj& obj);
	virtual ~CPlayerSubObj();

private:
	float		m_FireTime;
	float		m_FireTimeMax;

public:
	void SetFireTime(float Time)
	{
		m_FireTimeMax = Time;
	}

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual CPlayerSubObj* Clone()	const;

public:
	void BeginCollision(class CCollider* pSrc, class CCollider* pDest, float DeltaTime);
};

