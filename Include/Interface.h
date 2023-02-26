#pragma once
#include <combaseapi.h>

interface ITimeStopInterface
{
public:
    bool bTimeStop = false;

public:
    virtual void SetTimeMode(TimeMode eMode) = 0;

    // This Function call instead Update(float DeltaTime) if in Time Stop Mode 
    virtual void Update(float DeltaTime, TimeMode eMode = TimeMode::Stop) = 0;

};

interface ISlowTimeInterface // SlowTime ��� �� ������ ���� �ʴ� ������Ʈ��.
{
public:
    virtual void Update(float DeltaTime, TimeMode eMode = TimeMode::Snail) = 0;
};

interface DamageableInterface
{
public:
    virtual float GetAD() const = 0;
};