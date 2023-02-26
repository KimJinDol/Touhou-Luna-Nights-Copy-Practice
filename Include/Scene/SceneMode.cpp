
#include "SceneMode.h"
#include "../Input.h"
#include "../Scene/SceneManager.h"
#include "../Resource/ResourceManager.h"
#include "../Object/StaticObject.h"
#include "../Scene/Camera.h"

#ifdef _DEBUG
#include "../Scene/EditorSceneMode.h"
#include "../Scene/Stage00Mode.h"
#include "../Scene/Stage01Mode.h"
#include "../Scene/Stage02Mode.h"
#include "../Scene/Stage03Mode.h"
#include "../Scene/Stage04Mode.h"
#endif

TimeMode CSceneMode::m_CurTimeMode = TimeMode::Move;

CSceneMode::CSceneMode()	:
	m_WorldTimeSpeed(1.f),
	m_Start(false)
{
}

CSceneMode::~CSceneMode()
{
}

void CSceneMode::SetTimeMode(TimeMode Mode)
{
	m_CurTimeMode = Mode;

	switch (Mode)
	{
	case TimeMode::Move:
		m_WorldTimeSpeed = 1.f;
		GET_SINGLE(CResourceManager)->FindTexture("background_sprite")->SetIndex(0);
		GET_SINGLE(CResourceManager)->FindTexture("Stage00_Tile")->SetIndex(0);
		GET_SINGLE(CResourceManager)->FindTexture("Stage00_Tile_wet")->SetIndex(0);
		GET_SINGLE(CResourceManager)->FindTexture("Stage00_Group00")->SetIndex(0);
		GET_SINGLE(CResourceManager)->FindTexture("obj_Atlass00")->SetIndex(0);
		break;
	case TimeMode::Stop:
		m_WorldTimeSpeed = 1.f;
		GET_SINGLE(CResourceManager)->FindTexture("background_sprite")->SetIndex(1);
		GET_SINGLE(CResourceManager)->FindTexture("Stage00_Tile")->SetIndex(1);
		GET_SINGLE(CResourceManager)->FindTexture("Stage00_Tile_wet")->SetIndex(1);
		GET_SINGLE(CResourceManager)->FindTexture("Stage00_Group00")->SetIndex(1);
		GET_SINGLE(CResourceManager)->FindTexture("obj_Atlass00")->SetIndex(1);
		break;
	case TimeMode::Snail:
		m_WorldTimeSpeed = 0.5f;

		break;
	}
}

void CSceneMode::Start()
{
#ifdef _DEBUG
	CInput::GetInst()->SetKeyCallback<CSceneMode>("F1", EKeyState::Down,
		this, &CEditorSceneMode::SetIsShowTileSide);
	CInput::GetInst()->SetKeyCallback<CSceneMode>("F4", EKeyState::Down,
		this, &CSceneMode::ChangeEditorScene);

	CInput::GetInst()->SetKeyCallback<CSceneMode>("1", EKeyState::Down,
		this, &CSceneMode::ChangeStage00Scene);

	CInput::GetInst()->SetKeyCallback<CSceneMode>("2", EKeyState::Down,
		this, &CSceneMode::ChangeStage01Scene);

	CInput::GetInst()->SetKeyCallback<CSceneMode>("3", EKeyState::Down,
		this, &CSceneMode::ChangeStage02Scene);

	CInput::GetInst()->SetKeyCallback<CSceneMode>("4", EKeyState::Down,
		this, &CSceneMode::ChangeStage03Scene);

	CInput::GetInst()->SetKeyCallback<CSceneMode>("5", EKeyState::Down,
		this, &CSceneMode::ChangeStage04Scene);
#endif
}

bool CSceneMode::Init()
{
	CSharedPtr<CStaticObject>	pBackGround = m_pScene->CreateGameObject<CStaticObject>("BackGround",Vector2(0.f, 0.f), m_pScene->GetMainCamera()->GetResolution(), "BackGround");
	pBackGround->SetIsInfluenceCameraPos(false);
	pBackGround->SetPivot(0.f, 0.f);
	pBackGround->SetTexture("background_sprite");

	return true;
}

void CSceneMode::Update(float DeltaTime)
{
}

#ifdef _DEBUG
void CSceneMode::ChangeEditorScene(float DeltaTime)
{
	CScene* pScene = CSceneManager::GetInst()->CreateNextScene();

	CSceneManager::GetInst()->CreateSceneMode<CEditorSceneMode>(false);
}

void CSceneMode::ChangeStage00Scene(float DeltaTime)
{
	CScene* pScene = CSceneManager::GetInst()->CreateNextScene();

	CSceneManager::GetInst()->CreateSceneMode<CStage00Mode>(false)->
		SetPlayerSpawnPos(Vector2(600.f, 650.f));

	pScene->GetMainCamera()->SetPos(0.f, 350.f);
}

void CSceneMode::ChangeStage01Scene(float DeltaTime)
{
	CScene* pScene = CSceneManager::GetInst()->CreateNextScene();

	CSceneManager::GetInst()->CreateSceneMode<CStage01Mode>(false)->
		SetPlayerSpawnPos(Vector2(34.f, 350.f));

	pScene->GetMainCamera()->SetPos(0.f, 350.f);
}

void CSceneMode::ChangeStage02Scene(float DeltaTime)
{
	CScene* pScene = CSceneManager::GetInst()->CreateNextScene();

	CSceneManager::GetInst()->CreateSceneMode<CStage02Mode>(false)->
		SetPlayerSpawnPos(Vector2(34.f, 883.f));

	pScene->GetMainCamera()->SetPos(0.f, 350.f);
}

void CSceneMode::ChangeStage03Scene(float DeltaTime)
{
	CScene* pScene = CSceneManager::GetInst()->CreateNextScene();

	CSceneManager::GetInst()->CreateSceneMode<CStage03Mode>(false)->
		SetPlayerSpawnPos(Vector2(34.f, 2515.f));

	pScene->GetMainCamera()->SetPos(0.f, 3000.f);
}

void CSceneMode::ChangeStage04Scene(float DeltaTime)
{
	CScene* pScene = CSceneManager::GetInst()->CreateNextScene();

	CSceneManager::GetInst()->CreateSceneMode<CStage04Mode>(false)->
		SetPlayerSpawnPos(Vector2(34.f, 600.f));

	pScene->GetMainCamera()->SetPos(0.f, 0.f);
}
#endif
