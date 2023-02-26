
#include "AnimationUI.h"
#include "../Scene/Scene.h"
#include "../Object/GameObject.h"
#include "../Scene/SceneResource.h"

CAnimationUI::CAnimationUI() :
    m_pCurrentAnim(nullptr)
{
}

CAnimationUI::CAnimationUI(const CAnimationUI& Anim)
{
    m_mapAnimation.clear();

    auto    iter = Anim.m_mapAnimation.begin();
    auto    iterEnd = Anim.m_mapAnimation.end();

    for (; iter != iterEnd; ++iter)
    {
        AnimationInfo* pAnim = new AnimationInfo;

        pAnim->Name = iter->second->Name;
        pAnim->AnimType = iter->second->AnimType;
        pAnim->pTexture = iter->second->pTexture;
        pAnim->vecFrameData = iter->second->vecFrameData;
        pAnim->Frame = iter->second->Frame;
        pAnim->FrameCount = iter->second->FrameCount;
        pAnim->Time = iter->second->Time;
        pAnim->FrameTime = iter->second->FrameTime;
        pAnim->PlayTime = iter->second->PlayTime;
		pAnim->PlayScale = iter->second->PlayScale;
		pAnim->bLoop = iter->second->bLoop;
		pAnim->bReverse = iter->second->bReverse;
		pAnim->NextAnimation = iter->second->NextAnimation;
		pAnim->EndFunction = iter->second->EndFunction;
        pAnim->vecNotify = iter->second->vecNotify;

        m_mapAnimation.insert(std::make_pair(iter->first, pAnim));

        if (Anim.m_pCurrentAnim)
        {
            if (iter->first == Anim.m_pCurrentAnim->Name)
                m_pCurrentAnim = pAnim;
        }
    }
}

CAnimationUI::~CAnimationUI()
{
    auto    iter = m_mapAnimation.begin();
    auto    iterEnd = m_mapAnimation.end();

    for (; iter != iterEnd; ++iter)
    {
        SAFE_DELETE(iter->second);
    }

    m_mapAnimation.clear();
}

bool CAnimationUI::AddAnimation(const std::string& Name, AnimationType Type,
    bool Loop, CTexture* pTexture,
    const std::vector<AnimationFrameData>& vecFrame, float PlayTime)
{
    AnimationInfo* pAnim = FindAnimation(Name);

    if (pAnim)
        return false;

    pAnim = new AnimationInfo;

    pAnim->Name = Name;
    pAnim->AnimType = Type;
    pAnim->pTexture = pTexture;
    pAnim->bLoop = Loop;
    pAnim->PlayTime = PlayTime;
    pAnim->vecFrameData = vecFrame;
    pAnim->FrameCount = (int)vecFrame.size();
    pAnim->FrameTime = pAnim->PlayTime / pAnim->FrameCount;

    m_mapAnimation.insert(std::make_pair(Name, pAnim));

    if (!m_pCurrentAnim)
        m_pCurrentAnim = pAnim;

    return true;
}

bool CAnimationUI::AddAnimation(const std::string& Name, AnimationType Type,
    bool Loop, CTexture* pTexture, float PlayTime)
{
    AnimationInfo* pAnim = FindAnimation(Name);

    if (pAnim)
        return false;

    pAnim = new AnimationInfo;

    pAnim->Name = Name;
    pAnim->AnimType = Type;
    pAnim->pTexture = pTexture;
    pAnim->bLoop = Loop;
    pAnim->PlayTime = PlayTime;

    m_mapAnimation.insert(std::make_pair(Name, pAnim));

    if (!m_pCurrentAnim)
        m_pCurrentAnim = pAnim;

    return true;
}

bool CAnimationUI::AddAnimation(const std::string& Name, AnimationType Type,
    bool Loop, const std::string& TextureName, float PlayTime)
{
    AnimationInfo* pAnim = FindAnimation(Name);

    if (pAnim)
        return false;

    pAnim = new AnimationInfo;

    pAnim->Name = Name;
    pAnim->AnimType = Type;
    pAnim->pTexture = m_pScene->GetResourceManager()->FindTexture(TextureName);
    pAnim->bLoop = Loop;
    pAnim->PlayTime = PlayTime;

    m_mapAnimation.insert(std::make_pair(Name, pAnim));

    if (!m_pCurrentAnim)
        m_pCurrentAnim = pAnim;

    return true;
}

