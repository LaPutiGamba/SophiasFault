#include "Sophia.h"
#include "Macros.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Curves/CurveFloat.h"
#include "Core/GMS_MyGameStateBase.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryWidget.h"
#include "Inventory/Items/Flashlight.h"
#include "Inventory/Items/Stair.h"
#include "Inventory/Items/Mirror Light/MirrorLight.h"
#include "Inventory/Items/Piano/PianoKey.h"
#include "Interfaces/InteractiveInterface.h"
#include "Interfaces/PickUpInterface.h"
#include "Interfaces/OnActionInterface.h"
#include "Interfaces/CameraBlendInterface.h"
#include "Interfaces/ActorBlendInterface.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h" 
#include "Components/CapsuleComponent.h" 
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "Camera/CameraShakeBase.h"
#include "Kismet/GameplayStatics.h"

#define ECC_Selectable ECC_GameTraceChannel1

ASophia::ASophia()
{
	PrimaryActorTick.bCanEverTick = true;

	// Init of GLOBAL variables
	_sanity = 100;

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
	_bIsRunning = false;
	_bIsCrouching = false;
	_bCheckCrouchingUntilNot = false;
	_staminaTimer = 0.f;
	_staminaMax = 5.f;
	_staminaStatus = ST_IDLE;

	// Init of ITEMS variables
	_holdingComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HoldingComponent"));
	_holdingComponent->SetRelativeLocation(FVector(50.f, 25.f, -12.f));
	_holdingComponent->SetupAttachment(_cameraComponent);

	_flashlightCrankComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlashlightCrankComponent"));
	_flashlightCrankComponent->SetRelativeLocation(FVector(-0.4f, -8.5f, 0.f));
	_flashlightCrankComponent->SetupAttachment(_holdingComponent);

	_flashlightCrankHandleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlashlightCrankHandleComponent"));
	_flashlightCrankHandleComponent->SetRelativeLocation(FVector(-70.f, -7.2f, 0.f));
	_flashlightCrankHandleComponent->SetRelativeRotation(FRotator(0.f, -2.5f, 0.f));
	_flashlightCrankHandleComponent->SetupAttachment(_flashlightCrankComponent);

	_flashlightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("FlashlightComponent"));
	_flashlightComponent->SetRelativeLocation(FVector(58.f, 24.5f, -20.f));
	_flashlightComponent->SetupAttachment(_cameraComponent);

	_bCanMove = true;

	// Init of TIMELINE variables
	_timelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent"));
	_curveFloat = CreateDefaultSubobject<UCurveFloat>(TEXT("CurveFloat"));

	// Init of AUDIO variables
	_footstepsSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Footsteps Sound Component"));
	_footstepsSoundComponent->bAutoActivate = false;
	_footstepsSoundComponent->SetupAttachment(RootComponent);
	_footstepsSoundComponent->SetRelativeLocation(FVector(0.f, 0.0f, -34.0f));

	_breathSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Breath Sound Component"));
	_breathSoundComponent->bAutoActivate = false;
	_breathSoundComponent->SetupAttachment(RootComponent);
	_breathSoundComponent->SetRelativeLocation(FVector(0.f, 0.0f, 34.0f));

	_footstepsSounds = nullptr;
	_breathSounds = nullptr;
	_bBreathPlayed = true;
}

void ASophia::BeginPlay()
{
	Super::BeginPlay();

	_playerController = UGameplayStatics::GetPlayerController(this, 0);

	if (_playerController) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(_playerController->GetLocalPlayer()))
			Subsystem->AddMappingContext(_mainMappingContext, 0);
	}

	// Inventory Widget
	_inventoryHUD = CreateWidget<UInventoryWidget>(_playerController, _inventoryHUDClass);
	_inventory->_onInventoryUpdated.AddDynamic(_inventoryHUD, &UInventoryWidget::UpdateInventory);

	_cameraLocation = _springArmComponent->GetRelativeLocation();

	// Timeline
	if (_curveFloat) {
		_curveFloat->FloatCurve.UpdateOrAddKey(0.f, 0.f);
		_curveFloat->FloatCurve.UpdateOrAddKey(1.f, 1.f);

		FRichCurve& curveFloatKeys = _curveFloat->FloatCurve;
		for (auto& key : curveFloatKeys.Keys) {
			key.InterpMode = ERichCurveInterpMode::RCIM_Cubic;
		}

		_timelineCallback.BindUFunction(this, FName("CrouchingTimeline"));

		_timelineComponent->AddInterpFloat(_curveFloat, _timelineCallback);
	}

	// Audio
	if (_footstepsSounds != nullptr && _footstepsSoundComponent != nullptr)
		_footstepsSoundComponent->SetSound(_footstepsSounds);
	if (_breathSounds != nullptr && _breathSoundComponent != nullptr)
		_breathSoundComponent->SetSound(_breathSounds);
}

