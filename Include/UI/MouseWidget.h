#pragma once
#include "UIWidget.h"
#include "UIImage.h"
#include "../Collision/Collider.h"

class CMouseWidget :
    public CUIWidget
{
    friend class CScene;

protected:
    CMouseWidget();
    virtual ~CMouseWidget();

protected:
    CSharedPtr<CUIImage>    m_Image;
    CSharedPtr<CUIControl>  m_CollisionControl;
    CSharedPtr<CCollider>  m_CollisionCollider;
    bool                    m_ShowCursor;

public:
    void SetCollisionControl(CUIControl* pControl)
    {
        if (m_CollisionControl == pControl)
            return;

        if (m_CollisionControl)
            m_CollisionControl->CollisionEnd();

        m_CollisionControl = pControl;
        m_CollisionCollider = nullptr;
    }

    void SetCollisionObject(CCollider* pObj)
    {
        if (m_CollisionControl)
            m_CollisionControl->CollisionEnd();

        m_CollisionControl = nullptr;
        m_CollisionCollider = pObj;
    }

    bool CheckCollisionControl(CUIControl* pControl)
    {
        return m_CollisionControl == pControl;
    }

    bool CheckCollisionObject(CCollider* pObj)
    {
        return m_CollisionCollider == pObj;
    }

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual bool CollisionMouse();
    virtual void Collision();
    virtual void PrevRender();
    virtual void Render(HDC hDC, float DeltaTime);
};

