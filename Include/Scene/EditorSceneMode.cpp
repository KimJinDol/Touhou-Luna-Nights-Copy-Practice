
#include "EditorSceneMode.h"
#include "../Object/Player.h"
#include "Scene.h"
#include "../Collision/ColliderRect.h"
#include "../Input.h"
#include "EditorDlg.h"
#include "Camera.h"
#include "../Stage/Tile.h"
#include "../GameManager.h"

CEditorSceneMode::CEditorSceneMode() :
	m_pDlg(nullptr),
	m_ScrollSpeed(500.f)
{
}

CEditorSceneMode::~CEditorSceneMode()
{
	SAFE_DELETE(m_pDlg);
}

void CEditorSceneMode::Start()
{
	CSceneMode::Start();


	CInput::GetInst()->SetKeyCallback<CEditorSceneMode>("OnEditor", EKeyState::Press,
		this, &CEditorSceneMode::OpenEditorDlg);

	CInput::GetInst()->SetKeyCallback<CEditorSceneMode>("MoveUp", EKeyState::Press,
		this, &CEditorSceneMode::MoveUp);
	CInput::GetInst()->SetKeyCallback<CEditorSceneMode>("MoveDown", EKeyState::Press,
		this, &CEditorSceneMode::MoveDown);
	CInput::GetInst()->SetKeyCallback<CEditorSceneMode>("MoveLeft", EKeyState::Press,
		this, &CEditorSceneMode::MoveLeft);
	CInput::GetInst()->SetKeyCallback<CEditorSceneMode>("MoveRight", EKeyState::Press,
		this, &CEditorSceneMode::MoveRight);

	CInput::GetInst()->SetKeyCallback<CEditorSceneMode>("LButton", EKeyState::Press,
		this, &CEditorSceneMode::MousePress);

	CInput::GetInst()->SetKeyCallback<CEditorSceneMode>("LButton", EKeyState::Down,
		this, &CEditorSceneMode::MouseDown);

	CInput::GetInst()->SetKeyCallback<CEditorSceneMode>("LButton", EKeyState::Up,
		this, &CEditorSceneMode::MouseUp);

	CInput::GetInst()->SetKeyCallback<CEditorSceneMode>("Tab", EKeyState::Down,
		this, &CEditorSceneMode::ChangeTileScreen);

}

bool CEditorSceneMode::Init()
{
	//m_pStage = m_pScene->CreateStage<CStage>();
	//CMainStage* MainStage = m_pScene->CreateStage<CMainStage>();
	if (!m_pDlg)
	{
		m_pDlg = new CEditorDlg;

		m_pDlg->SetScene(m_pScene);
		m_pDlg->Init(IDD_DIALOG1);
	}
	// m_pScene->GetMouseWidget()->Enable(false);

	return true;
}

void CEditorSceneMode::Update(float DeltaTime)
{
	CStageEditMode* pStage = m_pDlg->GetStage();

	if (pStage)
	{
	}
}

void CEditorSceneMode::OpenEditorDlg(float DeltaTime)
{
	if (!m_pDlg)
	{
		m_pDlg = new CEditorDlg;

		m_pDlg->SetScene(m_pScene);
		m_pDlg->Init(IDD_DIALOG1);
	}

	else
		m_pDlg->Show();
}

void CEditorSceneMode::MoveUp(float DeltaTime)
{
	CCamera* pCamera = m_pScene->GetMainCamera();

	pCamera->Move(Vector2(0.f, -1.f) * m_ScrollSpeed * DeltaTime);
}

void CEditorSceneMode::MoveDown(float DeltaTime)
{
	CCamera* pCamera = m_pScene->GetMainCamera();

	pCamera->Move(Vector2(0.f, 1.f) * m_ScrollSpeed * DeltaTime);
}

void CEditorSceneMode::MoveLeft(float DeltaTime)
{
	CCamera* pCamera = m_pScene->GetMainCamera();

	pCamera->Move(Vector2(-1.f, 0.f) * m_ScrollSpeed * DeltaTime);
}

