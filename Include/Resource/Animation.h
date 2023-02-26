#pragma once

#include "AnimationInfo.h"

class CAnimation :
    public CRef
{
    friend class CGameObject;
    friend class CEffect;
    friend class CTile;

private:
    CAnimation();
    CAnimation(const CAnimation& Anim);
    ~CAnimation();

private:
    class CScene* m_pScene;
    class CGameObject* m_pOwner;
    std::unordered_map<std::string, AnimationInfo*> m_mapAnimation;
    AnimationInfo* m_pCurrentAnim;

public:
    bool AddAnimation(const std::string& Name, AnimationType Type, bool Loop,
        class CTexture* pTexture, const std::vector<AnimationFrameData>& vecFrame,
        float PlayTime = 1.f);
    bool AddAnimation(const std::string& Name, AnimationType Type, bool Loop,
        class CTexture* pTexture, float PlayTime = 1.f);
    bool AddAnimation(const std::string& Name, AnimationType Type, bool Loop,
        const std::string& TextureName, float PlayTime = 1.f);
    bool AddAnimation(const std::string& Name, AnimationType Type, bool Loop,
        const std::string& TextureName, const TCHAR* pTexFileName,
        float PlayTime = 1.f);
    bool AddAnimation(const std::string& Name, AnimationType Type, bool Loop,
        const std::string& TextureName, const std::vector<std::wstring>& vecFileName,
        float PlayTime = 1.f);
    bool AddAnimationFrameData(const std::string& Name,
        const Vector2& ImageStartPos, const Vector2& Size);
    bool AddAnimationFrameData(const std::string& Name,
        const Vector2& ImageStartPos, const Vector2& Size,
        int Count);
    bool AddAnimationFrameData(const std::string& Name, unsigned int x, unsigned int y,     // 아틀라스 기준.
        unsigned int Count, const Vector2& FramePixelSize = Vector2(512.f, 512.f), const Vector2& Pivot = Vector2(0.5f, 1.f));
    void SetPlayScale(const std::string& Name, float Scale);
    void SetCurrentAnimation(const std::string& Name);
    void ChangeAnimation(const std::string& Name);
    void SetAnimationReverse(const std::string& Name, bool bReverse);
    void Update(float DeltaTime);
    bool CheckAnimation(const std::string& Name);
    void SetNextAnimation(const std::string& SrcName, const std::string& DestName); // Loop돌지 않는 애니메이션일 경우, 다음으로 이어질 애니메이션을 세팅해주는 함수.
    AnimationInfo* FindAnimation(const std::string& Name);
    CAnimation* Clone();
    void Save(FILE* pFile);
    void Load(FILE* pFile);

public:
    std::string& GetCurrentAnimation()
    {
        return m_pCurrentAnim->Name;
    }

    int GetCurrentAnimationFrame()  const
    {
        return m_pCurrentAnim->Frame;
    }

public:
    template <typename T>
    void SetEndFunction(const std::string& Name, T* pObj,
        void (T::* pFunc)())
    {
        AnimationInfo* pInfo = FindAnimation(Name);

        if (!pInfo)
            return;

        pInfo->EndFunction =std::bind(pFunc, pObj);
    }

    template <typename T>
    void SetNotifyFunction(const std::string& Name, int Frame,
        T* pObj, void (T::* pFunc)())
    {
        AnimationInfo* pInfo = FindAnimation(Name);

        if (!pInfo)
            return;

        AnimationNotify* pNotify = new AnimationNotify;

        pNotify->Frame = Frame;
        pNotify->Function = std::bind(pFunc, pObj);
        pInfo->vecNotify.push_back(pNotify);
    }
};

