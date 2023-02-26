#include "Map.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../GameManager.h"

CMap::CMap() :
    m_Alpha(0),
    m_AlphaTime(3.f),
    m_curAlphaTime(3.f),
    m_Reverse(false)
{
}

CMap::CMap(const CMap& obj) :
    CStaticObject(obj)
{
}

CMap::~CMap()
{
}

bool CMap::Init()
{

    return true;
}


void CMap::Update(float DeltaTime)
{
    if (!m_Reverse)
        m_curAlphaTime = BasicMath::Clamp<float>(m_curAlphaTime - DeltaTime, 0.f, m_AlphaTime);
    else
        m_curAlphaTime = BasicMath::Clamp<float>(m_curAlphaTime + DeltaTime, 0.f, m_AlphaTime);

    m_Alpha = m_curAlphaTime / m_AlphaTime * 255;
}


void CMap::Render(HDC hDC, float DeltaTime)
{
    if (!m_Texture) return;

    BLENDFUNCTION bf;
    bf.BlendOp = 0;
    bf.BlendFlags = 0;
    bf.SourceConstantAlpha = m_Alpha;
    bf.AlphaFormat = AC_SRC_OVER;
    
    GdiAlphaBlend(hDC, 0, 0, GET_RESOLUTION.Width, GET_RESOLUTION.Height, m_Texture->GetTextureInfo()->hDC, 0, 0, m_vSize.x, m_vSize.y, bf);
}

CMap* CMap::Clone() const
{
    return new CMap(*this);
}
