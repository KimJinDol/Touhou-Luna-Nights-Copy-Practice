#pragma once
#include "Collider.h"
class CColliderSphere :
    public CCollider
{
    friend class CGameObject;
    friend class CScene;

private:
    CColliderSphere();
    CColliderSphere(const CColliderSphere& collider);
    virtual ~CColliderSphere();

private:
    Vector2         m_Offset;
    SphereInfo    m_Info;

public:
    SphereInfo GetInfo()  const
    {
        return m_Info;
    }

public:
    void SetOffset(float x, float y)
    {
        m_Offset = Vector2(x, y);
    }

    void SetOffset(const Vector2& Offset)
    {
        m_Offset = Offset;
    }

    void SetRadius(float Radius)
    {
        m_Info.Radius = Radius;
    }

public:
    virtual bool Init();
    virtual void Start();
    virtual void Update(float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime);
    virtual CColliderSphere* Clone()	const;
    virtual bool Collision(CCollider* pDest);
    virtual bool CollisionMouse();
};

