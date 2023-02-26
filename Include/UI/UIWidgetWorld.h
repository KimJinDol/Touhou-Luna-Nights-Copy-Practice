#pragma once

#include "../Ref.h"

class CUIWidgetWorld :
    public CRef
{
public:
    CUIWidgetWorld();
    CUIWidgetWorld(const CUIWidgetWorld& pWidgetWorld);
    ~CUIWidgetWorld();

protected:
    class CUIWidget* m_pWidget;
    class CGameObject* m_pOwner;
    class CScene* m_pScene;
    Vector2     m_vOffset;  // Owner로부터 떨어져있는 거리이다.
    Vector2     m_vPos;

    CSharedPtr<class CUIImage>  m_Image;

public:
    class CUIImage* GetImage()  const
    {
        return m_Image.Get();
    }

    class CUIWidget* GetWidget()   const
    {
        return m_pWidget;
    }

public:
    void SetWidget(class CUIWidget* pWidget)
    {
        m_pWidget = pWidget;
    }

    void SetOffset(const Vector2& vOffset)
    {
        m_vOffset = vOffset;
    }

    void SetOffset(float x, float y)
    {
        m_vOffset = Vector2(x, y);
    }

    void SetOwner(class CGameObject* pOwner)
    {
        m_pOwner = pOwner;
    }

    void SetScene(class CScene* pScene)
    {
        m_pScene = pScene;
    }

public:
    bool Init();
    void Update(float DeltaTime);
    bool CollisionMouse();
    void Collision();
    void PrevRender();
    void Render(HDC hDC, float DeltaTime);
    CUIWidgetWorld* Clone();
};

