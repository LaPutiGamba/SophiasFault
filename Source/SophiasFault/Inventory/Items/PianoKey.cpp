#include "PianoKey.h"
#include "../../Macros.h"
#include "../../Core/GMS_MyGameStateBase.h"
#include "Components/AudioComponent.h" 
#include "Sound/SoundCue.h" 
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h" 

APianoKey::APianoKey()
{
	_pianoKeyID = 0;

	_soundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("PianoKeyComponent"));
	_soundComponent->bAutoActivate = false; 
	_soundComponent->SetupAttachment(RootComponent);
	_soundComponent->SetRelativeLocation(FVector(0.f, 0.0f, 0.0f));

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

	if (_soundCue != nullptr) {
		if (_soundCue->IsValidLowLevelFast())
			_soundComponent->SetSound(_soundCue);
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
		if (_myGameState != nullptr && _myGameState->GetPianoKeysPressed()->Num() <= 12) {
			if (_pianoKeyID == (*_myGameState->GetPianoKeysResult())[_myGameState->GetPianoKeysPressed()->Num()]) {
				_myGameState->GetPianoKeysPressed()->Add(_pianoKeyID);
			} else {
				_myGameState->GetPianoKeysPressed()->Reset();

                if (FMath::RandRange(0, 7) == 0) {
                    APlayerController* playerController = GetWorld()->GetFirstPlayerController();
                    if (playerController != nullptr) {
                        UUserWidget* dialogueWidget = CreateWidget<UUserWidget>(playerController, _dialogueWidgetClass);
                        if (dialogueWidget) {
                            UTextBlock* dialogueTextBlock = Cast<UTextBlock>(dialogueWidget->GetWidgetFromName("DialogueText"));
                            if (dialogueTextBlock) {
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

			if (_myGameState->GetPianoKeysPressed()->Num() == 12) {
				_myGameState->ActivatePianoSolution();
				_myGameState->SetPianoPuzzleSolved(true);
				return;
			}
		}

		_bReadyState = false;

		if (_soundCue != nullptr && _myGameState->GetPianoKeysPressed()->Num() != 12)
			_soundComponent->Play();

		if (_myGameState->GetPianoKeysPressed()->Num() != 12)
			_timelineComponent->PlayFromStart();
	}
}