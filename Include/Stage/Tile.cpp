
#include "Tile.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Camera.h"
#include "../GameManager.h"
#include "../Scene/SceneCollision.h"
#include "../Collision/ColliderLine.h"
#include "../Scene/EditorSceneMode.h"

CTile::CTile() :
    m_pScene(nullptr),
    m_pStage(nullptr),
    m_pAnimation(nullptr),
    m_TileType(TileType::None),
    m_Render(false),
    m_Line(nullptr)
{
}

CTile::~CTile()
{
    SAFE_DELETE(m_pAnimation);
    SAFE_DELETE(m_Line);
    ClearImage();    
}

void CTile::CreateTileCollisionLine()
{
    SAFE_DELETE(m_Line);

    
	m_Line = new CColliderLine;
    m_Line->AddRef();
    m_Line->SetCollisionProfile("Map");
    m_Line->Init();
    m_Line->m_pScene = m_pScene;
	m_Line->SetStartPoint(m_Pos.x, m_Pos.y - 1.f);
	m_Line->SetEndPoint(m_Pos + m_Size);

    m_Render = true;
}

void CTile::SetTexture(const std::string& Name)
{
    m_vecTileDrawInfo[m_DrawInfoCount - 1]->m_Texture = m_pScene->GetResourceManager()->FindTexture(Name);

    m_Render = true;
}

void CTile::SetTexture(const std::string& Name, const TCHAR* pFileName, const std::string& PathName)
{
    m_pScene->GetResourceManager()->LoadTexture(Name, pFileName, PathName);
    m_vecTileDrawInfo[m_DrawInfoCount - 1]->m_Texture = m_pScene->GetResourceManager()->FindTexture(Name);

    m_Render = true;
}

void CTile::SetTexture(CTexture* pTexture)
{
    m_vecTileDrawInfo[m_DrawInfoCount - 1]->m_Texture = pTexture;

    if (pTexture)
        m_Render = true;
}

void CTile::SetColorKey(unsigned char r, unsigned char g, unsigned char b, int Index)
{
    if (m_vecTileDrawInfo[m_DrawInfoCount - 1]->m_Texture)
        m_vecTileDrawInfo[m_DrawInfoCount - 1]->m_Texture->SetColorKey(r, g, b, Index);
}

bool CTile::AddAnimation(const std::string& Name, AnimationType Type, bool Loop, CTexture* pTexture, const std::vector<AnimationFrameData>& vecFrame, float PlayTime)
{
    if (!m_pAnimation)
    {
        m_pAnimation = new CAnimation;

        m_pAnimation->m_pScene = m_pScene;
    }

    m_Render = true;

    return m_pAnimation->AddAnimation(Name, Type, Loop, pTexture,
        vecFrame, PlayTime);
}

bool CTile::AddAnimation(const std::string& Name, AnimationType Type, bool Loop, CTexture* pTexture, float PlayTime)
{
    if (!m_pAnimation)
    {
        m_pAnimation = new CAnimation;

        m_pAnimation->m_pScene = m_pScene;
    }

    m_Render = true;

    return m_pAnimation->AddAnimation(Name, Type, Loop, pTexture, PlayTime);
}

bool CTile::AddAnimation(const std::string& Name, AnimationType Type, bool Loop, const std::string& TextureName, float PlayTime)
{
    if (!m_pAnimation)
    {
        m_pAnimation = new CAnimation;

        m_pAnimation->m_pScene = m_pScene;
    }

    m_Render = true;

    return m_pAnimation->AddAnimation(Name, Type, Loop, TextureName, PlayTime);
}

bool CTile::AddAnimation(const std::string& Name, AnimationType Type, bool Loop, const std::string& TextureName, const TCHAR* pTexFileName, float PlayTime)
{
    if (!m_pAnimation)
    {
        m_pAnimation = new CAnimation;

        m_pAnimation->m_pScene = m_pScene;
    }

    m_Render = true;

    return m_pAnimation->AddAnimation(Name, Type, Loop, TextureName, pTexFileName, PlayTime);
}

bool CTile::AddAnimationFrameData(const std::string& Name, const Vector2& ImageStartPos, const Vector2& Size)
{
    if (!m_pAnimation)
        return false;

    return m_pAnimation->AddAnimationFrameData(Name, ImageStartPos, Size);
}

