
#include "ResourceManager.h"
#include "Sound.h"

DEFINITION_SINGLE(CResourceManager)

CResourceManager::CResourceManager() :
    m_pSystem(nullptr),
    m_pMasterGroup(nullptr)
{
}

CResourceManager::~CResourceManager()
{
    {
        auto    iter = m_mapTexture.begin();
        auto    iterEnd = m_mapTexture.end();

        for (; iter != iterEnd; ++iter)
        {
            SAFE_DELETE(iter->second);
        }
    }

    {
        auto    iter = m_mapSound.begin();
        auto    iterEnd = m_mapSound.end();

        for (; iter != iterEnd; ++iter)
		{
            SAFE_DELETE(iter->second);
        }
    }

    if (m_pSystem)
    {
        m_pSystem->close();
        m_pSystem->release();
    }
}

bool CResourceManager::Init()
{
    FMOD_RESULT result = FMOD::System_Create(&m_pSystem);

    if (result != FMOD_RESULT::FMOD_OK)
        return false;

    // FMOD System을 초기화한다.
    result = m_pSystem->init(128, FMOD_INIT_NORMAL, nullptr);

    if (result != FMOD_RESULT::FMOD_OK)
        return false;

    // Master Channel Group을 얻어온다.
    result = m_pSystem->getMasterChannelGroup(&m_pMasterGroup);

    if (result != FMOD_RESULT::FMOD_OK)
        return false;

    // 채널 그룹을 생성한다.
    CreateChannelGroup("BGM");
    SetVolume("BGM", 50);
#ifdef _DEBUG
    SetVolume("BGM", 60);
#endif
    CreateChannelGroup("Effect");
    SetVolume("Effect", 100);
    CreateChannelGroup("UI");
    SetVolume("UI", 100);

    // ---------- Texture ---------

    // _g
	LoadTexture("Stage00_Tile", TEXT("Tile/Stage00_Tile.bmp"));
	AddTexture("Stage00_Tile", TEXT("Tile/Stage00_Tile_g.bmp"));

	LoadTexture("Stage00_Tile_wet", TEXT("Tile/Stage00_Tile_wet.bmp"));
	AddTexture("Stage00_Tile_wet", TEXT("Tile/Stage00_Tile_wet_g.bmp"));

	LoadTexture("background_sprite", TEXT("BackGround/background_sprite.bmp"));
	AddTexture("background_sprite", TEXT("BackGround/background_sprite_g.bmp"));

	LoadTexture("Stage00_Group00", TEXT("BackGround/Stage00_Group00.bmp"));
	AddTexture("Stage00_Group00", TEXT("BackGround/Stage00_Group00_g.bmp"));

    LoadTexture("obj_Atlass00", TEXT("Object/obj_Atlass00.bmp"));
    AddTexture("obj_Atlass00", TEXT("Object/obj_Atlass00_g.bmp"));

    LoadTexture("UI_message1_mask", TEXT("UI/UI_message1_mask.bmp"));
    LoadTexture("UI_message2_mask", TEXT("UI/UI_message2_mask.bmp"));

	// Load Atlass
	LoadTexture("Player_Atlas00", TEXT("SakuyaIzayoi/Atlass00.bmp"));
	LoadTexture("Player_Atlas00_Left", TEXT("SakuyaIzayoi/Atlass00_Left.bmp"));
	LoadTexture("Player_Atlas01", TEXT("SakuyaIzayoi/Atlass01.bmp"));
	LoadTexture("Player_Atlas01_Left", TEXT("SakuyaIzayoi/Atlass01_Left.bmp"));
	LoadTexture("Effect_Atlass00", TEXT("Effect/Effect_Atlass00.bmp"));
	LoadTexture("Effect_Atlass01", TEXT("Effect/Effect_Atlass01_Left.bmp"));
	LoadTexture("Effect_Atlass01_Left", TEXT("Effect/Effect_Atlass01.bmp"));

	LoadTexture("UI_Atlass00", TEXT("UI/UI_Atlass00.bmp"));
	LoadTexture("UI_PrograssBar_Atlass00", TEXT("UI/UI_PrograssBar_Atlass00.bmp"));
	LoadTexture("UI_Snail_Atlass00", TEXT("UI/UI_Snail_Atlass00.bmp"));

	LoadTexture("Enemy_Atlas00", TEXT("Enemy/Enemy_Atlass00.bmp"));
	LoadTexture("Enemy_Atlas00_Left", TEXT("Enemy/Enemy_Atlass00_Left.bmp"));

    LoadTexture("BlackGround", TEXT("BackGround/BlackGround.bmp"));
    LoadTexture("BlackGroundWidget", TEXT("BackGround/BlackGroundWidget.bmp"));
    LoadTexture("ItemBackGround", TEXT("BackGround/ItemBackGround.bmp"));

    LoadTexture("UI_item_vision_timeStop", TEXT("UI/UI_item_vision_timeStop.bmp"));


    // ---------- Sound -----------
    // Player
    LoadSound("Effect", false, "kengeki00", "Effect/Player/kengeki00.wav");
    LoadSound("Effect", false, "knife_unplug", "Effect/Player/knife_unplug.wav");
    LoadSound("Effect", false, "knife_stop", "Effect/Player/knife_stop.wav");
    FindSound("knife_stop")->SetSingle(true);
    LoadSound("Effect", false, "landing", "Effect/Player/landing.wav");
    LoadSound("Effect", false, "whip", "Effect/Player/whip.wav");
    LoadSound("Effect", false, "down", "Effect/Player/down.wav");
    LoadSound("Effect", false, "secondHand", "Effect/Player/secondHand.wav");
    LoadSound("Effect", false, "TimeSlow", "Effect/Player/TimeSlow.wav");
    LoadSound("Effect", false, "timeStop", "Effect/Player/timeStop.wav");
    LoadSound("Effect", false, "timeStopEnd", "Effect/Player/timeStopEnd.wav");
    LoadSound("Effect", false, "message", "Effect/Player/message.wav");
    LoadSound("Effect", false, "absorber", "Effect/Player/absorber.wav");
    LoadSound("Effect", false, "swing_big", "Effect/Player/swing_big.wav");
    LoadSound("Effect", false, "thunder", "Effect/Effect/thunder.wav");


    // UI
    LoadSound("UI", false, "mp_full", "Effect/UI/mp_full.wav");
    LoadSound("UI", false, "time_full", "Effect/UI/time_full.wav");

    // Object
    LoadSound("Effect", false, "akikan", "Effect/Item/akikan.wav");
    LoadSound("Effect", false, "akikan2", "Effect/Item/akikan2.wav");
    LoadSound("Effect", false, "akikan3", "Effect/Item/akikan3.wav");
    LoadSound("Effect", false, "buy_juse", "Effect/Item/buy_juse.wav");
    LoadSound("Effect", false, "goku", "Effect/Item/goku.wav");
    LoadSound("Effect", false, "item_get", "Effect/Item/item_get.wav");
    LoadSound("Effect", false, "akikan", "Effect/Item/akikan.wav");

    // Enemy
    LoadSound("Effect", false, "cat", "Effect/enemy/cat.wav");
    LoadSound("Effect", false, "bomb", "Effect/enemy/bomb.wav");
    LoadSound("Effect", false, "destroy", "Effect/enemy/destroy.wav");
    LoadSound("Effect", false, "HitEnemy", "Effect/enemy/Hit.wav");

    // BGM
    LoadSound("BGM", true, "bgm00", "BGM/bgm00.ogg");
    FindSound("bgm00")->SetSingle(true);
    LoadSound("BGM", true, "title", "BGM/title.ogg");
    FindSound("title")->SetSingle(true);
    LoadSound("BGM", true, "ivent01", "BGM/ivent01.ogg");
    FindSound("ivent01")->SetSingle(true);

    return true;
}

