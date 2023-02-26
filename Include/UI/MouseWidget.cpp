
#include "MouseWidget.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Input.h"
#include "../GameManager.h"

CMouseWidget::CMouseWidget()
{
    SetZOrder(INT_MAX);
    m_ShowCursor = false;
}

CMouseWidget::~CMouseWidget()
{
}

bool CMouseWidget::Init()
{
    if (!CUIWidget::Init())
        return false;

    m_Image = CreateControl<CUIImage>("MouseImage");

    std::vector<std::wstring>   vecFileName;
    vecFileName.push_back(TEXT("Mouse/0.bmp"));
    vecFileName.push_back(TEXT("Mouse/1.bmp"));
    vecFileName.push_back(TEXT("Mouse/2.bmp"));
    vecFileName.push_back(TEXT("Mouse/3.bmp"));
    vecFileName.push_back(TEXT("Mouse/4.bmp"));
    vecFileName.push_back(TEXT("Mouse/5.bmp"));
    vecFileName.push_back(TEXT("Mouse/6.bmp"));
    vecFileName.push_back(TEXT("Mouse/7.bmp"));
    vecFileName.push_back(TEXT("Mouse/8.bmp"));
    vecFileName.push_back(TEXT("Mouse/9.bmp"));
    vecFileName.push_back(TEXT("Mouse/10.bmp"));

    m_Image->AddAnimation("Default", AnimationType::Frame, true, "MouseDefault",
        vecFileName);

    m_Image->AddAnimationFrameData("Default", Vector2(0.f, 0.f), Vector2(32.f, 31.f), 11);

    for (int i = 0; i <= 10; ++i)
    {
        m_pScene->GetResourceManager()->SetColorKey("MouseDefault",
            255, 0, 255, i);
    }

    return true;
}

void CMouseWidget::Update(float DeltaTime)
{
    CUIWidget::Update(DeltaTime);

    Vector2 MousePos = CInput::GetInst()->GetMousePos();

    m_Image->SetPos(MousePos);

    if (MousePos.x < 0.f || MousePos.x >(float)CGameManager::GetInst()->GetResolution().Width ||
        MousePos.y < 0.f || MousePos.y >(float)CGameManager::GetInst()->GetResolution().Height)
    {
        if (!m_ShowCursor)
        {
            m_ShowCursor = true;

            while (ShowCursor(true) != 0)
            {
            }
        }
    }
    else
    {
        if (m_ShowCursor)
        {
            m_ShowCursor = false;

            int Count = ShowCursor(false);

            /*while (ShowCursor(false) != 0)
            {
            }*/
        }
    }
}

bool CMouseWidget::CollisionMouse()
{
    return CUIWidget::CollisionMouse();
}

void CMouseWidget::Collision()
{
    CUIWidget::Collision();
}

void CMouseWidget::PrevRender()
{
    CUIWidget::PrevRender();
}

void CMouseWidget::Render(HDC hDC, float DeltaTime)
{
    CUIWidget::Render(hDC, DeltaTime);
}
