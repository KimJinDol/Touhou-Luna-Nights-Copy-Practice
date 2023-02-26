
#include "Stage.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Camera.h"
#include "Tile.h"
#include "../PathManager.h"

CStage::CStage() :
    m_pScene(nullptr),
    m_Start(false),
    m_pTileArray(nullptr),
    m_TileCount(0),
    m_TileCapacity(0),
    m_TileCountX(0),
    m_TileCountY(0),
    m_TileUpdate(false),
    m_TileRender(false),
    m_CreateTile(false)
{
}

CStage::~CStage()
{
    auto    iter = m_vecBackTexture.begin();
    auto    iterEnd = m_vecBackTexture.end();

    for (; iter != iterEnd; ++iter)
    {
        SAFE_DELETE((*iter));
    }

    m_vecBackTexture.clear();

    for (int i = 0; i < m_TileCount; ++i)
    {
        SAFE_DELETE(m_pTileArray[i]);
    }

    SAFE_DELETE_ARRAY(m_pTileArray);
}

void CStage::AddBackTexture(const std::string& Name)
{
    BackInfo* pInfo = new BackInfo;

    pInfo->BackTexture = m_pScene->GetResourceManager()->FindTexture(Name);
    pInfo->Size.x = (float)pInfo->BackTexture->GetWidth();
    pInfo->Size.y = (float)pInfo->BackTexture->GetHeight();

    m_vecBackTexture.push_back(pInfo);

    //Vector2 WorldSize;

    //size_t  Count = m_vecBackTexture.size();

    //for (size_t i = 0; i < Count; ++i)
    //{
    //    WorldSize += m_vecBackTexture[i]->Size;
    //}

    //CCamera* pCamera = m_pScene->GetMainCamera();

    //pCamera->SetWorldSize(WorldSize);
}

void CStage::AddBackTexture(const std::string& Name, const TCHAR* pFileName, const std::string& PathName)
{
    m_pScene->GetResourceManager()->LoadTexture(Name, pFileName, PathName);

    BackInfo* pInfo = new BackInfo;

    pInfo->BackTexture = m_pScene->GetResourceManager()->FindTexture(Name);
    pInfo->Size.x = (float)pInfo->BackTexture->GetWidth();
    pInfo->Size.y = (float)pInfo->BackTexture->GetHeight();

    m_vecBackTexture.push_back(pInfo);

    //Vector2 WorldSize;

    //size_t  Count = m_vecBackTexture.size();

    //for (size_t i = 0; i < Count; ++i)
    //{
    //    WorldSize += m_vecBackTexture[i]->Size;
    //}

    //CCamera* pCamera = m_pScene->GetMainCamera();

    //pCamera->SetWorldSize(WorldSize);
}

void CStage::SetBackTexture(int Index, const std::string& Name)
{
    m_vecBackTexture[Index]->BackTexture = m_pScene->GetResourceManager()->FindTexture(Name);
}

void CStage::SetBackTexture(int Index, const std::string& Name, const TCHAR* pFileName, const std::string& PathName)
{
    m_pScene->GetResourceManager()->LoadTexture(Name, pFileName, PathName);
    m_vecBackTexture[Index]->BackTexture = m_pScene->GetResourceManager()->FindTexture(Name);
}

void CStage::SetPos(int Index, const Vector2& Pos)
{
    m_vecBackTexture[Index]->Pos = Pos;
}

void CStage::SetPos(int Index, float x, float y)
{
    m_vecBackTexture[Index]->Pos = Vector2(x, y);
}

void CStage::SetSize(int Index, const Vector2& Size)
{
    m_vecBackTexture[Index]->Size = Size;
}

void CStage::SetSize(int Index, float x, float y)
{
    m_vecBackTexture[Index]->Size = Vector2(x, y);
}

