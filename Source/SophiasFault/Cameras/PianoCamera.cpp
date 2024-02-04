#include "PianoCamera.h"
#include "../Inventory/Items/PianoKey.h"
#include "../Macros.h"

#define ECC_Selectable ECC_GameTraceChannel1

APianoCamera::APianoCamera()
{
}

void APianoCamera::UseInteraction()
{
	if (_myGameState->_onBlendTime <= 0.0f) {
		_playerController->SetViewTargetWithBlend(this, 0.75f);
		_myGameState->_onBlendTime = 0.75f;
		_playerController->bShowMouseCursor = true;
		_playerController->bEnableClickEvents = true;
		_playerController->bEnableMouseOverEvents = true;

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(_playerController->GetLocalPlayer())) {
			Subsystem->RemoveMappingContext(_mainMappingContext);
			Subsystem->AddMappingContext(_puzzleMappingContext, 0);
		}

		if (_enhancedInputComponent) {
			_getUpHandle = &_enhancedInputComponent->BindAction(_getUpAction, ETriggerEvent::Triggered, this, &APianoCamera::BlendBack);
			_clickInteractiveHandle = &_enhancedInputComponent->BindAction(_clickInteractiveAction, ETriggerEvent::Triggered, this, &APianoCamera::ClickInteractive);
		}
	}
}

void APianoCamera::BlendBack()
{
	ACameraBlend::BlendBack();

	if (_enhancedInputComponent) {
		_enhancedInputComponent->RemoveBinding(*_getUpHandle);
		_enhancedInputComponent->RemoveBinding(*_clickInteractiveHandle);
	}
}

void APianoCamera::ClickInteractive(const FInputActionValue& value)
{
	FVector2D mousePosition;
	_playerController->GetMousePosition(mousePosition.X, mousePosition.Y);

	FVector worldLocation, worldDirection;
	_playerController->DeprojectScreenPositionToWorld(mousePosition.X, mousePosition.Y, worldLocation, worldDirection);

	FHitResult _hit;
	if (GetWorld()->LineTraceSingleByChannel(_hit, worldLocation, worldLocation + worldDirection * 350.f, ECC_Selectable, FCollisionQueryParams::DefaultQueryParam, FCollisionResponseParams::DefaultResponseParam)) {
		if (_hit.GetActor()->IsA<APianoKey>()) {
			IInteractiveInterface* mesh = Cast<IInteractiveInterface>(_hit.GetActor());
			mesh->UseInteraction();
		}
	}
}