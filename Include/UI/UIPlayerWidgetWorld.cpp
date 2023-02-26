#include "UIPlayerWidgetWorld.h"
#include "UIWidget.h"
#include "UITextNumber.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../GameManager.h"
#include "../Timer.h"

CUIPlayerWidgetWorld::CUIPlayerWidgetWorld()
{
}

CUIPlayerWidgetWorld::CUIPlayerWidgetWorld(const CUIPlayerWidgetWorld& pWidgetWorld)	:
	CUIWidgetWorld(pWidgetWorld)
{
}

CUIPlayerWidgetWorld::~CUIPlayerWidgetWorld()
{
	GET_TIMER->RemoveTimer("NoMP");
	GET_TIMER->RemoveTimer("FullMP");
	GET_TIMER->RemoveTimer("FullStat");
	GET_TIMER->RemoveTimer("NoTimeGauge");
	GET_TIMER->RemoveTimer("FullTimeGauge");
}

bool CUIPlayerWidgetWorld::Init()
{
	CUIWidgetWorld::Init();

	m_pSnailSocket = m_pWidget->CreateControl<CUIImage>("SnailSocket");
	m_pSnailSocket->SetTexture("UI_Snail_Atlass00");
	m_pSnailSocket->SetTextureImageStartPos(256.f, 256.f);
	m_pSnailSocket->SetSize(64.f, 64.f);
	m_pSnailSocket->SetZOrder(1);
	m_pSnailSocket->Enable(false);

	m_pSnailGauge1 = m_pWidget->CreateControl<CUIImage>("SnailGauge1");
	m_pSnailGauge1->SetSize(64.f, 64.f);
	m_pSnailGauge1->SetZOrder(0);
	m_pSnailGauge1->AddAnimation("SnailGauge_Anim1", AnimationType::Sprite, false, "UI_Snail_Atlass00", 0.75f / 4);
	m_pSnailGauge1->AddAnimationFrameData("SnailGauge_Anim1", 0, 0, 14, Vector2(64.f, 64.f));
	m_pSnailGauge1->SetPos(1.f, -23.f);
	m_pSnailGauge1->Enable(false);

	m_pSnailGauge2 = m_pWidget->CreateControl<CUIImage>("SnailGauge2");
	m_pSnailGauge2->SetSize(64.f, 64.f);
	m_pSnailGauge2->SetZOrder(0);
	m_pSnailGauge2->AddAnimation("SnailGauge_Anim2", AnimationType::Sprite, false, "UI_Snail_Atlass00", 0.75f / 4);
	m_pSnailGauge2->AddAnimationFrameData("SnailGauge_Anim2", 1, 1, 14, Vector2(64.f, 64.f));
	m_pSnailGauge2->SetPos(22.f, -1.f);
	m_pSnailGauge2->Enable(false);

	m_pSnailGauge3 = m_pWidget->CreateControl<CUIImage>("SnailGauge3");
	m_pSnailGauge3->SetSize(64.f, 64.f);
	m_pSnailGauge3->SetZOrder(0);
	m_pSnailGauge3->AddAnimation("SnailGauge_Anim3", AnimationType::Sprite, false, "UI_Snail_Atlass00", 0.75f / 4);
	m_pSnailGauge3->AddAnimationFrameData("SnailGauge_Anim3", 2, 2, 14, Vector2(64.f, 64.f));
	m_pSnailGauge3->SetPos(1.f, 21.f);
	m_pSnailGauge3->Enable(false);

	m_pSnailGauge4 = m_pWidget->CreateControl<CUIImage>("SnailGauge4");
	m_pSnailGauge4->SetSize(64.f, 64.f);
	m_pSnailGauge4->SetZOrder(0);
	m_pSnailGauge4->AddAnimation("SnailGauge_Anim4", AnimationType::Sprite, false, "UI_Snail_Atlass00", 0.75f / 4);
	m_pSnailGauge4->AddAnimationFrameData("SnailGauge_Anim4", 3, 3, 14, Vector2(64.f, 64.f));
	m_pSnailGauge4->SetPos(-21.f, -1.f);
	m_pSnailGauge4->Enable(false);

	// No Mp
	m_pNoMP = m_pWidget->CreateControl<CUIImage>("UI_NoMP");
	m_pNoMP->SetSize(256.f, 256.f);
	m_pNoMP->SetZOrder(0);
	m_pNoMP->AddAnimation("NoMP_Anim", AnimationType::Sprite, true, "UI_Atlass00", 0.2f);
	m_pNoMP->AddAnimationFrameData("NoMP_Anim", 8, 11, 2, Vector2(256.f, 256.f));
	m_pNoMP->SetPos(0.f, 20.f);
	m_pNoMP->Enable(false);

	// Full Mp
	m_pFullMP = m_pWidget->CreateControl<CUIImage>("UI_FullMP");
	m_pFullMP->SetSize(256.f, 256.f);
	m_pFullMP->SetZOrder(0);
	m_pFullMP->AddAnimation("FullMP_Anim", AnimationType::Sprite, true, "UI_Atlass00", 0.2f);
	m_pFullMP->AddAnimationFrameData("FullMP_Anim", 0, 12, 2, Vector2(256.f, 256.f));
	m_pFullMP->SetPos(0.f, 20.f);
	m_pFullMP->Enable(false);

	// Full Stat
	m_pFullStat = m_pWidget->CreateControl<CUIImage>("UI_FullStat");
	m_pFullStat->SetSize(256.f, 256.f);
	m_pFullStat->SetZOrder(0);
	m_pFullStat->AddAnimation("FullStat_Anim", AnimationType::Sprite, true, "UI_Atlass00", 0.2f);
	m_pFullStat->AddAnimationFrameData("FullStat_Anim", 2, 12, 2, Vector2(256.f, 256.f));
	m_pFullStat->SetPos(0.f, 20.f);
	m_pFullStat->Enable(false);

	// No TimeGauge
	m_pNoTimeGauge = m_pWidget->CreateControl<CUIImage>("UI_NoTimeGauge");
	m_pNoTimeGauge->SetSize(256.f, 256.f);
	m_pNoTimeGauge->SetZOrder(0);
	m_pNoTimeGauge->AddAnimation("NoTimeGauge_Anim", AnimationType::Sprite, true, "UI_Atlass00", 0.2f);
	m_pNoTimeGauge->AddAnimationFrameData("NoTimeGauge_Anim", 8, 12, 2, Vector2(256.f, 256.f));
	m_pNoTimeGauge->SetPos(0.f, 20.f);
	m_pNoTimeGauge->Enable(false);

	// Full TimeGauge
	m_pFullTimeGauge = m_pWidget->CreateControl<CUIImage>("UI_FulTimeGauge");
	m_pFullTimeGauge->SetSize(256.f, 256.f);
	m_pFullTimeGauge->SetZOrder(0);
	m_pFullTimeGauge->AddAnimation("FulTimeGauge_Anim", AnimationType::Sprite, true, "UI_Atlass00", 0.2f);
	m_pFullTimeGauge->AddAnimationFrameData("FulTimeGauge_Anim", 6, 12, 2, Vector2(256.f, 256.f));
	m_pFullTimeGauge->SetPos(0.f, 20.f);
	m_pFullTimeGauge->Enable(false);

	//// Graze
	//m_pGraze = m_pWidget->CreateControl<CUIImage>("UI_Graze");
	//m_pGraze->SetSize(256.f, 256.f);
	//m_pGraze->SetZOrder(0);
	//m_pGraze->AddAnimation("FulTimeGauge_Anim", AnimationType::Sprite, true, "UI_Atlass00", 0.2f);
	//m_pGraze->AddAnimationFrameData("FulTimeGauge_Anim", 6, 12, 2, Vector2(256.f, 256.f));
	//m_pGraze->SetPos(0.f, 20.f);
	//m_pGraze->Enable(false);

	// TimeStopKnifeNum Text
	m_pTimeStopKnifeNum = m_pWidget->CreateControl<CUITextNumber>("UI_TimeStopKnifeNum");
	m_pTimeStopKnifeNum->SetPos(Vector2(0.f, 10.f));
	m_pTimeStopKnifeNum->SetZOrder(2);
	m_pTimeStopKnifeNum->SetTexture("UI_Atlass00");
	m_pTimeStopKnifeNum->SetTextNumberTexture(0, 20, Vector2(256.f, 256.f));
	m_pTimeStopKnifeNum->SetSize(Vector2(25.f, 30.f));
	m_pTimeStopKnifeNum->SetMaxChipers(3);
	m_pTimeStopKnifeNum->SetTextImageSize(Vector2(256.f, 256.f));
	m_pTimeStopKnifeNum->SetTextXSize(24);
	m_pTimeStopKnifeNum->Enable(false);

	// Up Vision
	m_pUpVision = m_pWidget->CreateControl<CUIImage>("UI_UpVision");
	m_pUpVision->SetSize(256.f, 256.f);
	m_pUpVision->SetZOrder(0);
	m_pUpVision->AddAnimation("UpVision_Anim", AnimationType::Sprite, true, "UI_Atlass00", 0.2f);
	m_pUpVision->AddAnimationFrameData("UpVision_Anim", 0, 13, 2, Vector2(256.f, 256.f));
	m_pUpVision->SetPos(0.f, 20.f);
	m_pUpVision->Enable(false);

	return true;
}

