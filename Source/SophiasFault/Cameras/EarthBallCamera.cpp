#include "EarthBallCamera.h"
#include "../Sophia.h"
#include "../Inventory/InventoryComponent.h"
#include "EnhancedInputComponent.h"
#include "../Macros.h"

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

		if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(_playerController->GetLocalPlayer())) {
			subsystem->RemoveMappingContext(_mainMappingContext);
			subsystem->AddMappingContext(_puzzleMappingContext, 0);
		}

		if (_enhancedInputComponent) {
			_bindingHandle = &_enhancedInputComponent->BindAction(_getUpAction, ETriggerEvent::Triggered, this, &AEarthBallCamera::BlendBack);
		}
	}
}

void AEarthBallCamera::BlendBack()
{
	printText("MAYBE");
	ACameraBlend::BlendBack();

	if (_enhancedInputComponent) {
		_enhancedInputComponent->RemoveBinding(*_bindingHandle);
	}

	FVector2D screenPosition;
	FVector worldLocation;
	_playerController->ProjectWorldLocationToScreen(worldLocation, screenPosition);
}