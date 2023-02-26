#include "StageEditMode.h"
#include "../Scene/EditorSceneMode.h"
#include "../Scene/Scene.h"
#include "../Scene/EditorDlg.h"
#include "../Scene/Camera.h"
#include "../Stage/Tile.h"

CStageEditMode::CStageEditMode()
{
}

CStageEditMode::~CStageEditMode()
{
}


void CStageEditMode::TileClear(const Vector2& Pos)
{
    CTile* Tile = GetTile(Pos);

    if (Tile)
        Tile->ClearImage();
}

void CStageEditMode::Render(HDC hDC, float DeltaTime)
{
    if (m_bIsVisibleTileResource)
    {
        CCamera* pCamera = m_pScene->GetMainCamera();
        Vector2 RenderPos = Vector2(0,0) - pCamera->GetPos();
        CTexture* pTexture = m_pEditDlg->GetTileTexture();
        if (!pTexture) return;
        pTexture->Render(hDC, RenderPos, Vector2(0, 0), Vector2(pTexture->GetWidth(), pTexture->GetHeight()));
    }
    else
    {
        CStage::Render(hDC, DeltaTime);
    }
}
