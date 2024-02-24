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

	_doorFrameComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Frame"));
	RootComponent = _doorFrameComponent;

	_meshComponent->SetupAttachment(_doorFrameComponent);
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
}

void ADoor::ControlDoor(float value)
{
	_timelineValue = _timelineComponent->GetPlaybackPosition();
	_curveFloatValue = _bRotateDirection ? _curveFloat->GetFloatValue(_timelineValue) : (- 1.f * _curveFloat->GetFloatValue(_timelineValue));

	FQuat newRotation = FQuat(FRotator(0.f, _curveFloatValue, 0.f));
	_meshComponent->SetRelativeRotation(newRotation);
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
			if (_metaSound != nullptr) 
				_soundComponent->SetSound(_metaSound);

			_bReadyState = false;
			_timelineComponent->PlayFromStart();
			_soundComponent->Play();
		} else {
			if (_metaSoundClose != nullptr)
				_soundComponent->SetSound(_metaSoundClose);

			_bReadyState = false;
			_timelineComponent->Reverse();

			FTimerHandle playSoundTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(playSoundTimerHandle, [this]() {
				_soundComponent->Play();
			}, 0.5f, false);
		}
	}

	if (_bDoorLocked && !_soundComponent->IsPlaying()) {
		if (_metaSoundLocked != nullptr) {
			_soundComponent->SetSound(_metaSoundLocked);
			
			_soundComponent->Play();
		}
	}
}