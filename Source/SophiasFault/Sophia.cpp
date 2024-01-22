#include "Sophia.h"
#include "Macros.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Core/GMS_MyGameStateBase.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryWidget.h"
#include "Inventory/Items/Flashlight.h"
#include "Inventory/Items/Stair.h"
#include "Inventory/Items/MirrorLight.h"
#include "Inventory/Items/PianoKey.h"
#include "Inventory/Items/ActorBlendCamera.h"
#include "Interfaces/InteractiveInterface.h"
#include "Interfaces/PickUpInterface.h"
#include "Interfaces/OnActionInterface.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

#define ECC_Selectable ECC_GameTraceChannel1

ASophia::ASophia()
{
	PrimaryActorTick.bCanEverTick = true;

	// Init of GAMESTATE variable
	_myGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AGMS_MyGameStateBase>() : nullptr;

	// Init of INVENTORY variables
	_inventory = CreateDefaultSubobject<UInventoryComponent>("Inventory");
	_inventory->_capacity = 10;
	_inventoryHUD = nullptr;
	_bInventoryOpen = false;

	ConstructorHelpers::FClassFinder<UInventoryWidget> inventoryFinderClass(TEXT("/Game/Items/Widgets/WBP_Inventory"));
	if (inventoryFinderClass.Succeeded())
		_inventoryHUDClass = inventoryFinderClass.Class;
	else
		_inventoryHUDClass = nullptr;

	ConstructorHelpers::FClassFinder<UUserWidget> itemFinderClass(TEXT("/Game/Items/Widgets/WBP_Item"));
	if (itemFinderClass.Succeeded())
		_inventoryItemsClass = itemFinderClass.Class;
	else 
		_inventoryItemsClass = nullptr;

	// Init of CAMERA component
	_springArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	_springArmComponent->SetupAttachment(GetRootComponent());
	_springArmComponent->SetRelativeLocation(FVector(0, 0, 70));
	_springArmComponent->TargetArmLength = -10;
	_springArmComponent->bUsePawnControlRotation = true;

	_cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	_cameraComponent->SetupAttachment(_springArmComponent, USpringArmComponent::SocketName);
	_cameraComponent->bUsePawnControlRotation = false;

	// Init of STAMINA variables
	_bRunningOrCrouching = false;
	_staminaTimer = 0.f;
	_staminaMax = 5.f;
	_staminaStatus = ST_IDLE;

	// Init of ITEMS variables
	_attachComponent = CreateDefaultSubobject<USceneComponent>(TEXT("AttachComponent"));
	_attachComponent->SetupAttachment(RootComponent);
	_attachComponent->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	_holdingComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HoldingComponent"));
	_holdingComponent->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));
	_holdingComponent->SetupAttachment(_attachComponent);

	_bCanMove = true;
	_bInspectingPressed = false;

	// Init of PUZZLE variables
}

AItem* ASophia::GetCurrentHandItem()
{
	return _inventory->_currentHandItem;
}

void ASophia::BeginPlay()
{
	Super::BeginPlay();

	_playerController = UGameplayStatics::GetPlayerController(this, 0);

	if (_playerController) {
		if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(_playerController->GetLocalPlayer()))
			Subsystem->AddMappingContext(_mainMappingContext, 0);
	}

	if (GetWorld()->GetFirstPlayerController()->PlayerCameraManager)
		_pitchMax = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax;
	if (GetWorld()->GetFirstPlayerController()->PlayerCameraManager)
		_pitchMin = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin;
	
	TArray<AActor*> pianoCameraSearch;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "PianoCamera", pianoCameraSearch);

	for (const auto& result : pianoCameraSearch) {
		_pianoCameraActor = Cast<AActor>(result);
	}

	TArray<AActor*> earthCameraSearch;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "EarthCamera", earthCameraSearch);

	for (const auto& result : earthCameraSearch) {
		_earthCameraActor = Cast<AActor>(result);
	}

	// Inventory Widget
	_inventoryHUD = CreateWidget<UInventoryWidget>(_playerController, _inventoryHUDClass);
	_inventory->_onInventoryUpdated.AddDynamic(_inventoryHUD, &UInventoryWidget::UpdateInventory);

	_cameraLocation = _springArmComponent->GetRelativeLocation();
}

