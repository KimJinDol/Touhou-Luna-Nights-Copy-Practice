#pragma once
#include "Collider.h"
class CColliderRect :
    public CCollider
{
    friend class CGameObject;
    friend class CScene;

private:
    CColliderRect();
    CColliderRect(const CColliderRect& collider);
    virtual ~CColliderRect();

private:
    float       m_Width;
    float       m_Height;
    RectInfo    m_Info;

public:
    RectInfo GetInfo()  const
    {
        return m_Info;
    }

    float GetWidth()    const
    {
        return m_Width;
    }

    float GetHeight()   const
    {
        return m_Height;
    }
    

public:
    void SetExtent(float Width, float Height)
    {
        m_Width = Width;
        m_Height = Height;
    }

    void SetExtent(Vector2 Size)
    {
        m_Width = Size.x;
        m_Height = Size.y;
    }

public:
    virtual bool Init();
    virtual void Start();
    virtual void Update(float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime);
    virtual CColliderRect* Clone()	const;
    virtual bool Collision(CCollider* pDest);
    virtual bool CollisionMouse();
};

