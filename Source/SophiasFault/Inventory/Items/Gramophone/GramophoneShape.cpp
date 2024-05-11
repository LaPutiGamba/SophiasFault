#include "GramophoneShape.h"
#include "../../../Macros.h"

AGramophoneShape::AGramophoneShape()
{
}

void AGramophoneShape::UseInteraction()
{	
    if (!_soundComponent->IsPlaying()) {
        FQuat currentRotation = GetActorQuat();
        FQuat rotationDelta = FQuat(FRotator(-45.0f, 0.0f, 0.0f));
        FQuat newRotation = rotationDelta * currentRotation;
        SetActorRotation(newRotation.Rotator());

		_soundComponent->SetSound(_interactSound);
		_soundComponent->Play();
    }
}