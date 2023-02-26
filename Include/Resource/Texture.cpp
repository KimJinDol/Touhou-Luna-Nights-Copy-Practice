
#include "Texture.h"
#include "../PathManager.h"
#include "../GameManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "ResourceManager.h"

CTexture::CTexture()    :
    m_BasicColorKey(RGB(255,0,255)),
    m_Index(0),
    m_bAlpha(false),
    m_Alpha(100)
{
    
}

CTexture::~CTexture()
{
    size_t  Size = m_vecTexture.size();

    for (size_t i = 0; i < Size; ++i)
    {
        SAFE_DELETE(m_vecTexture[i]);
    }

    CResourceManager::GetInst()->EraseTexture(m_Name);
}


bool CTexture::LoadTexture(const std::string& Name, const TCHAR* pFileName,
    const std::string& PathName)
{
    m_Name = Name;
    m_PathName = PathName;

    m_vecFileName.push_back(pFileName);

    // ��ü��θ� ������ش�.
    TCHAR   FullPath[MAX_PATH] = {};

    // ��θ� ���´�.
    const PathInfo* pPathInfo = CPathManager::GetInst()->FindPath(PathName);

    if (pPathInfo)
        lstrcpy(FullPath, pPathInfo->Path);

    // ����� �ڿ� ���� �̸��� �ٿ��ش�.
    lstrcat(FullPath, pFileName);

    TextureInfo* pInfo = new TextureInfo;

    pInfo->hDC = CreateCompatibleDC(CGameManager::GetInst()->GetWindowDC());
    pInfo->hBmp = (HBITMAP)LoadImage(CGameManager::GetInst()->GetWindowInstance(),
        FullPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    pInfo->hPrev = SelectObject(pInfo->hDC, pInfo->hBmp);
    pInfo->ColorKey = RGB(255, 0, 255);
    pInfo->bColorKey = true;

    GetObject(pInfo->hBmp, sizeof(BITMAP), &pInfo->BmpInfo);

    m_vecTexture.push_back(pInfo);

    return true;
}

bool CTexture::LoadTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName, 
    const std::string& PathName)
{
    m_Name = Name;
    m_PathName = PathName;

    size_t  Size = vecFileName.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecFileName.push_back(vecFileName[i]);

        // ��ü��θ� ������ش�.
        TCHAR   FullPath[MAX_PATH] = {};

        // ��θ� ���´�.
        const PathInfo* pPathInfo = CPathManager::GetInst()->FindPath(PathName);

        if (pPathInfo)
            lstrcpy(FullPath, pPathInfo->Path);

        // ����� �ڿ� ���� �̸��� �ٿ��ش�.
        lstrcat(FullPath, vecFileName[i].c_str());

        TextureInfo* pInfo = new TextureInfo;

        pInfo->hDC = CreateCompatibleDC(CGameManager::GetInst()->GetWindowDC());
        pInfo->hBmp = (HBITMAP)LoadImage(CGameManager::GetInst()->GetWindowInstance(),
            FullPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        pInfo->hPrev = SelectObject(pInfo->hDC, pInfo->hBmp);
		pInfo->ColorKey = RGB(255, 0, 255);
		pInfo->bColorKey = true;

        GetObject(pInfo->hBmp, sizeof(BITMAP), &pInfo->BmpInfo);

        m_vecTexture.push_back(pInfo);
    }

    return true;
}

void CTexture::SetColorKey(unsigned char r, unsigned char g, unsigned char b, int Index)
{
    m_vecTexture[Index]->bColorKey = true;
    m_vecTexture[Index]->ColorKey = RGB(r, g, b);
}

void CTexture::SetColorKey(unsigned int ColorKey, int Index)
{
    m_vecTexture[Index]->bColorKey = true;
    m_vecTexture[Index]->ColorKey = ColorKey;
}

