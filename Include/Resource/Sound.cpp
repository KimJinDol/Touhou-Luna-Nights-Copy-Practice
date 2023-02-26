
#include "Sound.h"
#include "../PathManager.h"
#include "ResourceManager.h"

CSound::CSound()    :
    m_Loop(false),
    m_pSystem(nullptr),
    m_pSound(nullptr),
    m_pChannel(nullptr),
    m_pGroup(nullptr),
    m_Name{},
    m_bSingle(false)
{
}

CSound::~CSound()
{
    if (m_pSound)
        m_pSound->release();

    CResourceManager::GetInst()->EraseSound(m_Name);
}

bool CSound::LoadSound(FMOD::System* pSystem, FMOD::ChannelGroup* pGroup, 
    bool Loop, const std::string& Name, const char* pFileName,
    const std::string& PathName)
{
    m_Name = Name;
    m_pSystem = pSystem;
    m_pGroup = pGroup;
    m_Loop = Loop;

    // 파일을 읽어오기 위한 전체 경로를 구한다.
    char    FullPath[MAX_PATH] = {};

    const char* pPath = CPathManager::GetInst()->FindPath(PathName)->PathMultibyte;

    if (pPath)
        strcpy_s(FullPath, pPath);

    strcat_s(FullPath, pFileName);

    FMOD_MODE   Mode = FMOD_DEFAULT;

    if (m_Loop)
        Mode = FMOD_LOOP_NORMAL;

    m_pSystem->createSound(FullPath, Mode, nullptr, &m_pSound);

    return true;
}

void CSound::Play()
{
    if (m_pChannel)
    {
        bool    bPlaying = false;
        m_pChannel->isPlaying(&bPlaying);

        if (bPlaying && m_bSingle)
            return;
    }
    m_pSystem->playSound(m_pSound, m_pGroup, false, &m_pChannel);
}

void CSound::Stop()
{
    if (m_pChannel)
    {
        bool    bPlaying = false;
        m_pChannel->isPlaying(&bPlaying);

        if (bPlaying)
        {
            m_pChannel->stop();
            m_pChannel = nullptr;
        }
    }
}

void CSound::Pause()
{
    if (!m_pChannel)
        return;

    bool    bPlaying = false;
    m_pChannel->isPlaying(&bPlaying);

    if (bPlaying)
        m_pChannel->setPaused(true);
}

void CSound::Resume()
{
    if (!m_pChannel)
        return;

    bool    bPlaying = false;
    m_pChannel->isPlaying(&bPlaying);

    if (!bPlaying)
        m_pChannel->setPaused(false);
}
