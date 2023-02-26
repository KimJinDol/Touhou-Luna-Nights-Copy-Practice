#include "Stage00Mode.h"
#include "SceneResource.h"
#include "Scene.h"
#include "Camera.h"
#include "../Object/Player.h"
#include "../UI/StartSceneWidget.h"
#include "../Object/WolfAI.h"
#include "../Object/AIWill_o.h"
#include "../Object/Bullet.h"
#include "../Object/PlayerEffect.h"
#include "../Stage/Stage.h"
#include "../Object/Portal.h"
#include "../Timer.h"
#include "../GameManager.h"
#include "../UI/TextManager.h"
#include "../Object/Setsmei.h"
#include "../UI/PlayerWidget.h"
#include "../Object/Map.h"
#include "../Object/TimeStopItem.h"

bool CStage00Mode::m_bCheckDialogue = false;

CStage00Mode::CStage00Mode()
{
}

CStage00Mode::~CStage00Mode()
{
}

bool CStage00Mode::Init()
{
	CSceneMode::Init();


	CSharedPtr<CPlayer> Player = m_pScene->CreateGameObject<CPlayer>("Player",
		Vector2(0.f, 0.f), Vector2(100.f, 100.f));

	Player->SetPos(Vector2(600.f, 883.f));

#ifdef _DEBUG
#else
	if (!m_bCheckDialogue)
	{
		m_bCheckDialogue = true;
		PlaySound("ivent01");
		((CPlayer*)(Player.Get()))->SetEnable(false, false, false);
		GET_TIMER->StartTimer("Stage00ShowPlayer", 3.5f, this, &CStage00Mode::ShowPlayer);
		GET_TIMER->StartTimer("Stage00Dialogue", 5.5f, this, &CStage00Mode::StartDialogue);
		Player->SetEnableRender(false);
		Player->GetPlayerWidget()->SetWidgetEnable(false);
	}
	else
#endif
		PlaySound("bgm00");

	m_pScene->GetMainCamera()->SetTarget(Player);

	SetPlayer(Player);


	//CSharedPtr<CWolfAI> Wolf2 = m_pScene->CreateGameObject<CWolfAI>("Wolf2");
	//Wolf2->SetPos(Vector2(1600.f, 600.f));
	
	CStage* MainStage = m_pScene->CreateStage<CStage>();

	MainStage->LoadFile("Stage00.stg", DATA_PATH);
	MainStage->SetExitSide(false, true);

	m_pScene->GetMainCamera()->SetTargetPivot(0.5f, 0.f);
	m_pScene->GetMainCamera()->SetWorldSize(3876.f, 1500.f);
#ifdef _DEBUG
	m_pScene->GetMainCamera()->SetWorldSize(4000.f, 1500.f);
#endif

	CSharedPtr<CPortal> Portal = m_pScene->CreateGameObject<CPortal>("Portal",
		Vector2(3910.f, 300.f), Vector2(34.f, 500.f));
	Portal->SetCurrentSceneType(SceneType::Stage00);
	Portal->SetNextSceneType(SceneType::Stage01);


	// Map Object
	CSharedPtr<CStaticObject> TreeSprite00 = m_pScene->CreateGameObject<CStaticObject>("TreeSprite00", Vector2(650.f, 300.f), Vector2(512, 495), "BackObj1");
	TreeSprite00->SetTexture("Stage00_Group00");
	TreeSprite00->SetTextureImagePosAndSize(Vector2(1412.f, 4.f), Vector2(530.f, 512.f));

	CSharedPtr<CStaticObject> MountainSprite00 = m_pScene->CreateGameObject<CStaticObject>("MountainSprite00", Vector2(1500.f, 800.f), Vector2(1800, 342), "BackObj2");
	MountainSprite00->SetTexture("Stage00_Group00");
	MountainSprite00->SetTextureImagePosAndSize(Vector2(0.f, 969.f), Vector2(1406.f, 342.f));

	CSharedPtr<CSetsmei> Setsmei00 = m_pScene->CreateGameObject<CSetsmei>("Setsmei00", Vector2(722.f, 600.f), Vector2(256.f, 256.f), "Default");
	Setsmei00->SetDialCode("#sa1");

	CSharedPtr<CSetsmei> Setsmei01 = m_pScene->CreateGameObject<CSetsmei>("Setsmei01", Vector2(1470.f, 600.f), Vector2(256.f, 256.f), "Default");
	Setsmei01->SetDialCode("#sa2");

	//CSharedPtr<CStaticObject> StoneSprite00 = m_pScene->CreateGameObject<CStaticObject>("StoneSprite00", Vector2(30.f, 300.f), Vector2(64, 128), "Default");
	//StoneSprite00->SetTexture("Stage00_Group00");
	//StoneSprite00->SetTextureImagePosAndSize(Vector2(1412.f, 4.f), Vector2(530.f, 512.f));

	CSharedPtr<CTimeStopItem> TimeStopItem = m_pScene->CreateGameObject<CTimeStopItem>("TimeStopItem", Vector2(2800.f, 550.f), Vector2(256.f, 256.f), "Default");

	CSharedPtr<CMap> m_BlackMask = m_pScene->CreateGameObject<CMap>("BlackMask", Vector2(0.f, 0.f), Vector2(1280.f, 720.f), "Default");
	m_BlackMask->SetPivot(0.f, 0.f);
	m_BlackMask->SetTexture("BlackGround");

	return true;
}

void CStage00Mode::Start()
{
	CSceneMode::Start();
}

void CStage00Mode::PlaySound(const std::string& Name)
{
	CSceneResource* pResource = m_pScene->GetResourceManager();

	m_pScene->GetResourceManager()->SoundPlay(Name);
	// pResource->SetVolume(100);
}

void CStage00Mode::PlayMainBgm()
{
	CStage00Mode::PlaySound("bgm00");
	m_Player->GetPlayerWidget()->SetWidgetEnable(true);
	m_Player->GetPlayerWidget()->SetEnableTimeStopWidget(false);
	m_Player->GetPlayerWidget()->SetEnableItemWidget(false);
}

void CStage00Mode::StartDialogue()
{
	CTextManager::GetInst()->SetDialCode("#s00");
	CTextManager::GetInst()->SetEnableAllStop(false);
	((CPlayer*)(m_Player.Get()))->Dialogue();
	GET_SINGLE(CTextManager)->SetEndDialogueFunc<CStage00Mode>(this, &CStage00Mode::PlayMainBgm);
}

void CStage00Mode::ShowPlayer()
{
	m_Player->SetEnableRender(true);
	m_Player->ChangeAnimation("PlayerMiniDown");
	m_pScene->GetMainCamera()->CameraShake(12.f, 0.001f, 12.f, 0.001f, .4f);

	CSharedPtr<CPlayerEffect> pEffect2 = m_pScene->CreateGameObject<CPlayerEffect>("AI_Damage2_Effect", m_Player->GetCenterPos(), Vector2(512.f, 512.f));
	pEffect2->SetPivot(0.5f, 0.5f);
	pEffect2->AddAnimation("AI_Damage2_Effect_Anim", AnimationType::Sprite, "Effect_Atlass00", .3f);
	pEffect2->AddAnimationFrameData("AI_Damage2_Effect_Anim", 6, 10, 7, Vector2(512.f, 512.f));
	pEffect2->SetAutoDestroy();

	m_pScene->GetResourceManager()->SoundPlay("absorber");
	m_pScene->GetResourceManager()->SoundPlay("swing_big");
}
