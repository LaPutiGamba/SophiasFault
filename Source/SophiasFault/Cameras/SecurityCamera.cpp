#include "SecurityCamera.h"
#include "../Inventory/Items/CameraMonitor.h"

ASecurityCamera::ASecurityCamera()
{
}

void ASecurityCamera::UseInteraction()
{
	if (_myGameState->_onBlendTime <= 0.0f) {
		_playerController->SetViewTargetWithBlend(this, 0.75f);
		_myGameState->_onBlendTime = 0.75f;

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(_playerController->GetLocalPlayer())) {
			Subsystem->RemoveMappingContext(_mainMappingContext);
			Subsystem->AddMappingContext(_puzzleMappingContext, 0);
		}

		if (_enhancedInputComponent) {
			_getUpHandle = &_enhancedInputComponent->BindAction(_getUpAction, ETriggerEvent::Triggered, this, &ASecurityCamera::BlendBack);
			_clickInteractiveHandle = &_enhancedInputComponent->BindAction(_clickInteractiveAction, ETriggerEvent::Triggered, _cameraMonitor, &ACameraMonitor::ChangeSecurityCamera);
		}
	}
}

void ASecurityCamera::BlendBack()
{
	ACameraBlend::BlendBack();

	if (_enhancedInputComponent) {
		_enhancedInputComponent->RemoveBinding(*_getUpHandle);
		_enhancedInputComponent->RemoveBinding(*_clickInteractiveHandle);
	}
}