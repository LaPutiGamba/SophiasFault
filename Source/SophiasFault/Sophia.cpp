#include "Sophia.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GMS_MyGameStateBase.h"
#include "Inventory/Items/Item.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryWidget.h"
#include "Blueprint/UserWidget.h"

ASophia::ASophia()
{
	PrimaryActorTick.bCanEverTick = true;

	_inventory = CreateDefaultSubobject<UInventoryComponent>("Inventory");
	_inventory->_capacity = 10;
	_inventoryHUD = nullptr;
	ConstructorHelpers::FClassFinder<UInventoryWidget> _inventoryFinderClass(TEXT("/Game/Items/WBP_Inventory2"));
	if (_inventoryFinderClass.Succeeded()) {
		_inventoryHUDClass = _inventoryFinderClass.Class;
	}

	_myGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AGMS_MyGameStateBase>() : nullptr;

	// Init of STAMINA variables
	_runningOrCrouching = false;
	_staminaTimer = 0.f;
	_staminaMax = 5.f;
	_staminaStatus = IDLE;

	// Init of FLASHLIGHT variables
	_rechargingFlashlight = false;
	_flashlightTimer = 0.f;
	_flashlightMax = 10.f;
	_flashlightStatus = LIGHTOFF;
}

void ASophia::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(_defaultMappingContext, 0);
		}
	}
}

void ASophia::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	// STAMINA
	switch (_staminaStatus) {
	case RUNNING:
		if (_runningOrCrouching) {
			if (_myGameState->_onChase) {
				_speed = 1.f;
			} else {
				_speed = 0.25f;
			}

			if (_staminaTimer < _staminaMax) {
				_staminaTimer += deltaTime;
			} else {
				_staminaStatus = EXHAUSTED;
				_speed = 0.5f;
				printFloat(_speed);
			}
		} else {
			_staminaStatus = IDLE;
			_speed = 0.5f;
		}
		break;
	case EXHAUSTED:
		if (_runningOrCrouching && _staminaTimer < (_staminaMax / 2))
			_staminaStatus = RUNNING;

		_staminaTimer -= deltaTime;

		if (_staminaTimer < 0.f)
			_staminaStatus = IDLE;
		break;
	case IDLE:
		if (_runningOrCrouching)
			_staminaStatus = RUNNING;
		break;
	default:
		break;
	}

	// FLASHLIGHT
	switch (_flashlightStatus) {
	case LIGHTON:
		break;
	case LIGHTOFF:
		break;
	case NEEDRECHARGE:
		break;
	default:
		break;
	}
}

void ASophia::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
	Super::SetupPlayerInputComponent(playerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(playerInputComponent)) {
		EnhancedInputComponent->BindAction(_moveAction, ETriggerEvent::Triggered, this, &ASophia::Move);
		EnhancedInputComponent->BindAction(_lookAction, ETriggerEvent::Triggered, this, &ASophia::Look);
		EnhancedInputComponent->BindAction(_runOrCrouchAction, ETriggerEvent::Triggered, this, &ASophia::RunOrCrouch);
		EnhancedInputComponent->BindAction(_flashlightAction, ETriggerEvent::Triggered, this, &ASophia::Flashlight);
		EnhancedInputComponent->BindAction(_rechargeFlashlightAction, ETriggerEvent::Triggered, this, &ASophia::RechargeFlashlight);
		EnhancedInputComponent->BindAction(_inventoryAction, ETriggerEvent::Triggered, this, &ASophia::Inventory);
	}
}

void ASophia::Move(const FInputActionValue& value)
{
	FVector2D movementVector = value.Get<FVector2D>();

	if (Controller != nullptr) {
		AddMovementInput(GetActorForwardVector(), movementVector.Y * _speed);
		AddMovementInput(GetActorRightVector(), movementVector.X * _speed);
	}
}

void ASophia::Look(const FInputActionValue& value)
{
	FVector2D lookAxisValue = value.Get<FVector2D>();

	if (Controller != nullptr) {
		AddControllerYawInput(lookAxisValue.X * _cameraVelocity);
		AddControllerPitchInput(-lookAxisValue.Y * _cameraVelocity);
	}
}

void ASophia::RunOrCrouch(const FInputActionValue& value) 
{
	_runningOrCrouching = value.Get<bool>();
}

void ASophia::Flashlight(const FInputActionValue& value) 
{
	bool flashlightBool = value.Get<bool>();

	USpotLightComponent* flashlight = ASophia::GetComponentByClass<USpotLightComponent>();
	if (flashlight != nullptr) {
		if (flashlightBool) {
			flashlight->ToggleVisibility(true);
		}
		else {
			flashlight->ToggleVisibility(false);
		}
	}
}

void ASophia::RechargeFlashlight(const FInputActionValue& value)
{
	_rechargingFlashlight = value.Get<bool>();
}

void ASophia::Inventory(const FInputActionValue& value)
{
	if (IsLocallyControlled() && _inventoryHUDClass && !Cast<UInventoryWidget>(_inventoryHUD)) {
		APlayerController* SophiaController = GetController<APlayerController>();
		check(SophiaController);
		_inventoryHUD = CreateWidget<UInventoryWidget>(SophiaController, _inventoryHUDClass);
		check(_inventoryHUD);
		_inventoryHUD->AddToPlayerScreen();
		SophiaController->SetInputMode(FInputModeUIOnly());
		SophiaController->bShowMouseCursor = true;
		SophiaController->bEnableClickEvents = true;
		SophiaController->bEnableMouseOverEvents = true;
	} else if (Cast<UInventoryWidget>(_inventoryHUD)) {
		APlayerController* SophiaController = GetController<APlayerController>();
		check(SophiaController);
		_inventoryHUD->RemoveFromParent();
		SophiaController->SetInputMode(FInputModeGameOnly());
		SophiaController->bShowMouseCursor = false;
		SophiaController->bEnableClickEvents = false;
		SophiaController->bEnableMouseOverEvents = false;
		_inventoryHUD = nullptr;
	}
}

void ASophia::UseItem(UItem* item)
{
	if (item) {
		item->Use(this);
		item->OnUse(this); // BP event
	}
}