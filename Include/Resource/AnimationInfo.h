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
    AnimationType   AnimType;       // �ִϸ��̼� Ÿ��.
    CSharedPtr<CTexture>  pTexture; // �� �ִϸ��̼��� ����ϴ� �̹���
    std::vector<AnimationFrameData> vecFrameData;
    int             Frame;      // ���� �ִϸ��̼��� ����� ���۵Ǵ����� �����ϴ� �����Ӱ�.
    int             FrameCount; // �� �ִϸ��̼��� �������������� ����.
    float           Time;       // �ð��� üũ�ϱ� ���� ����
    float           FrameTime;  // 1�������� ����Ǳ� ���� �ʿ��� �ð�.
    float           PlayTime;   // �� �ִϸ��̼��� �ѹ� ���۵Ǳ� ���� �ʿ��� �ð�
    float           PlayScale;  // �ִϸ��̼� �������
    bool            bLoop;
    bool            bReverse;
    AnimationInfo* NextAnimation; // �� �ִϸ��̼��� ������ �̾��� �ִϸ��̼�
    std::function<void()>  EndFunction;
    bool            bEndFunction;   // EndFunction�� �����ߴ���
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
