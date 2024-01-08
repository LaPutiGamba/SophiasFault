#include "Flashlight.h"
#include "Components/SpotLightComponent.h"

AFlashlight::AFlashlight()
{
	_flashlight = nullptr;
	_flashlightOn = false;
	_rechargingFlashlight = false;
	_flashlightTimer = 0.f;
	_flashlightMaxDuration = 10.f;
	_flashlightStatus = LIGHTOFF;
	PrimaryActorTick.bCanEverTick = true;

}

void AFlashlight::BeginPlay()
{
	Super::BeginPlay();

	_flashlight = AFlashlight::GetComponentByClass<USpotLightComponent>();
	if (_flashlight != nullptr)
		_flashlight->ToggleVisibility(false);
}

void AFlashlight::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	switch (_flashlightStatus) {
	case LIGHTON:
		if (_flashlight != nullptr)
			_flashlight->SetVisibility(true);

		if (_flashlightTimer < _flashlightMaxDuration)
			_flashlightTimer += deltaTime;
		else
			_flashlightStatus = NEEDRECHARGE;

		if (!_flashlightOn)
			_flashlightStatus = LIGHTOFF;

		_rechargingFlashlight = false;
		break;
	case LIGHTOFF:
		if (_flashlight != nullptr)
			_flashlight->SetVisibility(false);

		if (_flashlightTimer > 0.f)
			_flashlightTimer -= deltaTime;

		if (_flashlightOn)
			_flashlightStatus = LIGHTON;
		break;
	case NEEDRECHARGE:
		_flashlightTimer = 0.f;

		if (_flashlight != nullptr)
			_flashlight->SetVisibility(false);

		if (_rechargingFlashlight) {
			if (_flashlightOn)
				_flashlightStatus = LIGHTON;
			else
				_flashlightStatus = LIGHTOFF;
		}
		break;
	default:
		break;
	}
}