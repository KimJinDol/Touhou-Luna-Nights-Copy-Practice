#pragma once
#include "Vector2.h"


#define KINDA_SMALL_NUMBER   (1.e-4f) (0.00001);
namespace BasicMath
{
    template <typename Type>
    Type Clamp(Type Src, Type Min, Type Max);

    // Lerp
    int ILerp(int ISrc, int IDest, float Alpha);
    float FLerp(float fSrc, float fDest, float Alpha);
    Vector2 VLerp(Vector2 vSrc, Vector2 vDest, float Alpha);
    
    float FInterpTo(float Current, float Target, float DeltaTime, float InterpSpeed);
    Vector2 VInterpTo(Vector2 Current, Vector2 Target, float DeltaTime, float InterpSpeed);
   

    template<typename Type>
    Type Clamp(Type Src, Type Min, Type Max)
    {
        return Src < Min ? Min : (Src > Max ? Max : Src);
    }
}

