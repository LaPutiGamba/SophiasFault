#include "Sophia.h"
#include "Macros.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Core/GMS_MyGameStateBase.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryWidget.h"
#include "Inventory/Items/ItemPhysic.h"
#include "Inventory/Items/ItemInteractive.h"
#include "Inventory/Items/PhysicItems/Flashlight.h"
#include "Inventory/Items/PhysicItems/Stair.h"
#include "Inventory/Items/PhysicItems/MirrorLight.h"
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

	// Init of MATERIALS variables

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
	_staminaStatus = IDLE;

	// Init of ITEMS variables
	_attachComponent = CreateDefaultSubobject<USceneComponent>(TEXT("AttachComponent"));
	_attachComponent->SetupAttachment(RootComponent);
	_attachComponent->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	_holdingComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HoldingComponent"));
	_holdingComponent->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));
	_holdingComponent->SetupAttachment(_attachComponent);

	_currentItemPhysic = nullptr;
	_currentItemInteractive = nullptr;
	_currentChangeCameraItem = nullptr;
	_flashlightItem = nullptr;
	_bCanMove = true;
	_bHoldingItem = false;
	_bInspecting = false;
	_bInspectingPressed = false;
	_bNoSwitchableItem = false;
	_bPositionActorPuzzle = false;
	_itemInspectDistance = 120.f;

	// Init of PUZZLE variables
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
	case RUNNING:
		if (_bRunningOrCrouching) {
			if (_myGameState->GetOnChase()) {
				_speed = 1.f;

				if (_staminaTimer < _staminaMax) {
					_staminaTimer += deltaTime;
				} else {
					_staminaStatus = EXHAUSTED;
					_speed = 0.5f;
				}
			} else {
				_speed = 0.25f;
				_springArmComponent->SetRelativeLocation(FVector(_cameraLocation.X, _cameraLocation.Y, (_cameraLocation.Z - 40.f)));
			}
		} else {
			_staminaStatus = IDLE;
			_speed = 0.5f;
		}

		if (_bNoSwitchableItem)
			_staminaStatus = IDLE;
		break;
		/*
			If the player is exhausted, it will start decreasing the _staminaTimer. If it reach half of the maximum stamina,
			and the player still pressing the running or crouching button, it will start running again until the half of the
			max stamina it's spent. If the person doesn't press the running or crouching button, it will change to IDLE status.
		*/
	case EXHAUSTED:
		if (_bRunningOrCrouching && _staminaTimer < (_staminaMax / 2))
			_staminaStatus = RUNNING;

		_staminaTimer -= deltaTime;

		if (!_myGameState->GetOnChase())
			_springArmComponent->SetRelativeLocation(_cameraLocation);

		if (_bNoSwitchableItem)
			_staminaStatus = IDLE;

		if (_staminaTimer < 0.f && !_bRunningOrCrouching)
			_staminaStatus = IDLE;
		break;
		/*
			If the player it's on the IDLE status, it the running or crouching button is pressed it will change to RUNNING status.
		*/
	case IDLE:
		if (!_myGameState->GetOnChase()) 
			_springArmComponent->SetRelativeLocation(_cameraLocation);

		if (_bNoSwitchableItem)
			_speed = 0.37f;
		else
			_speed = 0.5f;

		if (_bRunningOrCrouching && !_bNoSwitchableItem)
			_staminaStatus = RUNNING;
		break;
	default:
		break;
	}

	// ITEMS (PHYSICS AND STATICS)
	// Calculations to know the distance and in what point the player has to look to detect an Item.
	if (_cameraComponent != nullptr)
		_start = _cameraComponent->GetComponentLocation();
	if (_cameraComponent != nullptr)
		_forwardVector = _cameraComponent->GetForwardVector();
	_end = ((_forwardVector * 200.f) + _start);

	// Mirror light puzzle (potisioning the mirror lights on the mirror and activating the solution)
	if (_bHoldingItem && _currentHandItem->GetClass()->IsChildOf(AMirrorLight::StaticClass())) {
		if (GetWorld()->LineTraceSingleByChannel(_hit, _start, _end, ECC_Selectable, _defaultComponentQueryParams, _defaultResponseParams)) {
			if (_hit.GetActor()->ActorHasTag("MirrorLightPosition") && _bPositionActorPuzzle) {
				AMirrorLight* mirror = Cast<AMirrorLight>(_currentHandItem);
				mirror->PositionLight(_hit.GetActor()->GetActorLocation());
				_inventory->RemoveItem(_currentHandItem);

				if (_myGameState != nullptr) {
					if (_myGameState->GetPositionedMirrorLights()->Num() == 0) {
						_myGameState->GetPositionedMirrorLights()->Add(mirror->_mirrorLightID);
					} else {
						int value = _myGameState->GetPositionedMirrorLights()->Find(mirror->_mirrorLightID);
						if (value == -1)
							_myGameState->GetPositionedMirrorLights()->Add(mirror->_mirrorLightID);
						else
							_myGameState->GetPositionedMirrorLights()->Remove(mirror->_mirrorLightID);
					}

					if (_myGameState->GetPositionedMirrorLights()->Num() == 8)
						_myGameState->ActivateMirrorLightSolution();
				}

				_currentHandItem = nullptr;
				_bPositionActorPuzzle = false;
				_bHoldingItem = !_bHoldingItem;
			}
		}
	}

	if (!_bHoldingItem) {
		if (GetWorld()->LineTraceSingleByChannel(_hit, _start, _end, ECC_Visibility, _defaultComponentQueryParams, _defaultResponseParams)) {
			// If the player has in sight an Interactive Item, the _currentItemInteractive save a pointer to that Actor.
			if (_hit.GetActor()->GetClass()->IsChildOf(AItemInteractive::StaticClass()) && !_hit.GetActor()->ActorHasTag("EarthContinent"))
				_currentItemInteractive = Cast<AItemInteractive>(_hit.GetActor());

			// If the player has in sight an Physic Item, the _currentItemPhysic save a pointer to that Actor.
			if (_hit.GetActor()->GetClass()->IsChildOf(AItemPhysic::StaticClass()) && !_hit.GetActor()->GetClass()->IsChildOf(AFlashlight::StaticClass())) 
				_currentItemPhysic = Cast<AItemPhysic>(_hit.GetActor());

			// If the player has in sight the Flashlight, the _flashlightItem save a pointer to that Actor.
			if (_hit.GetActor()->GetClass()->IsChildOf(AFlashlight::StaticClass()))
				_flashlightItem = Cast<AFlashlight>(_hit.GetActor());

			// If the player has in sight the Piano Seat or the Earth Ball, the _currentChangeCameraItem will save a pointer to that Actor.
			if (_hit.GetActor()->ActorHasTag("PianoSeat") || _hit.GetActor()->ActorHasTag("EarthBall"))
				_currentChangeCameraItem = Cast<AActor>(_hit.GetActor());
		} else {
			// If the player doesn't has in sight any item, all the items are null.
			_currentItemPhysic = nullptr;
			_currentItemInteractive = nullptr;
			_flashlightItem = nullptr;
			_currentChangeCameraItem = nullptr;
		}
	}

	if (_bInspecting) {
		if (_bHoldingItem) {
			// If it's pressing the inspecting key and it's holding any item, it will put the field of view to the screen.
			_cameraComponent->SetFieldOfView(FMath::Lerp(_cameraComponent->FieldOfView, 90.f, 0.1f));

			// Also we get a vector calculating where to put the item. In this case, in the middle of the screen.
			FVector holdingComponentVector = (_cameraComponent->GetComponentLocation() + (_cameraComponent->GetForwardVector() * _itemInspectDistance));
			_holdingComponent->SetWorldLocation(holdingComponentVector);

			// We set the maximum and the minimum pith view to the highest possible values to be able to rotate the object 360�.
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax = 179.9000002f;
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin = -179.9000002f;

			// If we are holding an Item, we will call his rotate function.
			if (_currentHandItem != nullptr)
				_currentHandItem->RotateItem();
		}
	} else {
		// If it's not pressing the inspecting key we will put the field of view to the screen.
		_cameraComponent->SetFieldOfView(FMath::Lerp(_cameraComponent->FieldOfView, 90.f, 0.1f));

		// If it's not pressing the inspecting key and it's holding any item, it will put the item in the hand position.
		if (_bHoldingItem) {
			if (_currentHandItem != nullptr)
				_currentHandItem->SetActorRotation(_holdingComponent->GetRelativeRotation());

			_holdingComponent->SetRelativeLocation(FVector(50.f, -10.f, 20.f));
		}
	}

	// Blend time of the camera (ASohpia::BlendWithCamera)
	if (_myGameState->_onBlendTime > 0.0f)
		_myGameState->_onBlendTime -= deltaTime;

	if (_currentHandItem != nullptr) {
		if (_currentHandItem->GetClass()->IsChildOf(AStair::StaticClass())) {
			AStair* stair = Cast<AStair>(_currentHandItem);
			TArray<AActor*> stairSearch;
			UGameplayStatics::GetAllActorsWithTag(GetWorld(), "StairPositioned", stairSearch);

			if (stair->_triggered) {
				for (auto& result : stairSearch) {
					result->SetActorHiddenInGame(false);
					result->SetActorEnableCollision(false);
				}
			} else {
				for (auto& result : stairSearch) {
					result->SetActorHiddenInGame(true);
					result->SetActorEnableCollision(true);
				}
			}
		}
	}
}

