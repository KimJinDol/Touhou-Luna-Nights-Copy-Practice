#pragma once
#include "Bullet.h"
class CTornadoBullet :
    public CBullet
{
	friend class CScene;

protected:
	CTornadoBullet();
	CTornadoBullet(const CTornadoBullet& obj);
	virtual ~CTornadoBullet();

private:
	Vector2	m_MovePos;
	float	m_RotationAngle;
	float	m_RotationRange;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual CTornadoBullet* Clone()	const;
};

