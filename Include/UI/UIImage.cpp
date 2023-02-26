
#include "UIImage.h"

CUIImage::CUIImage()    :
    m_Text{},
    m_IsText(false)
{
}

CUIImage::~CUIImage()
{
}

bool CUIImage::Init()
{
	if (!CUIControl::Init())
		return false;

    return true;
}

void CUIImage::SetText(const char* Text, const Vector2& Pos)
{
    m_IsText = true;
	sprintf_s(m_Text, Text);	
    m_TextPos = Pos;
}

void CUIImage::Update(float DeltaTime)
{
    CUIControl::Update(DeltaTime);
}

bool CUIImage::CollisionMouse(const Vector2& MousePos)
{
    return CUIControl::CollisionMouse(MousePos);
}

void CUIImage::Collision()
{
    CUIControl::Collision();
}

void CUIImage::PrevRender()
{
    CUIControl::PrevRender();
}

void CUIImage::Render(HDC hDC, float DeltaTime)
{
    CUIControl::Render(hDC, DeltaTime);

	SetTextAlign(hDC, TA_CENTER);

	if (m_IsText)
		TextOutA(hDC, m_TextPos.x, m_TextPos.y, m_Text, (int)strlen(m_Text));


	SetTextAlign(hDC, TA_LEFT);
}

void CUIImage::Render(HDC hDC, float DeltaTime, const Vector2& vPos)
{
    CUIControl::Render(hDC, DeltaTime, vPos);

    if (m_IsText)
        TextOutA(hDC, m_TextPos.x, m_TextPos.y, m_Text, (int)strlen(m_Text));
}
