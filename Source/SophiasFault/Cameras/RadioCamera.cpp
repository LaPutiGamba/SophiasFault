#include "RadioCamera.h"
#include "../Inventory/Items/Radio/FrecuencyMeter.h"
#include "../Inventory/Items/Radio/Radio.h"
#include "../Inventory/KeysHUDWidget.h"

ARadioCamera::ARadioCamera()
{
}

void ARadioCamera::UseInteraction(ACameraBlend* item)
{
	if (_myGameState->_onBlendTime <= 0.001f) {
		ACameraBlend::UseInteraction(item);
	
		_playerController->SetViewTargetWithBlend(this, 0.75f);
		_myGameState->_onBlendTime = 0.75f;

		_myGameState->_keysHudWidget->AddKeyToHorizontalBox(FText::FromString("W"), FText::FromString("Volume +"), true);
		_myGameState->_keysHudWidget->AddKeyToHorizontalBox(FText::FromString("S"), FText::FromString("Volume -"));
		_myGameState->_keysHudWidget->AddKeyToHorizontalBox(FText::GetEmpty(), FText::FromString("Turn On"), false, UKeysHUDHelper::EKeysHUDHelperImage::MouseLeft);
		_myGameState->_keysHudWidget->AddKeyToHorizontalBox(FText::GetEmpty(), FText::FromString("Frequency +"), false, UKeysHUDHelper::EKeysHUDHelperImage::MouseWheel);
		_myGameState->_keysHudWidget->AddKeyToHorizontalBox(FText::GetEmpty(), FText::FromString("Frequency -"), false, UKeysHUDHelper::EKeysHUDHelperImage::MouseWheel);
		_myGameState->_keysHudWidget->ShowHUD();

		if (_enhancedInputComponent) {
			_getUpHandle = &_enhancedInputComponent->BindAction(_getUpAction, ETriggerEvent::Triggered, this, &ARadioCamera::BlendBack);
			_mouseAxisHandle = &_enhancedInputComponent->BindAction(_mouseAxisAction, ETriggerEvent::Triggered, _frecuencyMeter, &AFrecuencyMeter::ChangeFrequency);
			_turnOffHandle = &_enhancedInputComponent->BindAction(_turnOffAction, ETriggerEvent::Triggered, _radio, &ARadio::TurnOffRadio);
			_changeVolumeHandle = &_enhancedInputComponent->BindAction(_changeVolumeAction, ETriggerEvent::Triggered, _radio, &ARadio::ChangeVolume);
		}
	}
}

void ARadioCamera::BlendBack()
{
	ACameraBlend::BlendBack();

	if (_enhancedInputComponent) {
		_enhancedInputComponent->RemoveBinding(*_mouseAxisHandle);
		_enhancedInputComponent->RemoveBinding(*_turnOffHandle);
		_enhancedInputComponent->RemoveBinding(*_changeVolumeHandle);
	}

	if (_myGameState->_keysHudWidget != nullptr) {
		_myGameState->_keysHudWidget->RemoveFromParent();
	}
}