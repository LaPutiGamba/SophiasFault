#include "PianoKey.h"
#include "../../../Macros.h"
#include "../../../Core/GMS_MyGameStateBase.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h" 

APianoKey::APianoKey()
{
	_pianoKeyID = 0;

	_bReadyState = true;
}

void APianoKey::BeginPlay()
{
	Super::BeginPlay();

	if (_metaSound != nullptr) {
		if (_metaSound->IsValidLowLevelFast()) {
			if (_soundComponent != nullptr) {
				_soundComponent->SetSound(_metaSound);
				_soundComponent->SetIntParameter("Piano Note", (_pianoKeyID - 1));
			}
		}
	}

	// Timeline
	if (_curveFloat) {
		_timelineCallback.BindUFunction(this, FName("ControlPianoKey"));
		_timelineFinishedCallback.BindUFunction(this, FName("SetState"));

		_timelineComponent->AddInterpFloat(_curveFloat, _timelineCallback);
		_timelineComponent->SetTimelineFinishedFunc(_timelineFinishedCallback);
	}
}

void APianoKey::ControlPianoKey(float value)
{
	_timelineValue = _timelineComponent->GetPlaybackPosition();
	_curveFloatValue = _curveFloat->GetFloatValue(_timelineValue);

	FQuat newRotation = FQuat(FRotator(0.f, 180.f, -_curveFloatValue));
	_meshComponent->SetRelativeRotation(newRotation);
}

void APianoKey::SetState()
{
	_bReadyState = true;
}

void APianoKey::UseInteraction()
{
	if (_bReadyState && !_myGameState->GetPianoPuzzleSolved()) {
		if (_myGameState != nullptr && _myGameState->GetPianoKeysPressed()->Num() <= 9) {
			if (_pianoKeyID == (*_myGameState->GetPianoKeysResult())[_myGameState->GetPianoKeysPressed()->Num()]) {
				_myGameState->GetPianoKeysPressed()->Add(_pianoKeyID);
			} else {
				_myGameState->GetPianoKeysPressed()->Reset();
				
				if (_myGameState->_dialogueWidget != nullptr && GetWorld() != nullptr) {
					if (FMath::RandRange(0, 7) == 0 && !_myGameState->_dialogueWidget->IsInViewport()) {
						if (UTextBlock* dialogueTextBlock = Cast<UTextBlock>(_myGameState->_dialogueWidget->GetWidgetFromName("DialogueText"))) {
							FText newText = FText::FromString("This doesn't sound very good, I think I'm doing something wrong...");
							dialogueTextBlock->SetText(newText);
						}

						_myGameState->_dialogueWidget->AddToViewport();

						FTimerHandle dialogueTimerHandle;
						GetWorld()->GetTimerManager().SetTimer(dialogueTimerHandle, [this]() {
							_myGameState->_dialogueWidget->RemoveFromParent();
						}, 5.0f, false);
					}
				}
			}

			if (_myGameState->GetPianoKeysPressed()->Num() == 9) {
				_myGameState->ActivatePianoSolution();
				_myGameState->SetPianoPuzzleSolved(true);
				return;
			}
		}

		_bReadyState = false;

		if (_metaSound != nullptr && _myGameState->GetPianoKeysPressed()->Num() != 9 && _soundComponent != nullptr)
			_soundComponent->Play();

		if (_myGameState->GetPianoKeysPressed()->Num() != 9)
			_timelineComponent->PlayFromStart();
	}
}