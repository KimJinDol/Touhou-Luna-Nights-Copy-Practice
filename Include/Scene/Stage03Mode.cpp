#include "Stage03Mode.h"
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


CStage03Mode::CStage03Mode()
{
}

CStage03Mode::~CStage03Mode()
{
}

bool CStage03Mode::Init()
{
    CSceneMode::Init();

	CSharedPtr<CPlayer> Player = m_pScene->CreateGameObject<CPlayer>("Player",
		Vector2(0.f, 0.f), Vector2(100.f, 100.f));

	Player->SetPos(Vector2(200.f, 1500.f));
	m_pScene->GetMainCamera()->SetTarget(Player);
	SetPlayer(Player);

	CStage* MainStage = m_pScene->CreateStage<CStage>();

	MainStage->LoadFile("Stage03.stg", DATA_PATH);
	MainStage->SetExitSide(true, true);
	m_pScene->GetMainCamera()->SetTargetPivot(0.5f, 0.5f);
	m_pScene->GetMainCamera()->SetWorldSize(1360.f, 2700.f);
#ifdef _DEBUG
	m_pScene->GetMainCamera()->SetWorldSize(1450.f, 2700.f);
#endif
	// Left Portal
	{
		CSharedPtr<CPortal> Portal_Left = m_pScene->CreateGameObject<CPortal>("Portal",
			Vector2(-68.f, 2020.f), Vector2(34.f, 500.f));

		Portal_Left->SetCurrentSceneType(SceneType::Stage03);
		Portal_Left->SetNextSceneType(SceneType::Stage02);
	}
	// Right Portal
	{
		CSharedPtr<CPortal> Portal_Right = m_pScene->CreateGameObject<CPortal>("Portal_Right",
			Vector2(1400.f, 500.f), Vector2(34.f, 550.f));

		Portal_Right->SetCurrentSceneType(SceneType::Stage03);
		Portal_Right->SetNextSceneType(SceneType::Stage04);
	}

	Vector2 willPos = Vector2(550.f, 750.f);

	// Enemy -------------------
	CSharedPtr<CAIWill_o> Will_o01 = m_pScene->CreateGameObject<CAIWill_o>("Will_o01");
	Will_o01->SetPos(willPos.x + 200.f, willPos.y + 50.f);
	CSharedPtr<CAIWill_o> Will_o02 = m_pScene->CreateGameObject<CAIWill_o>("Will_o02");
	Will_o02->SetPos(Vector2(willPos.x - 76.f, willPos.y + 37.f));
	CSharedPtr<CAIWill_o> Will_o03 = m_pScene->CreateGameObject<CAIWill_o>("Will_o03");
	Will_o03->SetPos(Vector2(willPos.x + 18.f, willPos.y + 100.f));
	CSharedPtr<CAIWill_o> Will_o04 = m_pScene->CreateGameObject<CAIWill_o>("Will_o04");
	Will_o04->SetPos(Vector2(willPos.x + 170.f, willPos.y + 143.f));
	CSharedPtr<CAIWill_o> Will_o06 = m_pScene->CreateGameObject<CAIWill_o>("Will_o06");
	Will_o06->SetPos(Vector2(willPos.x - 15.f, willPos.y + 200.f));
	CSharedPtr<CAIWill_o> Will_o08 = m_pScene->CreateGameObject<CAIWill_o>("Will_o08");
	Will_o08->SetPos(Vector2(willPos.x + 87.f, willPos.y + 220.f));

	CSharedPtr<CGoblinAI> Goblin00 = m_pScene->CreateGameObject<CGoblinAI>("Goblin00");
	Goblin00->SetPos(Vector2( 280.f, 1600.f));
	Goblin00->SetView(CharacterView::Right);

	CSharedPtr<CGoblinAI> Goblin01 = m_pScene->CreateGameObject<CGoblinAI>("Goblin01");
	Goblin01->SetPos(Vector2(1100.f, 1400.f));
	Goblin01->SetView(CharacterView::Left);

	willPos = Vector2(700.f, 2200.f);

	CSharedPtr<CAIWill_o> Will_o09 = m_pScene->CreateGameObject<CAIWill_o>("Will_o09");
	Will_o09->SetPos(willPos);
	CSharedPtr<CAIWill_o> Will_o10 = m_pScene->CreateGameObject<CAIWill_o>("Will_o10");
	Will_o10->SetPos(willPos.x + 200.f, willPos.y + 50.f);
	CSharedPtr<CAIWill_o> Will_o11 = m_pScene->CreateGameObject<CAIWill_o>("Will_o11");
	Will_o11->SetPos(Vector2(willPos.x - 76.f, willPos.y + 37.f));
	CSharedPtr<CAIWill_o> Will_o12 = m_pScene->CreateGameObject<CAIWill_o>("Will_o12");
	Will_o12->SetPos(Vector2(willPos.x + 18.f, willPos.y + 100.f));
	CSharedPtr<CAIWill_o> Will_o13 = m_pScene->CreateGameObject<CAIWill_o>("Will_o13");
	Will_o13->SetPos(Vector2(willPos.x + 170.f, willPos.y + 143.f));



    return true;
}

void CStage03Mode::InitSound()
{
}