void CUIPlayerWidgetWorld::FillSnailGauge(int idx)
{
	switch (idx)
	{
	case 0:
		m_pSnailGauge1->Enable(true);
		break;
	case 1:
		m_pSnailGauge2->Enable(true);
		break;
	case 2:
		m_pSnailGauge3->Enable(true);
		break;
	case 3:
		m_pSnailGauge4->Enable(true);
		break;
	}
}

void CUIPlayerWidgetWorld::SnailGaugeEnable(bool Value)
{
	if (Value)
	{
		m_pSnailSocket->Enable(true);
	}
	else
	{
		m_pSnailGauge1->InitializeAnimationFrame();
		m_pSnailGauge1->Enable(false);
		m_pSnailGauge2->InitializeAnimationFrame();
		m_pSnailGauge2->Enable(false);
		m_pSnailGauge3->InitializeAnimationFrame();
		m_pSnailGauge3->Enable(false);
		m_pSnailGauge4->InitializeAnimationFrame();
		m_pSnailGauge4->Enable(false);
		m_pSnailSocket->Enable(false);
	}
}

void CUIPlayerWidgetWorld::ShowNoMP()
{
	if (GET_TIMER->FindTimer("NoMP"))
		return;

	m_pNoMP->Enable(true);
	GET_TIMER->StartTimer("NoMP", 1.f, this, &CUIPlayerWidgetWorld::HideNoMP);
}

