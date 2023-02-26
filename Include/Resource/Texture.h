#pragma once

#include "../Ref.h"

// 이미지는 HBITMAP 이라는 그리기 도구를 만들어낸 후에 그것을 DC에 지정하여
// 화면에 출력하는 방식이다.
struct TextureInfo
{
    HDC     hDC;
    HBITMAP hBmp;
    HGDIOBJ hPrev;
    BITMAP  BmpInfo;
    bool    bColorKey;
    unsigned int    ColorKey;

    TextureInfo()
    {
        hDC = 0;
        hBmp = 0;
        hPrev = 0;
        memset(&BmpInfo, 0, sizeof(BmpInfo));
        bColorKey = false;
        ColorKey = 0;
    }

    ~TextureInfo()
    {
        if (hPrev)
            SelectObject(hDC, hPrev);

        if (hBmp)
            DeleteObject(hBmp);

        if (hDC)
            DeleteDC(hDC);
    }
};

class CTexture :
    public CRef
{
    friend class CResourceManager;
    friend class CStage;
    friend class CTile;

private:
    CTexture();
    ~CTexture();

private:
    std::string                 m_Name;
    std::vector<TextureInfo*>   m_vecTexture;
    std::vector<std::wstring>   m_vecFileName;
    std::string                 m_PathName;
    unsigned int                m_BasicColorKey;
    int                         m_Index;

    bool                        m_bAlpha;
    int                         m_Alpha;

public:
    int GetIndex()  const
    {
        return m_Index;
    }

    std::string GetName()   const
    {
        return m_Name;
    }

    int GetWidth(int Index = 0)
    {
        return (int)m_vecTexture[Index]->BmpInfo.bmWidth;
    }

    int GetHeight(int Index = 0)
    {
        return (int)m_vecTexture[Index]->BmpInfo.bmHeight;
    }

public:
    void SetAlpha(int Alpha)
    {
        m_Alpha = Alpha;
    }

    void SetEnableAlpha(bool Value)
    {
        m_bAlpha = Value;
    }

    void SetIndex(int Index)
    {
        m_Index = BasicMath::Clamp<int>(Index, 0, m_vecTexture.size() - 1);        
    }

    void SetBasicColorKey(unsigned char r, unsigned char g, unsigned char b)
    {
        m_BasicColorKey = RGB(r, g, b);
    }

    TextureInfo* GetTextureInfo(int idx = 0) const
    {
        return m_vecTexture.size() > idx ? m_vecTexture[idx] : nullptr;
    }

    bool LoadTexture(const std::string& Name, const TCHAR* pFileName,
        const std::string& PathName = TEXTURE_PATH);
    bool LoadTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);

    void SetColorKey(unsigned char r, unsigned char g, unsigned char b, int Index = 0);
    void SetColorKey(unsigned int ColorKey, int Index = 0);
    void Render(HDC hDC, const Vector2& WindowPos, const Vector2& ImagePos,
        const Vector2& Size, int Index = 0);

public:
    void Save(FILE* pFile);
    static void Load(std::string& resultName, FILE* pFile, class CScene* pScene);
};

