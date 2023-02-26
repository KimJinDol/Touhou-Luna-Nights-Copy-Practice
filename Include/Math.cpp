#include "Math.h"




// Lerp .. 
int BasicMath::ILerp(int ISrc, int IDest, float Alpha)
{
    return ISrc + (IDest - ISrc) * Alpha;
}

float BasicMath::FLerp(float fSrc, float fDest, float Alpha)
{
    return fSrc + (fDest - fSrc) * Alpha;
}

Vector2 BasicMath::VLerp(Vector2 vSrc, Vector2 vDest, float Alpha)
{
    return vSrc + (vDest - vSrc) * Alpha;
}


// Interp ... Tries to reach Target based on distance from Current postiion.
float BasicMath::FInterpTo(float Current, float Target, float DeltaTime, float InterpSpeed)
{
    if (InterpSpeed <= 0.f)
    {
        return Target;
    }

    const float Dist = Target - Current;

    const float DeltaMove = Dist * Clamp<float>(DeltaTime * InterpSpeed, 0.f, 1.f);
    return Current + DeltaMove;
}

Vector2 BasicMath::VInterpTo(Vector2 Current, Vector2 Target, float DeltaTime, float InterpSpeed)
{
    if (InterpSpeed <= 0.f)
    {
        return Target;
    }

    const Vector2 Dist = Target - Current;

    const Vector2 DeltaMove = Dist * Clamp<float>(DeltaTime * InterpSpeed, 0.f, 1.f);
    return Current + DeltaMove;
}