bool CAnimationUI::AddAnimation(const std::string& Name, AnimationType Type,
    bool Loop, const std::string& TextureName,
    const TCHAR* pTexFileName, float PlayTime)
{
    AnimationInfo* pAnim = FindAnimation(Name);

    if (pAnim)
        return false;

    else if (!m_pScene->GetResourceManager()->LoadTexture(TextureName,
        pTexFileName))
        return false;

    pAnim = new AnimationInfo;

    pAnim->Name = Name;
    pAnim->AnimType = Type;
    pAnim->pTexture = m_pScene->GetResourceManager()->FindTexture(TextureName);
    pAnim->bLoop = Loop;
    pAnim->PlayTime = PlayTime;

    m_mapAnimation.insert(std::make_pair(Name, pAnim));

    if (!m_pCurrentAnim)
        m_pCurrentAnim = pAnim;

    return true;
}

bool CAnimationUI::AddAnimation(const std::string& Name, AnimationType Type, bool Loop, 
    const std::string& TextureName, const std::vector<std::wstring>& vecFileName, float PlayTime)
{
    AnimationInfo* pAnim = FindAnimation(Name);

    if (pAnim)
        return false;

    else if (!m_pScene->GetResourceManager()->LoadTexture(TextureName,
        vecFileName))
        return false;

    pAnim = new AnimationInfo;

    pAnim->Name = Name;
    pAnim->AnimType = Type;
    pAnim->pTexture = m_pScene->GetResourceManager()->FindTexture(TextureName);
    pAnim->bLoop = Loop;
    pAnim->PlayTime = PlayTime;

    m_mapAnimation.insert(std::make_pair(Name, pAnim));

    if (!m_pCurrentAnim)
        m_pCurrentAnim = pAnim;

    return true;
}

bool CAnimationUI::AddAnimationFrameData(const std::string& Name,
    const Vector2& ImageStartPos, const Vector2& Size)
{
    AnimationInfo* pAnim = FindAnimation(Name);

    if (!pAnim)
        return false;

    AnimationFrameData  Data = {};
    Data.ImageStartPos = ImageStartPos;
    Data.Size = Size;

    pAnim->vecFrameData.push_back(Data);

    pAnim->FrameCount = (int)pAnim->vecFrameData.size();
    pAnim->FrameTime = pAnim->PlayTime / pAnim->FrameCount;

    if (pAnim->bReverse)
        pAnim->Frame = pAnim->FrameCount - 1;

    return true;
}

bool CAnimationUI::AddAnimationFrameData(const std::string& Name,
    const Vector2& ImageStartPos, const Vector2& Size, int Count)
{
    AnimationInfo* pAnim = FindAnimation(Name);

    if (!pAnim)
        return false;

    AnimationFrameData  Data = {};
    Data.ImageStartPos = ImageStartPos;
    Data.Size = Size;

    for (int i = 0; i < Count; ++i)
    {
        pAnim->vecFrameData.push_back(Data);
    }

    pAnim->FrameCount = Count;
    pAnim->FrameTime = pAnim->PlayTime / Count;

    if (pAnim->bReverse)
        pAnim->Frame = pAnim->FrameCount - 1;

    return true;
}

bool CAnimationUI::AddAnimationFrameData(const std::string& Name, unsigned int x, unsigned int y, unsigned int Count, const Vector2& FramePixelSize, const Vector2& Pivot)
{
	AnimationInfo* pAnim = FindAnimation(Name);

	if (!pAnim)
		return false;

	AnimationFrameData  Data = {};
	Data.Size = FramePixelSize;
	Data.Pivot = Pivot;

	Vector2 ImageStartPos = Vector2(FramePixelSize.x * x, FramePixelSize.y * y);

	for (int i = 0; i < Count; ++i)
	{
		Data.ImageStartPos = ImageStartPos;
		pAnim->vecFrameData.push_back(Data);

		// 아틀라스의 왼쪽 끝에 도달했을 경우 Start Pos를 한 칸 아래 왼쪽으로 변경.
		if (ImageStartPos.x + FramePixelSize.x >= pAnim->pTexture->GetTextureInfo()->BmpInfo.bmWidth)
		{
			ImageStartPos.y += FramePixelSize.y;
			ImageStartPos.x = 0;
		}
		else
			ImageStartPos.x += FramePixelSize.x;
	}

	pAnim->FrameCount += Count;
	pAnim->FrameTime = pAnim->PlayTime / pAnim->FrameCount;

	if (pAnim->bReverse)
		pAnim->Frame = pAnim->FrameCount - 1;

	return true;
}

