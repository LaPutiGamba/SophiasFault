#include "PianoCamera.h"
#include "../Macros.h"

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
			_bindingHandle = &_enhancedInputComponent->BindAction(_getUpAction, ETriggerEvent::Triggered, this, &APianoCamera::BlendBack);
			if (_bindingHandle) {
				printText("Binding handle is valid");
			}
			else {
				printText("Binding handle is not valid");
			}
		}
	}
}

void APianoCamera::BlendBack()
{
	ACameraBlend::BlendBack();

	if (_enhancedInputComponent) {
		_enhancedInputComponent->RemoveBinding(*_bindingHandle);
	}
}