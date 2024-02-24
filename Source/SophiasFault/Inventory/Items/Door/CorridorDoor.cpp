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
	_curveFloatValue = _bRotateDirection ? _curveFloat->GetFloatValue(_timelineValue) : (- 1.f * _curveFloat->GetFloatValue(_timelineValue));

	FVector newLocation = FVector(_curveFloatValue, 0.f, 0.f);
	_meshComponent->SetRelativeLocation(newLocation);
}