bool CStage::CreateTile(bool NewStage, int CountX, int CountY, const Vector2& TileSize, const std::string& BaseTileTextureName,
    const TCHAR* pTextureFileName, const std::string& BaseTileTexturePathName)
{
    m_CreateTile = true;
    
    if (NewStage)
    {
        for (int i = 0; i < m_TileCount; ++i)
        {
            SAFE_DELETE(m_pTileArray[i]);
        }

        SAFE_DELETE_ARRAY(m_pTileArray);


        m_TileCountX = CountX;
        m_TileCountY = CountY;
        m_TileSize = TileSize;

        m_TileCount = m_TileCountX * m_TileCountY;
        m_TileCapacity = m_TileCount;

        m_pTileArray = new CTile * [m_TileCapacity];

        for (int i = 0; i < m_TileCountY; ++i)
        {
            for (int j = 0; j < m_TileCountX; ++j)
            {
                int Index = i * m_TileCountX + j;

                CTile* pTile = new CTile;

                pTile->m_pScene = m_pScene;
                pTile->m_pStage = this;

                pTile->SetPos(j * m_TileSize.x, i * m_TileSize.y);
                pTile->SetSize(m_TileSize);
                pTile->SetIndex(j, i, Index);

                if (!BaseTileTextureName.empty())
                {
                    m_TileRender = true;

                    if (pTextureFileName)
                    {
                        pTile->SetTexture(BaseTileTextureName, pTextureFileName, BaseTileTexturePathName);
                    }

                    else
                    {
                        pTile->SetTexture(BaseTileTextureName);
                    }
                }

                m_pTileArray[Index] = pTile;
            }
        }

        CCamera* pCamera = m_pScene->GetMainCamera();

        pCamera->SetWorldSize(m_TileSize * Vector2((float)m_TileCountX, (float)m_TileCountY));

    }
    else
    {
        int TempTileCountX = CountX;
        int TempTileCountY = CountY;
        int TempTileCount = TempTileCountX * TempTileCountY;
        Vector2 TempTileSize = TileSize;

        CTile** TempTileArray = new CTile * [TempTileCount];

        if (TempTileCountX < m_TileCountX || TempTileCountY < m_TileCountY)
        {
            for (int i = 0; i < TempTileCountY; ++i)
            {
                for (int j = 0; j < m_TileCountX; ++j)
                {
                    int Index = i * m_TileCountX + j;
                    
                    if (j > TempTileCountX || i > TempTileCountY)
                    {
                        SAFE_DELETE(m_pTileArray[Index]);
                    }
                    else
                    {
                        int index = i * TempTileCountX + j;
                        TempTileArray[index] = m_pTileArray[Index];
                    }
                }
            }
        }
        else
        { 
            for (int i = 0; i < TempTileCountY; ++i)
            {
                for (int j = 0; j < TempTileCountX; ++j)
                {
                    int Index = i * TempTileCountX + j;

                    if (j >= m_TileCountX || i >= m_TileCountY)
                    {
                        CTile* pTile = new CTile;

                        pTile->m_pScene = m_pScene;
                        pTile->m_pStage = this;

                        pTile->SetPos(j * m_TileSize.x, i * m_TileSize.y);
                        pTile->SetSize(m_TileSize);
                        pTile->SetIndex(j, i, Index);

                        if (!BaseTileTextureName.empty())
                        {
                            m_TileRender = true;

                            if (pTextureFileName)
                            {
                                pTile->SetTexture(BaseTileTextureName, pTextureFileName, BaseTileTexturePathName);
                            }

                            else
                            {
                                pTile->SetTexture(BaseTileTextureName);
                            }
                        }

                        TempTileArray[Index] = pTile;
                    }
                    else
                    {
                        int index = i * m_TileCountX + j;
                        TempTileArray[Index] = m_pTileArray[index];
                    }
                }
            }
        }

        SAFE_DELETE_ARRAY(m_pTileArray);

        m_TileCountX = TempTileCountX;
        m_TileCountY = TempTileCountY;
        m_TileCount = TempTileCountX * TempTileCountY;
        m_TileSize = TileSize;
        m_TileCapacity = m_TileCount;
        m_pTileArray = TempTileArray;
        CCamera* pCamera = m_pScene->GetMainCamera();

        pCamera->SetWorldSize(m_TileSize* Vector2((float)m_TileCountX, (float)m_TileCountY));
    }
    return true;
}

