
#include "PlayerWidget.h"
#include "Button.h"
#include "UIImage.h"
#include "UIProgressBar.h"
#include "UITextNumber.h"
#include "UIWidgetWorld.h"
#include "UIText.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneMode.h"
#include "../GameManager.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Texture.h"
#include "../Math.h"
#include "../Object/Player.h"
#include "../GameManager.h"
#include "../Timer.h"
#include "../Object/PlayerEffect.h"

CPlayerWidget::CPlayerWidget()  :
    m_bSecondHandSound(false),
    m_fSecondHandTime(0.2f),
    m_fCurrentSecondHandTime(m_fSecondHandTime)
{
}

CPlayerWidget::~CPlayerWidget()
{
}



void CPlayerWidget::SetLevel(int level)
{
    m_pLevelText->SetNumber(level);
}

void CPlayerWidget::SetHP(int hp)
{
    m_pHPBar->SetValue(BasicMath::Clamp<int>(hp, 0, 100));
    m_pHPText->SetNumber(hp);
}

void CPlayerWidget::SetMP(int mp)
{
    m_pMPBar->SetValue(BasicMath::Clamp<int>(mp, 0, 100));
    m_pMPText->SetNumber(mp);
}

void CPlayerWidget::SetTimeStopGauge(int TimeStopGauge)
{
    m_pTimeStopTextNumber->SetNumber(TimeStopGauge);
}

void CPlayerWidget::PlayTimeGaugeEffect()
{
    CSharedPtr<CPlayerEffect> timeGaugeEffect = m_pScene->CreateGameObject<CPlayerEffect>("TimeGauge_Effect", Vector2(651.5f, 95.f), Vector2(256.f, 256.f));
    timeGaugeEffect->SetIsInfluenceCameraPos(false);
    timeGaugeEffect->SetOwner(m_pOwner);
    timeGaugeEffect->SetPivot(0.5f, 0.5f);
    timeGaugeEffect->AddAnimation("TimeGauge_Effect_Anim", AnimationType::Sprite, "UI_Atlass00", .15f);
    timeGaugeEffect->AddAnimationFrameData("TimeGauge_Effect_Anim", 3, 21, 6, Vector2(256.f, 256.f), Vector2(0.5f, 0.5f));
    timeGaugeEffect->SetAutoDestroy();
    timeGaugeEffect->SetOffset(Vector2(-0.f, 0.f));
}

void CPlayerWidget::SetTimeState(TimeMode eMode)
{
    switch (eMode)
    {
    case TimeMode::Move:
    {
        if (m_pScene->GetSceneMode()->GetTimeMode() == TimeMode::Stop)
            m_pTimeState->ChangeAnimation("StopToMove");
        else if (m_pScene->GetSceneMode()->GetTimeMode() == TimeMode::Snail)
            m_pTimeState->ChangeAnimation("SnailToMove");
        else
            m_pTimeState->ChangeAnimation("MoveToMove");
    }
    break;
    case TimeMode::Stop:
    {
        if (m_pScene->GetSceneMode()->GetTimeMode() == TimeMode::Move)
            m_pTimeState->ChangeAnimation("MoveToStop");
        else if (m_pScene->GetSceneMode()->GetTimeMode() == TimeMode::Snail)
            m_pTimeState->ChangeAnimation("SnailToMStop");
        else
            m_pTimeState->ChangeAnimation("StopToStop");
    }
    break;
    case TimeMode::Snail:
    {
        if (m_pScene->GetSceneMode()->GetTimeMode() == TimeMode::Stop)
            m_pTimeState->ChangeAnimation("StopToSnail");
        else if (m_pScene->GetSceneMode()->GetTimeMode() == TimeMode::Move)
            m_pTimeState->ChangeAnimation("MoveToSnail");
        else
            m_pTimeState->ChangeAnimation("SnailToSnail");
    }
    break;
    }
}

void CPlayerWidget::PlaySecondeHandSound()
{
    m_bSecondHandSound = true;
    m_pScene->GetResourceManager()->SoundPlay("secondHand");
}

void CPlayerWidget::SetText(const std::string& Text)
{
    m_pText->Enable(true);
    m_pText->SetText(Text);
}

