#include "Setsmei.h"
#include "Player.h"
#include "../UI/UIPlayerWidgetWorld.h"
#include "../UI//UIImage.h"
#include "../UI/TextManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneMode.h"

CSetsmei::CSetsmei()	:
	m_BlinkTime(0.2f),
	m_CurBlinkTime(m_BlinkTime)
{
}

CSetsmei::CSetsmei(const CSetsmei& obj)
{
}

CSetsmei::~CSetsmei()
{
}

bool CSetsmei::Init()
{
    if (!CStaticObject::Init())
        return false;

	// Collider
	RectComponent = AddCollider<CColliderRect>("SetsmeiBody");
	RectComponent->SetExtent(85.f, 130.f);
	RectComponent->SetCollisionProfile("PlayerTrigger");
	RectComponent->SetBeginCollisionFunction(this, &CSetsmei::BeginCollision);
	RectComponent->SetEndCollisionFunction(this, &CSetsmei::EndCollision);
	RectComponent->SetOffset(130.f, 220.f);
	
	//m_pWorldWidget = CreateWorldWidget<CUIWidgetWorld>();
	//m_pWorldWidget->SetOffset(0.f, -180.f);
	//CSharedPtr<CUIImage> pImage = m_pWorldWidget->GetImage();
	//pImage = m_pWorldWidget->GetWidget()->CreateControl<CUIImage>("SnailGauge3");
	//pImage->SetSize(64.f, 64.f);
	//pImage->SetZOrder(0);
	//pImage->AddAnimation("malpungsun", AnimationType::Sprite, false, "UI_Atlass00", 0.5f);
	//pImage->AddAnimationFrameData("malpungsun", 0, 21, 2, Vector2(256.f, 256.f));
	//pImage->SetPos(0.f, 20.f);

	SetTexture("UI_Atlass00", Vector2(0.f, 5376.f), Vector2(256.f, 256.f));

    return true;
}

void CSetsmei::Start()
{
	CStaticObject::Start();
}

void CSetsmei::Update(float DeltaTime)
{
	CStaticObject::Update(DeltaTime);

	m_CurBlinkTime -= DeltaTime;
	if (m_CurBlinkTime <= 0.f)
	{
		m_CurBlinkTime = m_BlinkTime;
		if (m_bIsRender)
			m_bIsRender = false;
		else
			m_bIsRender = true;
	}
}

void CSetsmei::Collision()
{
	CStaticObject::Collision();
}

void CSetsmei::Render(HDC hDC, float DeltaTime)
{
	CStaticObject::Render(hDC, DeltaTime);
}

CSetsmei* CSetsmei::Clone() const
{
	return new CSetsmei(*this);
}

void CSetsmei::BeginCollision(CCollider* pSrc, CCollider* pDest, float DeltaTime)
{
	if (pDest->GetCollisionProfile()->Name == "Player")
	{
		CPlayer* Player = Cast<CPlayer*>(pDest->GetOwner());

		Player->GetPlayerWorldWidget()->ShowUpVision(true);
		CTextManager::GetInst()->SetDialCode(m_dialCode);
		CTextManager::GetInst()->SetEndDialogueFunc(m_pScene->GetSceneMode(), &CSceneMode::ReleaseAllStop);
	}
}

void CSetsmei::EndCollision(CCollider* pSrc, CCollider* pDest, float DeltaTime)
{
	if (pDest->GetCollisionProfile()->Name == "Player")
	{
		CPlayer* Player = Cast<CPlayer*>(pDest->GetOwner());

		Player->GetPlayerWorldWidget()->ShowUpVision(false);
		CTextManager::GetInst()->SetDialCode("");
	}
}
