#pragma once

#include "Texture.h"

class CResourceManager
{
private:
    FMOD::System* m_pSystem;
    FMOD::ChannelGroup* m_pMasterGroup;
    std::unordered_map<std::string, FMOD::ChannelGroup*>    m_mapChannelGroup;
    std::unordered_map<std::string, class CSound*>    m_mapSound;
    CSharedPtr<CSound>  m_curBGM;

private:
    std::unordered_map<std::string, CTexture*> m_mapTexture;

public:
    bool Init();
    void Update();
    void Destroy();

public:
	bool AddTexture(const std::string& Name, const TCHAR* pFileName,
		const std::string& PathName = TEXTURE_PATH);
    bool LoadTexture(const std::string& Name, const TCHAR* pFileName,
        const std::string& PathName = TEXTURE_PATH);
    bool LoadTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);

    void SetColorKey(const std::string& Name, 
        unsigned char r, unsigned char g, unsigned char b, int Index = 0);
    void SetColorKey(const std::string& Name, unsigned int ColorKey, int Index = 0);

    void ReleaseTexture(const std::string& Name);

    CTexture* FindTexture(const std::string& Name);

    void EraseTexture(const std::string& Name)
    {
        auto    iter = m_mapTexture.find(Name);

        if (iter == m_mapTexture.end())
            return;

        m_mapTexture.erase(iter);
    }

public:
    bool LoadSound(const std::string& GroupName, bool Loop, const std::string& Name,
        const char* pFileName, const std::string& PathName = SOUND_PATH);
    void SetVolume(int Volume);
    bool SetVolume(const std::string& GroupName, int Volume);
    bool SoundPlay(const std::string& Name);
    bool SoundStop(const std::string& Name);
    bool SoundPause(const std::string& Name);
    bool SoundResume(const std::string& Name);

    void ReleaseSound(const std::string& Name);
    void EraseSound(const std::string& Name);
    class CSound* FindSound(const std::string& Name);

    bool CreateChannelGroup(const std::string& Name);
    FMOD::ChannelGroup* FindChannelGroup(const std::string& Name);


    DECLARE_SINGLE(CResourceManager)
};

