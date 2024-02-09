#include "Door.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "../../Macros.h"

ADoor::ADoor()
{
	_openState = false;
	_readyState = true;

	_doorFrameComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Frame"));
	RootComponent = _doorFrameComponent;

	_meshComponent->SetupAttachment(_doorFrameComponent);
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	_rotateValue = 1.f;

	if (_curveFloat) {
		_timelineCallback.BindUFunction(this, FName("ControlDoor"));
		_timelineFinishedCallback.BindUFunction(this, FName("SetState"));

		_timelineComponent->AddInterpFloat(_curveFloat, _timelineCallback);
		_timelineComponent->SetTimelineFinishedFunc(_timelineFinishedCallback);
	}
}

void ADoor::ControlDoor(float value)
{
	_timelineValue = _timelineComponent->GetPlaybackPosition();
	printFloat(_timelineValue);
	_curveFloatValue = _rotateValue * _curveFloat->GetFloatValue(_timelineValue);

	FQuat newRotation = FQuat(FRotator(0.f, _curveFloatValue, 0.f));
	_meshComponent->SetRelativeRotation(newRotation);
}

void ADoor::SetState()
{
	_readyState = true;
}

void ADoor::UseInteraction()
{
	if (_readyState) {
		_openState = !_openState;

		APawn* ourPawn = UGameplayStatics::GetPlayerPawn(this, 0);
		FVector pawnLocation = ourPawn->GetActorLocation();
		FVector direction = GetActorLocation() - pawnLocation;
		direction = UKismetMathLibrary::LessLess_VectorRotator(direction, GetActorRotation());

		if (_openState) {
			if (direction.X > 0.f)
				_rotateValue = 1.f;
			else
				_rotateValue = -1.f;

			_readyState = false;
			_timelineComponent->PlayFromStart();
		} else {
			_readyState = false;
			_timelineComponent->Reverse();
		}
	} else {
		if (_timelineComponent->IsReversing()) {
			_openState = true;
			_timelineComponent->Play();
		} else {
			_openState = false;
			_timelineComponent->Reverse();
		}
	}
}