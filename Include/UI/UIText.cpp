#include "UIText.h"
#include "TextManager.h"
#include "../Scene/SceneMode.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/Camera.h"

CUIText::CUIText()
{
}

CUIText::~CUIText()
{
}


void CUIText::SetLeftTexture(const std::string& Name, const Vector2& ImageStartPos, const Vector2& PixelSize, const std::string& PathName)
{
    m_LeftTex = m_pScene->GetResourceManager()->FindTexture(Name);
    m_LeftStartPos = ImageStartPos;
    m_LeftSize = PixelSize;
}

void CUIText::SetRightTexture(const std::string& Name, const Vector2& ImageStartPos, const Vector2& PixelSize, const std::string& PathName)
{
    m_RightTex = m_pScene->GetResourceManager()->FindTexture(Name);
    m_RightStartPos = ImageStartPos;
    m_RightSize = PixelSize;
}

void CUIText::SetBoxType(MessageBoxType eType)
{
    m_BoxType = eType;

    switch (m_BoxType)
    {
    case MessageBoxType::MBT_1:
        SetTexture("UI_Atlass00", Vector2(0.f, 4864.f), Vector2(256.f, 256.f));
        break;
    case MessageBoxType::MBT_2:
        SetTexture("UI_Atlass00", Vector2(256.f, 4864.f), Vector2(256.f, 256.f));
        break;
    case MessageBoxType::MBT_3:
        SetTexture("UI_Atlass00", Vector2(512.f, 4864.f), Vector2(256.f, 256.f));
        break;
    case MessageBoxType::MBT_4:
        SetTexture("UI_Atlass00", Vector2(1024.f, 4864.f), Vector2(256.f, 256.f));
        break;
    }
}

void CUIText::SetText(const std::string& Text)
{
    m_Text = Text;

    MessageBoxType Type = GET_SINGLE(CTextManager)->GetBoxType();
    switch (Type)
    {
    case MessageBoxType::MBT_1:
    {
        SetTexture("UI_Atlass00", Vector2(122.f, 4974.f), Vector2(12.f, 36.f));
        m_Mask = m_pScene->GetResourceManager()->FindTexture("UI_message1_mask");
        Vector2 Pos = Vector2(m_pOwner->GetOwner()->GetPos().x - m_pScene->GetMainCamera()->GetPos().x, m_pOwner->GetOwner()->GetPos().y - m_pOwner->GetOwner()->GetSize().y - 50.f - m_pScene->GetMainCamera()->GetPos().y);
        m_vPos = Pos;
        SetLeftTexture("UI_Atlass00", Vector2(1128.f, 4974.f), Vector2(18.f, 36.f));
        SetRightTexture("UI_Atlass00", Vector2(1414.f, 4974.f), Vector2(18.f, 36.f));
        m_AddedCenterSize.x = 8.f;
        m_AddedCenterSize.y = 10.f;
    }
    break;
    case MessageBoxType::MBT_2:
    {
        SetTexture("UI_Atlass00", Vector2(372.f, 4968.f), Vector2(24.f, 48.f));
        m_Mask = m_pScene->GetResourceManager()->FindTexture("UI_message2_mask");
        Vector2 Pos = Vector2(1050.f, 250.f);
        m_vPos = Pos;
        SetLeftTexture("UI_Atlass00", Vector2(1640.f, 4968.f), Vector2(18.f, 48.f));
        SetRightTexture("UI_Atlass00", Vector2(1932, 4968.f), Vector2(18.f, 48.f));
        m_AddedCenterSize.x = 8.f;
        m_AddedCenterSize.y = 11.f;
    }
    break;
    case MessageBoxType::MBT_3:
    {
        SetTexture("UI_Atlass00", Vector2(887.f, 4968.f), Vector2(19.f, 48.f));
        m_Mask = m_pScene->GetResourceManager()->FindTexture("UI_message2_mask");
        Vector2 Pos = Vector2(m_pOwner->GetOwner()->GetPos().x - m_pScene->GetMainCamera()->GetPos().x, m_pOwner->GetOwner()->GetPos().y - m_pOwner->GetOwner()->GetSize().y - 50.f - m_pScene->GetMainCamera()->GetPos().y);
        m_vPos = Pos;
        SetLeftTexture("UI_Atlass00", Vector2(2163.f, 4968.f), Vector2(18.f, 48.f));
        SetRightTexture("UI_Atlass00", Vector2(2422.f, 4968.f), Vector2(18.f, 48.f));
        m_AddedCenterSize.x = 8.f;
        m_AddedCenterSize.y = 13.f;
    }
    break;
    }
    
}

