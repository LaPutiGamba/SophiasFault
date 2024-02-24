#include "RadioCamera.h"
#include "../Inventory/Items/Radio/FrecuencyMeter.h"
#include "../Inventory/Items/Radio/Radio.h"

ARadioCamera::ARadioCamera()
{
}

void ARadioCamera::UseInteraction()
{
	if (_myGameState->_onBlendTime <= 0.001f) {
		ACameraBlend::UseInteraction();
	
		_playerController->SetViewTargetWithBlend(this, 0.75f);
		_myGameState->_onBlendTime = 0.75f;

		if (_enhancedInputComponent) {
			_getUpHandle = &_enhancedInputComponent->BindAction(_getUpAction, ETriggerEvent::Triggered, this, &ARadioCamera::BlendBack);
			_mouseAxisHandle = &_enhancedInputComponent->BindAction(_mouseAxisAction, ETriggerEvent::Triggered, _frecuencyMeter, &AFrecuencyMeter::ChangeFrequency);
			_turnOffHandle = &_enhancedInputComponent->BindAction(_turnOffAction, ETriggerEvent::Triggered, _radio, &ARadio::TurnOffRadio);
		}
	}
}

void ARadioCamera::BlendBack()
{
	ACameraBlend::BlendBack();

	if (_enhancedInputComponent) {
		_enhancedInputComponent->RemoveBinding(*_mouseAxisHandle);
		_enhancedInputComponent->RemoveBinding(*_turnOffHandle);
	}
}