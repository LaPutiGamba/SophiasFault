#include "SecurityCamera.h"
#include "../Inventory/Items/Security Cameras/CameraMonitor.h"

ASecurityCamera::ASecurityCamera()
{
}

void ASecurityCamera::UseInteraction(ACameraBlend* item)
{
	if (_myGameState->_onBlendTime <= 0.001f) {
		ACameraBlend::UseInteraction(item);
		
		_playerController->SetViewTargetWithBlend(this, 0.75f);
		_myGameState->_onBlendTime = 0.75f;

		if (_enhancedInputComponent) {
			_getUpHandle = &_enhancedInputComponent->BindAction(_getUpAction, ETriggerEvent::Triggered, this, &ASecurityCamera::BlendBack);
			_clickInteractiveHandle = &_enhancedInputComponent->BindAction(_clickInteractiveAction, ETriggerEvent::Triggered, _cameraMonitor, &ACameraMonitor::ChangeSecurityCamera);
		}
	}
}

void ASecurityCamera::BlendBack()
{
	ACameraBlend::BlendBack();

	if (_enhancedInputComponent)
		_enhancedInputComponent->RemoveBinding(*_clickInteractiveHandle);
}