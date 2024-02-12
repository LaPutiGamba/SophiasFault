#include "GramophoneShape.h"
#include "../../Macros.h"

AGramophoneShape::AGramophoneShape()
{
}

void AGramophoneShape::UseInteraction()
{	
    FQuat currentRotation = GetActorQuat();
    FQuat rotationDelta = FQuat(FRotator(20.0f, 0.0f, 0.0f));
    FQuat newRotation = rotationDelta * currentRotation;
    SetActorRotation(newRotation.Rotator());
}