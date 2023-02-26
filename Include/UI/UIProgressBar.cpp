
#include "UIProgressBar.h"

CUIProgressBar::CUIProgressBar() :
    m_Min(0.f),
    m_Max(1.f),
    m_Value(1.f)
{
}

CUIProgressBar::~CUIProgressBar()
{
}

bool CUIProgressBar::Init()
{
    if (!CUIControl::Init())
        return false;

    return true;
}

void CUIProgressBar::Update(float DeltaTime)
{
    CUIControl::Update(DeltaTime);
}

bool CUIProgressBar::CollisionMouse(const Vector2& MousePos)
{
    bool    bResult = CUIControl::CollisionMouse(MousePos);

    return bResult;
}

void CUIProgressBar::Collision()
{
    CUIControl::PrevRender();
}

void CUIProgressBar::PrevRender()
{
    CUIControl::PrevRender();
}

void CUIProgressBar::Render(HDC hDC, float DeltaTime)
{
    float   Ratio = (m_Value - m_Min) / (m_Max - m_Min);
        
    RenderRatio(hDC, DeltaTime, Ratio);
}

void CUIProgressBar::Render(HDC hDC, float DeltaTime, const Vector2& vPos)
{
    float   Ratio = (m_Value - m_Min) / (m_Max - m_Min);

    RenderRatio(hDC, DeltaTime, vPos, Ratio);
}

void CUIProgressBar::CollisionEnd()
{
}