void CUIPlayerWidgetWorld::HideNoMP()
{
	m_pNoMP->Enable(false);
}

void CUIPlayerWidgetWorld::ShowFullMP()
{
	if (GET_TIMER->FindTimer("FullMP"))
		return;

	m_pFullMP->Enable(true);
	GET_TIMER->StartTimer("FullMP", 1.f, this, &CUIPlayerWidgetWorld::HideFullMP);

	m_pScene->GetResourceManager()->SoundPlay("mp_full");
}

void CUIPlayerWidgetWorld::HideFullMP()
{
	m_pFullMP->Enable(false);
}

void CUIPlayerWidgetWorld::ShowFullStat()
{
	if (GET_TIMER->FindTimer("FullStat"))
		return;

	if (m_pFullMP->IsEnable())
		m_pFullMP->Enable(false);

	if (m_pNoMP->IsEnable())
		m_pNoMP->Enable(false);

	m_pFullStat->Enable(true);
	GET_TIMER->StartTimer("FullStat", 1.f, this, &CUIPlayerWidgetWorld::HideFullStat);
}

void CUIPlayerWidgetWorld::HideFullStat()
{
	m_pFullStat->Enable(false);
}

void CUIPlayerWidgetWorld::ShowNoTimeGauge()
{
	if (GET_TIMER->FindTimer("NoTimeGauge"))
		return;

	m_pNoTimeGauge->Enable(true);
	GET_TIMER->StartTimer("NoTimeGauge", 1.f, this, &CUIPlayerWidgetWorld::HideNoTimeGauge);
}

void CUIPlayerWidgetWorld::HideNoTimeGauge()
{
	m_pNoTimeGauge->Enable(false);
}

void CUIPlayerWidgetWorld::ShowFullTimeGauge()
{
	if (GET_TIMER->FindTimer("FullTimeGauge"))
		return;

	m_pFullTimeGauge->Enable(true);
	GET_TIMER->StartTimer("FullTimeGauge", 1.f, this, &CUIPlayerWidgetWorld::HideFullTimeGauge);

	m_pScene->GetResourceManager()->SoundPlay("time_full");
}

void CUIPlayerWidgetWorld::HideFullTimeGauge()
{
	m_pFullTimeGauge->Enable(false);
}

void CUIPlayerWidgetWorld::SetKnifeNum(int KnifeNum)
{
	m_pTimeStopKnifeNum->Enable(true);
	m_pTimeStopKnifeNum->SetNumber(KnifeNum);
}

void CUIPlayerWidgetWorld::HideKnifeNum()
{
	m_pTimeStopKnifeNum->Enable(false);
}

void CUIPlayerWidgetWorld::ShowUpVision(bool Value)
{
	if (Value)
		m_pUpVision->Enable(true);
	else
		m_pUpVision->Enable(false);
}