void CStage::SetTileColorKey(unsigned char r, unsigned char g, unsigned char b)
{
    for (int i = 0; i < m_TileCount; ++i)
    {
        m_pTileArray[i]->SetColorKey(r, g, b);
    }
}

void CStage::SetAllTileTexture(const std::string& Name, const TCHAR* pFileName, const std::string& PathName)
{
    for (int i = 0; i < m_TileCount; ++i)
    {
        m_pTileArray[i]->SetTexture(Name, pFileName, PathName);
    }
}

void CStage::SetAllTileTexture(const std::string& Name)
{
    for (int i = 0; i < m_TileCount; ++i)
    {
        m_pTileArray[i]->SetTexture(Name);
    }
}

void CStage::ChangeTileType(const Vector2& Pos, TileType Type)
{
    CTile* pTile = GetTile(Pos);

    if (!pTile)
        return;

    pTile->SetTileType(Type);
}

void CStage::ChangeTileCollision(const Vector2& Pos)
{
	CTile* pTile = GetTile(Pos);
    if (!pTile)
        return;

    pTile->CreateTileCollisionLine();
}

void CStage::MouseOnTileCollision(const Vector2& Pos, CTile* pTile)
{
    if (!pTile)
        return;

    Vector2 LinePos = GetTile(Pos)->GetPos();

    float Angle = pTile->GetCollisionLine()->GetStartPoint().GetAngle(LinePos);

    // LT_RB 
    if (Angle < 180)
    {
        CColliderLine* pLine = pTile->GetCollisionLine();
        pLine->SetStartPoint(pLine->GetStartPoint().x, pLine->GetStartPoint().y - 1.f);
    }
    else if(Angle > 180)
        LinePos.y -= 1.f;

    pTile->GetCollisionLine()->SetEndPoint(LinePos);
}

void CStage::ChangeTileImage(const Vector2& Pos, CTexture* pTexture, const Vector2& StartPos, const Vector2& Size)
{
    CTile* pTile = GetTile(Pos);

    if (!pTile)
        return;

    pTile->SetTexture(pTexture);
    pTile->SetImagePos(StartPos);
    pTile->SetSize(Size);

    m_TileRender = true;
}

void CStage::AddTileImage(const Vector2& Pos, CTexture* pTexture, const Vector2& StartPos, const Vector2& Size)
{
    CTile* pTile = GetTile(Pos);

    if (!pTile)
        return;

    pTile->AddDrawInfo(StartPos, pTexture);
    pTile->SetSize(Size);

    m_TileRender = true;
}

void CStage::Start()
{
    m_Start = true;

    // 월드크기를 현재 스테이지에 지정된 모든 텍스처의 가로, 세로 크기를 합산한 크기로 설정한다.
    /*Vector2 WorldSize;

    size_t  Count = m_vecBackTexture.size();

    for (size_t i = 0; i < Count; ++i)
    {
        WorldSize += m_vecBackTexture[i]->Size;
    }

    CCamera* pCamera = m_pScene->GetMainCamera();

    pCamera->SetWorldSize(WorldSize);*/
}

bool CStage::Init()
{
    return true;
}