void CEditorSceneMode::MoveRight(float DeltaTime)
{
	CCamera* pCamera = m_pScene->GetMainCamera();

	pCamera->Move(Vector2(1.f, 0.f) * m_ScrollSpeed * DeltaTime);
}

void CEditorSceneMode::MousePress(float DeltaTime)
{
	CStageEditMode* pStage = m_pDlg->GetStage();

	if (!pStage)
		return;

	TileEditMode	EditMode = m_pDlg->GetEditMode();

	CCamera* pCamera = m_pScene->GetMainCamera();
	Vector2	MousePos = CInput::GetInst()->GetMousePos() + pCamera->GetPos();

	// ÇöÀç ¿ùµå»ó¿¡¼­ÀÇ Ä«¸Þ¶ó À§Ä¡ + È­¸é Å©±â
	Vector2 DisplayLimitPos = Vector2(pCamera->GetPos().x + GET_RESOLUTION.Width, pCamera->GetPos().y + GET_RESOLUTION.Height);
	
	if (MousePos.x < pCamera->GetPos().x || MousePos.x > DisplayLimitPos.x || 
		MousePos.y < pCamera->GetPos().y || MousePos.y > DisplayLimitPos.y)
		return;

	if (pStage->GetVisibleTileResource())
	{
		m_pDlg->SetTileResourceImageStartPos(MousePos);
	}
	else
	{
		switch (EditMode)
		{
		case TileEditMode::Type:
		{
			TileType	Type = (TileType)m_pDlg->GetTileType();

			pStage->ChangeTileType(MousePos, Type);
		}
		break;
		case TileEditMode::Image:
		{
			Vector2	ImageStartPos = m_pDlg->GetImageStartPos();
			Vector2	TileSize = m_pDlg->GetTileSize();

			pStage->AddTileImage(MousePos, m_pDlg->GetTileTexture(), ImageStartPos, TileSize);
		}
		break;
		case TileEditMode::Clear:
		{
			pStage->TileClear(MousePos);
		}
		break;
		}
	}
}

void CEditorSceneMode::MouseDown(float DeltaTime)
{
	CStageEditMode* pStage = m_pDlg->GetStage();

	if (!pStage)
		return;

	TileEditMode	EditMode = m_pDlg->GetEditMode();

	if (EditMode == TileEditMode::Collision)
	{
		CCamera* pCamera = m_pScene->GetMainCamera();
		Vector2	MousePos = CInput::GetInst()->GetMousePos() + pCamera->GetPos();

		CTile* pTile = pStage->GetTile(MousePos);
		m_CollTile = pTile;
		// ¿· ÀÎµ¦½Â
		//if(pStage->GetTile((pTile->GetTileIndex() - 1))->GetTileCollisionType() == ColiisionType)
		pStage->ChangeTileCollision(MousePos);
	}
}

void CEditorSceneMode::MouseUp(float DeltaTime)
{
	CStageEditMode* pStage = m_pDlg->GetStage();

	if (!pStage)
		return;
	
	TileEditMode	EditMode = m_pDlg->GetEditMode();

	if (EditMode == TileEditMode::Collision)
	{
		CCamera* pCamera = m_pScene->GetMainCamera();
		Vector2	MousePos = CInput::GetInst()->GetMousePos() + pCamera->GetPos();

		// ¿· ÀÎµ¦½Â
		//if(pStage->GetTile((pTile->GetTileIndex() - 1))->GetTileCollisionType() == ColiisionType)
		pStage->MouseOnTileCollision(MousePos, m_CollTile);
	}
}

void CEditorSceneMode::ChangeTileScreen(float DeltaTime)
{
	CStageEditMode* pStage = m_pDlg->GetStage();

	if (pStage->GetVisibleTileResource())
	{
		pStage->SetVisibleTileResource(false);
		m_TileResourceModeCamPos = m_pScene->GetMainCamera()->GetPos();
		m_pScene->GetMainCamera()->SetPos(m_EditorModeCamPos);
	}
	else
	{
		pStage->SetVisibleTileResource(true);
		m_EditorModeCamPos = m_pScene->GetMainCamera()->GetPos();
		m_pScene->GetMainCamera()->SetPos(m_TileResourceModeCamPos);
	}
}