void ASophia::SetupPlayerInputComponent(UInputComponent *playerInputComponent)
{
	Super::SetupPlayerInputComponent(playerInputComponent);

	if (UEnhancedInputComponent *EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(playerInputComponent)) {
		// Movement
		EnhancedInputComponent->BindAction(_moveAction, ETriggerEvent::Triggered, this, &ASophia::Move);
		EnhancedInputComponent->BindAction(_lookAction, ETriggerEvent::Triggered, this, &ASophia::Look);
		EnhancedInputComponent->BindAction(_runOrCrouchAction, ETriggerEvent::Triggered, this, &ASophia::RunOrCrouch);
		// Flashlight
		EnhancedInputComponent->BindAction(_flashlightAction, ETriggerEvent::Triggered, this, &ASophia::Flashlight);
		EnhancedInputComponent->BindAction(_rechargeFlashlightAction, ETriggerEvent::Triggered, this, &ASophia::RechargeFlashlight);
		// Change Camera
		EnhancedInputComponent->BindAction(_getUpAction, ETriggerEvent::Triggered, this, &ASophia::BlendWithCamera);
		// Items slots
		EnhancedInputComponent->BindAction(_changeItemAction, ETriggerEvent::Triggered, this, &ASophia::ChangeCurrentHandItem, 10);
		EnhancedInputComponent->BindAction(_handItem0Action, ETriggerEvent::Triggered, this, &ASophia::ChangeCurrentHandItem, 0);
		EnhancedInputComponent->BindAction(_handItem1Action, ETriggerEvent::Triggered, this, &ASophia::ChangeCurrentHandItem, 1);
		EnhancedInputComponent->BindAction(_handItem2Action, ETriggerEvent::Triggered, this, &ASophia::ChangeCurrentHandItem, 2);
		EnhancedInputComponent->BindAction(_handItem3Action, ETriggerEvent::Triggered, this, &ASophia::ChangeCurrentHandItem, 3);
		EnhancedInputComponent->BindAction(_handItem4Action, ETriggerEvent::Triggered, this, &ASophia::ChangeCurrentHandItem, 4);
		EnhancedInputComponent->BindAction(_handItem5Action, ETriggerEvent::Triggered, this, &ASophia::ChangeCurrentHandItem, 5);
		EnhancedInputComponent->BindAction(_handItem6Action, ETriggerEvent::Triggered, this, &ASophia::ChangeCurrentHandItem, 6);
		EnhancedInputComponent->BindAction(_handItem7Action, ETriggerEvent::Triggered, this, &ASophia::ChangeCurrentHandItem, 7);
		EnhancedInputComponent->BindAction(_handItem8Action, ETriggerEvent::Triggered, this, &ASophia::ChangeCurrentHandItem, 8);
		EnhancedInputComponent->BindAction(_handItem9Action, ETriggerEvent::Triggered, this, &ASophia::ChangeCurrentHandItem, 9);
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

	if ((!_bInventoryOpen && Controller != nullptr && !_bInspecting) || (_bInspecting && !_bHoldingItem)) {
		AddControllerYawInput(lookAxisValue.X * _cameraVelocity);
		AddControllerPitchInput(-lookAxisValue.Y * _cameraVelocity);
	}

	if (_bInspecting && _currentHandItem != nullptr) {
		FRotator sendRotator = Controller->GetControlRotation();
		sendRotator.Yaw = -lookAxisValue.X;
		sendRotator.Pitch = lookAxisValue.Y;

		if (_currentHandItem != nullptr)
			_currentHandItem->_controlRotation = sendRotator;
	}
}

void ASophia::RunOrCrouch(const FInputActionValue &value)
{
	_bRunningOrCrouching = value.Get<bool>();
}

void ASophia::Flashlight(const FInputActionValue &value)
{
	if (_currentHandItem != nullptr && _currentHandItem->GetClass()->IsChildOf(AFlashlight::StaticClass()))
		Cast<AFlashlight>(_currentHandItem)->ToggleFlashlightOn();
}

void ASophia::RechargeFlashlight(const FInputActionValue &value)
{
	if (_currentHandItem != nullptr && _currentHandItem->GetClass()->IsChildOf(AFlashlight::StaticClass()))
		Cast<AFlashlight>(_currentHandItem)->ToggleRechargingFlashlight();
}

void ASophia::Inventory(const FInputActionValue& value)
{
	if (IsLocallyControlled() && _inventoryHUDClass && !_bNoSwitchableItem) {
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
	if (_currentHandItem) {
		if (_currentHandItem->GetClass()->IsChildOf(AMirrorLight::StaticClass())) 
			_bPositionActorPuzzle = true;
		else 
			_bPositionActorPuzzle = false;
	} else if ((_currentItemPhysic || _flashlightItem)) {
		if (_currentItemPhysic != nullptr) {
			_inventory->AddItem(_currentItemPhysic);
			_currentItemPhysic->PickUpItem();
			_currentHandItem = _currentItemPhysic;
			_bHoldingItem = !_bHoldingItem;

			if (_currentItemPhysic->GetClass()->IsChildOf(AStair::StaticClass()))
				_bNoSwitchableItem = true;

			_currentItemPhysic = nullptr;
		}
		if (_flashlightItem != nullptr) {
			_inventory->AddItem(_flashlightItem);
			_flashlightItem->PickUpItem();
			_currentHandItem = _flashlightItem;
			_flashlightItem = nullptr;
			_bHoldingItem = !_bHoldingItem;
		}
	} else if (_currentItemInteractive) {
		_currentItemInteractive->UseInteraction();
	} else if (_currentChangeCameraItem) {
		if (_currentChangeCameraItem->ActorHasTag("PianoSeat")) {
			if (_myGameState->_onBlendTime <= 0.0f) {
				// Blend of the character camera to the Piano camera.
				_playerController->SetViewTargetWithBlend(_pianoCameraActor, 0.75f);
				_myGameState->_onBlendTime = 0.75f;
				_playerController->bShowMouseCursor = true;
				_playerController->bEnableClickEvents = true;
				_playerController->bEnableMouseOverEvents = true;
			}
		} else if (_currentChangeCameraItem->ActorHasTag("EarthBall")) {
			if (_myGameState->_onBlendTime <= 0.0f) {
				// Calculate the location and direction of the camera to center the sphere.
				FVector playerViewDirection = _playerController->GetControlRotation().Vector();
				float distanceBehindSphere = 220.0f;
				FVector newCameraPosition = _currentChangeCameraItem->GetActorLocation() - (playerViewDirection * distanceBehindSphere);
				_earthCameraActor->SetActorLocation(newCameraPosition);
				FVector LookAtDirection = (_currentChangeCameraItem->GetActorLocation() - _earthCameraActor->GetActorLocation()).GetSafeNormal();
				FRotator NewCameraRotation = LookAtDirection.Rotation();
				_earthCameraActor->SetActorRotation(NewCameraRotation);

				// Blend of the character camera to the Earth Ball camera.
				_playerController->SetViewTargetWithBlend(_earthCameraActor, 0.75f);
				_myGameState->_onBlendTime = 0.75f;
				_playerController->bShowMouseCursor = true;
				_playerController->bEnableClickEvents = true;
				_playerController->bEnableMouseOverEvents = true;
			}
		}

		// Removing the default mapping context and adding the new mapping context
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
				Subsystem->RemoveMappingContext(_mainMappingContext);

				if (_currentChangeCameraItem->ActorHasTag("PianoSeat"))
					Subsystem->AddMappingContext(_pianoMappingContext, 0);
				else if (_currentChangeCameraItem->ActorHasTag("EarthBall"))
					Subsystem->AddMappingContext(_earthMappingContext, 0);
			}
		}
	} else if (_bNoSwitchableItem) {
		if (_currentHandItem->GetClass()->IsChildOf(AStair::StaticClass())) {
			AStair* stair = Cast<AStair>(_currentHandItem);
			if (stair->_triggered) {
				_inventory->RemoveItem(_currentHandItem);
				_currentHandItem->Destroy();
				_currentHandItem = nullptr;
				_bHoldingItem = !_bHoldingItem;
				
				TArray<AActor*> stairSearch;
				UGameplayStatics::GetAllActorsWithTag(GetWorld(), "StairPositioned", stairSearch);

				for (auto& result : stairSearch) {
					result->SetActorHiddenInGame(false);
					result->SetActorEnableCollision(true);
					TArray<UActorComponent*> staticMeshComponents;
					UStaticMeshComponent* staticMesh = FindComponentByClass<UStaticMeshComponent>();
					staticMesh->SetMaterial(0, stair->_defaultMaterial);
					_bNoSwitchableItem = false;
				}
			}
		}
	}
}

