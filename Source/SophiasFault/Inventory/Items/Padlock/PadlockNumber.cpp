#include "PadlockNumber.h"

APadlockNumber::APadlockNumber()
{
}

void APadlockNumber::UseInteraction()
{
    if (!_soundComponent->IsPlaying()) {
        FQuat currentRotation = GetActorQuat();
        FQuat rotationDelta = FQuat(FRotator(36.0f, 0.0f, 0.0f));
        FQuat newRotation = rotationDelta * currentRotation;
        SetActorRotation(newRotation.Rotator());

        _soundComponent->SetSound(_interactSound);
        _soundComponent->Play();
    }
}