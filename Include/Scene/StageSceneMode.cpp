#include "StageSceneMode.h"
#include "../Object/Map.h"
#include "../Scene/Scene.h"
#include "../GameManager.h"
#include "../Timer.h"
#include "../UI/TextManager.h"
#include "../UI/PlayerWidget.h"
#include "SceneManager.h"
#include "Camera.h"
#include "Stage00Mode.h"

CStageSceneMode::CStageSceneMode()
{
}

CStageSceneMode::~CStageSceneMode()
{
}

bool CStageSceneMode::Init()
{
	CSceneMode::Init();

	CSharedPtr<CPlayer> Player = m_pScene->CreateGameObject<CPlayer>("Player", Vector2(0.f, 0.f), Vector2(0.f, 0.f), "BackGround");
	Player->SetEnable(false, false, false);
	Player->GetPlayerWidget()->SetWidgetEnable(false);
	SetPlayer(Player);

	CSharedPtr<CStaticObject>	BackGround = m_pScene->CreateGameObject<CStaticObject>("BackGround", Vector2(0.f, 0.f), Vector2(1280.f, 720.f), "BackGround");
	BackGround->SetPivot(0.f, 0.f);
	BackGround->SetTexture("BlackGround");

	CSharedPtr<CStaticObject>	CinemaImage = m_pScene->CreateGameObject<CStaticObject>("CinemaImage");
	CinemaImage->SetPos(GET_RESOLUTION.Width / 2, GET_RESOLUTION.Height /2);
	CinemaImage->AddAnimation("Cinema", AnimationType::Sprite, true, "Cinema00", TEXT("BackGround/Cinema00.bmp"), 0.5f);
	CinemaImage->AddAnimationFrameData("Cinema", 0, 0, 4, Vector2(720.f, 256.f), Vector2(0.5f, 0.5f));

	m_BlackMask = m_pScene->CreateGameObject<CMap>("BlackMask", Vector2(0.f, 0.f), Vector2(1280.f, 720.f), "Default");
	m_BlackMask->SetPivot(0.f, 0.f);
	m_BlackMask->SetTexture("BlackGround");

	GET_TIMER->StartTimer("StartCinema", 4.f, this, &CStageSceneMode::StartDialogue);

	return true;
}

void CStageSceneMode::Start()
{
	CSceneMode::Start();
}

void CStageSceneMode::StartDialogue()
{
	CTextManager::GetInst()->SetDialCode("#s0a");
	((CPlayer*)(m_Player.Get()))->Dialogue();
	GET_SINGLE(CTextManager)->SetEndDialogueFunc<CStageSceneMode>(this, &CStageSceneMode::BlackReverse);
}

void CStageSceneMode::BlackReverse()
{
	m_Player->Destroy();
	m_BlackMask->SetReverse(true);

	GET_TIMER->StartTimer("ChangeScene", 4.f, this, &CStageSceneMode::NextScene);
}

void CStageSceneMode::NextScene()
{
	CScene* pScene = CSceneManager::GetInst()->CreateNextScene();
	CSceneManager::GetInst()->CreateSceneMode<CStage00Mode>(false);
	CSceneManager::GetInst()->GetNextScene()->GetMainCamera()->SetPos(0.f, 350.f);
}