bool CUIText::Init()
{
    if (!CUIControl::Init())
        return false;

    return true;
}

void CUIText::Update(float DeltaTime)
{
    CUIControl::Update(DeltaTime);
}

bool CUIText::CollisionMouse(const Vector2& MousePos)
{
    return CUIControl::CollisionMouse(MousePos);
}

void CUIText::Collision()
{
    CUIControl::Collision();
}

void CUIText::PrevRender()
{
    CUIControl::PrevRender();
}

void CUIText::Render(HDC hDC, float DeltaTime)
{
    // CUIControl::Render(hDC, DeltaTime);

    Vector2 vRenderPos; 

    if (m_Texture)
    {
        Vector2 Size = Vector2(m_vTexturePixelSize.x + (m_AddedCenterSize.x * m_Text.length()), m_vTexturePixelSize.y + m_AddedCenterSize.y);

        vRenderPos = m_vPos - (m_vTexturePixelSize) * m_vPivot;

        Vector2 Center = vRenderPos + (m_vTexturePixelSize / 2);

        // 각 포인트마다 센터에서부터 거리 구하기
        float Distance[3];
        Distance[0] = Center.Distance(Center - Size / 2);
        Distance[1] = Center.Distance(Vector2(Center.x + Size.x / 2, Center.y - Size.y / 2));
        Distance[2] = Center.Distance(Vector2(Center.x - Size.x / 2, Center.y + Size.y / 2));

        // 회전 변환
        POINT PointArr[3];

        float Ratio = 45 * (Size.y / Size.x);
        PointArr[0].x = (LONG)(cosf((180 + Ratio) * PI / 180.f) * Distance[0]) + Center.x;
        PointArr[0].y = (LONG)(sinf((180 + Ratio) * PI / 180.f) * Distance[0]) + Center.y;

        PointArr[1].x = (LONG)(cosf((360 - Ratio) * PI / 180.f) * Distance[1]) + Center.x;
        PointArr[1].y = (LONG)(sinf((360 - Ratio) * PI / 180.f) * Distance[1]) + Center.y;

        PointArr[2].x = (LONG)(cosf((180 - Ratio) * PI / 180.f) * Distance[2]) + Center.x;
        PointArr[2].y = (LONG)(sinf((180 - Ratio) * PI / 180.f) * Distance[2]) + Center.y;

        PlgBlt(hDC, PointArr, m_Texture->GetTextureInfo()->hDC, m_vTextureImageStartPos.x, m_vTextureImageStartPos.y,
            (int)m_vTexturePixelSize.x, (int)m_vTexturePixelSize.y, m_Mask->GetTextureInfo()->hBmp, 0, 0);

        Vector2 vRenderPos2 = Vector2(PointArr[0].x - m_LeftSize.x / 2, PointArr[0].y);
        Vector2 vRenderPos3 = Vector2(PointArr[1].x, PointArr[1].y);

        TransparentBlt(hDC, (int)vRenderPos2.x, (int)vRenderPos2.y, (int)m_LeftSize.x,
            (int)m_LeftSize.y, m_LeftTex->GetTextureInfo()->hDC, (int)m_LeftStartPos.x, (int)m_LeftStartPos.y,
            (int)m_LeftSize.x, (int)m_LeftSize.y, RGB(255, 0, 255));

        TransparentBlt(hDC, (int)vRenderPos3.x, (int)vRenderPos3.y, (int)m_RightSize.x,
            (int)m_RightSize.y, m_RightTex->GetTextureInfo()->hDC, (int)m_RightStartPos.x, (int)m_RightStartPos.y,
            (int)m_RightSize.x, (int)m_RightSize.y, RGB(255,0,255));

    }

    else
    {
        Rectangle(hDC, (int)vRenderPos.x, (int)vRenderPos.y, (int)(vRenderPos.x + m_vSize.x),
            (int)(vRenderPos.y + m_vSize.y));
    }

    // 텍스트 출력,,,
    char text[256] = {};
    sprintf_s(text, m_Text.c_str());
    SetBkMode(hDC, TRANSPARENT);
    SetTextAlign(hDC, TA_CENTER);
    SetTextColor(hDC, RGB(255,255,255));
    TextOutA(hDC, m_vPos.x, m_vPos.y - 7.f, text, (int)strlen(text));
    SetTextAlign(hDC, TA_LEFT);
}