void CStage::Update(float DeltaTime)
{
    if (m_CreateTile)
    {
        // 시작과 끝 타일 인덱스를 구한다.
        CCamera* pCamera = m_pScene->GetMainCamera();

        Vector2 StartPos = pCamera->GetPos();
        Vector2 EndPos = pCamera->GetPos() + pCamera->GetResolution();

        m_StartX = (int)StartPos.x / (int)m_TileSize.x;
        m_StartY = (int)StartPos.y / (int)m_TileSize.y;

        m_EndX = (int)EndPos.x / (int)m_TileSize.x;
        m_EndY = (int)EndPos.y / (int)m_TileSize.y;

        if (m_StartX < 0)
            m_StartX = 0;

        else if (m_StartX >= m_TileCountX)
            m_StartX = m_TileCountX;

        if (m_StartY < 0)
            m_StartY = 0;

        else if (m_StartY >= m_TileCountY)
            m_StartY = m_TileCountY;

        if (m_EndX >= m_TileCountX)
            m_EndX = m_TileCountX - 1;

        else if (m_EndX < 0)
            m_EndX = -1;

        if (m_EndY >= m_TileCountY)
            m_EndY = m_TileCountY - 1;

        else if (m_EndY < 0)
            m_EndY = -1;
    }

    if (m_TileUpdate)
    {
        for (int i = m_StartY; i <= m_EndY; ++i)
        {
            for (int j = m_StartX; j <= m_EndX; ++j)
            {
                int Index = i * m_TileCountX + j;

                m_pTileArray[Index]->Update(DeltaTime);
            }
        }
    }
}

void CStage::Render(HDC hDC, float DeltaTime)
{
    CCamera* pCamera = m_pScene->GetMainCamera();
    
    size_t  Count = m_vecBackTexture.size();

    for (size_t i = 0; i < Count; ++i)
    {
        Vector2 RenderPos = m_vecBackTexture[i]->Pos - pCamera->GetPos();

  /*      m_vecBackTexture[i]->BackTexture->Render(hDC, Vector2(0, 0), Vector2(0.f, 0.f),
            m_vecBackTexture[i]->Size);*/
    }

    if (m_TileRender)
    {
        for (int i = m_StartY; i <= m_EndY; ++i)
        {
            for (int j = m_StartX; j <= m_EndX; ++j)
            {
                int Index = i * m_TileCountX + j;

                m_pTileArray[Index]->Render(hDC, DeltaTime);
            }
        }
    }

#ifdef _DEBUG
    if (m_CreateTile)
    {
        for (int i = m_StartY; i <= m_EndY; ++i)
        {
            for (int j = m_StartX; j <= m_EndX; ++j)
            {
                int Index = i * m_TileCountX + j;

                m_pTileArray[Index]->RenderDebug(hDC, DeltaTime);
            }
        }
    }
#endif // _DEBUG

}

void CStage::Collision(float DeltaTime)
{
    for (int i = 0; i < m_TileCount; i++)
    {
        m_pTileArray[i]->Collision(DeltaTime);
    }
}

void CStage::Save(FILE* pFile)
{
    int Count = (int)m_vecBackTexture.size();

    // BackTexture의 개수를 저장한다.
    
    fwrite(&Count, sizeof(int), 1, pFile);

    for (int i = 0; i < Count; ++i)
    {
        fwrite(&m_vecBackTexture[i]->Pos, sizeof(Vector2), 1, pFile);
        fwrite(&m_vecBackTexture[i]->Size, sizeof(Vector2), 1, pFile);

        m_vecBackTexture[i]->BackTexture->Save(pFile);
    }

    // fwrite(&m_TileCount, sizeof(int), 1, pFile);
    fwrite(&m_TileCapacity, sizeof(int), 1, pFile);
    fwrite(&m_TileCountX, sizeof(int), 1, pFile);
    fwrite(&m_TileCountY, sizeof(int), 1, pFile);

    fwrite(&m_TileSize, sizeof(Vector2), 1, pFile);
    fwrite(&m_TileUpdate, sizeof(bool), 1, pFile);
    fwrite(&m_TileRender, sizeof(bool), 1, pFile);
    fwrite(&m_CreateTile, sizeof(bool), 1, pFile);

    for (int i = 0; i < m_TileCount; ++i)
    {
        m_pTileArray[i]->Save(pFile);
    }
}