void ASophia::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	// STAMINA
	switch (_staminaStatus) {
	case ST_RUNNING:
		if (_inventory->_currentHandItem) {
			if (_inventory->_currentHandItem->_bNoSwitchableItem)
				_staminaStatus = ST_IDLE;
		}

		if (_bIsRunning) {
			_speed = 0.5f;

			if (_staminaTimer < _staminaMax) {
				_staminaTimer += deltaTime;
			} else {
				_staminaStatus = ST_EXHAUSTED;
				_speed = 0.3f;
				_breathSoundComponent->SetIntParameter("Breath Sound", 0);
				_breathSoundComponent->Play();
			}
		} else if (_bIsCrouching) {
			_speed = 0.15f;
		} else {
			_staminaStatus = ST_IDLE;
			_speed = 0.3f;
		}
		break;
	case ST_EXHAUSTED:
		if (_bIsRunning && _staminaTimer < (_staminaMax / 2))
			_staminaStatus = ST_RUNNING;

		_staminaTimer -= deltaTime;

		if (_inventory->_currentHandItem) {
			if (_inventory->_currentHandItem->_bNoSwitchableItem)
				_staminaStatus = ST_IDLE;
		}

		if (_staminaTimer < 0.f && !_bIsRunning)
			_staminaStatus = ST_IDLE;
		break;
	case ST_IDLE:
		if (_inventory->_currentHandItem) {
			if (_inventory->_currentHandItem->_bNoSwitchableItem) {
				_speed = _inventory->_currentHandItem->_noSwitchableItemSpeed;
			} else {
				if (_bIsRunning) 
					_staminaStatus = ST_RUNNING;
				else 
					_speed = 0.3f;
			}
		} else {
			if (_bIsRunning)
				_staminaStatus = ST_RUNNING;
			else 
				_speed = 0.3f;
		}
		break;
	default:
		break;
	}

	// CAMERA SHAKE AND FOOTSTEPS
	if (GetVelocity().SizeSquared() <= 0.1f) {
		_playerController->ClientStartCameraShake(_shakeIdle, 1.f);
		
		if (_footstepsSoundComponent->IsPlaying())
			_footstepsSoundComponent->Stop();
	} else if (_speed == 0.3f) {
		_playerController->ClientStartCameraShake(_shakeWalk, 1.f);

		if (!_footstepsSoundComponent->IsPlaying()) {
			_footstepsSoundComponent->SetFloatParameter("InputBPM", 100.f);
			_footstepsSoundComponent->Play();
		}
	} else if (_speed == 0.5f) {
		_playerController->ClientStartCameraShake(_shakeRun, 1.f);

		if (!_footstepsSoundComponent->IsPlaying()) {
			_footstepsSoundComponent->SetFloatParameter("InputBPM", 140.f);
			_footstepsSoundComponent->Play();
		}
	}

	// Blend time of the camera (ASohpia::BlendWithCamera)
	if (_myGameState->_onBlendTime > 0.001f)
		_myGameState->_onBlendTime -= deltaTime;
	if (_myGameState->_onBlendTime < 0.001f)
		_myGameState->_onBlendTime = 0.001f;

	// CROUCHING
	if (_bCheckCrouchingUntilNot) {
		FHitResult hit;
		FVector start = GetCapsuleComponent()->GetComponentLocation();
		FVector end = GetCapsuleComponent()->GetComponentRotation().Vector().UpVector * 200.f;
		end += start;

		if (!GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility, FComponentQueryParams::DefaultQueryParam, FCollisionResponseParams::DefaultResponseParam)) {
			_timelineComponent->Reverse();
			_bIsCrouching = false;
			_bCheckCrouchingUntilNot = false;
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
		EnhancedInputComponent->BindAction(_runAction, ETriggerEvent::Triggered, this, &ASophia::Running);
		EnhancedInputComponent->BindAction(_crouchAction, ETriggerEvent::Triggered, this, &ASophia::Crouching);
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
		EnhancedInputComponent->BindAction(_inspectAction, ETriggerEvent::Triggered, _inventory, &UInventoryComponent::InspectItem);
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

void ASophia::Running(const FInputActionValue &value)
{
	// Activate the running status
	if (_bIsCrouching)
		_bIsRunning = false;
	else 
		_bIsRunning = value.Get<bool>();
}

void ASophia::Crouching(const FInputActionValue& value)
{
	// Activate the crouching status
	bool tempValue = value.Get<bool>();

	if (tempValue) {
		_timelineComponent->Play();
		_bIsCrouching = true;
	} else {
		FHitResult hit;
		FVector start = GetCapsuleComponent()->GetComponentLocation();
		FVector end = GetCapsuleComponent()->GetComponentRotation().Vector().UpVector * 200.f;
		end += start;

		if (GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility, FComponentQueryParams::DefaultQueryParam, FCollisionResponseParams::DefaultResponseParam)) {
			_timelineComponent->Play();
			_bIsCrouching = true;
			_bCheckCrouchingUntilNot = true;
		} else {
			_timelineComponent->Reverse();
			_bIsCrouching = false;
		}
	}
}

