
#include "EditorDlg.h"
#include "../GameManager.h"
#include "../Scene/Scene.h"
#include "../Stage/Stage.h"
#include "../PathManager.h"
#include "../Scene/SceneResource.h"
#include "../Stage/StageEditMode.h"

CEditorDlg* g_pDlg = nullptr;

CEditorDlg::CEditorDlg() :
    m_hDlg(0),
    m_ID(0),
    m_pScene(nullptr),
    m_pStage(nullptr)
{
    g_pDlg = this;
}

CEditorDlg::~CEditorDlg()
{
    if (m_hDlg)
        DestroyWindow(m_hDlg);
}

CTexture* CEditorDlg::GetTileTexture()
{
    int Index = (int)SendMessage(m_hTileImageCombo, CB_GETCURSEL, 0, 0);

    if (Index == -1 || m_vecTileTexture.size() <= Index)
        return nullptr;

    return m_vecTileTexture[Index];
}

bool CEditorDlg::Init(int ID)
{
    if (m_hDlg)
        return true;
     
    m_ID = ID;

    m_hDlg = CreateDialog(CGameManager::GetInst()->GetWindowInstance(), MAKEINTRESOURCE(ID),
        CGameManager::GetInst()->GetWindowHandle(), CEditorDlg::WndProc);
  
    ShowCursor(true);
    ShowWindow(m_hDlg, SW_SHOW);

    SetDlgItemInt(m_hDlg, IDC_EDIT_TILECOUNTX, 38, TRUE);
    SetDlgItemInt(m_hDlg, IDC_EDIT_TILECOUNTY, 23, TRUE);

    SetDlgItemInt(m_hDlg, IDC_EDIT_TILESIZEX, 68, TRUE);
    SetDlgItemInt(m_hDlg, IDC_EDIT_TILESIZEY, 68, TRUE);

    SetDlgItemInt(m_hDlg, IDC_EDIT_TILESTARTPOSX, 0, TRUE);
    SetDlgItemInt(m_hDlg, IDC_EDIT_TILESTARTPOSY, 0, TRUE);

    m_hTileTypeCombo = GetDlgItem(m_hDlg, IDC_COMBO_TILETYPE);

    TCHAR   TileTypeItem[(int)TileType::End][30] =
    {
        TEXT("일반"),
        TEXT("이동불가"),
        TEXT("이동속도감소"),
        TEXT("데미지")
    };

    for (int i = 0; i < (int)TileType::End; ++i)
    {
        SendMessage(m_hTileTypeCombo, CB_ADDSTRING, 0, (LPARAM)TileTypeItem[i]);
    }

    SendMessage(m_hTileTypeCombo, CB_SETCURSEL, 1, 0);


    m_hTileImageCombo = GetDlgItem(m_hDlg, IDC_COMBO_TILERESOURCE);
    m_hTileEditCombo = GetDlgItem(m_hDlg, IDC_COMBO_TILEEDITMODE);

    TCHAR   TileEditItem[(int)TileEditMode::End][30] =
    {
        TEXT("타일 타입"),
        TEXT("타일 콜리전"),
        TEXT("타일 이미지"),
        TEXT("타일 비우기")
    };

    for (int i = 0; i < (int)TileEditMode::End; ++i)
    {
        SendMessage(m_hTileEditCombo, CB_ADDSTRING, 0, (LPARAM)TileEditItem[i]);
    }

    SendMessage(m_hTileEditCombo, CB_SETCURSEL, 0, 0);

    m_EditMode = TileEditMode::Type;

   
    return true;
}

void CEditorDlg::Show()
{
    ShowWindow(m_hDlg, SW_SHOW);
}

void CEditorDlg::CreateMap()
{
    if (!m_pStage)
    {
        m_pStage = m_pScene->CreateStage<CStageEditMode>();
        m_pStage->SetEidtDlg(this);
    }
    // Edit Control에서 정보를 가져온다.
    BOOL    bTransfer = FALSE;
    int TileCountX = GetDlgItemInt(m_hDlg, IDC_EDIT_TILECOUNTX, &bTransfer, TRUE);
    int TileCountY = GetDlgItemInt(m_hDlg, IDC_EDIT_TILECOUNTY, &bTransfer, TRUE);
    int TileSizeX = GetDlgItemInt(m_hDlg, IDC_EDIT_TILESIZEX, &bTransfer, TRUE);
    int TileSizeY = GetDlgItemInt(m_hDlg, IDC_EDIT_TILESIZEY, &bTransfer, TRUE);

    m_pStage->CreateTile(true, TileCountX, TileCountY, Vector2((float)TileSizeX, (float)TileSizeY));
}

