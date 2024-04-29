#include "Flashlight.h"
#include "Components/SpotLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstance.h" 
#include "../../Sophia.h"
#include "../../Core/GMS_MyGameStateBase.h"
#include "../../Macros.h"
#include "../KeysHUDWidget.h"
#include "Curves/CurveFloat.h"

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

	_curveFloat = CreateDefaultSubobject<UCurveFloat>(TEXT("CurveFloat"));
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

	if (_curveFloat) {
		_curveFloat->FloatCurve.UpdateOrAddKey(0.f, 0.f);
		_curveFloat->FloatCurve.UpdateOrAddKey(5.f, 1.f);

		FRichCurve& curveComponentRotation = _curveFloat->FloatCurve;
		for (auto& key : curveComponentRotation.Keys) {
			key.InterpMode = ERichCurveInterpMode::RCIM_Cubic;
		}

		_timelineCallback.BindUFunction(this, FName("ControlCrankHandle"));
		_timelineComponent->AddInterpFloat(_curveFloat, _timelineCallback);
	}
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

			if (_metaSound != nullptr && _soundComponent != nullptr)
				_soundComponent->SetSound(_metaSound);
			_soundComponent->SetIntParameter("Flashlight State", _bTurnedOn ? 0 : 1);
			_soundComponent->Play();
		}
	}
}

void AFlashlight::ControlCrankHandle(float value)
{
	_timelineValue = _timelineComponent->GetPlaybackPosition();
	_curveFloatValue = _curveFloat->GetFloatValue(_timelineValue);

	_owningInventory->_flashlightCrankComponent->AddLocalRotation(FRotator(-2.f, 0.f, 0.f));
	_owningInventory->_flashlightCrankHandleComponent->AddLocalRotation(FRotator(2.f, 0.f, 0.f));
}

void AFlashlight::RechargeFlashlight(const FInputActionValue& value)
{
	if (_owningInventory->_currentHandItem != nullptr) {
		if (_owningInventory->_currentHandItem->IsA<AFlashlight>()) {
			_bNoSwitchableItem = true;
			_bIsRechargingFlashlight = true;

			if (_rechargeFlashlightSound != nullptr && _soundComponent != nullptr) {
				_soundComponent->SetSound(_rechargeFlashlightSound);
				_soundComponent->SetBoolParameter("Is Pressing R", true);
			}
			_soundComponent->Play();
			_timelineComponent->PlayFromStart();
		}
	}
}

void AFlashlight::FinishRechargeFlashlight()
{
	if (_owningInventory->_currentHandItem != nullptr) {
		if (_owningInventory->_currentHandItem->IsA<AFlashlight>()) {
			_bNoSwitchableItem = false;
			_bIsRechargingFlashlight = false;
			_soundComponent->SetBoolParameter("Is Pressing R", false);
			_soundComponent->Stop();
			_timelineComponent->Stop();
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
			_rechargeFlashlightBindingHandle = &enhancedInputComponent->BindAction(_rechargeFlashlightAction, ETriggerEvent::Started, this, &AFlashlight::RechargeFlashlight);
			_rechargeFlashlightBindingHandle = &enhancedInputComponent->BindAction(_rechargeFlashlightAction, ETriggerEvent::Triggered, this, &AFlashlight::FinishRechargeFlashlight);
			_rechargeFlashlightBindingHandle = &enhancedInputComponent->BindAction(_rechargeFlashlightAction, ETriggerEvent::Canceled, this, &AFlashlight::FinishRechargeFlashlight);
		}
	}
}

void AFlashlight::DropItem(AItem* item)
{
}