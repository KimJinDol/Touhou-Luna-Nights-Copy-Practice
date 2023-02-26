#pragma once
#include "Collider.h"

class CColliderLine :		// 타일 전용 콜리전
    public CCollider
{
	friend class CGameObject;
	friend class CScene;
	friend class CTile;
	friend class CSceneCollision;
	friend struct RaycastInfo;

private:
	CColliderLine();
	CColliderLine(const CColliderLine& collider);
	virtual ~CColliderLine();

private:
	Vector2		m_StartPoint;
	Vector2		m_EndPoint;
	Vector2		m_InterectPoint;

public:
	Vector2 GetStartPoint()	const
	{
		return m_StartPoint;
	}

	Vector2 GetEndPoint()		const
	{
		return m_EndPoint;
	}

	Vector2 GetInterectPoint()	const
	{
		return m_InterectPoint;
	}

public:
	void SetStartPoint(const Vector2& StartPoint)
	{
		m_StartPoint = StartPoint;
	}

	void SetStartPoint(float x, float y)
	{
		m_StartPoint = Vector2(x, y);
	}

	void SetEndPoint(const Vector2& EndPoint)
	{
 		m_EndPoint = EndPoint;
	}

	void SetEndPoint(float x, float y)
	{
		m_EndPoint = Vector2(x, y);
	}


public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual CColliderLine* Clone()	const;
	virtual bool Collision(CCollider* pDest);
	bool Collision(CCollider* pDest, Vector2& vInterect);
	virtual bool CollisionMouse();
};