void CPlayerWidget::HideText()
{
    m_pText->Enable(false);
}

void CPlayerWidget::SetEnableTimeStopWidget(bool Value)
{
    m_pTimeStopTextNumber->Enable(Value);
}

void CPlayerWidget::SetWidgetEnable(bool Value)
{
    m_pMainUIImage->Enable(Value);
	m_pLevelText->Enable(Value);
	m_pHPBar->Enable(Value);
	m_pHPText->Enable(Value);
	m_pMPBar->Enable(Value);
	m_pMPText->Enable(Value);
	m_pTimeStopTextNumber->Enable(Value);
	m_pTimeState->Enable(Value);
    m_pBlackMask->Enable(Value);
    m_pItemBack->Enable(Value);
}

void CPlayerWidget::SetEnableItemWidget(bool Value)
{
    m_pItemBack->Enable(false);
    m_pBlackMask->Enable(false);
    m_pItemImage->Enable(false);
}

void CPlayerWidget::GetItem(const std::string& Name)
{
    m_pBlackMask->Enable(true);
    m_pItemBack->Enable(true);
    m_pItemImage->Enable(true);

    if (Name == "TimeStop")
    {
        m_pItemBack->SetText("영구 시계를 획득했다. 시간을 자유로이 멈출 수 있다.",
            Vector2(m_pItemBack->GetPos().x + m_pItemBack->GetSize().x / 2, m_pItemBack->GetPos().y + 50.f));
        /*Vector2(m_pItemBack->GetPos().x + m_pItemBack->GetSize().x / 2, m_pItemBack->GetPos().y + 30.f)*/
    }

}

