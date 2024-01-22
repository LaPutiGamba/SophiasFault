#include "MirrorLight.h"
#include "Curves/CurveVector.h" 
#include "Camera/CameraComponent.h"
#include "../../Core/GMS_MyGameStateBase.h"
#include "../../Macros.h"

#define ECC_Selectable ECC_GameTraceChannel1

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

void AMirrorLight::OnAction()
{
	FHitResult hit;
	FVector start, forwardVector, end;
	if (_playerCamera != nullptr)
		start = _playerCamera->GetComponentLocation();
	if (_playerCamera != nullptr)
		forwardVector = _playerCamera->GetForwardVector();
	end = ((forwardVector * 200.f) + start);

	if (GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Selectable, FComponentQueryParams::DefaultQueryParam, FCollisionResponseParams::DefaultResponseParam)) {
		if (hit.GetActor()->ActorHasTag("MirrorLightPosition")) {
			_mirrorLightPositioned = hit.GetActor();

			if (_mirrorLightPositioned != nullptr) {
				PositionLight(_mirrorLightPositioned->GetActorLocation());
				_owningInventory->RemoveItem(this);

				if (_myGameState != nullptr) {
					if (_myGameState->GetPositionedMirrorLights()->Num() == 0) {
						_myGameState->GetPositionedMirrorLights()->Add(_mirrorLightID);
					}
					else {
						int value = _myGameState->GetPositionedMirrorLights()->Find(_mirrorLightID);
						if (value == -1)
							_myGameState->GetPositionedMirrorLights()->Add(_mirrorLightID);
						else
							_myGameState->GetPositionedMirrorLights()->Remove(_mirrorLightID);
					}

					if (_myGameState->GetPositionedMirrorLights()->Num() == 8)
						_myGameState->ActivateMirrorLightSolution();
				}

				_owningInventory->_currentHandItem = nullptr;
				_owningInventory->_bHoldingItem = !_owningInventory->_bHoldingItem;
			}
		} else {
			_mirrorLightPositioned = nullptr;
		}
	}
}