#pragma once

#include "../Resource/Texture.h"
#include "../Resource/Sound.h"

class CSceneResource
{
    friend class CScene;

private:
	CSceneResource();
	~CSceneResource();

private:
    std::unordered_map<std::string, CTexture*> m_mapTexture;
    std::unordered_map<std::string, CSound*> m_mapSound;

public:
    bool LoadTexture(const std::string& Name, const TCHAR* pFileName,
        const std::string& PathName = TEXTURE_PATH);
    bool LoadTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);

    void SetColorKey(const std::string& Name,
        unsigned char r, unsigned char g, unsigned char b, int Index = 0);
    void SetColorKey(const std::string& Name, unsigned int ColorKey, int Index = 0);

    CTexture* FindTexture(const std::string& Name);


public:
    bool LoadSound(const std::string& GroupName, bool Loop, const std::string& Name,
        const char* pFileName, const std::string& PathName = SOUND_PATH);
    void SetVolume(int Volume);
    bool SetVolume(const std::string& GroupName, int Volume);
    bool SoundPlay(const std::string& Name);
    bool SoundStop(const std::string& Name);
    bool SoundPause(const std::string& Name);
    bool SoundResume(const std::string& Name);

    class CSound* FindSound(const std::string& Name);

    bool CreateChannelGroup(const std::string& Name);
    FMOD::ChannelGroup* FindChannelGroup(const std::string& Name);
};

