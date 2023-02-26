#include "Effect.h"
#include "../Resource/Animation.h"
#include "Character.h"
#include "../Timer.h"
#include "../GameManager.h"

CEffect::CEffect()  :
    m_bUseLocalSpace(false),
    m_bOwnerIsCharacter(false),
    m_bIsLoop(false),
    m_vOffset(0.f, 0.f)
{
}

CEffect::CEffect(const CEffect& obj) :
    CGameObject(obj)
{
    m_bUseLocalSpace = obj.m_bUseLocalSpace;
    m_vOffset = obj.m_vOffset;
}

CEffect::~CEffect()
{
}

void CEffect::SetOwner(CGameObject* pOwner)
{
    m_pOwner = pOwner;

    if (CCharacter* pCharacter = dynamic_cast<CCharacter*>(pOwner))
    {
        m_bOwnerIsCharacter = true;
        m_pOwnerCharacter = pCharacter;
    }
}

bool CEffect::AddAnimation(const std::string& Name, AnimationType Type, CTexture* pTexture, const std::vector<AnimationFrameData>& vecFrame, float PlayTime)
{
    if (!m_pAnimation)
    {
        m_pAnimation = new CAnimation;

        m_pAnimation->m_pScene = m_pScene;
        m_pAnimation->m_pOwner = this;
    }

    return CGameObject::AddAnimation(Name, Type, m_bIsLoop, pTexture,
        vecFrame, PlayTime);
}

bool CEffect::AddAnimation(const std::string& Name, AnimationType Type, CTexture* pTexture, float PlayTime)
{
    if (!m_pAnimation)
    {
        m_pAnimation = new CAnimation;

        m_pAnimation->m_pScene = m_pScene;
        m_pAnimation->m_pOwner = this;
    }

    return CGameObject::AddAnimation(Name, Type, m_bIsLoop, pTexture, PlayTime);
}

bool CEffect::AddAnimation(const std::string& Name, AnimationType Type, const std::string& TextureName, float PlayTime)
{
    if (!m_pAnimation)
    {
        m_pAnimation = new CAnimation;

        m_pAnimation->m_pScene = m_pScene;
        m_pAnimation->m_pOwner = this;
    }

    return  CGameObject::AddAnimation(Name, Type, m_bIsLoop, TextureName, PlayTime);
}

bool CEffect::AddAnimation(const std::string& Name, AnimationType Type, const std::string& TextureName, const TCHAR* pTexFileName, float PlayTime)
{
    if (!m_pAnimation)
    {
        m_pAnimation = new CAnimation;

        m_pAnimation->m_pScene = m_pScene;
        m_pAnimation->m_pOwner = this;
    }

    return CGameObject::AddAnimation(Name, Type, m_bIsLoop, TextureName, pTexFileName, PlayTime);
}

bool CEffect::Init()
{
    SetPivot(0.5f, 0.5f);

    return true;
}

void CEffect::Start()
{
    CGameObject::Start();

    m_vPos += m_vOffset;
}

void CEffect::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);

    if (m_bUseLocalSpace && m_pOwner)
    {
        m_vPos = m_pOwner->GetPos() + m_vOffset;

        if (m_bOwnerIsCharacter)
        {
            if(m_pOwnerCharacter->GetView() == CharacterView::Left)
                m_vPos = m_pOwner->GetPos() + m_vOffset;
        }        
    }
}

void CEffect::Render(HDC hDC, float DeltaTime)
{
    CGameObject::Render(hDC, DeltaTime);
}

CEffect* CEffect::Clone() const
{
    return new CEffect(*this);
}

void CEffect::SetAutoDestroy()
{
    SetAnimationEndFunction<CEffect>(m_pAnimation->GetCurrentAnimation(), this, &CEffect::EffectEnd);
}

void CEffect::EffectEnd()
{
    CRef::Destroy();
}
