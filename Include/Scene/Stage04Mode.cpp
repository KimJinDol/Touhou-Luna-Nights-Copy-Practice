#include "Stage04Mode.h"
#include "SceneResource.h"
#include "Scene.h"
#include "Camera.h"
#include "../Object/Player.h"
#include "../UI/StartSceneWidget.h"
#include "../Object/Map.h"
#include "../Object/WolfAI.h"
#include "../Object/GoblinAI.h"
#include "../Object/Bullet.h"
#include "../Object/PlayerEffect.h"
#include "../Stage/Stage.h"
#include "../Object/Portal.h"
#include "../Object/AIWill_o.h"
#include "../Object/Jihanki.h"
#include "../Object/Thunder.h"

CStage04Mode::CStage04Mode()
{
}

CStage04Mode::~CStage04Mode()
{
}

bool CStage04Mode::Init()
{
    CSceneMode::Init();

	CSharedPtr<CPlayer> Player = m_pScene->CreateGameObject<CPlayer>("Player",
		Vector2(0.f, 0.f), Vector2(100.f, 100.f));

	Player->SetPos(Vector2(200.f, 1500.f));
	m_pScene->GetMainCamera()->SetTarget(Player);
	SetPlayer(Player);

	CStage* MainStage = m_pScene->CreateStage<CStage>();

	MainStage->LoadFile("Stage04.stg", DATA_PATH);
	MainStage->SetExitSide(true, true);
	m_pScene->GetMainCamera()->SetTargetPivot(0.5f, 0.f);
	m_pScene->GetMainCamera()->SetWorldSize(3740.f, 740.f);
#ifdef _DEBUG
	m_pScene->GetMainCamera()->SetWorldSize(3876.f, 740.f);
#endif
	// Left Portal
	{
		CSharedPtr<CPortal> Portal_Left = m_pScene->CreateGameObject<CPortal>("Portal",
			Vector2(-68.f, 300.f), Vector2(34.f, 500.f));

		Portal_Left->SetCurrentSceneType(SceneType::Stage04);
		Portal_Left->SetNextSceneType(SceneType::Stage03);
	}
	// Right Portal
	{
		CSharedPtr<CPortal> Portal_Right = m_pScene->CreateGameObject<CPortal>("Portal_Right",
			Vector2(3808.f, 232.f), Vector2(34.f, 500.f));

		Portal_Right->SetCurrentSceneType(SceneType::Stage04);
		Portal_Right->SetNextSceneType(SceneType::StageEnd);
	}

	// Enemy

	Vector2 wolfVec = Vector2(2000.f, 540.f);

	CSharedPtr<CWolfAI> Wolf00 = m_pScene->CreateGameObject<CWolfAI>("Wolf00");
	Wolf00->SetPos(wolfVec);

	CSharedPtr<CWolfAI> Wolf01 = m_pScene->CreateGameObject<CWolfAI>("Wolf01");
	Wolf01->SetPos(wolfVec.x + 300.f , wolfVec.y);

	CSharedPtr<CWolfAI> Wolf02 = m_pScene->CreateGameObject<CWolfAI>("Wolf02");
	Wolf02->SetPos(wolfVec.x + 700.f, wolfVec.y);

	CSharedPtr<CWolfAI> Wolf03 = m_pScene->CreateGameObject<CWolfAI>("Wolf03");
	Wolf03->SetPos(wolfVec.x + 1000.f, wolfVec.y);

	CSharedPtr<CWolfAI> Wolf04 = m_pScene->CreateGameObject<CWolfAI>("Wolf04");
	Wolf04->SetPos(wolfVec.x + 1600.f, wolfVec.y);

	// Map Obj
	CSharedPtr<CStaticObject> MountainSprite00 = m_pScene->CreateGameObject<CStaticObject>("MountainSprite00", Vector2(1000.f, 450.f), Vector2(1800, 342), "BackObj2");
	MountainSprite00->SetTexture("Stage00_Group00");
	MountainSprite00->SetTextureImagePosAndSize(Vector2(0.f, 969.f), Vector2(1406.f, 342.f));

	CSharedPtr<CThunder> Thunder00 = m_pScene->CreateGameObject<CThunder>("Thunder00", Vector2(952.f, 476.f), Vector2(1800, 342), "Default");



	return true;
}
