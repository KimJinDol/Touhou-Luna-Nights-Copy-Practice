#pragma once
#include "../Ref.h"

class CCollider :
    public CRef
{
    friend class CGameObject;
    friend class CScene;

protected:
    CCollider();
    CCollider(const CCollider& collider);
    virtual ~CCollider() = 0;

protected:
    class CScene* m_pScene;
    class CGameObject* m_Owner;
    ColliderType    m_Type;
    std::string	    m_Name;
    std::string     m_tag;
    // Offset은 이 충돌체를 가지고 있는 오브젝트의 위치로부터 얼마만큼 떨어져서 충돌체를
    // 만들지를 지정하는 값이다.
    Vector2     m_vOffsetPos;
    CollisionProfile* m_pProfile;
    std::function<void(CCollider*, CCollider*, float)>     m_BeginCollisionFunction;
    std::function<void(CCollider*, CCollider*, float)>     m_EndCollisionFunction;

    // 이전에 충돌되고 있던 목록을 정의한다.
    // 이 목록은 충돌되면 들어오고 충돌되다가 떨어지면 제거된다.
    std::list<CSharedPtr<CCollider>>    m_CollisionList;


    bool        m_bCollisionMouse;
    bool        m_bIsBlock;
    RECT        m_BlockRect;

public:
    void SetBlockRect(bool Left = 0, bool Right = 0, bool Top = 0, bool Bottom = 0)
    {
        m_BlockRect.left = Left;
        m_BlockRect.right = Right;
        m_BlockRect.top = Top;
        m_BlockRect.bottom = Bottom;

    }
    void SetBlock(bool Value)
    {
        m_bIsBlock = Value;
    }

public:
    bool GetBlock() const
    {
        return m_bIsBlock;
    }

    RECT GetBlockRectInfo() const
    {
        return m_BlockRect;
    }

    void CollisionMouseEnable(bool bEnable)
    {
        m_bCollisionMouse = bEnable;
    }

    void SetName(const std::string& Name)
    {
        m_Name = Name;
    }

    void SetOffset(float x, float y)
    {
        m_vOffsetPos.x = x;
        m_vOffsetPos.y = y;
    }

    void SetOffset(const Vector2& vOffset)
    {
        m_vOffsetPos = vOffset;
    }

    void AddOffset(float x, float y)
    {
        m_vOffsetPos.x += x;
        m_vOffsetPos.y += y;
    }

    std::string GetName()   const
    {
        return m_Name;
    }

    Vector2 GetOffset() const
    {
        return m_vOffsetPos;
    }

    ColliderType GetColliderType()  const
    {
        return m_Type;
    }

public:
    class CGameObject* GetOwner()   const
    {
        return m_Owner;
    }

    CollisionProfile* GetCollisionProfile() const
    {
        return m_pProfile;
    }

    void SetCollisionProfile(const std::string& Name);
    void AddCollisionList(CCollider* pCollider);
    bool CheckCollisionList(CCollider* pCollider);
    void DeleteCollisionList(CCollider* pCollider);
    void ClearCollisionList();
    std::list<CSharedPtr<CCollider>> GetCollisionLinst()    const
    {
        return m_CollisionList;
    }

public:
    virtual bool Init();
    virtual void Start();
    virtual void Update(float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime);
    virtual CCollider* Clone()	const;
    virtual bool Collision(CCollider* pDest) = 0;
    virtual bool CollisionMouse() = 0;

public:
    template <typename T>
    void SetBeginCollisionFunction(T* pObj, void(T::* pFunc)(CCollider*, CCollider*, float))
    {
        m_BeginCollisionFunction = bind(pFunc, pObj, std::placeholders::_1, std::placeholders::_2,
            std::placeholders::_3);
    }

    template <typename T>
    void SetEndCollisionFunction(T* pObj, void(T::* pFunc)(CCollider*, CCollider*, float))
    {
        m_EndCollisionFunction = bind(pFunc, pObj, std::placeholders::_1, std::placeholders::_2,
            std::placeholders::_3);
    }

    void CallBeginCollision(CCollider* pDest, float DeltaTime)
    {
        if (m_BeginCollisionFunction)
            m_BeginCollisionFunction(this, pDest, DeltaTime);
    }

    void CallEndCollision(CCollider* pDest, float DeltaTime)
    {
        if (m_EndCollisionFunction)
            m_EndCollisionFunction(this, pDest, DeltaTime);
    }
};

