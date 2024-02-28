#include "PianoKey.h"
#include "../../../Macros.h"
#include "../../../Core/GMS_MyGameStateBase.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h" 

APianoKey::APianoKey()
{
	_pianoKeyID = 0;

	_bReadyState = true;

	ConstructorHelpers::FClassFinder<UUserWidget> dialogueFinderClass(TEXT("/Game/Items/Widgets/WBP_Dialogue"));
	if (dialogueFinderClass.Succeeded())
		_dialogueWidgetClass = dialogueFinderClass.Class;
	else
		_dialogueWidgetClass = nullptr;
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
		if (_myGameState != nullptr && _myGameState->GetPianoKeysPressed()->Num() <= 16) {
			if (_pianoKeyID == (*_myGameState->GetPianoKeysResult())[_myGameState->GetPianoKeysPressed()->Num()]) {
				_myGameState->GetPianoKeysPressed()->Add(_pianoKeyID);
			} else {
				_myGameState->GetPianoKeysPressed()->Reset();

                if (FMath::RandRange(0, 7) == 0) {
					if (APlayerController* playerController = GetWorld()->GetFirstPlayerController()) {
                        if (UUserWidget* dialogueWidget = CreateWidget<UUserWidget>(playerController, _dialogueWidgetClass)) {
                            if (UTextBlock* dialogueTextBlock = Cast<UTextBlock>(dialogueWidget->GetWidgetFromName("DialogueText"))) {
                                FText newText = FText::FromString("This doesn't sound very good, I think I'm doing something wrong...");
                                dialogueTextBlock->SetText(newText);
                            }

                            dialogueWidget->AddToViewport();

                            FTimerHandle dialogueTimerHandle;
                            GetWorld()->GetTimerManager().SetTimer(dialogueTimerHandle, [dialogueWidget]() {
                                dialogueWidget->RemoveFromParent();
                            }, 5.0f, false);
                        }
                    }
                }
			}

			if (_myGameState->GetPianoKeysPressed()->Num() == 16) {
				_myGameState->ActivatePianoSolution();
				_myGameState->SetPianoPuzzleSolved(true);
				return;
			}
		}

		_bReadyState = false;

		if (_metaSound != nullptr && _myGameState->GetPianoKeysPressed()->Num() != 16 && _soundComponent != nullptr)
			_soundComponent->Play();

		if (_myGameState->GetPianoKeysPressed()->Num() != 16)
			_timelineComponent->PlayFromStart();
	}
}