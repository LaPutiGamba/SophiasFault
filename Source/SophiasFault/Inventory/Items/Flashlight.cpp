#include "Flashlight.h"
#include "Components/SpotLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../../Macros.h"

AFlashlight::AFlashlight()
{
	_flashlight = nullptr;
	_flashlightOn = false;
	_rechargingFlashlight = false;
	_flashlightTimer = 0.f;
	_flashlightMaxDuration = 10.f;
	_flashlightStatus = ST_LIGHTOFF;
	PrimaryActorTick.bCanEverTick = true;
}

void AFlashlight::BeginPlay()
{
	Super::BeginPlay();

	_flashlight = AFlashlight::GetComponentByClass<USpotLightComponent>();
	if (_flashlight != nullptr)
		_flashlight->ToggleVisibility(false);

	_playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

void AFlashlight::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	switch (_flashlightStatus) {
	case ST_LIGHTON:
		if (_flashlight != nullptr)
			_flashlight->SetVisibility(true);

		if (_flashlightTimer < _flashlightMaxDuration)
			_flashlightTimer += deltaTime;
		else
			_flashlightStatus = ST_NEEDRECHARGE;

		if (!_flashlightOn)
			_flashlightStatus = ST_LIGHTOFF;

		_rechargingFlashlight = false;
		break;
	case ST_LIGHTOFF:
		if (_flashlight != nullptr)
			_flashlight->SetVisibility(false);

		if (_flashlightTimer > 0.f)
			_flashlightTimer -= deltaTime;

		if (_flashlightOn)
			_flashlightStatus = ST_LIGHTON;
		break;
	case ST_NEEDRECHARGE:
		_flashlightTimer = 0.f;

		if (_flashlight != nullptr)
			_flashlight->SetVisibility(false);

		if (_rechargingFlashlight) {
			if (_flashlightOn)
				_flashlightStatus = ST_LIGHTON;
			else
				_flashlightStatus = ST_LIGHTOFF;
		}
		break;
	default:
		break;
	}
}

void AFlashlight::UseFlashlight(const FInputActionValue& value)
{
	if (_owningInventory->_flashlightItem != nullptr)
		ToggleFlashlightOn();
}

void AFlashlight::RechargeFlashlight(const FInputActionValue& value)
{
	if (_owningInventory->_flashlightItem != nullptr)
		ToggleRechargingFlashlight();
}

void AFlashlight::PickUpItem(AItem* item)
{
	IPickUpInterface::PickUpItem(item);

	if (UEnhancedInputComponent* enhancedInputComponent = CastChecked<UEnhancedInputComponent>(_playerController->InputComponent)) {
		_flashlightBindingHandle = &enhancedInputComponent->BindAction(_flashlightAction, ETriggerEvent::Triggered, this, &AFlashlight::UseFlashlight);
		_rechargeFlashlightBindingHandle = &enhancedInputComponent->BindAction(_rechargeFlashlightAction, ETriggerEvent::Triggered, this, &AFlashlight::RechargeFlashlight);
	}
}

void AFlashlight::DropItem(AItem* item, UCameraComponent* cameraComponent)
{
	IPickUpInterface::DropItem(item, cameraComponent);

	if (UEnhancedInputComponent* enhancedInputComponent = CastChecked<UEnhancedInputComponent>(_playerController->InputComponent)) {
		enhancedInputComponent->RemoveBinding(*_flashlightBindingHandle);
		enhancedInputComponent->RemoveBinding(*_rechargeFlashlightBindingHandle);
	}
}