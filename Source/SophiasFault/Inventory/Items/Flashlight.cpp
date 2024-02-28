#include "Flashlight.h"
#include "Components/SpotLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../../Sophia.h"
#include "../../Core/GMS_MyGameStateBase.h"
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

	_playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	_flashlight = Cast<ASophia>(_playerController->GetPawn())->GetFlashlightComponent();
	_flashlight->SetRelativeLocation(FVector(0.f, 5.f, 0.f));
	_flashlight->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	if (_flashlight != nullptr)
		_flashlight->ToggleVisibility(false);
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
	if (_owningInventory->_currentHandItem != nullptr)
		if (_owningInventory->_currentHandItem->IsA<AFlashlight>())
			ToggleFlashlightOn();
}

void AFlashlight::RechargeFlashlight(const FInputActionValue& value)
{
	if (_owningInventory->_currentHandItem != nullptr)
		if (_owningInventory->_currentHandItem->IsA<AFlashlight>())
			ToggleRechargingFlashlight();
}

void AFlashlight::PickUpItem(AItem* item)
{
	if (_myGameState->_bStairPositioned) {
		IPickUpInterface::PickUpItem(this);

		if (UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(_playerController->InputComponent)) {
			_flashlightBindingHandle = &enhancedInputComponent->BindAction(_flashlightAction, ETriggerEvent::Triggered, this, &AFlashlight::UseFlashlight);
			_rechargeFlashlightBindingHandle = &enhancedInputComponent->BindAction(_rechargeFlashlightAction, ETriggerEvent::Triggered, this, &AFlashlight::RechargeFlashlight);
		}
	}
}

void AFlashlight::DropItem(AItem* item)
{
}