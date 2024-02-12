#include "Door.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "../../Sophia.h"
#include "../../Macros.h"

ADoor::ADoor()
{
	_openState = false;
	_readyState = true;
	_doorOpened = false;

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
	_curveFloatValue = _rotateDirection ? _curveFloat->GetFloatValue(_timelineValue) : (- 1.f * _curveFloat->GetFloatValue(_timelineValue));

	FQuat newRotation = FQuat(FRotator(0.f, _curveFloatValue, 0.f));
	_meshComponent->SetRelativeRotation(newRotation);
}

void ADoor::SetState()
{
	_readyState = true;
}

void ADoor::UseInteraction()
{
	if (_doorOpened && _readyState) {
		_openState = !_openState;

		if (_openState) {
			_readyState = false;
			_timelineComponent->PlayFromStart();
		} else {
			_readyState = false;
			_timelineComponent->Reverse();
		}
	}

	if (_sophiaCharacter->GetInventory()->GetCurrentHandItem() != nullptr) {
		if (_sophiaCharacter->GetInventory()->GetCurrentHandItem()->IsA<ADoorKey>()) {
			if (ADoorKey* doorKey = Cast<ADoorKey>(_sophiaCharacter->GetInventory()->GetCurrentHandItem())) {
				if (doorKey->_keyID == _keyID) {
					if (_readyState) {
						_openState = !_openState;

						if (_openState) {
							_readyState = false;
							_timelineComponent->PlayFromStart();
						} else {
							_readyState = false;
							_timelineComponent->Reverse();
						}
					}
				}
			}
		}
	}
}