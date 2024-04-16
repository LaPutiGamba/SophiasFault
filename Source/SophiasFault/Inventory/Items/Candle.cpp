#include "Candle.h"
#include "Particles/ParticleSystemComponent.h" 
#include "Components/PointLightComponent.h" 
#include "Curves/CurveFloat.h" 

ACandle::ACandle()
{
	// Create the particle system component
	_particleSystem = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystem");
	_particleSystem->SetupAttachment(_meshComponent);
	_particleSystem->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	_particleSystem->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	_particleSystem->bAutoActivate = true;
	_particleSystem->bAutoDestroy = false;

	// Create the point light component
	_pointLight = CreateDefaultSubobject<UPointLightComponent>("CandleLight");
	_pointLight->SetupAttachment(RootComponent);
}

void ACandle::BeginPlay()
{
	Super::BeginPlay();

	SetLight(_bEnabled);

	if (_curveFloat) {
		// Timeline
		_timelineCallback.BindUFunction(this, FName("ControlCandle"));
		_timelineComponent->AddInterpFloat(_curveFloat, _timelineCallback);

		_timelineComponent->SetLooping(true);
		_timelineComponent->PlayFromStart();
	}
}

void ACandle::ControlCandle(float value)
{
	_timelineValue = _timelineComponent->GetPlaybackPosition();
	_curveFloatValue = _curveFloat->GetFloatValue(_timelineValue);

	// Lerp the intensity of the point light with the max and min intensity
	_pointLight->SetIntensity(FMath::Lerp(_minIntensity, _maxIntensity, _curveFloatValue));
}

void ACandle::SetLight(bool bVisibility)
{
	// Toggle the light
	_bEnabled = bVisibility;

	// Set the visibility of the point light
	_pointLight->SetVisibility(bVisibility);

	// Set the visibility of the particle system
	_particleSystem->SetVisibility(bVisibility);

	// Set the cast shadow of the static mesh component
	_meshComponent->SetCastShadow(!bVisibility);
}