void CEditorDlg::ExpendMap()
{
    if (!m_pStage)
    {
        m_pStage = m_pScene->CreateStage<CStageEditMode>();
        m_pStage->SetEidtDlg(this);
    }

    BOOL    bTransfer = FALSE;
    int TileCountX = GetDlgItemInt(m_hDlg, IDC_EDIT_TILECOUNTX, &bTransfer, TRUE);
    int TileCountY = GetDlgItemInt(m_hDlg, IDC_EDIT_TILECOUNTY, &bTransfer, TRUE);
    int TileSizeX = GetDlgItemInt(m_hDlg, IDC_EDIT_TILESIZEX, &bTransfer, TRUE);
    int TileSizeY = GetDlgItemInt(m_hDlg, IDC_EDIT_TILESIZEY, &bTransfer, TRUE);

    m_pStage->CreateTile(false, TileCountX, TileCountY, Vector2((float)TileSizeX, (float)TileSizeY));
}


void CEditorDlg::Save()
{
    if (!m_pStage)
        return;

    TCHAR   FilePath[MAX_PATH] = {};

    OPENFILENAME    OFN = {};

    OFN.lStructSize = sizeof(OPENFILENAME);
    OFN.hwndOwner = m_hDlg;
    OFN.lpstrFilter = TEXT("모든파일\0*.*\0StageFile\0*.stg");
    OFN.lpstrFile = FilePath;
    OFN.nMaxFile = MAX_PATH;
    OFN.lpstrInitialDir = CPathManager::GetInst()->FindPath(DATA_PATH)->Path;

    if (GetSaveFileName(&OFN) != 0)
    {
        char    MultibytePath[MAX_PATH] = {};

        int Count = WideCharToMultiByte(CP_ACP, 0, FilePath, -1, 0, 0, 0, 0);
        WideCharToMultiByte(CP_ACP, 0, FilePath, -1, MultibytePath, Count, 0, 0);

        m_pStage->SaveFile(MultibytePath);
    }
}

void CEditorDlg::Load()
{
    /*if (m_pStage)
    {
        SAFE_DELETE(m_pStage);
    }*/

    m_pStage = m_pScene->CreateStage<CStageEditMode>();
    m_pStage->SetEidtDlg(this);

    TCHAR   FilePath[MAX_PATH] = {};

    OPENFILENAME    OFN = {};

    OFN.lStructSize = sizeof(OPENFILENAME);
    OFN.hwndOwner = m_hDlg;
    OFN.lpstrFilter = TEXT("모든파일\0*.*\0StageFile\0*.stg");
    OFN.lpstrFile = FilePath;
    OFN.nMaxFile = MAX_PATH;
    OFN.lpstrInitialDir = CPathManager::GetInst()->FindPath(DATA_PATH)->Path;

    if (GetOpenFileName(&OFN) != 0)
    {
        char    MultibytePath[MAX_PATH] = {};

        int Count = WideCharToMultiByte(CP_ACP, 0, FilePath, -1, 0, 0, 0, 0);
        WideCharToMultiByte(CP_ACP, 0, FilePath, -1, MultibytePath, Count, 0, 0);

        m_pStage->LoadFile(MultibytePath);
    }
}

void CEditorDlg::LoadBackTexture()
{
    if (!m_pStage)
    {
        m_pStage = m_pScene->CreateStage<CStageEditMode>();
        m_pStage->SetEidtDlg(this);
    }
    TCHAR   FilePath[MAX_PATH] = {};

    OPENFILENAME    OFN = {};

    OFN.lStructSize = sizeof(OPENFILENAME);
    OFN.hwndOwner = m_hDlg;
    OFN.lpstrFilter = TEXT("모든파일\0*.*\0BmpFile\0*.bmp");
    OFN.lpstrFile = FilePath;
    OFN.nMaxFile = MAX_PATH;
    OFN.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->Path;

    if (GetOpenFileName(&OFN) != 0)
    {
        char    MultibytePath[MAX_PATH] = {};

        int Count = WideCharToMultiByte(CP_ACP, 0, FilePath, -1, 0, 0, 0, 0);
        WideCharToMultiByte(CP_ACP, 0, FilePath, -1, MultibytePath, Count, 0, 0);

        _strupr_s(MultibytePath);

        TCHAR   FileName[MAX_PATH] = {};

        // 최종 경로를 Texture 폴더 이후의 파일 이름으로 분리한다.
        size_t  Length = strlen(MultibytePath) - 1;

        for (size_t i = Length; i >= 7; --i)
        {
            if (MultibytePath[i] == '\\')
            {
                bool    bFind = true;
                // Texture
                char   FolderName[8] = {};
                strcpy_s(FolderName, "ERUTXET");
                for (size_t j = 1; j <= 7; ++j)
                {
                    if (MultibytePath[i - j] != FolderName[j - 1])
                    {
                        bFind = false;
                        break;
                    }
                }

                if (bFind)
                {
                    lstrcpy(FileName, &FilePath[i + 1]);
                }
            }
        }

        TCHAR   Key[MAX_PATH] = {};

        _wsplitpath_s(FilePath, 0, 0, 0, 0, Key, MAX_PATH, 0, 0);

        char    MultibyteKey[MAX_PATH] = {};

        Count = WideCharToMultiByte(CP_ACP, 0, Key, -1, 0, 0, 0, 0);
        WideCharToMultiByte(CP_ACP, 0, Key, -1, MultibyteKey, Count, 0, 0);

        m_pStage->AddBackTexture(MultibyteKey, FileName);
    }
}

