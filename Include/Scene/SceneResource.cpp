
#include "SceneResource.h"
#include "../Resource/ResourceManager.h"

CSceneResource::CSceneResource()
{
}

CSceneResource::~CSceneResource()
{
    {
        auto    iter = m_mapTexture.begin();
        auto    iterEnd = m_mapTexture.end();

        for (; iter != iterEnd; ++iter)
        {
            CResourceManager::GetInst()->ReleaseTexture(iter->first);
            // SAFE_RELEASE(iter->second);
        }
    }
    m_mapTexture.clear();

    {
        auto    iter = m_mapSound.begin();
        auto    iterEnd = m_mapSound.end();

        for (; iter != iterEnd; ++iter)
        {
            CResourceManager::GetInst()->ReleaseSound(iter->first);
            // SAFE_RELEASE(iter->second);
        }
    }
    m_mapSound.clear();
}

bool CSceneResource::LoadTexture(const std::string& Name, const TCHAR* pFileName, 
    const std::string& PathName)
{
    if (FindTexture(Name))
        return true;

    if (!CResourceManager::GetInst()->LoadTexture(Name,
        pFileName, PathName))
        return false;

	CTexture* pTexture = CResourceManager::GetInst()->FindTexture(Name);

	pTexture->AddRef();

    m_mapTexture.insert(std::make_pair(Name, CResourceManager::GetInst()->FindTexture(Name)));
   
    return true;
}

bool CSceneResource::LoadTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName,
    const std::string& PathName)
{
    if (FindTexture(Name))
        return true;

    if (!CResourceManager::GetInst()->LoadTexture(Name,
        vecFileName, PathName))
        return false;

	CTexture* pTexture = CResourceManager::GetInst()->FindTexture(Name);

	pTexture->AddRef();

    m_mapTexture.insert(std::make_pair(Name, CResourceManager::GetInst()->FindTexture(Name)));
    
    return true;
}

void CSceneResource::SetColorKey(const std::string& Name, unsigned char r, unsigned char g, unsigned char b, int Index)
{
    CResourceManager::GetInst()->SetColorKey(Name, r, g, b, Index);
}

void CSceneResource::SetColorKey(const std::string& Name, unsigned int ColorKey, int Index)
{
    CResourceManager::GetInst()->SetColorKey(Name, ColorKey, Index);
}

CTexture* CSceneResource::FindTexture(const std::string& Name)
{
    CTexture* pTexture = CResourceManager::GetInst()->FindTexture(Name);

    if (pTexture == nullptr)
    {
        return nullptr;
    }
    return pTexture;
}

bool CSceneResource::LoadSound(const std::string& GroupName, bool Loop, 
    const std::string& Name, const char* pFileName, const std::string& PathName)
{
    if (FindSound(Name))
        return true;

    if (!CResourceManager::GetInst()->LoadSound(GroupName, Loop, 
        Name, pFileName, PathName))
        return false;

	CSound* pSound = CResourceManager::GetInst()->FindSound(Name);

	pSound->AddRef();

    m_mapSound.insert(std::make_pair(Name, CResourceManager::GetInst()->FindSound(Name)));

    return true;
}

void CSceneResource::SetVolume(int Volume)
{
    CResourceManager::GetInst()->SetVolume(Volume);
}

bool CSceneResource::SetVolume(const std::string& GroupName, int Volume)
{
    return CResourceManager::GetInst()->SetVolume(GroupName, Volume);
}

bool CSceneResource::SoundPlay(const std::string& Name)
{
    return CResourceManager::GetInst()->SoundPlay(Name);
}

bool CSceneResource::SoundStop(const std::string& Name)
{
    return CResourceManager::GetInst()->SoundStop(Name);
}

bool CSceneResource::SoundPause(const std::string& Name)
{
    return CResourceManager::GetInst()->SoundPause(Name);
}

bool CSceneResource::SoundResume(const std::string& Name)
{
    return CResourceManager::GetInst()->SoundResume(Name);
}

CSound* CSceneResource::FindSound(const std::string& Name)
{
    CSound* pSound = CResourceManager::GetInst()->FindSound(Name);

    if (pSound == nullptr)
        return nullptr;

    return pSound;
}

bool CSceneResource::CreateChannelGroup(const std::string& Name)
{
    return CResourceManager::GetInst()->CreateChannelGroup(Name);
}

FMOD::ChannelGroup* CSceneResource::FindChannelGroup(const std::string& Name)
{
    return CResourceManager::GetInst()->FindChannelGroup(Name);
}