void CAnimationUI::SetPlayScale(const std::string& Name, float Scale)
{
    AnimationInfo* pAnim = FindAnimation(Name);

    if (!pAnim)
        return;

    pAnim->PlayScale = Scale;
}

void CAnimationUI::SetCurrentAnimation(const std::string& Name)
{
    m_pCurrentAnim = FindAnimation(Name);
}

void CAnimationUI::ChangeAnimation(const std::string& Name)
{
    if (m_pCurrentAnim->Name == Name)
        return;

    m_pCurrentAnim->Frame = 0;
    m_pCurrentAnim->Time = 0.f;
    m_pCurrentAnim->bEndFunction = false;

    m_pCurrentAnim = FindAnimation(Name);
}

void CAnimationUI::SetAnimationReverse(const std::string& Name, bool bReverse)
{
    AnimationInfo* pInfo = FindAnimation(Name);

    if (!pInfo)
        return;

    pInfo->bReverse = bReverse;
    pInfo->Frame = pInfo->FrameCount - 1;
}

void CAnimationUI::Update(float DeltaTime)
{
    m_pCurrentAnim->Time += DeltaTime * m_pCurrentAnim->PlayScale;

    if (m_pCurrentAnim->Time >= m_pCurrentAnim->FrameTime)
    {
        m_pCurrentAnim->Time -= m_pCurrentAnim->FrameTime;

        bool    bAnimEnd = false;

        if (m_pCurrentAnim->bReverse)
        {
            --m_pCurrentAnim->Frame;

            if (m_pCurrentAnim->Frame < 0)
                bAnimEnd = true;
        }

        else
        {
            ++m_pCurrentAnim->Frame;

            if (m_pCurrentAnim->Frame == m_pCurrentAnim->FrameCount)
                bAnimEnd = true;
        }

        size_t  Size = m_pCurrentAnim->vecNotify.size();

        for (size_t i = 0; i < Size; ++i)
        {
            if (m_pCurrentAnim->vecNotify[i]->Frame == m_pCurrentAnim->Frame)
            {
                m_pCurrentAnim->vecNotify[i]->Function();
            }
        }

        if (bAnimEnd)
        {
            // 반복해서 동작되어야 하는 애니메이션 일 경우 프레임을
            // 첫번째 프레임으로 옮겨준다.
            if (m_pCurrentAnim->bLoop)
            {
                if (m_pCurrentAnim->bReverse)
                    m_pCurrentAnim->Frame = m_pCurrentAnim->Frame - 1;

                else
                {
                    m_pCurrentAnim->Frame = 0;
                    m_pCurrentAnim->bEndFunction = false;
                }
            }

            else
            {
                if (m_pCurrentAnim->bReverse)
                    m_pCurrentAnim->Frame = 0;

                else
                    m_pCurrentAnim->Frame = m_pCurrentAnim->FrameCount - 1;

                if (m_pCurrentAnim->NextAnimation)
                    ChangeAnimation(m_pCurrentAnim->NextAnimation->Name);
            }

            if (m_pCurrentAnim->EndFunction && !m_pCurrentAnim->bEndFunction)
            {
                m_pCurrentAnim->EndFunction();
                m_pCurrentAnim->bEndFunction = true;
            }
        }
    }
}

bool CAnimationUI::CheckAnimation(const std::string& Name)
{
    return FindAnimation(Name) == nullptr ? false : true;
}

AnimationInfo* CAnimationUI::FindAnimation(const std::string& Name)
{
    auto    iter = m_mapAnimation.find(Name);

    if (iter == m_mapAnimation.end())
        return nullptr;

    return iter->second;
}

CAnimationUI* CAnimationUI::Clone()
{
    return new CAnimationUI(*this);
}
