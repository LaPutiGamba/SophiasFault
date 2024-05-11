#include "Flashlight.h"
#include "Components/SpotLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstance.h" 
#include "../../Sophia.h"
#include "../../Core/GMS_MyGameStateBase.h"
#include "../DialogueWidget.h"
#include "../../Macros.h"
#include "../KeysHUDWidget.h"
#include "Curves/CurveFloat.h"
#include "Materials/MaterialParameterCollection.h" 
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetMaterialLibrary.h"

AFlashlight::AFlashlight()
{
	PrimaryActorTick.bCanEverTick = true;

	_flashlightComponent = nullptr;
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

    _flashlightComponent = _sophia->GetFlashlightComponent();
	_flashlightIntensityMax = _flashlightComponent->Intensity;
	_flashlightIntensity = (_flashlightComponent->Intensity - 2000.f) / (_flashlightMaxDuration * 0.8);
	_flashlightTimer = _flashlightMaxDuration;

    if (_flashlightComponent != nullptr)
        _flashlightComponent->ToggleVisibility(false);

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
			_flashlightComponent->SetVisibility(false);
			_flashlightTimer = _flashlightMaxDuration;
			GetWorld()->GetTimerManager().ClearTimer(_intensityTimerHandle);
		}

		if (!_bTurnedOn) {
			_flashlightState = ST_LIGHTOFF;
			_flashlightComponent->SetVisibility(false);
			GetWorld()->GetTimerManager().ClearTimer(_intensityTimerHandle);
		}

		if (_flashlightTimer < (_flashlightMaxDuration * 0.025))
			_flashlightComponent->SetLightFunctionMaterial(_flickerMaterial);

		if (_bIsRechargingFlashlight) {
			_flashlightState = ST_NEEDRECHARGE;
			GetWorld()->GetTimerManager().ClearTimer(_intensityTimerHandle);
		}
		break;
	case ST_LIGHTOFF:
		if (_bTurnedOn) {
			_flashlightState = ST_LIGHTON;

			if (_flashlightTimer < (_flashlightMaxDuration * 0.025))
				_flashlightComponent->SetLightFunctionMaterial(_flickerMaterial);
			else
			_flashlightComponent->SetLightFunctionMaterial(_flashlightMaterial);
			
			_flashlightComponent->SetVisibility(true);

			GetWorld()->GetTimerManager().SetTimer(_intensityTimerHandle, [this]() {
				_flashlightComponent->SetIntensity(_flashlightComponent->Intensity - _flashlightIntensity);
			}, 1.f, true); 
		}

		if (_bIsRechargingFlashlight)
			_flashlightState = ST_NEEDRECHARGE;
		break;
	case ST_NEEDRECHARGE:
		if (_bIsRechargingFlashlight) {
			_bTurnedOn = true;
			_flashlightState = ST_LIGHTON;
			_flashlightComponent->SetLightFunctionMaterial(_flashlightMaterial);
			_flashlightComponent->SetVisibility(true);

			_bIsRechargingFlashlight = false;

			_flashlightComponent->SetIntensity(_flashlightIntensityMax);
			GetWorld()->GetTimerManager().SetTimer(_intensityTimerHandle, [this]() {
				_flashlightComponent->SetIntensity(_flashlightComponent->Intensity - _flashlightIntensity);
			}, 1.f, true); 
		}
		break;
	default:
		break;
	} 

	if (_bTurnedOn && _flashlightComponent->LightColor != FColor(255, 255, 255, 255)) {
		FVector locationVector = _flashlightComponent->GetComponentLocation();
		FVector forwardVector = _flashlightComponent->GetForwardVector();
		forwardVector = -forwardVector;

		if (_flashlightComponent->LightColor == FColor(0, 0, 255, 255)) {
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("BlueLocation"), UKismetMathLibrary::Conv_VectorToLinearColor(locationVector));
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("BlueDirection"), UKismetMathLibrary::Conv_VectorToLinearColor(forwardVector));
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("RedLocation"), FLinearColor(0.f, 0.f, 0.f, 0.f));
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("RedDirection"), FLinearColor(0.f, 0.f, 0.f, 0.f));
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("YellowLocation"), FLinearColor(0.f, 0.f, 0.f, 0.f));
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("YellowDirection"), FLinearColor(0.f, 0.f, 0.f, 0.f));
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("GreenLocation"), FLinearColor(0.f, 0.f, 0.f, 0.f));
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("GreenDirection"), FLinearColor(0.f, 0.f, 0.f, 0.f));
		} else if (_flashlightComponent->LightColor == FColor(255, 0, 0, 255)) {
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("RedLocation"), UKismetMathLibrary::Conv_VectorToLinearColor(locationVector));
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("RedDirection"), UKismetMathLibrary::Conv_VectorToLinearColor(forwardVector));
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("BlueLocation"), FLinearColor(0.f, 0.f, 0.f, 0.f));
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("BlueDirection"), FLinearColor(0.f, 0.f, 0.f, 0.f));
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("YellowLocation"), FLinearColor(0.f, 0.f, 0.f, 0.f));
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("YellowDirection"), FLinearColor(0.f, 0.f, 0.f, 0.f));
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("GreenLocation"), FLinearColor(0.f, 0.f, 0.f, 0.f));
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("GreenDirection"), FLinearColor(0.f, 0.f, 0.f, 0.f));
		} else if (_flashlightComponent->LightColor == FColor(255, 255, 0, 255)) {
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("YellowLocation"), UKismetMathLibrary::Conv_VectorToLinearColor(locationVector));
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("YellowDirection"), UKismetMathLibrary::Conv_VectorToLinearColor(forwardVector));
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("BlueLocation"), FLinearColor(0.f, 0.f, 0.f, 0.f));
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("BlueDirection"), FLinearColor(0.f, 0.f, 0.f, 0.f));
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("RedLocation"), FLinearColor(0.f, 0.f, 0.f, 0.f));
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("RedDirection"), FLinearColor(0.f, 0.f, 0.f, 0.f));
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("GreenLocation"), FLinearColor(0.f, 0.f, 0.f, 0.f));
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("GreenDirection"), FLinearColor(0.f, 0.f, 0.f, 0.f));
		} else if (_flashlightComponent->LightColor == FColor(0, 255, 0, 255)) {
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("GreenLocation"), UKismetMathLibrary::Conv_VectorToLinearColor(locationVector));
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("GreenDirection"), UKismetMathLibrary::Conv_VectorToLinearColor(forwardVector));
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("BlueLocation"), FLinearColor(0.f, 0.f, 0.f, 0.f));
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("BlueDirection"), FLinearColor(0.f, 0.f, 0.f, 0.f));
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("RedLocation"), FLinearColor(0.f, 0.f, 0.f, 0.f));
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("RedDirection"), FLinearColor(0.f, 0.f, 0.f, 0.f));
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("YellowLocation"), FLinearColor(0.f, 0.f, 0.f, 0.f));
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("YellowDirection"), FLinearColor(0.f, 0.f, 0.f, 0.f));
		}
	} else {
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("BlueLocation"), FLinearColor(0.f, 0.f, 0.f, 0.f));
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("BlueDirection"), FLinearColor(0.f, 0.f, 0.f, 0.f));
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("RedLocation"), FLinearColor(0.f, 0.f, 0.f, 0.f));
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("RedDirection"), FLinearColor(0.f, 0.f, 0.f, 0.f));
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("YellowLocation"), FLinearColor(0.f, 0.f, 0.f, 0.f));
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("YellowDirection"), FLinearColor(0.f, 0.f, 0.f, 0.f));
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("GreenLocation"), FLinearColor(0.f, 0.f, 0.f, 0.f));
		UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), _parameterCollection, FName("GreenDirection"), FLinearColor(0.f, 0.f, 0.f, 0.f));
	}
}

void AFlashlight::ToggleFlashlightOn()
{
	_bTurnedOn = !_bTurnedOn;
	_flashlightComponent->ToggleVisibility();
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
	if (_myGameState->_bStairPositioned && _sophia->GetActorLocation().Z > 540.f) {
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
	} else {
		if (_myGameState->_dialogueWidget != nullptr)
			_myGameState->_dialogueWidget->SetDialogueTextAndShow(FText::FromString("No llego. Necesito buscar algo para subir."), 3.5f);
	}
}

void AFlashlight::DropItem(AItem* item)
{
}