bool CTile::AddAnimationFrameData(const std::string& Name, const Vector2& ImageStartPos, const Vector2& Size, int Count)
{
    if (!m_pAnimation)
        return false;

    return m_pAnimation->AddAnimationFrameData(Name, ImageStartPos, Size, Count);
}

void CTile::SetPlayScale(const std::string& Name, float Scale)
{
    if (!m_pAnimation)
        return;

    m_pAnimation->SetPlayScale(Name, Scale);
}

void CTile::SetCurrentAnimation(const std::string& Name)
{
    if (!m_pAnimation)
        return;

    m_pAnimation->SetCurrentAnimation(Name);
}

void CTile::ChangeAnimation(const std::string& Name)
{
    if (!m_pAnimation)
        return;

    m_pAnimation->ChangeAnimation(Name);
}

void CTile::SetAnimationReverse(const std::string& Name, bool bReverse)
{
    if (!m_pAnimation)
        return;

    m_pAnimation->SetAnimationReverse(Name, bReverse);
}

void CTile::Start()
{
    if (m_pAnimation)
    {
        const AnimationFrameData& FrameData =
            m_pAnimation->m_pCurrentAnim->vecFrameData[m_pAnimation->m_pCurrentAnim->Frame];

        m_Size = FrameData.Size;
    }
}

bool CTile::Init()
{
    return true;
}

void CTile::Update(float DeltaTime)
{
    if (m_pAnimation)
        m_pAnimation->Update(DeltaTime);

    if (m_Line)
        m_Line->Update(DeltaTime);
}

void CTile::Render(HDC hDC, float DeltaTime)
{
    if (!m_Render)
        return;

    Vector2 RenderPos = m_Pos - m_pScene->GetMainCamera()->GetPos();

    // 애니메이션이 있을 경우와 없을 경우로 나누어준다.
    if (m_pAnimation)
    {
        const AnimationFrameData& FrameData =
            m_pAnimation->m_pCurrentAnim->vecFrameData[m_pAnimation->m_pCurrentAnim->Frame];

        if (m_pAnimation->m_pCurrentAnim->AnimType == AnimationType::Sprite)
        {
            m_pAnimation->m_pCurrentAnim->pTexture->Render(hDC, RenderPos,
                FrameData.ImageStartPos, FrameData.Size);
        }

        else
        {
            m_pAnimation->m_pCurrentAnim->pTexture->Render(hDC, RenderPos,
                FrameData.ImageStartPos, FrameData.Size,
                m_pAnimation->m_pCurrentAnim->Frame);
        }
    }

    else
    {
        // Texture가 있을 경우 Texture로 출력을 하고 Texture가 없을 경우 Rect로 출력을
        // 해주도록 한다.
        if (m_DrawInfoCount > 0)
        {
            for (int i = 0; i < m_DrawInfoCount; i++)
            {
                m_vecTileDrawInfo[i]->m_Texture->Render(hDC, RenderPos, m_vecTileDrawInfo[i]->m_ImagePos, m_Size);
            }
        }
    }
    
	if (m_Line)
		m_Line->Render(hDC, DeltaTime);

}

void CTile::Collision(float DeltaTime)
{
	if (m_Line)
		m_pScene->GetCollision()->AddCollider((CCollider*)m_Line);
}

void CTile::RenderDebug(HDC hDC, float DeltaTime)
{
    if (!m_pScene->GetSceneMode()->IsShowTileSide()) return;
    
#ifdef _DEBUG

    Vector2 RenderPos = m_Pos - m_pScene->GetMainCamera()->GetPos();

    HBRUSH  Brush = CGameManager::GetInst()->GetGreenBrush();

    // TileType에 따라 사각형을 그려준다.
    switch (m_TileType)
    {
    case TileType::None:
        Brush = CGameManager::GetInst()->GetGreenBrush();
        break;
    case TileType::Block:
        Brush = CGameManager::GetInst()->GetRedBrush();
        break;
    case TileType::Slow:
        Brush = CGameManager::GetInst()->GetGreenBrush();
        break;
    case TileType::Damage:
        Brush = CGameManager::GetInst()->GetGreenBrush();
        break;
    }

    HGDIOBJ Prev = SelectObject(hDC, Brush);

    Rectangle(hDC, (int)RenderPos.x, (int)RenderPos.y, (int)RenderPos.x + (int)m_Size.x, (int)RenderPos.y + 3);
    Rectangle(hDC, (int)RenderPos.x, (int)RenderPos.y + (int)m_Size.y - 3, (int)RenderPos.x + (int)m_Size.x, (int)RenderPos.y + (int)m_Size.y);
    Rectangle(hDC, (int)RenderPos.x, (int)RenderPos.y, (int)RenderPos.x + 3, (int)RenderPos.y + (int)m_Size.y);
    Rectangle(hDC, (int)RenderPos.x + (int)m_Size.x - 3, (int)RenderPos.y, (int)RenderPos.x + (int)m_Size.x, (int)RenderPos.y + (int)m_Size.y);


    SelectObject(hDC, Prev);

#endif

}

