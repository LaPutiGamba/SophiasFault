#include "EarthBallCamera.h"
#include "../Sophia.h"
#include "../Inventory/InventoryComponent.h"
#include "../Inventory/Items/EarthContinent.h"
#include "../Inventory/Items/ActorBlendCamera.h"
#include "../Interfaces/InteractiveInterface.h"
#include "EnhancedInputComponent.h"
#include "../Macros.h"

#define ECC_Selectable ECC_GameTraceChannel1

AEarthBallCamera::AEarthBallCamera()
{

}

void AEarthBallCamera::UseInteraction()
{
	if (_myGameState->_onBlendTime <= 0.0f) {
		// Calculate the location and direction of the camera to center the sphere.
		FVector playerViewDirection = _playerController->GetControlRotation().Vector();
		float distanceBehindSphere = 220.0f;
		FVector newCameraPosition = _earthBallLocation - (playerViewDirection * distanceBehindSphere);
		SetActorLocation(newCameraPosition);
		FVector LookAtDirection = (_earthBallLocation - GetActorLocation()).GetSafeNormal();
		FRotator NewCameraRotation = LookAtDirection.Rotation();
		SetActorRotation(NewCameraRotation);

		// Blend of the character camera to the Earth Ball camera.
		_playerController->SetViewTargetWithBlend(this, 0.75f);
		_myGameState->_onBlendTime = 0.75f;
		_playerController->bShowMouseCursor = true;
		_playerController->bEnableClickEvents = true;
		_playerController->bEnableMouseOverEvents = true;

		ACameraBlend::UseInteraction();

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
		_enhancedInputComponent->RemoveBinding(*_getUpHandle);
		_enhancedInputComponent->RemoveBinding(*_clickInteractiveHandle);
		_enhancedInputComponent->RemoveBinding(*_clickRotationHandle);
	}

	FVector2D screenPosition;
	FVector worldLocation;
	_playerController->ProjectWorldLocationToScreen(worldLocation, screenPosition);
}

void AEarthBallCamera::EarthRotation(const FInputActionValue& value)
{
	FVector2D mousePosition;
	_playerController->GetMousePosition(mousePosition.X, mousePosition.Y);
	FVector2D screenSize;
	GEngine->GameViewport->GetViewportSize(screenSize);

	FRotator sendRotator = FRotator::ZeroRotator;

	if (((mousePosition.X - (screenSize.X / 2)) > 30.f || (mousePosition.X - (screenSize.X / 2)) < -30.f) ||
		((mousePosition.Y - (screenSize.Y / 2)) > 30.f || (mousePosition.Y - (screenSize.Y / 2)) < -30.f)) {
		sendRotator.Yaw = -((mousePosition.X - (screenSize.X / 2)) / screenSize.X) * 1.25f;
		sendRotator.Pitch = ((mousePosition.Y - (screenSize.Y / 2)) / screenSize.Y) * 1.25f;
	}

	if (UInventoryComponent* inventory = Cast<ASophia>(_playerController->GetPawn())->GetInventory())
		if (inventory->_currentChangeCameraItem != nullptr)
			inventory->_currentChangeCameraItem->SetActorRelativeRotation(inventory->_currentChangeCameraItem->GetActorRotation() + sendRotator);
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
			IInteractiveInterface* mesh = Cast<IInteractiveInterface>(_hit.GetActor());
			mesh->UseInteraction();
		}
	}
}