void ASophia::DropItem(const FInputActionValue& value)
{
	if (_currentHandItem != nullptr && !_bNoSwitchableItem) {
		_inventory->RemoveItem(_currentHandItem);
		_currentHandItem->DropItem();
		_currentHandItem = nullptr;
		_bHoldingItem = !_bHoldingItem;
	}
}

void ASophia::OnInspect(const FInputActionValue &value)
{
	_bInspectingPressed = !_bInspectingPressed;

	if (_currentHandItem && !_bNoSwitchableItem) {
		if (_bInspectingPressed) {
			if (_bHoldingItem) {
				_lastRotation = GetControlRotation();
				ToggleMovement();
			} else {
				_bInspecting = true;
			}
		} else {
			if (_bInspecting && _bHoldingItem) {
				GetController()->SetControlRotation(_lastRotation);
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax = _pitchMax;
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin = _pitchMin;
				ToggleMovement();
			} else {
				_bInspecting = false;
			}
		}
	}
}

void ASophia::ToggleMovement()
{
	_bCanMove = !_bCanMove;
	_bInspecting = !_bInspecting;
	_cameraComponent->bUsePawnControlRotation = ~_cameraComponent->bUsePawnControlRotation;
	bUseControllerRotationYaw = ~bUseControllerRotationYaw;
}