void ASophia::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	// STAMINA
	switch (_staminaStatus) {
		/*
			If the player is running, depending if it's on chase or not, will increase or decrease her speed. That will simulate
			the player running or like crouching. Also, it has a maximum amount of time to run or crouch, so it will increase
			the _staminaTimer and if it reach the max, it will change to the EXHAUSTED status.
		*/
	case ST_RUNNING:
		if (_bRunningOrCrouching) {
			if (_myGameState->GetOnChase()) {
				_speed = 1.f;

				if (_staminaTimer < _staminaMax) {
					_staminaTimer += deltaTime;
				} else {
					_staminaStatus = ST_EXHAUSTED;
					_speed = 0.5f;
				}
			} else {
				_speed = 0.25f;
				_springArmComponent->SetRelativeLocation(FVector(_cameraLocation.X, _cameraLocation.Y, (_cameraLocation.Z - 40.f)));
			}
		} else {
			_staminaStatus = ST_IDLE;
			_speed = 0.5f;
		}

		if (_inventory->_currentHandItem) {
			if (_inventory->_currentHandItem->_bNoSwitchableItem)
				_staminaStatus = ST_IDLE;
		}
		break;
		/*
			If the player is exhausted, it will start decreasing the _staminaTimer. If it reach half of the maximum stamina,
			and the player still pressing the running or crouching button, it will start running again until the half of the
			max stamina it's spent. If the person doesn't press the running or crouching button, it will change to IDLE status.
		*/
	case ST_EXHAUSTED:
		if (_bRunningOrCrouching && _staminaTimer < (_staminaMax / 2))
			_staminaStatus = ST_RUNNING;

		_staminaTimer -= deltaTime;

		if (!_myGameState->GetOnChase())
			_springArmComponent->SetRelativeLocation(_cameraLocation);

		if (_inventory->_currentHandItem) {
			if (_inventory->_currentHandItem->_bNoSwitchableItem)
				_staminaStatus = ST_IDLE;
		}

		if (_staminaTimer < 0.f && !_bRunningOrCrouching)
			_staminaStatus = ST_IDLE;
		break;
		/*
			If the player it's on the IDLE status, it the running or crouching button is pressed it will change to RUNNING status.
		*/
	case ST_IDLE:
		if (!_myGameState->GetOnChase()) 
			_springArmComponent->SetRelativeLocation(_cameraLocation);

		if (_inventory->_currentHandItem) {
			if (_inventory->_currentHandItem->_bNoSwitchableItem)
				_speed = 0.375f;
			else
				_speed = 0.5f;
		} else {
			_speed = 0.5f;
		}

		if (_inventory->_currentHandItem) {
			if (_bRunningOrCrouching && !_inventory->_currentHandItem->_bNoSwitchableItem)
				_staminaStatus = ST_RUNNING;
		} else if (_bRunningOrCrouching) {
			_staminaStatus = ST_RUNNING;
		}
		break;
	default:
		break;
	}

	// Blend time of the camera (ASohpia::BlendWithCamera)
	if (_myGameState->_onBlendTime > 0.0f)
		_myGameState->_onBlendTime -= deltaTime;
}

void ASophia::SetupPlayerInputComponent(UInputComponent *playerInputComponent)
{
	Super::SetupPlayerInputComponent(playerInputComponent);

	if (UEnhancedInputComponent *EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(playerInputComponent)) {
		// Movement
		EnhancedInputComponent->BindAction(_moveAction, ETriggerEvent::Triggered, this, &ASophia::Move);
		EnhancedInputComponent->BindAction(_lookAction, ETriggerEvent::Triggered, this, &ASophia::Look);
		EnhancedInputComponent->BindAction(_runOrCrouchAction, ETriggerEvent::Triggered, this, &ASophia::RunOrCrouch);
		// Items slots
		EnhancedInputComponent->BindAction(_changeItemMouseAction, ETriggerEvent::Triggered, _inventory, &UInventoryComponent::ChangeCurrentHandItem, 10);
		EnhancedInputComponent->BindAction(_changeHandItem1Action, ETriggerEvent::Triggered, _inventory, &UInventoryComponent::ChangeCurrentHandItem, 0);
		EnhancedInputComponent->BindAction(_changeHandItem2Action, ETriggerEvent::Triggered, _inventory, &UInventoryComponent::ChangeCurrentHandItem, 1);
		EnhancedInputComponent->BindAction(_changeHandItem3Action, ETriggerEvent::Triggered, _inventory, &UInventoryComponent::ChangeCurrentHandItem, 2);
		EnhancedInputComponent->BindAction(_changeHandItem4Action, ETriggerEvent::Triggered, _inventory, &UInventoryComponent::ChangeCurrentHandItem, 3);
		EnhancedInputComponent->BindAction(_changeHandItem5Action, ETriggerEvent::Triggered, _inventory, &UInventoryComponent::ChangeCurrentHandItem, 4);
		EnhancedInputComponent->BindAction(_changeHandItem6Action, ETriggerEvent::Triggered, _inventory, &UInventoryComponent::ChangeCurrentHandItem, 5);
		EnhancedInputComponent->BindAction(_changeHandItem7Action, ETriggerEvent::Triggered, _inventory, &UInventoryComponent::ChangeCurrentHandItem, 6);
		EnhancedInputComponent->BindAction(_changeHandItem8Action, ETriggerEvent::Triggered, _inventory, &UInventoryComponent::ChangeCurrentHandItem, 7);
		EnhancedInputComponent->BindAction(_changeHandItem9Action, ETriggerEvent::Triggered, _inventory, &UInventoryComponent::ChangeCurrentHandItem, 8);
		EnhancedInputComponent->BindAction(_changeHandItem0Action, ETriggerEvent::Triggered, _inventory, &UInventoryComponent::ChangeCurrentHandItem, 9);
		// Inventory and Items
		EnhancedInputComponent->BindAction(_inventoryAction, ETriggerEvent::Triggered, this, &ASophia::Inventory);
		EnhancedInputComponent->BindAction(_pickUpAction, ETriggerEvent::Triggered, this, &ASophia::OnAction);
		EnhancedInputComponent->BindAction(_dropItemAction, ETriggerEvent::Triggered, this, &ASophia::DropItem);
		EnhancedInputComponent->BindAction(_inspectAction, ETriggerEvent::Triggered, this, &ASophia::OnInspect);
		// Earth Puzzle
		EnhancedInputComponent->BindAction(_clickRotationAction, ETriggerEvent::Triggered, this, &ASophia::EarthRotation);
		EnhancedInputComponent->BindAction(_clickInteractiveAction, ETriggerEvent::Triggered, this, &ASophia::ClickInteractive);
	}
}