void CEditorDlg::LoadTileTexture()
{
    if (!m_pStage)
    {
        m_pStage = m_pScene->CreateStage<CStageEditMode>();
        m_pStage->SetEidtDlg(this);
    }
    TCHAR   FilePath[MAX_PATH] = {};

    OPENFILENAME    OFN = {};

    OFN.lStructSize = sizeof(OPENFILENAME);
    OFN.hwndOwner = m_hDlg;
    OFN.lpstrFilter = TEXT("모든파일\0*.*\0BmpFile\0*.bmp");
    OFN.lpstrFile = FilePath;
    OFN.nMaxFile = MAX_PATH;
    OFN.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->Path;

    if (GetOpenFileName(&OFN) != 0)
    {
        char    MultibytePath[MAX_PATH] = {};

        int Count = WideCharToMultiByte(CP_ACP, 0, FilePath, -1, 0, 0, 0, 0);
        WideCharToMultiByte(CP_ACP, 0, FilePath, -1, MultibytePath, Count, 0, 0);

        _strupr_s(MultibytePath);

        TCHAR   FileName[MAX_PATH] = {};

        // 최종 경로를 Texture 폴더 이후의 파일 이름으로 분리한다.
        size_t  Length = strlen(MultibytePath) - 1;

        for (size_t i = Length; i >= 7; --i)
        {
            if (MultibytePath[i] == '\\')
            {
                bool    bFind = true;
                // Texture
                char   FolderName[8] = {};
                strcpy_s(FolderName, "ERUTXET");
                for (size_t j = 1; j <= 7; ++j)
                {
                    if (MultibytePath[i - j] != FolderName[j - 1])
                    {
                        bFind = false;
                        break;
                    }
                }

                if (bFind)
                {
                    lstrcpy(FileName, &FilePath[i + 1]);
                }
            }
        }

        TCHAR   Key[MAX_PATH] = {};

        _wsplitpath_s(FilePath, 0, 0, 0, 0, Key, MAX_PATH, 0, 0);

        char    MultibyteKey[MAX_PATH] = {};

        Count = WideCharToMultiByte(CP_ACP, 0, Key, -1, 0, 0, 0, 0);
        WideCharToMultiByte(CP_ACP, 0, Key, -1, MultibyteKey, Count, 0, 0);

        m_pScene->GetResourceManager()->LoadTexture(MultibyteKey, FileName);

        m_vecTileTexture.push_back(m_pScene->GetResourceManager()->FindTexture(MultibyteKey));

        SendMessage(m_hTileImageCombo, CB_ADDSTRING, 0, (LPARAM)Key);

        //SendMessage(m_hTileImageCombo, CB_SETCURSEL, 1, 0);
    }
}

LRESULT CEditorDlg::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        break;
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            break;
        case IDCANCEL:
            DestroyWindow(hWnd);
            break;
        case IDC_BUTTON_CREATE:
            g_pDlg->CreateMap();
            break;
        case IDC_BUTTON_SAVE:
            g_pDlg->Save();
            break;
        case IDC_BUTTON_LOAD:
            g_pDlg->Load();
            break;
        case IDC_BUTTON_LOADBACKTEXTURE:
            g_pDlg->LoadBackTexture();
            break;
        case IDC_BUTTON_LOADTILEIMAGE:
            g_pDlg->LoadTileTexture();
            break;
        case IDC_COMBO_TILERESOURCE:
            if (HIWORD(wParam) == CBN_SELCHANGE)
            {
            }
            break;
        case IDC_STAGEEXPEND:
            g_pDlg->ExpendMap();
            break;
        }
    }

    return 0;
}
