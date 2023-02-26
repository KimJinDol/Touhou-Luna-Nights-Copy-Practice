
#include "DungeonSceneMode.h"
#include "../Object/Player.h"
#include "Scene.h"
#include "../Collision/ColliderRect.h"

CDungeonSceneMode::CDungeonSceneMode()
{
}

CDungeonSceneMode::~CDungeonSceneMode()
{
}

bool CDungeonSceneMode::Init()
{
	CSharedPtr<CPlayer> Player = m_pScene->CreateGameObject<CPlayer>("Player",
		Vector2(0.f, 0.f), Vector2(100.f, 100.f));

	CSharedPtr<CColliderRect>	PlayerBody = Player->AddCollider<CColliderRect>("PlayerBody");

	PlayerBody->SetExtent(102.f, 102.f);
	PlayerBody->SetOffset(50.f, 50.f);
	PlayerBody->SetCollisionProfile("Player");

	SetPlayer(Player);


	/*CSharedPtr<CMonster> Monster = m_pScene->CreateGameObject<CMonster>("Monster",
		Vector2(1000.f, 300.f), Vector2(100.f, 100.f));

	CSharedPtr<CColliderRect>	MonsterBody = Monster->AddCollider<CColliderRect>("MonsterBody");

	MonsterBody->SetExtent(102.f, 102.f);
	MonsterBody->SetCollisionProfile("Monster");

	Monster->SetPivot(0.5f, 0.5f);*/

	return true;
}