void ASophia::Move(const FInputActionValue& value)
{
	FVector2D movementVector = value.Get<FVector2D>();

	if (!_bInventoryOpen && Controller != nullptr && _bCanMove) {
		AddMovementInput(GetActorForwardVector(), movementVector.Y * _speed);
		AddMovementInput(GetActorRightVector(), movementVector.X * _speed);
	}
}

void ASophia::Look(const FInputActionValue& value)
{
	FVector2D lookAxisValue = value.Get<FVector2D>();

	if ((!_bInventoryOpen && Controller != nullptr && !_inventory->_bInspecting) || (_inventory->_bInspecting && !_inventory->_bHoldingItem)) {
		AddControllerYawInput(lookAxisValue.X * _cameraVelocity);
		AddControllerPitchInput(-lookAxisValue.Y * _cameraVelocity);
	}

	if (_inventory->_bInspecting && _inventory->_currentHandItem != nullptr) {
		FRotator sendRotator = Controller->GetControlRotation();
		sendRotator.Yaw = -lookAxisValue.X;
		sendRotator.Pitch = lookAxisValue.Y;

		if (_inventory->_currentHandItem != nullptr)
			if (Cast<IPickUpInterface>(_inventory->_currentHandItem))
				Cast<IPickUpInterface>(_inventory->_currentHandItem)->_controlRotation = sendRotator;
	}
}

void ASophia::RunOrCrouch(const FInputActionValue &value)
{
	// Activate the running or crouching status
	_bRunningOrCrouching = value.Get<bool>();
}

void ASophia::Inventory(const FInputActionValue& value)
{
	if (IsLocallyControlled() && _inventoryHUDClass && !_inventory->_currentHandItem->_bNoSwitchableItem) {
		if (!_bInventoryOpen) {
			// Show the inventory 
			_inventoryHUD->AddToPlayerScreen();
			_playerController->bShowMouseCursor = true;
			_playerController->bEnableClickEvents = true;
			_playerController->bEnableMouseOverEvents = true;

			_bInventoryOpen = true;
		} else {
			// Close the inventory widget
			_inventoryHUD->RemoveFromParent();
			_playerController->bShowMouseCursor = false;
			_playerController->bEnableClickEvents = false;
			_playerController->bEnableMouseOverEvents = false;

			_bInventoryOpen = false;
		}
	}
}

void ASophia::OnAction(const FInputActionValue &value)
{
	if (_inventory->_currentHandItem) {
		if (IOnActionInterface* onActionItem = Cast<IOnActionInterface>(_inventory->_currentHandItem)) {
			onActionItem->OnAction();
			return;
		}
	}

	if (_inventory->_currentItemInSight) {
		if (IInteractiveInterface* interactiveItem = Cast<IInteractiveInterface>(_inventory->_currentItemInSight)) {
			interactiveItem->UseInteraction();
		} else if (IPickUpInterface* pickUpItem = Cast<IPickUpInterface>(_inventory->_currentItemInSight)) {
			pickUpItem->PickUpItem(_inventory->_currentItemInSight);
		}
	} else if (_inventory->_flashlightItem) {
		if (IPickUpInterface* flashlightItem = Cast<IPickUpInterface>(_inventory->_flashlightItem)) {
			flashlightItem->PickUpItem(_inventory->_flashlightItem);
		}
	} else if (_inventory->_currentChangeCameraItem) {
		if (AActorBlendCamera* changeCameraItem = Cast<AActorBlendCamera>(_inventory->_currentChangeCameraItem)) {
			if (changeCameraItem->_cameraActorBlend) {
				if (IInteractiveInterface* item = Cast<IInteractiveInterface>(changeCameraItem->_cameraActorBlend))				
					printText("Camera actor blend");
					//item->UseInteraction();
			}
		}
	}
}

