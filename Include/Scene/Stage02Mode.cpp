#include "Stage02Mode.h"
#include "SceneResource.h"
#include "Scene.h"
#include "Camera.h"
#include "../Object/Player.h"
#include "../UI/StartSceneWidget.h"
#include "../Object/Map.h"
#include "../Object/WolfAI.h"
#include "../Object/Bullet.h"
#include "../Object/PlayerEffect.h"
#include "../Stage/Stage.h"
#include "../Object/Portal.h"
#include "../Object/Jihanki.h"

CStage02Mode::CStage02Mode()
{
}

CStage02Mode::~CStage02Mode()
{
}

bool CStage02Mode::Init()
{
	CSceneMode::Init();

	CSharedPtr<CPlayer> Player = m_pScene->CreateGameObject<CPlayer>("Player",
		Vector2(0.f, 0.f), Vector2(100.f, 100.f));

	Player->SetPos(Vector2(600.f, 300.f));
	m_pScene->GetMainCamera()->SetTarget(Player);
	SetPlayer(Player);

	CStage* MainStage = m_pScene->CreateStage<CStage>();

	MainStage->LoadFile("Stage02.stg", DATA_PATH);
	MainStage->SetExitSide(true, true);
	m_pScene->GetMainCamera()->SetTargetPivot(0.5f, 0.f);
	m_pScene->GetMainCamera()->SetWorldSize(1360, 1500.f);
#ifdef _DEBUG
	m_pScene->GetMainCamera()->SetWorldSize(1496.f, 1500.f);
#endif
	// Left Portal
	{
		CSharedPtr<CPortal> Portal_Left = m_pScene->CreateGameObject<CPortal>("Portal",
			Vector2(-68.f, 380.f), Vector2(34.f, 500.f));

		Portal_Left->SetCurrentSceneType(SceneType::Stage02);
		Portal_Left->SetNextSceneType(SceneType::Stage01);
	}
	// Right Portal
	{
		CSharedPtr<CPortal> Portal_Right = m_pScene->CreateGameObject<CPortal>("Portal_Right",
			Vector2(1400.f, 500.f), Vector2(34.f, 500.f));

		Portal_Right->SetCurrentSceneType(SceneType::Stage02);
		Portal_Right->SetNextSceneType(SceneType::Stage03);
	}

	m_pScene->GetResourceManager()->SoundPlay("bgm00");

	// Map Object -------------------------------
	CSharedPtr<CStaticObject> Tyaya00 = m_pScene->CreateGameObject<CStaticObject>("Tyaya00", Vector2(-140.f, 627.f), Vector2(512, 512), "Stage");
	Tyaya00->AddAnimation("Tyaya_Anim", AnimationType::Sprite, true, "obj_Atlass00", 2.f);
	Tyaya00->AddAnimationFrameData("Tyaya_Anim", 7, 2, 16);

	CSharedPtr<CStaticObject> Tyaya01 = m_pScene->CreateGameObject<CStaticObject>("Tyaya01", Vector2(130, 627.f), Vector2(512, 512), "Stage");
	Tyaya01->AddAnimation("Tyaya_Anim", AnimationType::Sprite, true, "obj_Atlass00", 2.f);
	Tyaya01->AddAnimationFrameData("Tyaya_Anim", 7, 2, 16);

	CSharedPtr<CStaticObject> Tyaya02 = m_pScene->CreateGameObject<CStaticObject>("Tyaya02", Vector2(400.f, 627.f), Vector2(512, 512), "Stage");
	Tyaya02->AddAnimation("Tyaya_Anim", AnimationType::Sprite, true, "obj_Atlass00", 2.f);
	Tyaya02->AddAnimationFrameData("Tyaya_Anim", 7, 2, 16);

	CSharedPtr<CJihanki> Jihanki00 = m_pScene->CreateGameObject<CJihanki>("Jihanki00", Vector2(600.f, 627.f), Vector2(512, 512), "Stage");
			
	//-------------------------------------------

	//CSharedPtr<CStaticObject> BoxCollision00 = m_pScene->CreateGameObject<CStaticObject>("BoxCollision00", Vector2(1227.f, 903.f), Vector2(512, 50.f), "Stage");
	//BoxCollision00->SetEnableRender(false);
	//CSharedPtr<CColliderRect>	Box = BoxCollision00->AddCollider<CColliderRect>("Box");
	//Box->SetExtent(BoxCollision00->GetSize());
	//Box->SetCollisionProfile("Block");

	//CSharedPtr<CStaticObject> LineCollision00 = m_pScene->CreateGameObject<CStaticObject>("BoxCollision00", Vector2(1230.f, 903.f), Vector2(512, 50.f), "Stage");
	//LineCollision00->SetEnableRender(false);
	//CSharedPtr<CColliderLine>	Line = LineCollision00->AddCollider<CColliderLine>("Line");
	//Line->SetStartPoint(1235.f, 903.f);
	//Line->SetEndPoint(1500.f, 903.f);
	//Line->SetCollisionProfile("Map");



	return true;
}

void CStage02Mode::InitSound()
{
}
