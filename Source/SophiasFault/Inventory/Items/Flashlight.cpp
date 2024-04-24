#include "Flashlight.h"
#include "Components/SpotLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstance.h" 
#include "../../Sophia.h"
#include "../../Core/GMS_MyGameStateBase.h"
#include "../../Macros.h"
#include "../KeysHUDWidget.h"

AFlashlight::AFlashlight()
{
	PrimaryActorTick.bCanEverTick = true;

	_flashlight = nullptr;
	_flashlightIntensity = 0.f;
	_flashlightIntensityMax = 5000.0f;
	_bTurnedOn = false;
	_bIsRechargingFlashlight = false;
	_noSwitchableItemSpeed = 0.1f;
	_flashlightTimer = 0.f;
	_flashlightMaxDuration = 600.f;
	_flashlightState = ST_LIGHTOFF;
}

void AFlashlight::BeginPlay()
{
    Super::BeginPlay();

    _playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	_sophia = Cast<ASophia>(_playerController->GetPawn());

    _flashlight = _sophia->GetFlashlightComponent();
	_flashlightIntensityMax = _flashlight->Intensity;
	_flashlightIntensity = (_flashlight->Intensity - 2000.f) / (_flashlightMaxDuration * 0.8);
	_flashlightTimer = _flashlightMaxDuration;

    if (_flashlight != nullptr)
        _flashlight->ToggleVisibility(false);
}

void AFlashlight::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	switch (_flashlightState) {
	case ST_LIGHTON:
		if (_flashlightTimer > 0.f) {
			_flashlightTimer -= deltaTime;
		} else {
			_flashlightState = ST_NEEDRECHARGE;
			_flashlight->SetVisibility(false);
			_flashlightTimer = _flashlightMaxDuration;
			GetWorld()->GetTimerManager().ClearTimer(_intensityTimerHandle);
		}

		if (!_bTurnedOn) {
			_flashlightState = ST_LIGHTOFF;
			_flashlight->SetVisibility(false);
			GetWorld()->GetTimerManager().ClearTimer(_intensityTimerHandle);
		}

		if (_flashlightTimer < (_flashlightMaxDuration * 0.025))
			_flashlight->SetLightFunctionMaterial(_flickerMaterial);

		if (_bIsRechargingFlashlight) {
			_flashlightState = ST_NEEDRECHARGE;
			GetWorld()->GetTimerManager().ClearTimer(_intensityTimerHandle);
		}
		break;
	case ST_LIGHTOFF:
		if (_bTurnedOn) {
			_flashlightState = ST_LIGHTON;

			if (_flashlightTimer < (_flashlightMaxDuration * 0.025))
				_flashlight->SetLightFunctionMaterial(_flickerMaterial);
			else
			_flashlight->SetLightFunctionMaterial(_flashlightMaterial);
			
			_flashlight->SetVisibility(true);

			GetWorld()->GetTimerManager().SetTimer(_intensityTimerHandle, [this]() {
				_flashlight->SetIntensity(_flashlight->Intensity - _flashlightIntensity);
			}, 1.f, true); 
		}

		if (_bIsRechargingFlashlight)
			_flashlightState = ST_NEEDRECHARGE;
		break;
	case ST_NEEDRECHARGE:
		if (_bIsRechargingFlashlight) {
			_bTurnedOn = true;
			_flashlightState = ST_LIGHTON;
			_flashlight->SetLightFunctionMaterial(_flashlightMaterial);
			_flashlight->SetVisibility(true);

			_bIsRechargingFlashlight = false;

			_flashlight->SetIntensity(_flashlightIntensityMax);
			GetWorld()->GetTimerManager().SetTimer(_intensityTimerHandle, [this]() {
				_flashlight->SetIntensity(_flashlight->Intensity - _flashlightIntensity);
			}, 1.f, true); 
		}
		break;
	default:
		break;
	}
}

void AFlashlight::ToggleFlashlightOn()
{
	_bTurnedOn = !_bTurnedOn;
	_flashlight->ToggleVisibility();
}

void AFlashlight::UseFlashlight(const FInputActionValue& value)
{
	if (_owningInventory->_currentHandItem != nullptr) {
		if (_owningInventory->_currentHandItem->IsA<AFlashlight>()) {
			ToggleFlashlightOn();
			_soundComponent->SetIntParameter("Flashlight State", _bTurnedOn ? 0 : 1);
			_soundComponent->Play();
		}
	}
}

void AFlashlight::RechargeFlashlight(const FInputActionValue& value)
{
	bool boolValue = value.Get<bool>();

	if (_owningInventory->_currentHandItem != nullptr) {
		if (_owningInventory->_currentHandItem->IsA<AFlashlight>()) {
			if (boolValue)
				_bNoSwitchableItem = true;
			else
				_bNoSwitchableItem = false;

			_bIsRechargingFlashlight = true;
			_soundComponent->SetIntParameter("Flashlight State", 2);
			_soundComponent->Play();
		}
	}
}

void AFlashlight::PickUpItem(AItem* item)
{
	if (_myGameState->_bStairPositioned) {
		IPickUpInterface::PickUpItem(this);

		_myGameState->_keysHudWidget->AddKeyToHorizontalBox(FText::FromString("F"), FText::FromString("Toggle flashlight"), true, UKeysHUDHelper::EKeysHUDHelperImage::KeyboardKey);
		_myGameState->_keysHudWidget->AddKeyToHorizontalBox(FText::FromString("R"), FText::FromString("Recharge flashlight"), false, UKeysHUDHelper::EKeysHUDHelperImage::KeyboardKey);
		_myGameState->_keysHudWidget->AddKeyToHorizontalBox(FText::FromString("Q"), FText::FromString("Inspect item"), false, UKeysHUDHelper::EKeysHUDHelperImage::KeyboardKey);
		_myGameState->_keysHudWidget->ShowHUD(10.f);

		if (UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(_playerController->InputComponent)) {
			_flashlightBindingHandle = &enhancedInputComponent->BindAction(_flashlightAction, ETriggerEvent::Triggered, this, &AFlashlight::UseFlashlight);
			_rechargeFlashlightBindingHandle = &enhancedInputComponent->BindAction(_rechargeFlashlightAction, ETriggerEvent::Triggered, this, &AFlashlight::RechargeFlashlight);
		}
	}
}

void AFlashlight::DropItem(AItem* item)
{
}