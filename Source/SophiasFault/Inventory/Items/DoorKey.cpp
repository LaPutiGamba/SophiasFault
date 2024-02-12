#include "DoorKey.h"
#include "Curves/CurveVector.h" 
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Door.h"
#include "../../Sophia.h"
#include "../../Macros.h"

ADoorKey::ADoorKey()
{
	PrimaryActorTick.bCanEverTick = true;

	_keyID = 0;
}

void ADoorKey::BeginPlay()
{
	Super::BeginPlay();

	_meshComponent->SetSimulatePhysics(false);
	_meshComponent->SetEnableGravity(false);

	if (_curveVector) {
		// Timeline
		_timelineCallback.BindUFunction(this, FName("ControlKey"));
		_timelineFinishedCallback.BindUFunction(this, FName("SetState"));

		_timelineComponent->AddInterpVector(_curveVector, _timelineCallback);
		_timelineComponent->SetTimelineFinishedFunc(_timelineFinishedCallback);
	}

	_initialLocation = _meshComponent->GetComponentLocation();
}

void ADoorKey::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	_timelineComponent->TickComponent(deltaTime, ELevelTick::LEVELTICK_TimeOnly, NULL);
}

void ADoorKey::ControlKey(FVector value)
{
	_timelineValue = _timelineComponent->GetPlaybackPosition();
	_curveVectorValue = _curveVector->GetVectorValue(_timelineValue);

	FVector newLocation = _initialLocation + _curveVectorValue;
	_meshComponent->SetWorldLocation(newLocation);
}

void ADoorKey::SetState()
{
    PickUpItem(this);
}

void ADoorKey::OnAction()
{
	FHitResult hit;
	FVector start, forwardVector, end;
	if (_playerCamera != nullptr)
		start = _playerCamera->GetComponentLocation();
	if (_playerCamera != nullptr)
		forwardVector = _playerCamera->GetForwardVector();
	end = ((forwardVector * 200.f) + start);

	if (GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility, FComponentQueryParams::DefaultQueryParam, FCollisionResponseParams::DefaultResponseParam)) {
		if (ADoor* door = Cast<ADoor>(hit.GetActor())) {
			door->UseInteraction();
			door->_doorOpened = true;
			
			Cast<ASophia>(GetWorld()->GetFirstPlayerController()->GetPawn())->GetInventory()->RemoveItem(this);
			Destroy();
		}
	}
}
