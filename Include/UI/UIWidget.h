#pragma once

#include "../Ref.h"

class CUIWidget :
    public CRef
{
    friend class CScene;

public:
    CUIWidget();
    virtual ~CUIWidget();

protected:
    class CGameObject* m_pOwner;
    class CScene* m_pScene;
    std::string m_Name;
    std::vector<class CUIControl*>  m_ControlList;
    int         m_ZOrder;
    Vector2     m_vDefaultPos;

public:
    void SetOwner(class CGameObject* pOwner)
    {
        m_pOwner = pOwner;
    }

    class CGameObject* GetOwner()   const
    {
        return m_pOwner;
    }

    std::string GetName()   const
    {
        return m_Name;
    }

    int GetZOrder() const
    {
        return m_ZOrder;
    }

    Vector2 GetDefaultPos() const
    {
        return m_vDefaultPos;
    }

public:
    void SetName(const std::string& Name)
    {
        m_Name = Name;
    }

    void SetZOrder(int ZOrder)
    {
        m_ZOrder = ZOrder;
    }

    void SetScene(class CScene* pScene);

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual bool CollisionMouse();
    virtual void Collision();
    virtual void PrevRender();
    virtual void Render(HDC hDC, float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime, const Vector2& vPos);

public:
	class CUIControl* FindControl(const std::string& Name);

public:
    template <typename T>
    T* CreateControl(const std::string& Name)
    {
        T* pControl = new T;

        pControl->SetName(Name);
        pControl->m_pScene = m_pScene;
        pControl->m_pOwner = this;

        if (!pControl->Init())
        {
            SAFE_DELETE(pControl);
            return nullptr;
        }

        m_ControlList.push_back(pControl);

        return pControl;
    }

public:
    static int SortZOrder(const void* pSrc, const void* pDest);
};