bool CPlayerWidget::Init()
{
    if (!CUIWidget::Init())
        return false;

    m_vDefaultPos = Vector2(20.f, 30.f);

    // In Game Main HUD
    m_pMainUIImage = CreateControl<CUIImage>("UI_InGameMain");
    
    m_pMainUIImage->SetTexture("UI_InGameMain_Tex", TEXT("UI/UI_InGame_Main.bmp"));
    m_pMainUIImage->SetPivot(0.5f, 0.5f);


	Vector2 vSize = Vector2(m_pScene->GetResourceManager()->FindTexture("UI_InGameMain_Tex")->GetTextureInfo()->BmpInfo.bmWidth,
		m_pScene->GetResourceManager()->FindTexture("UI_InGameMain_Tex")->GetTextureInfo()->BmpInfo.bmHeight);
    m_pMainUIImage->SetSize(vSize);
    m_pMainUIImage->SetPos(Vector2(GET_RESOLUTION.Width / 2 - 60, 129 / 2));
    m_pMainUIImage->SetZOrder(1);

	// Level Text
	m_pLevelText = CreateControl<CUITextNumber>("UI_LevelText");
	m_pLevelText->SetPos(Vector2(118.f, 12.f));
	m_pLevelText->SetZOrder(2);
	m_pLevelText->SetTexture("UI_Atlass00");
	m_pLevelText->SetTextNumberTexture(0, 9, Vector2(256.f, 256.f));
	m_pLevelText->SetSize(Vector2(25.f, 30.f));
	m_pLevelText->SetMaxChipers(3);
	m_pLevelText->SetTextImageSize(Vector2(256.f, 256.f));
	m_pLevelText->SetTextXSize(24);

    // HP Bar
    m_pHPBar = CreateControl<CUIProgressBar>("UI_HP");
    m_pHPBar->SetPos(Vector2(300.f, 36.f));
    m_pHPBar->AddAnimation("UI_HP", AnimationType::Sprite, true, "UI_PrograssBar_Atlass00", 0.5f);
    m_pHPBar->AddAnimationFrameData("UI_HP", 0, 0, 20, Vector2(200.f, 200.f), Vector2(0.5f, 0.5f));
    m_pHPBar->SetZOrder(0);
    m_pHPBar->SetMax(100.f);
    m_pHPBar->SetMin(0.f);
    m_pHPBar->SetValue(m_pHPBar->GetMax());
    m_pHPBar->SetSize(Vector2(200.f, 13.f));

    // HP Text
	m_pHPText = CreateControl<CUITextNumber>("UI_HPText");
	m_pHPText->SetPos(Vector2(166.f, 3.f));
	m_pHPText->SetZOrder(2);
	m_pHPText->SetTexture("UI_Atlass00");
	m_pHPText->SetTextNumberTexture(0, 10, Vector2(256.f, 256.f));
	m_pHPText->SetSize(Vector2(25.f, 30.f));
	m_pHPText->SetMaxChipers(3);
	m_pHPText->SetTextImageSize(Vector2(256.f, 256.f));
	m_pHPText->SetTextXSize(7);

    // MP Bar
    m_pMPBar = CreateControl<CUIProgressBar>("UI_MP");
    m_pMPBar->SetPos(Vector2(300.f, 56.f));
    m_pMPBar->AddAnimation("UI_MP", AnimationType::Sprite, true, "UI_PrograssBar_Atlass00", 0.5f);
    m_pMPBar->AddAnimationFrameData("UI_MP", 0, 2, 20, Vector2(200.f, 200.f), Vector2(0.5f, 0.5f));
    m_pMPBar->SetZOrder(0);
    m_pMPBar->SetMax(100.f);
    m_pMPBar->SetMin(0.f);
    m_pMPBar->SetValue(m_pMPBar->GetMax());
    m_pMPBar->SetSize(Vector2(200.f, 13.f));

	// MP Text
	m_pMPText = CreateControl<CUITextNumber>("UI_MPText");
	m_pMPText->SetPos(Vector2(166.f, 27.f));
	m_pMPText->SetZOrder(2);
	m_pMPText->SetTexture("UI_Atlass00");
	m_pMPText->SetTextNumberTexture(0, 10, Vector2(256.f, 256.f));
	m_pMPText->SetSize(Vector2(25.f, 30.f));
	m_pMPText->SetMaxChipers(3);
	m_pMPText->SetTextImageSize(Vector2(256.f, 256.f));
	m_pMPText->SetTextXSize(7);

    // TimeStopGauge Text
    m_pTimeStopTextNumber = CreateControl<CUITextNumber>("UI_TimeStopText");
    m_pTimeStopTextNumber->SetPos(Vector2(610.f, 35.f));
    m_pTimeStopTextNumber->SetZOrder(2);
    m_pTimeStopTextNumber->SetTexture("UI_Atlass00");
    m_pTimeStopTextNumber->SetTextNumberTexture(6, 7, Vector2(256.f, 256.f));
    m_pTimeStopTextNumber->SetSize(Vector2(25.f, 30.f));
    m_pTimeStopTextNumber->SetMaxChipers(3);
    m_pTimeStopTextNumber->SetTextImageSize(Vector2(256.f, 256.f));
    m_pTimeStopTextNumber->SetTextXSize(24);

    // Time State
    m_pTimeState = CreateControl<CUIImage>("UI_TimeState");
    m_pTimeState->SetPivot(0.5f, 0.5f);
    m_pTimeState->SetSize(40.f, 20.f);
    m_pTimeState->SetPos(490.f, 175.f);
    m_pTimeState->SetZOrder(2);
    m_pTimeState->AddAnimation("SnailToMove", AnimationType::Sprite, false, "UI_Atlass00", 0.3f);
    m_pTimeState->AddAnimationFrameData("SnailToMove", 1, 14, 8, Vector2(256.f, 256.f));
    m_pTimeState->AddAnimation("StopToMove", AnimationType::Sprite, false, "UI_Atlass00", 0.6f);
    m_pTimeState->AddAnimationFrameData("StopToMove", 3, 13, 16, Vector2(256.f, 256.f));
    m_pTimeState->AddAnimation("MoveToMove", AnimationType::Sprite, false, "UI_Atlass00", 0.1f); 
    m_pTimeState->AddAnimationFrameData("MoveToMove", 8, 14, 1, Vector2(256.f, 256.f));

    m_pTimeState->AddAnimation("MoveToStop", AnimationType::Sprite, false, "UI_Atlass00", 0.3f);
    m_pTimeState->AddAnimationFrameData("MoveToStop", 9, 14, 7, Vector2(256.f, 256.f));
    m_pTimeState->AddAnimationFrameData("MoveToStop", 2, 13, 1, Vector2(256.f, 256.f));
    m_pTimeState->AddAnimation("SnailToStop", AnimationType::Sprite, false, "UI_Atlass00", 0.6f);
    m_pTimeState->AddAnimationFrameData("SnailToStop", 1, 14, 16, Vector2(256.f, 256.f));
    m_pTimeState->AddAnimation("StopToStop", AnimationType::Sprite, false, "UI_Atlass00", 0.1f);
    m_pTimeState->AddAnimationFrameData("StopToStop", 2, 13, 1, Vector2(256.f, 256.f));

    m_pTimeState->AddAnimation("StopToSnail", AnimationType::Sprite, false, "UI_Atlass00", 0.3f);
    m_pTimeState->AddAnimationFrameData("StopToSnail", 3, 13, 8, Vector2(256.f, 256.f));
    m_pTimeState->AddAnimation("MoveToSnail", AnimationType::Sprite, false, "UI_Atlass00", 0.6f);
    m_pTimeState->AddAnimationFrameData("MoveToSnail", 9, 14, 7, Vector2(256.f, 256.f));
    m_pTimeState->AddAnimationFrameData("MoveToSnail", 2, 13, 9, Vector2(256.f, 256.f));
    m_pTimeState->AddAnimation("SnailToSnail", AnimationType::Sprite, false, "UI_Atlass00", 0.1f);
    m_pTimeState->AddAnimationFrameData("SnailToSnail", 0, 14, 1, Vector2(256.f, 256.f));

    // BlackMask
	m_pBlackMask = CreateControl<CUIImage>("UI_BlackMase");
    m_pBlackMask->SetPivot(0.f, 0.f);
    m_pBlackMask->SetSize(GET_RESOLUTION.Width, GET_RESOLUTION.Height);
    m_pBlackMask->SetPos(m_vDefaultPos * -1);
    m_pBlackMask->SetZOrder(3);
    m_pBlackMask->SetTexture("BlackGroundWidget");
    m_pBlackMask->GetTexture()->SetEnableAlpha(true);
    m_pBlackMask->GetTexture()->SetAlpha(200);


	// Item BackGround
	m_pItemBack = CreateControl<CUIImage>("UI_ItemBackGround");
    m_pItemBack->SetPivot(0.f, 0.f);
    m_pItemBack->SetSize(1280.f, 275.f);
    m_pItemBack->SetPos(m_vDefaultPos.x * -1, m_vDefaultPos.y * -1 + 200.f);
    m_pItemBack->SetZOrder(4);
    m_pItemBack->SetTexture("ItemBackGround");

	// Item Image
	m_pItemImage = CreateControl<CUIImage>("UI_ItemImage");
    m_pItemImage->SetPivot(0.5f, 0.5f);
    m_pItemImage->SetSize(480.f, 100.f);
    m_pItemImage->SetPos(m_pItemBack->GetPos().x + m_pItemBack->GetSize().x / 2, m_pItemBack->GetPos().y + 150.f);
    m_pItemImage->SetZOrder(5);
    m_pItemImage->SetTexture("UI_item_vision_timeStop");
    
    SetEnableItemWidget(false);

    // Text
    m_pText = CreateControl<CUIText>("UI_TextBox");
    m_pText->SetZOrder(2);

    return true;
}

void CPlayerWidget::Update(float DeltaTime)
{
    if (!m_pOwner || m_pOwner->IsDestroy()) return;

	CPlayer* pPlayer = Cast<CPlayer*>(m_pOwner);

	SetHP(pPlayer->GetHP());
	SetMP(pPlayer->GetMP());

    if (m_bSecondHandSound && m_pScene->GetSceneMode()->GetTimeMode() == TimeMode::Stop)
    {
        m_fCurrentSecondHandTime -= DeltaTime;

        if (m_fCurrentSecondHandTime <= 0.f)
        {
            m_pScene->GetResourceManager()->SoundPlay("secondHand");
            m_fCurrentSecondHandTime = m_fSecondHandTime;
        }
    }

	CUIWidget::Update(DeltaTime);
}
