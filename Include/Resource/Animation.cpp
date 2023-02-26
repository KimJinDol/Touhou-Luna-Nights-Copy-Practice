
#include "Animation.h"
#include "../Scene/Scene.h"
#include "../Object/GameObject.h"
#include "../Scene/SceneResource.h"

CAnimation::CAnimation()    :
    m_pCurrentAnim(nullptr)
{
}

CAnimation::CAnimation(const CAnimation& Anim)
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

        auto    iter1 = iter->second->vecFrameData.begin();
		auto    iterEnd1 = iter->second->vecFrameData.end();

        for (; iter1 != iterEnd1; ++iter1)
        {
            pAnim->vecFrameData.push_back(*iter1);
        }
               
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

CAnimation::~CAnimation()
{
	auto    iter = m_mapAnimation.begin();
	auto    iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	m_mapAnimation.clear();
}

bool CAnimation::AddAnimation(const std::string& Name, AnimationType Type,
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

bool CAnimation::AddAnimation(const std::string& Name, AnimationType Type,
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

bool CAnimation::AddAnimation(const std::string& Name, AnimationType Type,
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

bool CAnimation::AddAnimation(const std::string& Name, AnimationType Type,
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

bool CAnimation::AddAnimation(const std::string& Name, AnimationType Type, bool Loop, 
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

bool CAnimation::AddAnimationFrameData(const std::string& Name,
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

bool CAnimation::AddAnimationFrameData(const std::string& Name, 
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

bool CAnimation::AddAnimationFrameData(const std::string& Name, unsigned int x, unsigned int y, unsigned int Count, const Vector2& FramePixelSize, const Vector2& Pivot)
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


void CAnimation::SetPlayScale(const std::string& Name, float Scale)
{
    AnimationInfo* pAnim = FindAnimation(Name);

    if (!pAnim)
        return;

    pAnim->PlayScale = Scale;
}

void CAnimation::SetCurrentAnimation(const std::string& Name)
{
    m_pCurrentAnim = FindAnimation(Name);
}

void CAnimation::ChangeAnimation(const std::string& Name)
{
    if (m_pCurrentAnim->Name == Name)
        return;

    m_pCurrentAnim->Frame = 0;
    m_pCurrentAnim->Time = 0.f;

    m_pCurrentAnim = FindAnimation(Name);
}

void CAnimation::SetAnimationReverse(const std::string& Name, bool bReverse)
{
    AnimationInfo* pInfo = FindAnimation(Name);

    if (!pInfo)
        return;

    pInfo->bReverse = bReverse;
    pInfo->Frame = pInfo->FrameCount - 1;
}

void CAnimation::Update(float DeltaTime)
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
                    m_pCurrentAnim->Frame = 0;
            }

            else
            {
                if (m_pCurrentAnim->bReverse)
                    m_pCurrentAnim->Frame = 0;

                else
                    m_pCurrentAnim->Frame = m_pCurrentAnim->FrameCount - 1;

                if (m_pCurrentAnim->NextAnimation)
                {
                    ChangeAnimation(m_pCurrentAnim->NextAnimation->Name);
                }
            }

            if (m_pCurrentAnim->EndFunction)
                m_pCurrentAnim->EndFunction();
        }
    }
}

bool CAnimation::CheckAnimation(const std::string& Name)
{
    return FindAnimation(Name) == nullptr ? false : true;
}

void CAnimation::SetNextAnimation(const std::string& SrcName, const std::string& DestName)
{
    AnimationInfo* pSrcInfo = FindAnimation(SrcName);
    AnimationInfo* pDestInfo = FindAnimation(DestName);

    if (!pSrcInfo || !pDestInfo)
        return;

    pSrcInfo->NextAnimation = pDestInfo;
}

AnimationInfo* CAnimation::FindAnimation(const std::string& Name)
{
    auto    iter = m_mapAnimation.find(Name);

    if (iter == m_mapAnimation.end())
        return nullptr;

    return iter->second;
}

CAnimation* CAnimation::Clone()
{
    return new CAnimation(*this);
}

void CAnimation::Save(FILE* pFile)
{
    int Count = (int)m_mapAnimation.size();

    fwrite(&Count, sizeof(int), 1, pFile);

    auto    iter = m_mapAnimation.begin();
    auto    iterEnd = m_mapAnimation.end();

    for (; iter != iterEnd; ++iter)
    {
        int Length = (int)(*iter).second->Name.length();
        fwrite(&Length, sizeof(int), 1, pFile);
        fwrite((*iter).second->Name.c_str(), sizeof(char), Length, pFile);
        fwrite(&((*iter).second->AnimType), sizeof(AnimationType), 1, pFile);
        (*iter).second->pTexture->Save(pFile);
        int vecCount = (*iter).second->vecFrameData.size();
        fwrite(&vecCount, sizeof(int), 1, pFile);
        for (int i = 0; i < vecCount; i++)
        {
            fwrite(&(*iter).second->vecFrameData[i].ImageStartPos, sizeof(Vector2), 1, pFile);
            fwrite(&(*iter).second->vecFrameData[i].Size, sizeof(Vector2), 1, pFile);
            fwrite(&(*iter).second->vecFrameData[i].Pivot, sizeof(Vector2), 1, pFile);
        }
        fwrite(&((*iter).second->Frame), sizeof(int), 1, pFile);
        fwrite(&((*iter).second->FrameCount), sizeof(int), 1, pFile);
        fwrite(&((*iter).second->FrameTime), sizeof(float), 1, pFile);
        fwrite(&((*iter).second->PlayTime), sizeof(float), 1, pFile);
        fwrite(&((*iter).second->PlayScale), sizeof(float), 1, pFile);
        fwrite(&((*iter).second->bLoop), sizeof(bool), 1, pFile);
        fwrite(&((*iter).second->bReverse), sizeof(bool), 1, pFile);
        
        fwrite(&Length, sizeof(int), 1, pFile);
        fwrite((*iter).second->NextAnimation->Name.c_str(), sizeof(char), Length, pFile);

        /*
    std::string     Name;
    AnimationType   AnimType;       // 애니메이션 타입.
    CSharedPtr<CTexture>  pTexture; // 이 애니메이션이 사용하는 이미지
    std::vector<AnimationFrameData> vecFrameData;
    int             Frame;      // 현재 애니메이션이 어디쯤 동작되는지를 저장하는 프레임값.
    int             FrameCount; // 이 애니메이션이 몇프레임인지를 저장.
    float           Time;       // 시간을 체크하기 위한 변수
    float           FrameTime;  // 1프레임이 진행되기 위해 필요한 시간.
    float           PlayTime;   // 이 애니메이션이 한번 동작되기 위해 필요한 시간
    float           PlayScale;  // 애니메이션 재생배율
    bool            bLoop;
    bool            bReverse;
    AnimationInfo* NextAnimation; // 이 애니메이션이 끝나면 이어질 애니메이션
    std::function<void()>  EndFunction;
    bool            bEndFunction;   // EndFunction을 실행했는지
    std::vector<AnimationNotify*>   vecNotify;
        */
    }
}

void CAnimation::Load(FILE* pFile)
{
    int Count;

    fread(&Count, sizeof(int), 1, pFile);


    for (int i = 0; i < Count; ++i)
    {
        AnimationInfo* pInfo = new AnimationInfo;

        int Length;
        char name[MAX_PATH] = {};
        fread(&Length, sizeof(int), 1, pFile);
        fread(name, sizeof(char), Length, pFile);
        pInfo->Name = name;

        fread(&pInfo->AnimType, sizeof(AnimationType), 1, pFile);
        std::string TexName;
        CTexture::Load(TexName, pFile, m_pScene);
        pInfo->pTexture = m_pScene->GetResourceManager()->FindTexture(TexName);

        int vecCount;
        fread(&vecCount, sizeof(int), 1, pFile);
        for (int i = 0; i < vecCount; i++)
        {
            fread(&pInfo->vecFrameData[i].ImageStartPos, sizeof(Vector2), 1, pFile);
            fread(&pInfo->vecFrameData[i].Size, sizeof(Vector2), 1, pFile);
            fread(&pInfo->vecFrameData[i].Pivot, sizeof(Vector2), 1, pFile);
        }
        fread(&pInfo->Frame, sizeof(int), 1, pFile);
        fread(&pInfo->FrameCount, sizeof(int), 1, pFile);
        fread(&pInfo->FrameTime, sizeof(float), 1, pFile);
        fread(&pInfo->PlayTime, sizeof(float), 1, pFile);
        fread(&pInfo->PlayScale, sizeof(float), 1, pFile);
        fread(&pInfo->bLoop, sizeof(bool), 1, pFile);
        fread(&pInfo->bReverse, sizeof(bool), 1, pFile);

        fread(&Length, sizeof(int), 1, pFile);
        fread(name, sizeof(char), Length, pFile);
        pInfo->NextAnimation = FindAnimation(name);
    }

    if (Count)
    {
        m_pCurrentAnim = m_mapAnimation[0];
    }
}
