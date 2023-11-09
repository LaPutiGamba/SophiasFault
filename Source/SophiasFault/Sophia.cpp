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
	_stamina = 0.f;
	_staminaCooldown = 5.f;
	_staminaWasted = false;
	_staminaStatus = IDLE;
}

void ASophia::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(_defaultMappingContext, 0);
		}
	}

	printFloat(_staminaCooldown);
}

void ASophia::Tick(float deltaTime)
{
	Super::Tick(deltaTime);


	switch (_staminaStatus) {
	case RUNNING:
		printText("RUNNING");
		if (_runningOrCrouching) {
			if (_stamina < _staminaCooldown) 
				_stamina += deltaTime;
		
			printFloat(_stamina);

			if (_stamina > _staminaCooldown)
				_staminaStatus = EXHAUSTED;
		} else {
			_staminaStatus = IDLE;
		}
		break;
	case EXHAUSTED:
		printText("EXHAUSTED");
		_runningOrCrouching = false;
		_stamina -= deltaTime;
		if (_stamina < 0.f)
			_staminaStatus = IDLE;
		break;
	case IDLE:
		//printText("IDLE");
		if (_runningOrCrouching)
			_staminaStatus = RUNNING;
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

	if (_runningOrCrouching) {
		if (_myGameState->_onChase) {
			_speed = 1.f;
		} else {
			_speed = 0.25f;
		}
	} else {
		_speed = 0.5f;
	}
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