#include "FlashlightCrystal.h"
#include "Flashlight.h"
#include "Components/SpotLightComponent.h"

AFlashlightCrystal::AFlashlightCrystal()
{
	_crystalColor = "Blue";
	_flashlight = nullptr;
}

void AFlashlightCrystal::OnAction()
{
	if (_flashlight != nullptr) {
		if (_crystalColor == "Blue") {
			_flashlight->_flashlightComponent->LightColor = FColor(0, 0, 255, 255);
		} else if (_crystalColor == "Red") {
			_flashlight->_flashlightComponent->LightColor = FColor(255, 0, 0, 255);
		} else if (_crystalColor == "Yellow") {
			_flashlight->_flashlightComponent->LightColor = FColor(255, 255, 0, 255);
		} else if (_crystalColor == "Green") {
			_flashlight->_flashlightComponent->LightColor = FColor(0, 255, 0, 255);
		}
	}
}