void CResourceManager::Update()
{
    auto iter = m_mapTexture.begin();
    auto iterEnd = m_mapTexture.end();

    for (; iter != iterEnd;)
    {
        if ((*iter).second->IsDestroy())
        {
            iter = m_mapTexture.erase(iter);
            iterEnd = m_mapTexture.end();
        }
        else
            iter++;
    }

    m_pSystem->update();
}

void CResourceManager::Destroy()
{
    m_mapTexture.clear();

    {
        auto    iter = m_mapSound.begin();
        auto    iterEnd = m_mapSound.end();

        for (; iter != iterEnd; ++iter)
        {
            iter->second->Release();
        }

        m_mapSound.clear();
    }

    {
        auto    iter = m_mapChannelGroup.begin();
        auto    iterEnd = m_mapChannelGroup.end();

        for (; iter != iterEnd; ++iter)
        {
            iter->second->release();
        }
    }
}

bool CResourceManager::AddTexture(const std::string& Name, const TCHAR* pFileName, const std::string& PathName)
{
	CTexture* pTex = FindTexture(Name);

	if (!pTex)
		return false;

	if (!pTex->LoadTexture(Name, pFileName, PathName))
	{
		return false;
	}

	return true;
}

bool CResourceManager::LoadTexture(const std::string& Name, const TCHAR* pFileName,
    const std::string& PathName)
{
    CTexture* pTex = FindTexture(Name);

    if (pTex)
        return true;

    pTex = new CTexture;

    if (!pTex->LoadTexture(Name, pFileName, PathName))
    {
        SAFE_DELETE(pTex);
        return false;
    }

    m_mapTexture.insert(std::make_pair(Name, pTex));

    return true;
}

