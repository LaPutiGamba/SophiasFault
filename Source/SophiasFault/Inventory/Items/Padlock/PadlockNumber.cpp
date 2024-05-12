#include "PadlockNumber.h"

APadlockNumber::APadlockNumber()
{
}

void APadlockNumber::UseInteraction()
{
    if (!_soundComponent->IsPlaying()) {
        FQuat currentRotation = GetActorQuat();
        FQuat rotationDelta = FQuat(FRotator(0.f, 36.f, 0.f));
        FQuat newRotation = rotationDelta * currentRotation;
        SetActorRotation(newRotation.Rotator());

        _soundComponent->SetSound(_interactSound);
        _soundComponent->Play();
    }
}