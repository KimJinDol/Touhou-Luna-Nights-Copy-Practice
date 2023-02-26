#pragma once

#include "../Ref.h"

class CSound :
    public CRef
{
    friend class CResourceManager;

private:
    CSound();
    ~CSound();

private:
    std::string     m_Name;
    FMOD::System* m_pSystem;
    FMOD::Sound* m_pSound;
    FMOD::ChannelGroup* m_pGroup;
    FMOD::Channel* m_pChannel;
    bool        m_Loop;
    bool        m_bSingle;
    std::string     m_GroupName;

public:
    bool IsLoop()   const
    {
        return m_Loop;
    }

    bool IsSingle() const
    {
        return m_bSingle;
    }

    void SetSingle(bool Value)
    {
        m_bSingle = Value;
    }

public:
    bool LoadSound(FMOD::System* pSystem, FMOD::ChannelGroup* pGroup,
        bool Loop, const std::string& Name, const char* pFileName,
        const std::string& PathName = SOUND_PATH);
    void Play();
    void Stop();
    void Pause();
    void Resume();
};

