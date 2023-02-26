#include "Jihanki.h"
#include "Player.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneMode.h"
#include "../GameManager.h"
#include "../Timer.h"
#include "../Scene/SceneResource.h"
#include "../UI/TextManager.h"

bool CJihanki::m_bCheckFirst = false;

CJihanki::CJihanki()	:
	m_bCanBuyJuse(true)
{
}

CJihanki::CJihanki(const CJihanki& obj)
{
}

CJihanki::~CJihanki()
{
	GET_TIMER->RemoveTimer("buy_juse_sound");
	GET_TIMER->RemoveTimer("goku_sound");
	GET_TIMER->RemoveTimer("item_get_sound");
	GET_TIMER->RemoveTimer("akikan");
	GET_TIMER->RemoveTimer("akikan2");
	GET_TIMER->RemoveTimer("akikan3");
	GET_TIMER->RemoveTimer("akikan4");
	GET_TIMER->RemoveTimer("CanBuyJuse");
}

bool CJihanki::Init()
{
	if (!CStaticObject::Init())
		return false;

	// Collider
	RectComponent = AddCollider<CColliderRect>("JihankyBody");
	RectComponent->SetExtent(100.f, 160.f);
	RectComponent->SetCollisionProfile("PlayerTrigger");
	RectComponent->SetBeginCollisionFunction(this, &CJihanki::BeginCollision);
	RectComponent->SetOffset(255.f, 175.f);

	//m_pWorldWidget = CreateWorldWidget<CUIWidgetWorld>();
	//m_pWorldWidget->SetOffset(0.f, -180.f);
	//CSharedPtr<CUIImage> pImage = m_pWorldWidget->GetImage();
	//pImage = m_pWorldWidget->GetWidget()->CreateControl<CUIImage>("SnailGauge3");
	//pImage->SetSize(64.f, 64.f);
	//pImage->SetZOrder(0);
	//pImage->AddAnimation("malpungsun", AnimationType::Sprite, false, "UI_Atlass00", 0.5f);
	//pImage->AddAnimationFrameData("malpungsun", 0, 21, 2, Vector2(256.f, 256.f));
	//pImage->SetPos(0.f, 20.f);

	AddAnimation("Jihanki_Anim", AnimationType::Sprite, true, "obj_Atlass00", 0.3f);
	AddAnimationFrameData("Jihanki_Anim", 0, 0, 2);

	return true;
}

CJihanki* CJihanki::Clone() const
{
	return new CJihanki(*this);
}

void CJihanki::CanBuyJuse()
{
	m_bCanBuyJuse = true;

	if (!m_bCheckFirst)
	{
		m_bCheckFirst = true;

#ifdef _DEBUG
#else
		CTextManager* pInst = GET_SINGLE(CTextManager);
		CPlayer* pPlayer = m_pScene->GetSceneMode()->GetPlayer();
		pInst->SetDialCode("#t15");
		pInst->SetEnableAllStop(true);
		pInst->SetEndDialogueFunc(pPlayer, &CPlayer::ReleaseDialogue);
		pInst->FindDialogueCursor();
		pPlayer->Dialogue();
#endif
	}
}

void CJihanki::BeginCollision(CCollider* pSrc, CCollider* pDest, float DeltaTime)
{
	if (pDest->GetCollisionProfile()->Name == "Player")
	{
		CPlayer* Player = Cast<CPlayer*>(pDest->GetOwner());

		Player->SetMaxStat();

		GET_TIMER->StartTimer("buy_juse_sound", 0.2f, this, &CJihanki::PlayBuyJuse);
		GET_TIMER->StartTimer("goku_sound", 1.f, this, &CJihanki::PlayGoku);
		GET_TIMER->StartTimer("item_get_sound", 1.2f, this, &CJihanki::PlayItem_get);
		GET_TIMER->StartTimer("akikan", 1.6f, this, &CJihanki::PlayAkikan);
		GET_TIMER->StartTimer("akikan2", 2.1f, this, &CJihanki::PlayAkikan);
		GET_TIMER->StartTimer("akikan3", 2.4f, this, &CJihanki::PlayAkikan);
		GET_TIMER->StartTimer("akikan4", 2.6f, this, &CJihanki::PlayAkikan2);
		GET_TIMER->StartTimer("CanBuyJuse", 3.f, this, &CJihanki::CanBuyJuse);

		m_bCanBuyJuse = false;
	}
}

void CJihanki::PlayBuyJuse()
{
	m_pScene->GetResourceManager()->SoundPlay("buy_juse");
}

void CJihanki::PlayAkikan()
{
	m_pScene->GetResourceManager()->SoundPlay("akikan");
}

void CJihanki::PlayAkikan2()
{
	m_pScene->GetResourceManager()->SoundPlay("akikan2");
}

void CJihanki::PlayGoku()
{
	m_pScene->GetResourceManager()->SoundPlay("goku");
}

void CJihanki::PlayItem_get()
{
	m_pScene->GetResourceManager()->SoundPlay("item_get");
}
