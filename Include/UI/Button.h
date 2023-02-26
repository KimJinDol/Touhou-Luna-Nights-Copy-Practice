#pragma once

#include "UIControl.h"
#include "../Resource/Sound.h"

enum class ButtonState
{
    Normal,
    MouseOn,
    Click,
    Disable,
    Max
};

class CButton :
    public CUIControl
{
    friend class CUIWidget;

protected:
    CButton();
    virtual ~CButton();

protected:
    ButtonState m_State;
    bool        m_Down;
    bool        m_Push;
    bool        m_Up;

protected:
    CSharedPtr<CSound> m_pMouseOnSound;
    CSharedPtr<CSound> m_pClickSound;
    std::function<void()>   m_ClickCallback;

public:
    ButtonState GetButtonState()    const
    {
        return m_State;
    }

public:
    void DisableButton()
    {
        m_State = ButtonState::Disable;
    }

public:
    void SetNormalTexture(const std::string& Name, const TCHAR* pFileName = nullptr,
        const std::string& PathName = TEXTURE_PATH);
    void SetMouseOnTexture(const std::string& Name, const TCHAR* pFileName = nullptr,
        const std::string& PathName = TEXTURE_PATH);
    void SetClickTexture(const std::string& Name, const TCHAR* pFileName = nullptr,
        const std::string& PathName = TEXTURE_PATH);
    void SetDisableTexture(const std::string& Name, const TCHAR* pFileName = nullptr,
        const std::string& PathName = TEXTURE_PATH);
    void SetNormalTextureFrame(const Vector2& Start, const Vector2& Size);
    void SetMouseOnTextureFrame(const Vector2& Start, const Vector2& Size);
    void SetClickTextureFrame(const Vector2& Start, const Vector2& Size);
    void SetDisableTextureFrame(const Vector2& Start, const Vector2& Size);
    void SetMouseOnSound(const std::string& Name, const char* pFileName = nullptr,
        const std::string& PathName = SOUND_PATH);
    void SetClickSound(const std::string& Name, const char* pFileName = nullptr,
        const std::string& PathName = SOUND_PATH);

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual bool CollisionMouse(const Vector2& MousePos);
    virtual void Collision();
    virtual void PrevRender();
    virtual void Render(HDC hDC, float DeltaTime);

public:
    virtual void CollisionEnd();

public:
    template <typename T>
    void SetClickCallback(T* pObj, void (T::* pFunc)())
    {
        m_ClickCallback = std::bind(pFunc, pObj);
    }
};

