#pragma once

#include "../GameInfo.h"
#include "../Resource/Texture.h"

class CEditorDlg
{
public:
    CEditorDlg();
    ~CEditorDlg();

private:
    class CScene* m_pScene;
    class CStageEditMode* m_pStage;
    HWND    m_hDlg;
    HWND    m_hTileTypeCombo;
    HWND    m_hTileImageCombo;
    HWND    m_hTileEditCombo;
    HWND    m_hTileCollisionCombo;
    int     m_ID;
    std::vector<CSharedPtr<CTexture>>   m_vecTileTexture;
    TileEditMode    m_EditMode;


public:
    void SetTileResourceImageStartPos(const Vector2& Pos)
    {
        BOOL bTransfer = FALSE;
        int TileSizeX =  GetDlgItemInt(m_hDlg, IDC_EDIT_TILESIZEX, &bTransfer, TRUE);
        int TileSizeY = GetDlgItemInt(m_hDlg, IDC_EDIT_TILESIZEY, &bTransfer, TRUE);
        int x = (int)(Pos.x / TileSizeX) * TileSizeX;
        int y = (int)(Pos.y / TileSizeY) * TileSizeY;

        SetDlgItemInt(m_hDlg, IDC_EDIT_TILESTARTPOSX, x, TRUE);
        SetDlgItemInt(m_hDlg, IDC_EDIT_TILESTARTPOSY, y, TRUE);
    }

public:
    TileEditMode GetEditMode()
    {
        m_EditMode = (TileEditMode)SendMessage(m_hTileEditCombo, CB_GETCURSEL, 0, 0);
        return m_EditMode;
    }

    Vector2 GetImageStartPos()
    {
        Vector2 StartPos;
        BOOL    bTransfer = FALSE;
        StartPos.x = GetDlgItemInt(m_hDlg, IDC_EDIT_TILESTARTPOSX, &bTransfer, TRUE);
        StartPos.y = GetDlgItemInt(m_hDlg, IDC_EDIT_TILESTARTPOSY, &bTransfer, TRUE);

        return StartPos;
    }

    Vector2 GetTileSize()
    {
        Vector2 TileSize;
        BOOL    bTransfer = FALSE;
        TileSize.x = GetDlgItemInt(m_hDlg, IDC_EDIT_TILESIZEX, &bTransfer, TRUE);
        TileSize.y = GetDlgItemInt(m_hDlg, IDC_EDIT_TILESIZEY, &bTransfer, TRUE);

        return TileSize;
    }

    CTexture* GetTileTexture();

public:
    void SetScene(class CScene* pScene)
    {
        m_pScene = pScene;
    }

    class CStageEditMode* GetStage()    const
    {
        return m_pStage;
    }

    int GetTileType()   const
    {
        return (int)SendMessage(m_hTileTypeCombo, CB_GETCURSEL, 0, 0);
    }

    int GetTileCollision()  const
    {
        return (int)SendMessage(m_hTileCollisionCombo, CB_GETCURSEL, 0, 0);
    }

public:
    bool Init(int ID);
    void Show();
    void CreateMap();
    void ExpendMap();
    void Save();
    void Load();
    void LoadBackTexture();
    void LoadTileTexture();

private:
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