void ASophia::BlendWithCamera(const FInputActionValue &value)
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

void ASophia::ChangeCurrentHandItem(const FInputActionValue& value, int index)
{
	if (_bNoSwitchableItem) return;

	// Hide the actual item in the _inventory and in the hand
	if (_inventory->_items[_currentItemSlotIndex] != nullptr) {
		_inventory->_items[_currentItemSlotIndex]->SetVisibility(false);
		_currentHandItem->SetVisibility(false);
	}

	// If the index is not 10, it means that the player has pressed a number key, so we will change the item to that slot
	if (index != 10) {
		_currentItemSlotIndex = index;
	// If the index is 10, it means that the player has pressed the change item key, so we will change the item to the next slot
	} else {
		float mouseAxis = value.Get<float>();

		if (mouseAxis > 0.f) {
			if ((_currentItemSlotIndex < 9))
				_currentItemSlotIndex++;
		} else if (mouseAxis < 0.f) {
			if (_currentItemSlotIndex > 0)
				_currentItemSlotIndex--;
		}
	}

	// Show the new item in the _inventory and in the hand
	if (_inventory->_items[_currentItemSlotIndex] != nullptr) {
		_inventory->_items[_currentItemSlotIndex]->SetVisibility(true);
		_currentHandItem = _inventory->_items[_currentItemSlotIndex];
		_currentHandItem->SetVisibility(true);
		_bHoldingItem = true;
	// If the slot is empty, we will set the _currentHandItem to nullptr
	} else {
		_currentHandItem = nullptr;
		_bHoldingItem = false;
	}
}

