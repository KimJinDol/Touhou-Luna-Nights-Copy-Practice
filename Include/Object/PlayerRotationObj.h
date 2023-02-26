#pragma once
#include "MoveableObject.h"
class CPlayerRotationObj :
    public CMoveableObject
{
	friend class CScene;

protected:
	CPlayerRotationObj();
	CPlayerRotationObj(const CPlayerRotationObj& obj);
	virtual ~CPlayerRotationObj();

private:
	float	m_TargetLength;
	float	m_Angle;

public:
	void SetTargetLength(float Length)
	{
		m_TargetLength = Length;
	}

	void SetAngle(float Angle)
	{
		m_Angle = Angle;
	}

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual CPlayerRotationObj* Clone()	const;

public:
	void BeginCollision(class CCollider* pSrc, class CCollider* pDest, float DeltaTime);
};

