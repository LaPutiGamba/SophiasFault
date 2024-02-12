#include "MirrorLight.h"
#include "Curves/CurveVector.h" 
#include "Camera/CameraComponent.h"
#include "./MirrorSolution.h"
#include "../../Core/GMS_MyGameStateBase.h"
#include "../../Macros.h"

#define ECC_Selectable ECC_GameTraceChannel1

AMirrorLight::AMirrorLight()
{
	PrimaryActorTick.bCanEverTick = true;

	_mirrorLightID = 0;

	_curveVector = CreateDefaultSubobject<UCurveVector>(TEXT("CurveVector"));
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

void AMirrorLight::PickUpItem(AItem* item)
{
	IPickUpInterface::PickUpItem(item);

	if (_myGameState != nullptr) {
		for (int i = 0; i < _myGameState->GetPositionedMirrorLights()->Num(); i++) {
			if (_myGameState->GetPositionedMirrorLights()->IsValidIndex(i)) {
				if (_myGameState->GetPositionedMirrorLights()->operator[](i) == _mirrorLightID) {
					_myGameState->GetPositionedMirrorLights()->operator[](i) = 0;

					if (_mirrorLightPositioned != nullptr) {
						_mirrorLightPositioned->SetActorEnableCollision(true);
						_mirrorLightPositioned = nullptr;
					}
					break;
				}
			}
		}
	}
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
	_curveVector->FloatCurves[0].UpdateOrAddKey(10.f, placeablePosition.X);
	_curveVector->FloatCurves[1].UpdateOrAddKey(10.f, placeablePosition.Y);
	_curveVector->FloatCurves[2].UpdateOrAddKey(10.f, placeablePosition.Z);

	// Activate the cubic interpolation mode
	for (int i = 0; i < 3; i++) {
		FRichCurve& curveComponent = _curveVector->FloatCurves[i];

		for (auto& key : curveComponent.Keys) {
			key.InterpMode = ERichCurveInterpMode::RCIM_Cubic;
		}
	}

	_meshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	_owningInventory->RemoveItem(this);
	_timelineComponent->PlayFromStart();
}

void AMirrorLight::OnAction()
{
	if (_timelineComponent->IsPlaying())
		return;

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
				_mirrorLightPositioned->SetActorEnableCollision(false);

				PositionLight(_mirrorLightPositioned->GetActorLocation());

				if (_myGameState != nullptr) {
					for (int i = 0; i < _myGameState->GetPositionedMirrorLights()->Num(); i++) {
						FName tag = FName(*FString::Printf(TEXT("MirrorLightPosition%d"), i + 1));
						printFName(tag);
						if (_mirrorLightPositioned->ActorHasTag(tag)) {
							_myGameState->GetPositionedMirrorLights()->operator[](i) = _mirrorLightID;
							break;
						}
					}
				}

				_owningInventory->_currentHandItem = nullptr;
			}
		} else {
			_mirrorLightPositioned = nullptr;
		}
	}
}