void ASophia::CrouchingTimeline(float value)
{
	_timelineValue = _timelineComponent->GetPlaybackPosition();
	_curveFloatValue = _curveFloat->GetFloatValue(_timelineValue);

	GetCapsuleComponent()->SetCapsuleHalfHeight(FMath::Lerp(88.f, 54.f, _curveFloatValue));
}

void ASophia::Inventory(const FInputActionValue& value)
{
	//if (IsLocallyControlled() && _inventoryHUDClass) {
	//	if (!_bInventoryOpen) {
	//		// Show the inventory 
	//		_inventoryHUD->AddToPlayerScreen();
	//		_playerController->bShowMouseCursor = true;
	//		_playerController->bEnableClickEvents = true;
	//		_playerController->bEnableMouseOverEvents = true;

	//		_bInventoryOpen = true;
	//	} else {
	//		// Close the inventory widget
	//		_inventoryHUD->RemoveFromParent();
	//		_playerController->bShowMouseCursor = false;
	//		_playerController->bEnableClickEvents = false;
	//		_playerController->bEnableMouseOverEvents = false;

	//		_bInventoryOpen = false;
	//	}
	//}
}

void ASophia::OnAction(const FInputActionValue &value)
{
	if (_inventory->_currentHandItem) {
		if (IOnActionInterface* onActionItem = Cast<IOnActionInterface>(_inventory->_currentHandItem)) {
			onActionItem->OnAction();
		}
	}

	if (_inventory->_bInspectingPressed && _inventory->_bHoldingItem)
		return;

	if (_inventory->_currentChangeCameraItem) {
		if (_inventory->_currentChangeCameraItem->GetClass()->ImplementsInterface(UActorBlendInterface::StaticClass())) {
			IActorBlendInterface* changeCameraItem = Cast<IActorBlendInterface>(_inventory->_currentChangeCameraItem);

			if (changeCameraItem->_cameraActorBlend) {
				if (IInteractiveInterface* cameraItem = Cast<IInteractiveInterface>(changeCameraItem))
					cameraItem->UseInteraction(nullptr);

				if (ICameraBlendInterface* item = Cast<ICameraBlendInterface>(changeCameraItem->_cameraActorBlend))
					item->UseInteraction(changeCameraItem->_cameraActorBlend);
			}
		}
	} else if (_inventory->_currentItemInSight) {
		if (IInteractiveInterface* interactiveItem = Cast<IInteractiveInterface>(_inventory->_currentItemInSight)) {
			interactiveItem->UseInteraction(_inventory->_currentItemInSight);
		} else if (IPickUpInterface* pickUpItem = Cast<IPickUpInterface>(_inventory->_currentItemInSight)) {
			pickUpItem->PickUpItem(_inventory->_currentItemInSight);
		}
	} else if (_inventory->_currentAnimatedItemInSight) {
		if (AItem* interactiveItem = Cast<AItem>(_inventory->_currentAnimatedItemInSight))
			_inventory->_currentAnimatedItemInSight->UseInteraction(interactiveItem);
		else 
			_inventory->_currentAnimatedItemInSight->UseInteraction(nullptr);
	} 
}

void ASophia::DropItem(const FInputActionValue& value)
{
	if (_inventory->_currentHandItem != nullptr)
		if (!_inventory->_currentHandItem->_bNoSwitchableItem)
			if (Cast<IPickUpInterface>(_inventory->_currentHandItem))
				Cast<IPickUpInterface>(_inventory->_currentHandItem)->DropItem(_inventory->_currentHandItem);
}

void ASophia::ToggleMovement(bool& bInspecting)
{
	_bCanMove = !_bCanMove;
	bInspecting = !bInspecting;
	_cameraComponent->bUsePawnControlRotation = ~_cameraComponent->bUsePawnControlRotation;
	bUseControllerRotationYaw = ~bUseControllerRotationYaw;
}

AItem* ASophia::GetCurrentHandItem()
{
	return _inventory->_currentHandItem;
}