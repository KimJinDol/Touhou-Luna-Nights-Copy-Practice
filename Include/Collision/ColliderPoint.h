#pragma once
#include "Collider.h"
class CColliderPoint :
    public CCollider
{
    friend class CGameObject;
    friend class CScene;

private:
    CColliderPoint();
    CColliderPoint(const CColliderPoint& collider);
    virtual ~CColliderPoint();

private:
    Vector2    m_Pos;
    Vector2    m_Offset;

public:
    Vector2 GetInfo()  const
    {
        return m_Pos;
    }

public:
    void SetOffset(float x, float y)
    {
        m_Offset.x = x;
        m_Offset.y = y;
    }

    void SetOffset(const Vector2& Offset)
    {
        m_Offset = Offset;
    }

public:
    virtual bool Init();
    virtual void Start();
    virtual void Update(float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime);
    virtual CColliderPoint* Clone()	const;
    virtual bool Collision(CCollider* pDest);
    virtual bool CollisionMouse();
};

