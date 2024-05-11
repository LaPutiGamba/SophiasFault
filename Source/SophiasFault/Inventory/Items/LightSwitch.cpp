#include "LightSwitch.h"
#include "Components/PointLightComponent.h" 
#include "Curves/CurveFloat.h"

ALightSwitch::ALightSwitch()
{
	_bLightOn = false;
	_bOldLight = false;

	_curveFloat = CreateDefaultSubobject<UCurveFloat>(TEXT("CurveFloat"));

	_lightSwitch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LightSwitch"));
}

void ALightSwitch::BeginPlay()
{
	Super::BeginPlay();

	// Timeline
	if (_curveFloat) {
		_curveFloat->FloatCurve.UpdateOrAddKey(0.f, 0.f);

		if (_bOldLight)
			_curveFloat->FloatCurve.UpdateOrAddKey(1.f, 90.f);
		else
			_curveFloat->FloatCurve.UpdateOrAddKey(1.f, 45.f);

		_timelineCallback.BindUFunction(this, FName("ControlLightSwitch"));

		_timelineComponent->AddInterpFloat(_curveFloat, _timelineCallback);
	}
}

void ALightSwitch::UseInteraction(AItem* item)
{
	IInteractiveInterface::UseInteraction(item);

	if (!_timelineComponent->IsPlaying()) {
		if (_bLightOn) {
			_bLightOn = false;
			for (auto& lights : _lightActor) {
				lights->GetLightComponent()->SetVisibility(false);
			}

			_timelineComponent->PlayFromStart();
			_soundComponent->Play();
		}
		else {
			_bLightOn = true;
			for (auto& lights : _lightActor) {
				lights->GetLightComponent()->SetVisibility(true);
			}

			_timelineComponent->ReverseFromEnd();
			_soundComponent->Play();
		}
	}
}

void ALightSwitch::ControlLightSwitch()
{
	_timelineValue = _timelineComponent->GetPlaybackPosition();
	_curveFloatValue = _curveFloat->GetFloatValue(_timelineValue);

	if (_bOldLight)
		_lightSwitch->SetRelativeRotation(FRotator(_curveFloatValue, 0.f, 0.f));
	else
		_lightSwitch->SetRelativeRotation(FRotator(0.f, 0.f, _curveFloatValue));
}