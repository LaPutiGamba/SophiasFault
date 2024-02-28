#include "Door.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "../../../Sophia.h"
#include "../../../Macros.h"

ADoor::ADoor()
{
	_bOpenState = false;
	_bReadyState = true;
	_bDoorLocked = true;
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	if (_curveFloat) {
		_timelineCallback.BindUFunction(this, FName("ControlDoor"));
		_timelineFinishedCallback.BindUFunction(this, FName("SetState"));

		_timelineComponent->AddInterpFloat(_curveFloat, _timelineCallback);
		_timelineComponent->SetTimelineFinishedFunc(_timelineFinishedCallback);
	}

	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);

	if (playerController != nullptr) 
		_sophiaCharacter = Cast<ASophia>(playerController->GetPawn());

	if (_metaSound != nullptr)
		_soundComponent->SetSound(_metaSound);
}

void ADoor::ControlDoor(float value)
{
	_timelineValue = _timelineComponent->GetPlaybackPosition();
	_curveFloatValue = _bRotateDirection ? _curveFloat->GetFloatValue(_timelineValue) : (- 1.f * _curveFloat->GetFloatValue(_timelineValue));
	float newRotation = _curveFloatValue - _rotationApplied;
	_rotationApplied += newRotation;

	AddActorLocalRotation(FRotator(0.f, newRotation, 0.f));
}

void ADoor::SetState()
{
	_bReadyState = true;
}

void ADoor::UseInteraction()
{
	if (!_bDoorLocked && _bReadyState) {
		_bOpenState = !_bOpenState;

		if (_bOpenState) {
			_bReadyState = false;
			_timelineComponent->PlayFromStart();

			_soundComponent->SetIntParameter("Door State", 0);
			_soundComponent->Play();
		} else {
			_bReadyState = false;
			_timelineComponent->Reverse();

			_soundComponent->SetIntParameter("Door State", 1);
			_soundComponent->Play();
		}
	}

	if (_bDoorLocked && !_soundComponent->IsPlaying()) {
		_soundComponent->SetIntParameter("Door State", 2);
		_soundComponent->Play();
	}
}