#include "TimeStopItem.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/SceneMode.h"
#include "../Scene/Scene.h"
#include "../Object/Player.h"

CTimeStopItem::CTimeStopItem()
{
}

CTimeStopItem::CTimeStopItem(const CTimeStopItem& obj)
{
}

CTimeStopItem::~CTimeStopItem()
{
}

bool CTimeStopItem::Init()
{
	if (!CStaticObject::Init())
		return false;

	// Collider
	RectComponent = AddCollider<CColliderRect>("TimeStopItemBody");
	RectComponent->SetExtent(60.f, 60.f);
	RectComponent->SetCollisionProfile("PlayerTrigger");
	RectComponent->SetBeginCollisionFunction(this, &CTimeStopItem::BeginCollision);
	RectComponent->SetOffset(120.f, 130.f);

	SetTexture("UI_Atlass00", Vector2(256.f, 5376.f), Vector2(256.f, 256.f));

    return true;
}

CTimeStopItem* CTimeStopItem::Clone() const
{
    return nullptr;
}

void CTimeStopItem::BeginCollision(CCollider* pSrc, CCollider* pDest, float DeltaTime)
{
	GET_SINGLE(CResourceManager)->SoundPlay("item_get");
	m_pScene->GetSceneMode()->GetPlayer()->LearnSkill("TimeStop");
	CRef::Destroy();
}
