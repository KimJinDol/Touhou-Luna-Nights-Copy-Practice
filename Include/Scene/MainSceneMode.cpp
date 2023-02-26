#include "MainSceneMode.h"
#include "../Object/Archer.h"
#include "Scene.h"
#include "../Collision/ColliderRect.h"
#include "../Object/Portal.h"
#include "SceneResource.h"
#include "../Object/Smoke.h"
#include "Camera.h"
#include "../UI/MainSceneWidget.h"

CMainSceneMode::CMainSceneMode()
{
}

CMainSceneMode::~CMainSceneMode()
{
}

bool CMainSceneMode::Init()
{


	// 사운드 초기화
	//InitSound();

	//m_pScene->GetMainCamera()->SetWorldSize(5000.f, 5000.f);
	//m_pScene->GetMainCamera()->SetTargetPivot(0.5f, 0.5f);

	//CSharedPtr<CArcher> Player = m_pScene->CreateGameObject<CArcher>("Player",
	//	Vector2(0.f, 0.f), Vector2(82.f, 73.f));

	//m_pScene->GetMainCamera()->SetTarget(Player);

	//SetPlayer(Player);


	//CSharedPtr<CMonster> Monster = m_pScene->CreateGameObject<CMonster>("Monster",
	//	Vector2(1000.f, 300.f), Vector2(100.f, 100.f));

	//CSharedPtr<CColliderRect>	MonsterBody = Monster->AddCollider<CColliderRect>("MonsterBody");

	//MonsterBody->SetExtent(102.f, 102.f);
	////MonsterBody->SetOffset(50.f, 50.f);
	//MonsterBody->SetCollisionProfile("Monster");

	//Monster->SetPivot(0.5f, 0.5f);

	////Player->SetPivot(0.9f, 0.5f);

	//CSharedPtr<CPortal> Portal = m_pScene->CreateGameObject<CPortal>("Portal",
	//	Vector2(600.f, 600.f), Vector2(100.f, 100.f));

	//Portal->SetSceneType(SceneType::Dungeon);

	//CSharedPtr<CSmoke>	Smoke = m_pScene->CreatePrototype<CSmoke>("Smoke");

	//// BGM을 재생시켜준다.
	//m_pScene->GetResourceManager()->SoundPlay("BGM1");




	//// UI 생성
	//CSharedPtr<CMainSceneWidget> MainWidget = m_pScene->CreateWidget<CMainSceneWidget>("MainSceneWidget");


	return true;
}

void CMainSceneMode::InitSound()
{
	CSceneResource* pResource = m_pScene->GetResourceManager();

	/*pResource->LoadSound("BGM", true, "BGM1", "MainBgm.mp3");*/
	pResource->LoadSound("Effect", false, "Hit", "1Up.wav");
	pResource->LoadSound("Effect", false, "Portal", "Demasia.mp3");
	pResource->SetVolume(100);
}