bool CResourceManager::LoadTexture(const std::string& Name, 
    const std::vector<std::wstring>& vecFileName, const std::string& PathName)
{
    CTexture* pTex = FindTexture(Name);

    if (pTex)
        return true;

    pTex = new CTexture;

    if (!pTex->LoadTexture(Name, vecFileName, PathName))
    {
        SAFE_DELETE(pTex);
        return false;
    }

    m_mapTexture.insert(std::make_pair(Name, pTex));

    return true;
}

void CResourceManager::SetColorKey(const std::string& Name, unsigned char r,
    unsigned char g, unsigned char b, int Index)
{
    CTexture* pTex = FindTexture(Name);

    if (!pTex)
        return;

    pTex->SetColorKey(r, g, b, Index);
}

void CResourceManager::SetColorKey(const std::string& Name, unsigned int ColorKey, int Index)
{
    CTexture* pTex = FindTexture(Name);

    if (!pTex)
        return;

    pTex->SetColorKey(ColorKey, Index);
}

void CResourceManager::ReleaseTexture(const std::string& Name)
{
    auto    iter = m_mapTexture.find(Name);

    if (iter == m_mapTexture.end())
        return;

    iter->second->Release();

    if (iter->second->IsDestroy())
        m_mapTexture.erase(iter);
}

CTexture* CResourceManager::FindTexture(const std::string& Name)
{
    auto    iter = m_mapTexture.find(Name);

    if (iter == m_mapTexture.end())
        return nullptr;

    return iter->second;;
}

bool CResourceManager::LoadSound(const std::string& GroupName, bool Loop, 
    const std::string& Name, const char* pFileName,
    const std::string& PathName)
{
    CSound* pSound = FindSound(Name);

    if (pSound)
        return true;

    FMOD::ChannelGroup* pGroup = FindChannelGroup(GroupName);

    if (!pGroup)
        return true;

    pSound = new CSound;
    pSound->m_GroupName = GroupName;

    if (!pSound->LoadSound(m_pSystem, pGroup, Loop, Name, pFileName, PathName))
    {
        SAFE_DELETE(pSound);
        return false;
    }

    m_mapSound.insert(std::make_pair(Name, pSound));

    return true;
}

void CResourceManager::SetVolume(int Volume)
{
    m_pMasterGroup->setVolume(Volume / 100.f);
}

bool CResourceManager::SetVolume(const std::string& GroupName, int Volume)
{
    FMOD::ChannelGroup* pGroup = FindChannelGroup(GroupName);

    if (!pGroup)
        return false;

    pGroup->setVolume(Volume / 100.f);

    return true;
}

bool CResourceManager::SoundPlay(const std::string& Name)
{
    CSound* pSound = FindSound(Name);

    if (!pSound)
        return false;

    if (pSound->m_GroupName == "BGM" && pSound != m_curBGM)
    {
        if (m_curBGM != nullptr)
            m_curBGM->Stop();
        m_curBGM = pSound;
    }

    pSound->Play();
  

    return true;
}

bool CResourceManager::SoundStop(const std::string& Name)
{
    CSound* pSound = FindSound(Name);

    if (!pSound)
        return false;

    pSound->Stop();

    return true;
}

bool CResourceManager::SoundPause(const std::string& Name)
{
    CSound* pSound = FindSound(Name);

    if (!pSound)
        return false;

    pSound->Pause();

    return true;
}

bool CResourceManager::SoundResume(const std::string& Name)
{
    CSound* pSound = FindSound(Name);

    if (!pSound)
        return false;

    pSound->Resume();

    return true;
}

void CResourceManager::ReleaseSound(const std::string& Name)
{
    auto    iter = m_mapSound.find(Name);

    if (iter == m_mapSound.end())
        return;

    iter->second->Release();

    if (iter->second->IsDestroy())
        m_mapSound.erase(iter);
}

void CResourceManager::EraseSound(const std::string& Name)
{
    auto    iter = m_mapSound.find(Name);

    if (iter == m_mapSound.end())
        return;

    m_mapSound.erase(iter);
}

CSound* CResourceManager::FindSound(const std::string& Name)
{
    auto    iter = m_mapSound.find(Name);

    if (iter == m_mapSound.end())
        return nullptr;

    return iter->second;
}

bool CResourceManager::CreateChannelGroup(const std::string& Name)
{
    FMOD::ChannelGroup* pGroup = FindChannelGroup(Name) ;

    if (pGroup)
        return true;

    m_pSystem->createChannelGroup(Name.c_str(), &pGroup);

    // 생성한 그룹을 마스터그룹에 추가한다.
    m_pMasterGroup->addGroup(pGroup, false);

    m_mapChannelGroup.insert(std::make_pair(Name, pGroup));

    return false;
}

FMOD::ChannelGroup* CResourceManager::FindChannelGroup(const std::string& Name)
{
    auto    iter = m_mapChannelGroup.find(Name);

    if (iter == m_mapChannelGroup.end())
        return nullptr;

    return iter->second;
}