void ASophia::DropItem(const FInputActionValue& value)
{
	if (_inventory->_currentHandItem != nullptr)
		if (!_inventory->_currentHandItem->_bNoSwitchableItem)
			if (Cast<IPickUpInterface>(_inventory->_currentHandItem))
				Cast<IPickUpInterface>(_inventory->_currentHandItem)->DropItem(_inventory->_currentHandItem, _cameraComponent);
}

void ASophia::OnInspect(const FInputActionValue &value)
{
	/*_bInspectingPressed = !_bInspectingPressed;

	if (_inventory->_currentHandItem && !_inventory->_bNoSwitchableItem) {
		if (_bInspectingPressed) {
			if (_inventory->_bHoldingItem) {
				_lastRotation = GetControlRotation();
				ToggleMovement();
			} else {
				_bInspecting = true;
			}
		} else {
			if (_bInspecting && _inventory->_bHoldingItem) {
				GetController()->SetControlRotation(_lastRotation);
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax = _pitchMax;
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin = _pitchMin;
				ToggleMovement();
			} else {
				_bInspecting = false;
			}
		}
	}*/
}

void ASophia::ToggleMovement()
{
	_bCanMove = !_bCanMove;
	//_bInspecting = !_bInspecting;
	_cameraComponent->bUsePawnControlRotation = ~_cameraComponent->bUsePawnControlRotation;
	bUseControllerRotationYaw = ~bUseControllerRotationYaw;
}

void ASophia::BlendBackWithCamera(const FInputActionValue &value)
{
	if (_myGameState->_onBlendTime <= 0.0f) {
		_playerController->SetViewTargetWithBlend(this, 0.75f);
		_myGameState->_onBlendTime = 0.75f;
		_playerController->bShowMouseCursor = false;
		_playerController->bEnableClickEvents = false;
		_playerController->bEnableMouseOverEvents = false;

		if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
				if (_currentChangeCameraItem->ActorHasTag("PianoSeat")) {
					Subsystem->RemoveMappingContext(_pianoMappingContext);
				} else if (_currentChangeCameraItem->ActorHasTag("EarthBall")) {
					Subsystem->RemoveMappingContext(_earthMappingContext);	
					FVector2D screenPosition;
					FVector worldLocation;
					_playerController->ProjectWorldLocationToScreen(worldLocation, screenPosition);
				}

				Subsystem->AddMappingContext(_mainMappingContext, 0);
			}
		}
	}
}

void ASophia::EarthRotation(const FInputActionValue& value)
{
	FVector2D mousePosition;
	_playerController->GetMousePosition(mousePosition.X, mousePosition.Y);
	FVector2D screenSize;
	GEngine->GameViewport->GetViewportSize(screenSize);

	//_playerController->

	FRotator sendRotator = FRotator::ZeroRotator;
	
	if (((mousePosition.X - (screenSize.X / 2)) > 30.f || (mousePosition.X - (screenSize.X / 2)) < -30.f) ||
		((mousePosition.Y - (screenSize.Y / 2)) > 30.f || (mousePosition.Y - (screenSize.Y / 2)) < -30.f)) {
		sendRotator.Yaw = -((mousePosition.X - (screenSize.X / 2)) / screenSize.X) * 1.25f;
		sendRotator.Pitch = ((mousePosition.Y - (screenSize.Y / 2)) / screenSize.Y) * 1.25f;
	}

	if (_currentChangeCameraItem != nullptr)
		_currentChangeCameraItem->SetActorRelativeRotation(_currentChangeCameraItem->GetActorRotation() + sendRotator);
}

void ASophia::ClickInteractive(const FInputActionValue &value)
{
	FVector2D mousePosition;
	_playerController->GetMousePosition(mousePosition.X, mousePosition.Y);

	FVector worldLocation, worldDirection;
	_playerController->DeprojectScreenPositionToWorld(mousePosition.X, mousePosition.Y, worldLocation, worldDirection);

	FHitResult _hit;
	if (GetWorld()->LineTraceSingleByChannel(_hit, worldLocation, worldLocation + worldDirection * 350.f, ECC_Selectable, FCollisionQueryParams::DefaultQueryParam, FCollisionResponseParams::DefaultResponseParam)) {
		if (_hit.GetActor()->ActorHasTag("EarthContinent")) {
			IInteractiveInterface* mesh = Cast<IInteractiveInterface>(_hit.GetActor());
			mesh->UseInteraction();
		}
	}
}