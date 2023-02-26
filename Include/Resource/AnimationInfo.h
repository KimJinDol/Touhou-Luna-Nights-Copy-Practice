#pragma once

#include "Texture.h"

struct AnimationFrameData
{
    Vector2 ImageStartPos;
    Vector2 Size;
    Vector2 Pivot;

    AnimationFrameData()
    {
    }

    ~AnimationFrameData()
    {
    }
};

struct AnimationNotify
{
    int             Frame;
    std::function<void()>   Function;
};

struct AnimationInfo
{
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

    AnimationInfo() :
        Name{},
        AnimType(AnimationType::Sprite),
        Frame(0),
        FrameCount(0),
        Time(0.f),
        FrameTime(0.f),
        PlayTime(0.f),
        PlayScale(1.f),
        bLoop(false),
        bReverse(false),
        NextAnimation(nullptr),
        EndFunction(nullptr),
        bEndFunction(false)
    {
    }

    ~AnimationInfo()
    {
        size_t  Size = vecNotify.size();

        for (int i =0; i < Size; ++i)
        {
            SAFE_DELETE(vecNotify[i]);
        }

        vecNotify.clear();
        vecFrameData.clear();
        /*std::vector<AnimationFrameData>().swap(vecFrameData);*/
    }
};
