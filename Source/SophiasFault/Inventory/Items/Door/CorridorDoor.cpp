#include "CorridorDoor.h"

ACorridorDoor::ACorridorDoor()
{
}

void ACorridorDoor::BeginPlay()
{
	ADoor::BeginPlay();

	_timelineCallback.Unbind();
	_timelineCallback.BindUFunction(this, FName("ControlDoor"));
}

void ACorridorDoor::ControlDoor(float value)
{
	_timelineValue = _timelineComponent->GetPlaybackPosition();
	_curveFloatValue = _bRotateDirection ? _selectedCurveFloat->GetFloatValue(_timelineValue) : (- 1.f * _selectedCurveFloat->GetFloatValue(_timelineValue));
	float newRotation = _curveFloatValue - _rotationApplied;
	_rotationApplied += newRotation;

	AddActorLocalOffset(FVector(newRotation, 0.f, 0.f));
}