void CTexture::Render(HDC hDC, const Vector2& WindowPos, const Vector2& ImagePos,
    const Vector2& Size, int Index)
{
    if (m_bAlpha)
    {
		BLENDFUNCTION bf;
		bf.BlendOp = 0;
		bf.BlendFlags = 0;
		bf.SourceConstantAlpha = m_Alpha;
		bf.AlphaFormat = AC_SRC_OVER;

		GdiAlphaBlend(hDC, WindowPos.x, WindowPos.y, Size.x, Size.y, m_vecTexture[m_Index]->hDC, ImagePos.x, ImagePos.y, Size.x, Size.y, bf);
    }
    else if (m_vecTexture[m_Index]->bColorKey)
    {
        // 1������ : �̹����� ����� DC
        // 2������ : ����� DC������ X��ǥ
        // 3������ : ����� DC������ Y��ǥ
        // 4������ : ����� DC������ ����ũ��
        // 5������ : ����� DC������ ����ũ��
        // 6������ : �׸����� �ϴ� DC
        // 7������ : ����ϰ��� �ϴ� DC������ X��ǥ
        // 8������ : ����ϰ��� �ϴ� DC������ Y��ǥ
        // 9������ : ����ϰ��� �ϴ� DC������ ����ũ��
        // 10������ : ����ϰ��� �ϴ� DC������ ����ũ��
        // 11������ : �����ϰ��� �ϴ� ����
        TransparentBlt(hDC, (int)WindowPos.x, (int)WindowPos.y, (int)Size.x,
            (int)Size.y, m_vecTexture[m_Index]->hDC, (int)ImagePos.x, (int)ImagePos.y,
            (int)Size.x, (int)Size.y, m_vecTexture[m_Index]->ColorKey);
    }

    else
    {
        BitBlt(hDC, (int)WindowPos.x, (int)WindowPos.y, (int)Size.x,
            (int)Size.y, m_vecTexture[m_Index]->hDC, (int)ImagePos.x, (int)ImagePos.y,
            SRCCOPY);
    }
}

void CTexture::Save(FILE* pFile)
{
    // �̸��� �����Ѵ�.
    int Length = (int)m_Name.length();
    fwrite(&Length, sizeof(int), 1, pFile);
    fwrite(m_Name.c_str(), sizeof(char), Length, pFile);

    // PathName�� �����Ѵ�.
    // ���ڿ� ���̸� �����Ѵ�.
    Length = (int)m_PathName.length();
    fwrite(&Length, sizeof(int), 1, pFile);
    fwrite(m_PathName.c_str(), sizeof(char), Length, pFile);

    // �����̸� ������ �����Ѵ�.
    int Count = (int)m_vecFileName.size();
    fwrite(&Count, sizeof(int), 1, pFile);

    for (int i = 0; i < Count; ++i)
    {
        Length = (int)m_vecFileName[i].length();
        fwrite(&Length, sizeof(int), 1, pFile);
        fwrite(m_vecFileName[i].c_str(), sizeof(wchar_t), Length, pFile);
    }

    for (int i = 0; i < Count; ++i)
    {
        fwrite(&m_vecTexture[i]->bColorKey, sizeof(bool), 1, pFile);
        fwrite(&m_vecTexture[i]->ColorKey, sizeof(unsigned int), 1, pFile);
    }
}

void CTexture::Load(std::string& resultName, FILE* pFile, CScene* pScene)
{
    int Length = 0;

    // �̸��� �ҷ��´�.
    fread(&Length, sizeof(int), 1, pFile);

    char    Name[MAX_PATH] = {};
    fread(Name, sizeof(char), Length, pFile);

    resultName = Name;

    fread(&Length, sizeof(int), 1, pFile);

    char    PathName[MAX_PATH] = {};
    fread(PathName, sizeof(char), Length, pFile);

    int    Count = 0;
    fread(&Count, sizeof(int), 1, pFile);

    std::vector<std::wstring> vecFileName;

    for (int i = 0; i < Count; ++i)
    {
        TCHAR   FileName[MAX_PATH] = {};
        fread(&Length, sizeof(int), 1, pFile);
        fread(FileName, sizeof(wchar_t), Length, pFile);

        vecFileName.push_back(FileName);
    }

    if (vecFileName.size() == 1)
    {
        pScene->GetResourceManager()->LoadTexture(Name, vecFileName[0].c_str(), PathName);
    }

    else
    {
        pScene->GetResourceManager()->LoadTexture(Name, vecFileName, PathName);
    }

    for (int i = 0; i < Count; ++i)
    {
        bool    bColorKey = false;
        unsigned int    ColorKey = 0;
        fread(&bColorKey, sizeof(bool), 1, pFile);
        fread(&ColorKey, sizeof(unsigned int), 1, pFile);

        if (bColorKey)
        {
            // pScene->GetResourceManager()->SetColorKey(Name, ColorKey, i);
        }
    }
}