void CTile::Save(FILE* pFile)
{
    fwrite(&m_Pos, sizeof(Vector2), 1, pFile);

    fwrite(&m_Size, sizeof(Vector2), 1, pFile);
    fwrite(&m_TileType, sizeof(TileType), 1, pFile);
    fwrite(&m_IndexX, sizeof(int), 1, pFile);
    fwrite(&m_IndexY, sizeof(int), 1, pFile);
    fwrite(&m_Index, sizeof(int), 1, pFile);
    fwrite(&m_Render, sizeof(bool), 1, pFile);
    fwrite(&m_DrawInfoCount, sizeof(int), 1, pFile);
    for (size_t i = 0; i < m_DrawInfoCount; i++)
    {
        fwrite(&m_vecTileDrawInfo[i]->m_ImagePos, sizeof(Vector2), 1, pFile);
        if (m_vecTileDrawInfo[i]->m_Texture)
        {
            bool    bTex = true;
            fwrite(&bTex, sizeof(bool), 1, pFile);

            m_vecTileDrawInfo[i]->m_Texture->Save(pFile);
        }

        else
        {
            bool    bTex = false;
            fwrite(&bTex, sizeof(bool), 1, pFile);
        }
    }
    bool    isLine = m_Line ? true : false;

    fwrite(&isLine, sizeof(bool), 1, pFile);
    if (isLine)
    {
        Vector2 StartPoint = (m_Line->GetStartPoint());
        Vector2 EndPoint = (m_Line->GetEndPoint());
        fwrite(&StartPoint, sizeof(Vector2), 1, pFile);
        fwrite(&EndPoint, sizeof(Vector2), 1, pFile);
    }
    //if (m_pAnimation)
    //{
    //    bool    bAnim = true;
    //    fwrite(&bAnim, sizeof(bool), 1, pFile);

    //    m_pAnimation->Save(pFile);
    //}

    //else
    //{
    //    bool    bAnim = false;
    //    fwrite(&bAnim, sizeof(bool), 1, pFile);
    //}
}

void CTile::Load(FILE* pFile)
{
    fread(&m_Pos, sizeof(Vector2), 1, pFile);

    fread(&m_Size, sizeof(Vector2), 1, pFile);
    fread(&m_TileType, sizeof(TileType), 1, pFile);
    fread(&m_IndexX, sizeof(int), 1, pFile);
    fread(&m_IndexY, sizeof(int), 1, pFile);
    fread(&m_Index, sizeof(int), 1, pFile);
    fread(&m_Render, sizeof(bool), 1, pFile);

    fread(&m_DrawInfoCount, sizeof(int), 1, pFile);
    for (size_t i = 0; i < m_DrawInfoCount; i++)
    {
        TileDrawInfo* pInfo = new TileDrawInfo;
        fread(&pInfo->m_ImagePos, sizeof(Vector2), 1, pFile);

        bool    bTex = false;
        fread(&bTex, sizeof(bool), 1, pFile);
        if (bTex)
        {
            std::string TexName;
            CTexture::Load(TexName, pFile, m_pScene);

            pInfo->m_Texture = m_pScene->GetResourceManager()->FindTexture(TexName);
        }
        m_vecTileDrawInfo.push_back(pInfo);
    }

    bool    isLine;

    fread(&isLine, sizeof(bool), 1, pFile);
	if (isLine)
	{
        Vector2 StartPoint;
        Vector2 EndPoint;
        fread(&StartPoint, sizeof(Vector2), 1, pFile);
        fread(&EndPoint, sizeof(Vector2), 1, pFile);

        CreateTileCollisionLine();
        m_Line->SetStartPoint(StartPoint);
        m_Line->SetEndPoint(EndPoint);
	}
}
