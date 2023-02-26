
#include "UIWidgetWorld.h"
#include "UIWidget.h"
#include "../Object/GameObject.h"
#include "../Scene/Scene.h"
#include "../Scene/Camera.h"
#include "UIImage.h"

CUIWidgetWorld::CUIWidgetWorld() :
    m_pWidget(nullptr),
    m_pOwner(nullptr),
    m_pScene(nullptr)
{
}

CUIWidgetWorld::CUIWidgetWorld(const CUIWidgetWorld& pWidgetWorld)
{
    *this = pWidgetWorld;

    m_pOwner = nullptr;
    m_pScene = nullptr;
}

CUIWidgetWorld::~CUIWidgetWorld()
{
    m_pWidget->SetScene(nullptr);
    SAFE_RELEASE(m_pWidget);
}

bool CUIWidgetWorld::Init()
{
    m_pWidget = new CUIWidget;
    m_pWidget->SetScene(m_pScene);
    m_pWidget->SetOwner(m_pOwner);
    m_pWidget->SetName("WorldWidget");

    return true;
}

void CUIWidgetWorld::Update(float DeltaTime)
{
    CCamera* pCamera = m_pScene->GetMainCamera();

    m_vPos = m_vOffset - pCamera->GetPos();

    if (m_pOwner)
        m_vPos = m_pOwner->GetPos() + m_vOffset - pCamera->GetPos();

    if (m_pWidget)
    {
        m_pWidget->Update(DeltaTime);
    }
}

bool CUIWidgetWorld::CollisionMouse()
{
    return false;
}

void CUIWidgetWorld::Collision()
{
    /*if (m_pWidget)
        m_pWidget->Collision();*/
}

void CUIWidgetWorld::PrevRender()
{
    if (m_pWidget)
        m_pWidget->PrevRender();
}

void CUIWidgetWorld::Render(HDC hDC, float DeltaTime)
{
    if (m_pWidget)
        m_pWidget->Render(hDC, DeltaTime, m_vPos);
}

CUIWidgetWorld* CUIWidgetWorld::Clone()
{
    return new CUIWidgetWorld(*this);
}
