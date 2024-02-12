#include "FloatAnimatedItem.h"
#include "Curves/CurveFloat.h" 

AFloatAnimatedItem::AFloatAnimatedItem()
{
    PrimaryActorTick.bCanEverTick = true;

    _curveFloat = CreateDefaultSubobject<UCurveFloat>(TEXT("CurveFloat"));

	_bIsActivated = false;

    _bLocationOrRotation = false;

    _initialTime = 0.f;
    _finalTime = 1.f;

    _initialPosition = 0.f;
    _finalPosition = 1.f;

    _XorYorZ = 1;

    _curveFloatValue = 0.f;
    _timelineValue = 0.f;
}

void AFloatAnimatedItem::BeginPlay()
{
	Super::BeginPlay();

	_meshComponent->SetSimulatePhysics(false);
	_meshComponent->SetEnableGravity(false);

    if (_curveFloat) {
        // Timeline
        _timelineCallback.BindUFunction(this, FName("ControlFloatAnimation"));
        _timelineFinishedCallback.BindUFunction(this, FName("SetState"));

        _timelineComponent->AddInterpFloat(_curveFloat, _timelineCallback);
        _timelineComponent->SetTimelineFinishedFunc(_timelineFinishedCallback);

        _curveFloat->FloatCurve.UpdateOrAddKey(_initialTime, _initialPosition);
        _curveFloat->FloatCurve.UpdateOrAddKey(_finalTime, _finalPosition);
        FRichCurve& curveComponent = _curveFloat->FloatCurve;

        for (auto& key : curveComponent.Keys) {
            key.InterpMode = ERichCurveInterpMode::RCIM_Cubic;
        }
    }
}

void AFloatAnimatedItem::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	_timelineComponent->TickComponent(deltaTime, ELevelTick::LEVELTICK_TimeOnly, NULL);
}

void AFloatAnimatedItem::ControlFloatAnimation(float value)
{
	_timelineValue = _timelineComponent->GetPlaybackPosition();
	_curveFloatValue = _curveFloat->GetFloatValue(_timelineValue);

	if (_bLocationOrRotation) {
		FVector newLocation = GetActorLocation();
		
		switch (_XorYorZ) {
		case 1:
			newLocation.X = _curveFloatValue;
			break;
		case 2:
			newLocation.Y = _curveFloatValue;
			break;
		case 3:
			newLocation.Z = _curveFloatValue;
			break;
		}

		SetActorLocation(newLocation);
	} else {
		FRotator newRotation = GetActorRotation();

		switch (_XorYorZ) {
		case 1:
			newRotation.Roll = _curveFloatValue;
			break;
		case 2:
			newRotation.Pitch = _curveFloatValue;
			break;
		case 3:
			newRotation.Yaw = _curveFloatValue;
			break;
		}

		SetActorRotation(newRotation);
	}
}

void AFloatAnimatedItem::SetState()
{
	_bIsActivated = true;
}

void AFloatAnimatedItem::UseInteraction()
{
	if (!_bIsActivated)
		_timelineComponent->PlayFromStart();
}