void ASophia::EarthRotation(const FInputActionValue& value)
{
	FVector2D mousePosition;
	_playerController->GetMousePosition(mousePosition.X, mousePosition.Y);
	FVector2D screenSize;
	GEngine->GameViewport->GetViewportSize(screenSize);

	FRotator sendRotator = FRotator::ZeroRotator;
	
	if (((mousePosition.X - (screenSize.X / 2)) > 30.f || (mousePosition.X - (screenSize.X / 2)) < -30.f) ||
		((mousePosition.Y - (screenSize.Y / 2)) > 30.f || (mousePosition.Y - (screenSize.Y / 2)) < -30.f)) {
		sendRotator.Yaw = -((mousePosition.X - (screenSize.X / 2)) / screenSize.X) * 1.25f;
		sendRotator.Pitch = ((mousePosition.Y - (screenSize.Y / 2)) / screenSize.Y) * 1.25f;
	}

	if (_currentChangeCameraItem != nullptr)
		_currentChangeCameraItem->AddActorWorldRotation(sendRotator);
}

void ASophia::ClickInteractive(const FInputActionValue &value)
{
	FVector2D mousePosition;
	_playerController->GetMousePosition(mousePosition.X, mousePosition.Y);

	FVector worldLocation, worldDirection;
	_playerController->DeprojectScreenPositionToWorld(mousePosition.X, mousePosition.Y, worldLocation, worldDirection);

	if (GetWorld()->LineTraceSingleByChannel(_hit, worldLocation, worldLocation + worldDirection * 350.f, ECC_Selectable, _defaultComponentQueryParams, _defaultResponseParams)) {
		if (_hit.GetActor()->ActorHasTag("EarthContinent")) {
			AItemInteractive* mesh = Cast<AItemInteractive>(_hit.GetActor());
			mesh->UseInteraction();
		}
	}
}