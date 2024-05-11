#include "Gramophone.h"
#include "../../../Macros.h"
#include "Curves/CurveFloat.h"
#include "../../../Cameras/GramophoneCamera.h"

AGramophone::AGramophone()
{
	_firstShape = nullptr;
	_secondShape = nullptr;
	_thirdShape = nullptr;

	_curveFloat = CreateDefaultSubobject<UCurveFloat>(TEXT("CurveFloat"));
	_bReadyState = false;
}

void AGramophone::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> camerasActorBlend;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGramophoneCamera::StaticClass(), camerasActorBlend);

	for (AActor* cameraActor : camerasActorBlend) {
		_cameraActorBlend = Cast<AGramophoneCamera>(cameraActor);
		if (_cameraActorBlend)
			break;
	}

	// Timeline
	if (_curveFloat) {
		_curveFloat->FloatCurve.UpdateOrAddKey(0.f, 0.f);
		_keyHandle = _curveFloat->FloatCurve.UpdateOrAddKey(1.f, 1.f);

		_timelineCallback.BindUFunction(this, FName("ControlGramophone"));

		_timelineComponent->AddInterpFloat(_curveFloat, _timelineCallback);
	}
}

void AGramophone::UseInteraction()
{
	if (_firstShape != nullptr && _secondShape != nullptr && _thirdShape != nullptr) {
		FRotator firstShapeRot = _firstShape->GetActorRotation();
		FRotator secondShapeRot = _secondShape->GetActorRotation();
		FRotator thirdShapeRot = _thirdShape->GetActorRotation();

		if (firstShapeRot.Roll >= 19.f && firstShapeRot.Roll <= 21.f &&
			secondShapeRot.Roll >= 19.f && secondShapeRot.Roll <= 21.f &&
			thirdShapeRot.Roll >= 19.f && thirdShapeRot.Roll <= 21.f && !_bReadyState) {
			
			_curveFloat->FloatCurve.DeleteKey(_keyHandle);
			_curveFloat->FloatCurve.UpdateOrAddKey(16.f, 1.f);
			_timelineComponent->PlayFromStart();
			if (_metaSound != nullptr) {
				_soundComponent->SetSound(_metaSound);
				_soundComponent->Play();
			}

			_bReadyState = true;
		} else {
			if (!_bReadyState)
				_timelineComponent->PlayFromStart();

			if (_interactSound != nullptr) {
				_soundComponent->SetSound(_interactSound);
				_soundComponent->Play();
			}
		}
	}
}

void AGramophone::ControlGramophone()
{
	if (_bReadyState) {
		if (_timelineComponent->GetPlaybackPosition() < 0.2f) {
			_crank->AddActorLocalRotation(FRotator(-0.6f, 0.f, 0.f));
		} else if (_timelineComponent->GetPlaybackPosition() >= 0.2f && _timelineComponent->GetPlaybackPosition() < 0.4f) {
			_crank->AddActorLocalRotation(FRotator(-1.2f, 0.f, 0.f));
		} else if (_timelineComponent->GetPlaybackPosition() >= 0.4f && _timelineComponent->GetPlaybackPosition() < 0.6f) {
			_crank->AddActorLocalRotation(FRotator(-1.8f, 0.f, 0.f));
		} else if (_timelineComponent->GetPlaybackPosition() >= 0.6f && _timelineComponent->GetPlaybackPosition() < 0.8f) {
			_crank->AddActorLocalRotation(FRotator(-2.4f, 0.f, 0.f));
		} else if (_timelineComponent->GetPlaybackPosition() >= 0.8f && _timelineComponent->GetPlaybackPosition() < 1.f) {
			_crank->AddActorLocalRotation(FRotator(-3.f, 0.f, 0.f));
		} else if (_timelineComponent->GetPlaybackPosition() >= 1.f && _timelineComponent->GetPlaybackPosition() < 1.2f) {
			_crank->AddActorLocalRotation(FRotator(-3.f, 0.f, 0.f));
			_record->AddActorLocalRotation(FRotator(0.f, 0.2f, 0.f));
		}  else if (_timelineComponent->GetPlaybackPosition() >= 1.2f && _timelineComponent->GetPlaybackPosition() < 1.4f) {
			_crank->AddActorLocalRotation(FRotator(-3.f, 0.f, 0.f));
			_record->AddActorLocalRotation(FRotator(0.f, 0.4f, 0.f));
		} else if (_timelineComponent->GetPlaybackPosition() >= 1.4f && _timelineComponent->GetPlaybackPosition() < 1.6f) {
			_crank->AddActorLocalRotation(FRotator(-3.f, 0.f, 0.f));
			_record->AddActorLocalRotation(FRotator(0.f, 0.6f, 0.f));
		} else if (_timelineComponent->GetPlaybackPosition() >= 1.6f && _timelineComponent->GetPlaybackPosition() < 1.8f) {
			_crank->AddActorLocalRotation(FRotator(-3.f, 0.f, 0.f));
			_record->AddActorLocalRotation(FRotator(0.f, 0.8f, 0.f));
		} else if (_timelineComponent->GetPlaybackPosition() >= 1.8f && _timelineComponent->GetPlaybackPosition() < 12.f) {
			_crank->AddActorLocalRotation(FRotator(-3.f, 0.f, 0.f));
			_record->AddActorLocalRotation(FRotator(0.f, 1.f, 0.f));
		} else if (_timelineComponent->GetPlaybackPosition() >= 13.f) {
			_cover->AddActorLocalRotation(FRotator(-0.15f, 0.f, 0.f));
		}
	} else {
		if (_timelineComponent->GetPlaybackPosition() < 0.25f) {
			_crank->AddActorLocalRotation(FRotator(-1.f, 0.f, 0.f));
		} else if (_timelineComponent->GetPlaybackPosition() >= 0.25f && _timelineComponent->GetPlaybackPosition() < 0.50f) {
			_crank->AddActorLocalRotation(FRotator(1.f, 0.f, 0.f));
		} else if (_timelineComponent->GetPlaybackPosition() >= 0.50f && _timelineComponent->GetPlaybackPosition() < 0.75f) {
			_crank->AddActorLocalRotation(FRotator(-1.f, 0.f, 0.f));
		} else if (_timelineComponent->GetPlaybackPosition() >= 0.75f) {
			_crank->AddActorLocalRotation(FRotator(1.f, 0.f, 0.f));
		}
	}
}