#include "Stage01Mode.h"
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

CStage01Mode::CStage01Mode()
{
}

CStage01Mode::~CStage01Mode()
{
}

bool CStage01Mode::Init()
{
	CSceneMode::Init();

	/*InitSound();*/

	CSharedPtr<CPlayer> Player = m_pScene->CreateGameObject<CPlayer>("Player",
		Vector2(0.f, 300.f), Vector2(100.f, 100.f));

	Player->SetPos(Vector2(600.f, 300.f));
	m_pScene->GetMainCamera()->SetTarget(Player);
	SetPlayer(Player);


	// Enemy -----------------------
	CSharedPtr<CWolfAI> Wolf00 = m_pScene->CreateGameObject<CWolfAI>("Wolf00");
	Wolf00->SetPos(Vector2(550.f, 640.f));

	CSharedPtr<CWolfAI> Wolf01 = m_pScene->CreateGameObject<CWolfAI>("Wolf01");
	Wolf01->SetPos(Vector2(670.f, 810.f));

	CSharedPtr<CWolfAI> Wolf02 = m_pScene->CreateGameObject<CWolfAI>("Wolf02");
	Wolf02->SetPos(Vector2(1600.f, 730.f));

	CSharedPtr<CWolfAI> Wolf03 = m_pScene->CreateGameObject<CWolfAI>("Wolf03");
	Wolf03->SetPos(Vector2(2000.f, 800.f));

	CStage* pStage = m_pScene->CreateStage<CStage>();

	pStage->LoadFile("Stage01.stg", DATA_PATH);
	pStage->SetExitSide(true, true);

	m_pScene->GetMainCamera()->SetTargetPivot(0.5f, 0.f);
	m_pScene->GetMainCamera()->SetWorldSize(2788.f, 1500.f);
#ifdef _DEBUG
	m_pScene->GetMainCamera()->SetWorldSize(2924.f, 1500.f);
#endif
	// Left Portal
	{
		CSharedPtr<CPortal> Portal_Left = m_pScene->CreateGameObject<CPortal>("Portal_Left",
			Vector2(-68.f, 300.f), Vector2(34.f, 500.f));

		Portal_Left->SetCurrentSceneType(SceneType::Stage01);
		Portal_Left->SetNextSceneType(SceneType::Stage00);
	}
	// Right Portal
	{
		CSharedPtr<CPortal> Portal_Right = m_pScene->CreateGameObject<CPortal>("Portal_Right",
			Vector2(2822.f, 380.f), Vector2(34.f, 500.f));

		Portal_Right->SetCurrentSceneType(SceneType::Stage01);
		Portal_Right->SetNextSceneType(SceneType::Stage02);
	}

	m_pScene->GetResourceManager()->SoundPlay("bgm00");

	// Map Object -------------------------------------------
	CSharedPtr<CStaticObject> TreeSprite00 = m_pScene->CreateGameObject<CStaticObject>("TreeSprite00", Vector2(300.f, 300.f), Vector2(512, 495), "BackObj1");
	TreeSprite00->SetTexture("Stage00_Group00");
	TreeSprite00->SetTextureImagePosAndSize(Vector2(1412.f, 4.f), Vector2(530.f, 512.f));

	CSharedPtr<CStaticObject> MountainSprite00 = m_pScene->CreateGameObject<CStaticObject>("MountainSprite00", Vector2(700.f, 800.f), Vector2(1800, 342), "BackObj2");
	MountainSprite00->SetTexture("Stage00_Group00");
	MountainSprite00->SetTextureImagePosAndSize(Vector2(0.f, 969.f), Vector2(1406.f, 342.f));

	CSharedPtr<CStaticObject> Tyaya00 = m_pScene->CreateGameObject<CStaticObject>("Tyaya00", Vector2(2640.f, 627.f), Vector2(512, 512), "Stage");
	Tyaya00->AddAnimation("Tyaya_Anim", AnimationType::Sprite, true, "obj_Atlass00", 2.f);
	Tyaya00->AddAnimationFrameData("Tyaya_Anim", 7, 2, 16);

	return true;
}

void CStage01Mode::InitSound()
{
    CSceneResource* pResource = m_pScene->GetResourceManager();

    pResource->LoadSound("BGM", true, "bgm00", "BGM/bgm00.ogg");
    //pResource->SetVolume(100);
}
