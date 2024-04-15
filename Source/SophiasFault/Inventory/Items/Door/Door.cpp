#include "Door.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Curves/CurveFloat.h"
#include "Components/TextBlock.h" 
#include "../../DialogueWidget.h"
#include "../../../Core/GMS_MyGameStateBase.h"
#include "../../../Sophia.h"
#include "../../../Macros.h"
#include "../../Item.h"

ADoor::ADoor()
{
	_bOpenState = false;
	_bReadyState = true;
	_bDoorLocked = true;

	_doorHandle = nullptr;

	_timelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline Component"));

	_doorLockedText = FText::FromString("The door is locked.");
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	if (_selectedCurveFloat) {
		_timelineCallback.BindUFunction(this, FName("ControlDoor"));
		_timelineFinishedCallback.BindUFunction(this, FName("SetState"));

		_timelineComponent->AddInterpFloat(_selectedCurveFloat, _timelineCallback);
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
	_curveFloatValue = _bRotateDirection ? _selectedCurveFloat->GetFloatValue(_timelineValue) : (- 1.f * _selectedCurveFloat->GetFloatValue(_timelineValue));
	float newRotation = _curveFloatValue - _rotationApplied;
	_rotationApplied += newRotation;

	_skeletalMeshComponent->AddLocalRotation(FRotator(0.f, newRotation, 0.f));
}

void ADoor::SetState()
{
	_bReadyState = true;
}

void ADoor::UseInteraction(AItem* item)
{
	if (!_bDoorLocked && _bReadyState) {
		_bOpenState = !_bOpenState;

		_selectedCurveFloat = _openCurveFloat;

		if (_bOpenState) {
			_bReadyState = false;
			if (_doorHandle != nullptr) {
				_doorHandle->UseInteraction(nullptr);

				FTimerHandle timerHandle;
				GetWorld()->GetTimerManager().SetTimer(timerHandle, [this]() {
					_timelineComponent->PlayFromStart();

					_soundComponent->SetIntParameter("Door State", 0);
					_soundComponent->Play();
					}, _doorHandle->_animationSequence->GetPlayLength() / 2, false);
			} else {
				_timelineComponent->PlayFromStart();

				_soundComponent->SetIntParameter("Door State", 0);
				_soundComponent->Play();
			}
		} else {
			_bReadyState = false;
			_timelineComponent->Reverse();

			_soundComponent->SetIntParameter("Door State", 1);
			_soundComponent->Play();
		}
	}

	if (_bDoorLocked && !_soundComponent->IsPlaying() && _bReadyState) {
		_bReadyState = false;

		if (_doorHandle != nullptr) {
			_myGameState->_dialogueWidget->SetDialogueTextAndShow(_doorLockedText, 4.f);

			_doorHandle->UseInteraction(nullptr);
			_selectedCurveFloat = _lockedCurveFloat;

			FTimerHandle timerHandle;
			GetWorld()->GetTimerManager().SetTimer(timerHandle, [this]() {
				_timelineComponent->PlayFromStart();

				_soundComponent->SetIntParameter("Door State", 2);
				_soundComponent->Play();
			}, _doorHandle->_animationSequence->GetPlayLength() / 2, false);
		} else {
			_myGameState->_dialogueWidget->SetDialogueTextAndShow(_doorLockedText, 4.f);

			_selectedCurveFloat = _lockedCurveFloat;

			_timelineComponent->PlayFromStart();

			_soundComponent->SetIntParameter("Door State", 2);
			_soundComponent->Play();
		}
	}
}