#include "EarthBallCamera.h"
#include "../Sophia.h"
#include "../Inventory/InventoryComponent.h"
#include "../Inventory/Items/Earth/EarthContinent.h"
#include "../Interfaces/ActorBlendInterface.h"
#include "../Interfaces/InteractiveInterface.h"
#include "../Inventory/Items/BasicBlendActor.h"
#include "Components/BoxComponent.h"
#include "EnhancedInputComponent.h"
#include "../Macros.h"

#define ECC_Selectable ECC_GameTraceChannel1

AEarthBallCamera::AEarthBallCamera()
{

}

void AEarthBallCamera::UseInteraction(ACameraBlend* item)
{
	if (_myGameState->_onBlendTime <= 0.001f && !_bPuzzleSolved) {
		ACameraBlend::UseInteraction(item);

		// Blend of the character camera to the Earth Ball camera.
		_playerController->SetViewTargetWithBlend(this, 0.75f);
		_myGameState->_onBlendTime = 0.75f;
		_playerController->bShowMouseCursor = true;
		_playerController->bEnableClickEvents = true;
		_playerController->bEnableMouseOverEvents = true;

		UActorComponent* boxComponent;
		if (_basicBlendActor) {
			if ((boxComponent = _basicBlendActor->GetComponentByClass(UBoxComponent::StaticClass())) != nullptr)
				Cast<UBoxComponent>(boxComponent)->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		if (_enhancedInputComponent) {
			_getUpHandle = &_enhancedInputComponent->BindAction(_getUpAction, ETriggerEvent::Triggered, this, &AEarthBallCamera::BlendBack);
			_clickInteractiveHandle = &_enhancedInputComponent->BindAction(_clickInteractiveAction, ETriggerEvent::Triggered, this, &AEarthBallCamera::ClickInteractive);
			_clickRotationHandle = &_enhancedInputComponent->BindAction(_clickRotationAction, ETriggerEvent::Triggered, this, &AEarthBallCamera::EarthRotation);
		}
	}
}

void AEarthBallCamera::BlendBack()
{
	ACameraBlend::BlendBack();

	if (_enhancedInputComponent) {
		_enhancedInputComponent->RemoveBinding(*_clickInteractiveHandle);
		_enhancedInputComponent->RemoveBinding(*_clickRotationHandle);
	}

	FVector2D screenPosition;
	FVector worldLocation;
	_playerController->ProjectWorldLocationToScreen(worldLocation, screenPosition);

	UActorComponent* boxComponent;
	if (_basicBlendActor) {
		if ((boxComponent = _basicBlendActor->GetComponentByClass(UBoxComponent::StaticClass())) != nullptr)
			Cast<UBoxComponent>(boxComponent)->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void AEarthBallCamera::EarthRotation(const FInputActionValue& value)
{
	if (UInventoryComponent* inventory = Cast<ASophia>(_playerController->GetPawn())->GetInventory()) {
		if (_basicBlendActor) {
			FRotator newRotation = _basicBlendActor->GetActorRotation();

			if (_playerController->PlayerInput->IsPressed(EKeys::A)) 
				newRotation.Yaw -= 1.5f;
			else if (_playerController->PlayerInput->IsPressed(EKeys::D))
				newRotation.Yaw += 1.5f;

			_basicBlendActor->SetActorRotation(newRotation);
		}
	}
}

void AEarthBallCamera::ClickInteractive(const FInputActionValue& value)
{
	FVector2D mousePosition;
	_playerController->GetMousePosition(mousePosition.X, mousePosition.Y);

	FVector worldLocation, worldDirection;
	_playerController->DeprojectScreenPositionToWorld(mousePosition.X, mousePosition.Y, worldLocation, worldDirection);

	FHitResult _hit;
	if (GetWorld()->LineTraceSingleByChannel(_hit, worldLocation, worldLocation + worldDirection * 350.f, ECC_Selectable, FCollisionQueryParams::DefaultQueryParam, FCollisionResponseParams::DefaultResponseParam)) {
		if (_hit.GetActor()->IsA<AEarthContinent>()) {
			AEarthContinent* mesh = Cast<AEarthContinent>(_hit.GetActor());
			mesh->AddOrRemoveContinent();
		}
	}
}