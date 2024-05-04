#include "Gramophone.h"
#include "../../../Macros.h"
#include "Curves/CurveFloat.h"

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

	// Timeline
	if (_curveFloat) {
		_curveFloat->FloatCurve.UpdateOrAddKey(0.f, 0.f);
		_curveFloat->FloatCurve.UpdateOrAddKey(15.f, 1.f);

		_timelineCallback.BindUFunction(this, FName("ControlGramophone"));

		_timelineComponent->AddInterpFloat(_curveFloat, _timelineCallback);
	}
}

void AGramophone::UseInteraction(AItem* item)
{
	IInteractiveInterface::UseInteraction(item);

	if (_firstShape != nullptr && _secondShape != nullptr && _thirdShape != nullptr) {
		FRotator firstShapeRot = _firstShape->GetActorRotation();
		FRotator secondShapeRot = _secondShape->GetActorRotation();
		FRotator thirdShapeRot = _thirdShape->GetActorRotation();

		if (firstShapeRot.Roll >= 19.f && firstShapeRot.Roll <= 21.f &&
			secondShapeRot.Roll >= 19.f && secondShapeRot.Roll <= 21.f &&
			thirdShapeRot.Roll >= 19.f && thirdShapeRot.Roll <= 21.f && !_bReadyState) {
			
			_timelineComponent->PlayFromStart();
			if (_metaSound != nullptr) {
				_soundComponent->SetSound(_metaSound);
				_soundComponent->Play();
			}

			_bReadyState = true;
		} else {
			if (_interactSound != nullptr) {
				_soundComponent->SetSound(_interactSound);
				_soundComponent->Play();
			}
		}
	}
}

void AGramophone::ControlGramophone()
{
	if (_timelineComponent->GetPlaybackPosition() < 5.f) {
		_crank->AddActorLocalRotation(FRotator(-3.f, 0.f, 0.f));
	} else if (_timelineComponent->GetPlaybackPosition() >= 5.f && _timelineComponent->GetPlaybackPosition() < 12.f) {
		_record->AddActorLocalRotation(FRotator(0.f, 3.f, 0.f));
	} else if (_timelineComponent->GetPlaybackPosition() >= 12.f) {
		_cover->AddActorLocalRotation(FRotator(-0.15f, 0.f, 0.f));
	}
}