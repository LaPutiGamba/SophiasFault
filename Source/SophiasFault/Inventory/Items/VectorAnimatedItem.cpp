#include "VectorAnimatedItem.h"
#include "Curves/CurveVector.h"
#include "../../Macros.h"

AVectorAnimatedItem::AVectorAnimatedItem()
{
	PrimaryActorTick.bCanEverTick = true;

	_curveVectorLocation = CreateDefaultSubobject<UCurveVector>(TEXT("CurveVectorLocation"));
	_curveVectorRotation = CreateDefaultSubobject<UCurveVector>(TEXT("CurveVectorRotation"));

	_bUseOnce = false;
	_bUseState = false;

	_bLocation = false;
	_bRotation = false;

	_initialLocationTime = 0.f;
	_finalLocationTime = 1.f;
	_initialRotationTime = 0.f;
	_finalRotationTime = 1.f;

	_initialPosition = FVector(0.f, 0.f, 0.f);
	_finalPosition = FVector(0.f, 0.f, 0.f);
	_initialRotation = FVector(0.f, 0.f, 0.f);
	_finalRotation = FVector(0.f, 0.f, 0.f);

	_timelineValue = 0.f;
	_curveVectorLocValue = FVector(0.f, 0.f, 0.f);
	_curveVectorRotValue = FVector(0.f, 0.f, 0.f);
	_locationApplied = FVector(0.f, 0.f, 0.f);
	_rotationApplied = FVector(0.f, 0.f, 0.f);

	_interpolationMode = ERichCurveInterpMode::RCIM_Cubic;
}

void AVectorAnimatedItem::BeginPlay()
{
    Super::BeginPlay();

    _meshComponent->SetSimulatePhysics(false);
    _meshComponent->SetEnableGravity(false);

    if (_curveVectorLocation) {
        // Timeline
        _timelineCallback.BindUFunction(this, FName("ControlVectorAnimation"));
        _timelineFinishedCallback.BindUFunction(this, FName("SetState"));

        _timelineComponent->AddInterpVector(_curveVectorLocation, _timelineCallback);
        _timelineComponent->SetTimelineFinishedFunc(_timelineFinishedCallback);

		_curveVectorLocation->FloatCurves[0].UpdateOrAddKey(_initialLocationTime, _initialPosition.X);
		_curveVectorLocation->FloatCurves[1].UpdateOrAddKey(_initialLocationTime, _initialPosition.Y);
		_curveVectorLocation->FloatCurves[2].UpdateOrAddKey(_initialLocationTime, _initialPosition.Z);

		_curveVectorLocation->FloatCurves[0].UpdateOrAddKey(_finalLocationTime, _finalPosition.X);
		_curveVectorLocation->FloatCurves[1].UpdateOrAddKey(_finalLocationTime, _finalPosition.Y);
		_curveVectorLocation->FloatCurves[2].UpdateOrAddKey(_finalLocationTime, _finalPosition.Z);

		for (int i = 0; i < 3; i++) {
			FRichCurve& curveComponent = _curveVectorLocation->FloatCurves[i];

			for (auto& key : curveComponent.Keys) {
				key.InterpMode = _interpolationMode;
			}
		}
    }

	if (_curveVectorRotation) {
		// Timeline
		_timelineCallback.BindUFunction(this, FName("ControlVectorAnimation"));
		_timelineFinishedCallback.BindUFunction(this, FName("SetState"));

		_timelineComponent->AddInterpVector(_curveVectorRotation, _timelineCallback);
		_timelineComponent->SetTimelineFinishedFunc(_timelineFinishedCallback);

		_curveVectorRotation->FloatCurves[0].UpdateOrAddKey(_initialRotationTime, _initialRotation.X);
		_curveVectorRotation->FloatCurves[1].UpdateOrAddKey(_initialRotationTime, _initialRotation.Y);
		_curveVectorRotation->FloatCurves[2].UpdateOrAddKey(_initialRotationTime, _initialRotation.Z);

		_curveVectorRotation->FloatCurves[0].UpdateOrAddKey(_finalRotationTime, _finalRotation.X);
		_curveVectorRotation->FloatCurves[1].UpdateOrAddKey(_finalRotationTime, _finalRotation.Y);
		_curveVectorRotation->FloatCurves[2].UpdateOrAddKey(_finalRotationTime, _finalRotation.Z);

		for (int i = 0; i < 3; i++) {
			FRichCurve& curveComponent = _curveVectorRotation->FloatCurves[i];

			for (auto& key : curveComponent.Keys) {
				key.InterpMode = _interpolationMode;
			}
		}
	}
}

void AVectorAnimatedItem::Tick(float deltaTime)
{
    Super::Tick(deltaTime);

    _timelineComponent->TickComponent(deltaTime, ELevelTick::LEVELTICK_TimeOnly, NULL);
}

void AVectorAnimatedItem::ControlVectorAnimation(FVector value)
{
	_timelineValue = _timelineComponent->GetPlaybackPosition();

	_curveVectorLocValue = _curveVectorLocation->GetVectorValue(_timelineValue);
	newLocation = _curveVectorLocValue - _locationApplied;
	_locationApplied += newLocation;

	_curveVectorRotValue = _curveVectorRotation->GetVectorValue(_timelineValue);
	newRotation = _curveVectorRotValue - _rotationApplied;
	_rotationApplied += newRotation;

	if (_bLocation) {
		AddActorLocalOffset(newLocation);
	} 
	if (_bRotation) {
		AddActorLocalRotation(FRotator(newRotation.X, newRotation.Y, newRotation.Z));
	}
}

void AVectorAnimatedItem::SetState()
{
}

void AVectorAnimatedItem::UseInteraction(AItem* item)
{
	IInteractiveInterface::UseInteraction(item);

	if (!_bUseState) {
		_timelineComponent->Play();
		_soundComponent->SetIntParameter(_metasoundParameter, 0);
		_soundComponent->Play();
		_bUseState = true;
	} else {
		_timelineComponent->Reverse();
		_soundComponent->SetIntParameter(_metasoundParameter, 1);
		_soundComponent->Play();
		_bUseState = false;
	}
}