
#include "Button.h"
#include "../Input.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CButton::CButton()  :
    m_State(ButtonState::Normal),
    m_Down(false),
    m_Push(false),
    m_Up(false),
    m_pMouseOnSound(nullptr),
    m_pClickSound(nullptr)
{
}

CButton::~CButton()
{
}

void CButton::SetNormalTexture(const std::string& Name, const TCHAR* pFileName, const std::string& PathName)
{
    if (pFileName)
        m_pScene->GetResourceManager()->LoadTexture(Name, pFileName, PathName);

    AddAnimation("Normal", AnimationType::Sprite, true, Name);
}

void CButton::SetMouseOnTexture(const std::string& Name, const TCHAR* pFileName, const std::string& PathName)
{
    if (pFileName)
        m_pScene->GetResourceManager()->LoadTexture(Name, pFileName, PathName);

    AddAnimation("MouseOn", AnimationType::Sprite, true, Name);
}

void CButton::SetClickTexture(const std::string& Name, const TCHAR* pFileName, const std::string& PathName)
{
    if (pFileName)
        m_pScene->GetResourceManager()->LoadTexture(Name, pFileName, PathName);

    AddAnimation("Click", AnimationType::Sprite, true, Name);
}

void CButton::SetDisableTexture(const std::string& Name, const TCHAR* pFileName, const std::string& PathName)
{
    if (pFileName)
        m_pScene->GetResourceManager()->LoadTexture(Name, pFileName, PathName);

    AddAnimation("Disable", AnimationType::Sprite, true, Name);
}

void CButton::SetNormalTextureFrame(const Vector2& Start, const Vector2& Size)
{
    AddAnimationFrameData("Normal", Start, Size);
}

void CButton::SetMouseOnTextureFrame(const Vector2& Start, const Vector2& Size)
{
    AddAnimationFrameData("MouseOn", Start, Size);
}

void CButton::SetClickTextureFrame(const Vector2& Start, const Vector2& Size)
{
    AddAnimationFrameData("Click", Start, Size);
}

void CButton::SetDisableTextureFrame(const Vector2& Start, const Vector2& Size)
{
    AddAnimationFrameData("Disable", Start, Size);
}

void CButton::SetMouseOnSound(const std::string& Name, const char* pFileName, const std::string& PathName)
{
    if (pFileName)
        m_pScene->GetResourceManager()->LoadSound("UI", false, Name, pFileName, PathName);

    m_pMouseOnSound = m_pScene->GetResourceManager()->FindSound(Name);
}

void CButton::SetClickSound(const std::string& Name, const char* pFileName, const std::string& PathName)
{
    if (pFileName)
        m_pScene->GetResourceManager()->LoadSound("UI", false, Name, pFileName, PathName);

    m_pClickSound = m_pScene->GetResourceManager()->FindSound(Name);
}

bool CButton::Init()
{
    if (!CUIControl::Init())
        return false;

    return true;
}

void CButton::Update(float DeltaTime)
{
    CUIControl::Update(DeltaTime);

    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
    {
        if (!m_Down && !m_Push)
        {
            m_Down = true;
            m_Push = true;
        }

        else
        {
            m_Down = false;
            m_Push = true;
        }
    }

    else if (m_Push)
    {
        m_Down = false;
        m_Push = false;
        m_Up = true;
    }

    else if (m_Up)
    {
        m_Up = false;
    }
}

bool CButton::CollisionMouse(const Vector2& MousePos)
{
    bool    bResult = CUIControl::CollisionMouse(MousePos);

    if (m_State != ButtonState::Disable)
    {
        if (bResult)
        {
            // 클릭을 했는지 판단한다.
            if (m_Down)
            {
                if (m_pClickSound)
                    m_pClickSound->Play();
                m_State = ButtonState::Click;
                ChangeAnimation("Click");
            }

            else if (m_Push)
            {
                if (m_State == ButtonState::Normal)
                {

                    m_State = ButtonState::MouseOn;

                    ChangeAnimation("MouseOn");
                }
            }

            else if (m_Up && m_State == ButtonState::Click)
            {
                // 버튼 기능을 동작한다.
                if (m_ClickCallback)
                    m_ClickCallback();
            }

            else
            {
                if (m_State == ButtonState::Normal)
                {
                    if (m_pMouseOnSound)
                        m_pMouseOnSound->Play();
                }

                m_State = ButtonState::MouseOn;

                ChangeAnimation("MouseOn");
            }

        }

        else
        {
            ChangeAnimation("Normal");
            m_State = ButtonState::Normal;
        }
    }

    return bResult;
}

void CButton::Collision()
{
    CUIControl::Collision();
}

void CButton::PrevRender()
{
    CUIControl::PrevRender();
}

void CButton::Render(HDC hDC, float DeltaTime)
{
    CUIControl::Render(hDC, DeltaTime);
}

void CButton::CollisionEnd()
{
    ChangeAnimation("Normal");
    m_State = ButtonState::Normal;
}
