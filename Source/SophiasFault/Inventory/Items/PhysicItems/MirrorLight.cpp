#include "MirrorLight.h"
#include "Curves/CurveVector.h" 
#include "../../../Macros.h"

AMirrorLight::AMirrorLight()
{
	PrimaryActorTick.bCanEverTick = true;

	_mirrorLightID = 0;

	_timelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent"));
}

void AMirrorLight::BeginPlay()
{
	Super::BeginPlay();

	_meshComponent->SetSimulatePhysics(false);
	_meshComponent->SetEnableGravity(false);

	if (_curveVector) {
		// Timeline
		_timelineCallback.BindUFunction(this, FName("ControlMirrorLight"));
		_timelineFinishedCallback.BindUFunction(this, FName("SetState"));

		_timelineComponent->AddInterpVector(_curveVector, _timelineCallback);
		_timelineComponent->SetTimelineFinishedFunc(_timelineFinishedCallback);
	}
}

void AMirrorLight::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	_timelineComponent->TickComponent(deltaTime, ELevelTick::LEVELTICK_TimeOnly, NULL);
}

void AMirrorLight::ControlMirrorLight(FVector value)
{
	_timelineValue = _timelineComponent->GetPlaybackPosition();
	_curveVectorValue = _curveVector->GetVectorValue(_timelineValue);

	_meshComponent->SetWorldLocation(_curveVectorValue);
}

void AMirrorLight::SetState()
{
	FVector finishActorLocation = FVector(_curveVector->FloatCurves[0].GetLastKey().Value, _curveVector->FloatCurves[1].GetLastKey().Value, _curveVector->FloatCurves[2].GetLastKey().Value);
	SetActorLocation(finishActorLocation);
	_meshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AMirrorLight::PositionLight(FVector placeablePosition)
{
	// Add first key with the hand holding component location
	_curveVector->FloatCurves[0].UpdateOrAddKey(0.f, _itemComponent->GetComponentLocation().X);
	_curveVector->FloatCurves[1].UpdateOrAddKey(0.f, _itemComponent->GetComponentLocation().Y);
	_curveVector->FloatCurves[2].UpdateOrAddKey(0.f, _itemComponent->GetComponentLocation().Z);

	// Add last key with the light finish position
	_curveVector->FloatCurves[0].UpdateOrAddKey(5.f, placeablePosition.X);
	_curveVector->FloatCurves[1].UpdateOrAddKey(5.f, placeablePosition.Y);
	_curveVector->FloatCurves[2].UpdateOrAddKey(5.f, placeablePosition.Z);

	// Activate the cubic interpolation mode
	for (int i = 0; i < 3; i++) {
		FRichCurve& curveComponent = _curveVector->FloatCurves[i];

		for (auto& key : curveComponent.Keys) {
			key.InterpMode = ERichCurveInterpMode::RCIM_Cubic;
		}
	}

	_meshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	_timelineComponent->PlayFromStart();
}