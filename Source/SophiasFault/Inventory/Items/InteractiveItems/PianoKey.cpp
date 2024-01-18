#include "PianoKey.h"
#include "../../../Macros.h"
#include "../../../Core/GMS_MyGameStateBase.h"
#include "Components/AudioComponent.h" 
#include "Sound/SoundCue.h" 

APianoKey::APianoKey()
{
	_myGameState = nullptr;

	_pianoKeyID = 0;

	_soundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("PianoKeyComponent"));
	_soundComponent->bAutoActivate = false; 
	_soundComponent->SetupAttachment(RootComponent);
	_soundComponent->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));

	_bReadyState = true;
}

void APianoKey::BeginPlay()
{
	Super::BeginPlay();

	if (_soundCue != nullptr) {
		if (_soundCue->IsValidLowLevelFast())
			_soundComponent->SetSound(_soundCue);
	}

	_myGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AGMS_MyGameStateBase>() : nullptr;

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

	FQuat newRotation = FQuat(FRotator(0.f, 0.f, _curveFloatValue));
	_meshComponent->SetRelativeRotation(newRotation);
}

void APianoKey::SetState()
{
	_bReadyState = true;
}

void APianoKey::UseInteraction()
{
	if (_bReadyState && !_myGameState->GetPianoPuzzleSolved()) {
		if (_soundCue != nullptr)
			_soundComponent->Play();

		if (_myGameState != nullptr && _myGameState->GetPianoKeysPressed()->Num() <= 11) {
			// Revisa esto bobo
			if (_pianoKeyID == (*_myGameState->GetPianoKeysResult())[_myGameState->GetPianoKeysPressed()->Num()]) {
				_myGameState->GetPianoKeysPressed()->Add(_pianoKeyID);
				for (size_t i = 0; i < _myGameState->GetPianoKeysPressed()->Num(); i++) {
					printInt((*_myGameState->GetPianoKeysPressed())[i]);
				}
				printText("-----------------");
			} else {
				_myGameState->GetPianoKeysPressed()->Reset();
				// Escribir texto tipo: Mmmm no suena muy bien
			}

			if (_myGameState->GetPianoKeysPressed()->Num() == 11) {
				_myGameState->ActivatePianoSolution();
				_myGameState->SetPianoPuzzleSolved(true);
			}
		}

		_bReadyState = false;
		_timelineComponent->PlayFromStart();
	}
}