void CStage::Load(FILE* pFile)
{
    auto    iter = m_vecBackTexture.begin();
    auto    iterEnd = m_vecBackTexture.end();

    for (; iter != iterEnd; ++iter)
    {
        SAFE_DELETE((*iter));
    }

    m_vecBackTexture.clear();

    int Count = 0;

    // BackTexture의 개수를 저장한다.
    fread(&Count, sizeof(int), 1, pFile);

    for (int i = 0; i < Count; ++i)
    {
        BackInfo* pInfo = new BackInfo;

        fread(&pInfo->Pos, sizeof(Vector2), 1, pFile);
        fread(&pInfo->Size, sizeof(Vector2), 1, pFile);

        std::string TexName;
        CTexture::Load(TexName, pFile, m_pScene);

        pInfo->BackTexture = m_pScene->GetResourceManager()->FindTexture(TexName);

        m_vecBackTexture.push_back(pInfo);
    }

    // fread(&m_TileCount, sizeof(int), 1, pFile);
    fread(&m_TileCapacity, sizeof(int), 1, pFile);
    fread(&m_TileCountX, sizeof(int), 1, pFile);
    fread(&m_TileCountY, sizeof(int), 1, pFile);

    fread(&m_TileSize, sizeof(Vector2), 1, pFile);
    fread(&m_TileUpdate, sizeof(bool), 1, pFile);
    fread(&m_TileRender, sizeof(bool), 1, pFile);
    fread(&m_CreateTile, sizeof(bool), 1, pFile);

    CreateTile(true, m_TileCountX, m_TileCountY, Vector2(m_TileSize));
 
    for (int i = 0; i < m_TileCount; ++i)
    {
        m_pTileArray[i]->m_pScene = m_pScene;
        m_pTileArray[i]->m_pStage = this;
        m_pTileArray[i]->Load(pFile);
    }

    //Vector2 WorldSize;

    //size_t  Count1 = m_vecBackTexture.size();

    //for (size_t i = 0; i < Count1; ++i)
    //{
    //    WorldSize += m_vecBackTexture[i]->Size;
    //}

    //CCamera* pCamera = m_pScene->GetMainCamera();

    //pCamera->SetWorldSize(WorldSize);

    m_TileRender = true;
}

void CStage::SaveFile(const char* pFullPath)
{
    FILE* pFile = nullptr;

    fopen_s(&pFile, pFullPath, "wb");

    if (!pFile)
        return;

    Save(pFile);

    fclose(pFile);
}

void CStage::LoadFile(const char* pFullPath)
{
    FILE* pFile = nullptr;

    fopen_s(&pFile, pFullPath, "rb");

    if (!pFile)
        return;

    Load(pFile);

    fclose(pFile);
}

void CStage::SaveFile(const char* pFileName, const std::string& PathName)
{
    char    FullPath[MAX_PATH] = {};

    const PathInfo* pPathInfo = CPathManager::GetInst()->FindPath(PathName);

    if (pPathInfo)
        strcpy_s(FullPath, pPathInfo->PathMultibyte);

    strcat_s(FullPath, pFileName);

    FILE* pFile = nullptr;

    fopen_s(&pFile, FullPath, "wb");

    if (!pFile)
        return;

    Save(pFile);

    fclose(pFile);
}

void CStage::LoadFile(const char* pFileName, const std::string& PathName)
{
    char    FullPath[MAX_PATH] = {};

    const PathInfo* pPathInfo = CPathManager::GetInst()->FindPath(PathName);

    if (pPathInfo)
        strcpy_s(FullPath, pPathInfo->PathMultibyte);

    strcat_s(FullPath, pFileName);

    FILE* pFile = nullptr;

    fopen_s(&pFile, FullPath, "rb");

    if (!pFile)
        return;

    Load(pFile);

    fclose(pFile);
}
