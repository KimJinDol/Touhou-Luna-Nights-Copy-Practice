#pragma once

#include "../Ref.h"
#include "../Resource/Texture.h"
#include "../Resource/AnimationUI.h"
#include "../GameInfo.h"
#include "UIWidget.h"

class CUIControl :
    public CRef
{
    friend class CUIWidget;

protected:
    CUIControl();
    virtual ~CUIControl() = 0;

protected:
    class CScene* m_pScene;
    class CUIWidget* m_pOwner;

protected:
    UIShape     m_Shape;
    CAnimationUI*  m_pAnimation;
    CSharedPtr<CTexture>	m_Texture;
    bool        m_bIsTextureImageStartPos;
    Vector2     m_vTextureImageStartPos;
    Vector2     m_vTexturePixelSize;
    bool		m_Start;
    std::string	m_Name;
    Vector2     m_vPos;
    Vector2     m_vSize;
    Vector2     m_vPivot;
    int         m_ZOrder;
    bool        m_bCollisionMouse;

public:
    bool IsTextureImageStartPos()   const
    {
        return m_bIsTextureImageStartPos;
    }

    Vector2 GetTextureImageStartPos()   const
    {
        return m_vTextureImageStartPos;
    }

    bool IsMouseCollision() const
    {
        return m_bCollisionMouse;
    }

    UIShape GetShape()  const
    {
        return m_Shape;
    }

    int GetZOrder() const
    {
        return m_ZOrder;
    }

    Vector2 GetPos()	const
    {
        return m_vPos;
    }

    Vector2 GetSize()	const
    {
        return m_vSize;
    }

    Vector2 GetPivot()	const
    {
        return m_vPivot;
    }

    const std::string& GetName()	const
    {
        return m_Name;
    }

    CTexture* GetTexture()  const
    {
        return m_Texture.Get();
    }

public:
    void SetTextureImageStartPos(const Vector2& vPos)
    {
        if (m_pAnimation) return;
        m_bIsTextureImageStartPos = true;
        m_vTextureImageStartPos = vPos;
    }

    void SetTextureImageStartPos(float x, float y)
    {
        if (m_pAnimation) return;
		m_bIsTextureImageStartPos = true;
		m_vTextureImageStartPos.x = x;
		m_vTextureImageStartPos.y = y;
    }

    void SetShape(UIShape Shape)
    {
        m_Shape = Shape;
    }

    void SetZOrder(int ZOrder)
    {
        m_ZOrder = ZOrder;
    }

    void SetPos(const Vector2& vPos)
    {
        m_vPos = vPos + m_pOwner->GetDefaultPos();
    }

    void SetPos(float x, float y)
    {
        m_vPos.x = x + m_pOwner->GetDefaultPos().x;
        m_vPos.y = y + m_pOwner->GetDefaultPos().y;
    }

    void SetSize(const Vector2& vSize)
    {
        m_vSize = vSize;
    }

    void SetSize(float x, float y)
    {
        m_vSize.x = x;
        m_vSize.y = y;
    }

    void SetPivot(const Vector2& vPivot)
    {
        m_vPivot = vPivot;
    }

    void SetPivot(float x, float y)
    {
        m_vPivot.x = x;
        m_vPivot.y = y;
    }

    void SetName(const std::string& Name)
    {
        m_Name = Name;
    }

public:
    void SetTexture(const std::string& Name);
    void SetTexture(const std::string& Name, const TCHAR* pFileName,
        const std::string& PathName = TEXTURE_PATH);
    void SetTexture(const std::string& Name, const Vector2& ImageStartPos, const Vector2& PixelSize, const std::string& PathName = TEXTURE_PATH);
    void SetColorKey(unsigned char r, unsigned char g, unsigned char b,
        int Index = 0);

public:
    bool AddAnimation(const std::string& Name, AnimationType Type, bool Loop,
        class CTexture* pTexture, const std::vector<AnimationFrameData>& vecFrame,
        float PlayTime = 1.f);
    bool AddAnimation(const std::string& Name, AnimationType Type, bool Loop,
        class CTexture* pTexture, float PlayTime = 1.f);
    bool AddAnimation(const std::string& Name, AnimationType Type, bool Loop,
        const std::string& TextureName, float PlayTime = 1.f);
    bool AddAnimation(const std::string& Name, AnimationType Type, bool Loop,
        const std::string& TextureName, const TCHAR* pTexFileName,
        float PlayTime = 1.f);
    bool AddAnimation(const std::string& Name, AnimationType Type, bool Loop,
        const std::string& TextureName, const std::vector<std::wstring>& vecFileName,
        float PlayTime = 1.f);
    bool AddAnimationFrameData(const std::string& Name, unsigned int x, unsigned int y,     // 아틀라스 기준.
        unsigned int Count, const Vector2& FramePixelSize = Vector2(512.f, 512.f), const Vector2& Pivot = Vector2(0.5f, 1.f));
    bool AddAnimationFrameData(const std::string& Name,
        const Vector2& ImageStartPos, const Vector2& Size);
    bool AddAnimationFrameData(const std::string& Name,
        const Vector2& ImageStartPos, const Vector2& Size,
        int Count);
    void SetPlayScale(const std::string& Name, float Scale);
    void SetCurrentAnimation(const std::string& Name);
    void ChangeAnimation(const std::string& Name);
    void SetAnimationReverse(const std::string& Name, bool bReverse);

    void InitializeAnimationFrame(); // 현재 애니메이션 프레임을 0으로 바꿔준다.

public:
    template <typename T>
    void SetAnimationEndFunction(const std::string& Name,
        T* pObj, void (T::* pFunc)())
    {
        if (!m_pAnimation)
            return;

        m_pAnimation->SetEndFunction<T>(Name, pObj, pFunc);
    }

    template <typename T>
    void SetAnimationNotifyFunction(const std::string& Name, int Frame,
        T* pObj, void (T::* pFunc)())
    {
        if (!m_pAnimation)
            return;

        m_pAnimation->SetNotifyFunction<T>(Name, Frame, pObj, pFunc);
    }

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual bool CollisionMouse(const Vector2& MousePos);
    virtual void Collision();
    virtual void PrevRender();
    virtual void Render(HDC hDC, float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime, const Vector2& vPos);
    virtual void Render(HDC hDC, float DeltaTime, float ImageStartPosX, float ImageStartPosY); // 아틀라스형 텍스처를 가진 obj를 위한 함수..
    virtual void Render(HDC hDC, float DeltaTime, const Vector2& vPos, float ImageStartPosX, float ImageStartPosY);

public:
    virtual void CollisionEnd();

protected:
    void RenderRatio(HDC hDC, float DeltaTime, float Ratio);
    void RenderRatio(HDC hDC, float DeltaTime, const Vector2& vPos, float Ratio);
};