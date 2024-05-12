#include "InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Items/Mirror Light/MirrorLight.h"
#include "Items/Stair.h"
#include "Items/Flashlight.h"
#include "AnimatedItem.h"
#include "../Interfaces/InteractiveInterface.h"
#include "../Interfaces/PickUpInterface.h"
#include "../Interfaces/OnActionInterface.h"
#include "../Interfaces/ActorBlendInterface.h"
#include "../Core/GMS_MyGameStateBase.h"
#include "Components/SpotLightComponent.h"
#include "Blueprint/UserWidget.h"
#include "Items/Notes.h"
#include "KeysHUDWidget.h"
#include "../Sophia.h"
#include "../Macros.h"

UInventoryComponent::UInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
	_capacity = 10;

    _bHoldingItem = false;
    _bInspecting = false;
    _bInspectingPressed = false;
	_bChangeFlashlightVisible = false;

	_currentItemSlotIndex = 0;
    _currentItemInSight = nullptr;
    _currentAnimatedItemInSight = nullptr;
	_currentChangeCameraItem = nullptr;
	_currentHandItem = nullptr;
	_cameraComponent = nullptr;
	_holdingComponent = nullptr;

    _bShowNoteHUDOnce = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

    for (int i = 0; i < _capacity; i++) {
        _items.Add(nullptr);
    }

    _playerController = GetWorld()->GetFirstPlayerController();
	_sophia = Cast<ASophia>(_playerController->GetPawn());
    _cameraComponent = _sophia->GetCameraComponent();
    _holdingComponent = _sophia->GetHoldingComponent();
	_flashlightCrankComponent = _sophia->GetFlashlightCrankComponent();
	_flashlightCrankHandleComponent = _sophia->GetFlashlightCrankHandleComponent();
    _flashlightComponent = _sophia->GetFlashlightComponent();

    if (_playerController->PlayerCameraManager)
        _pitchMax = _playerController->PlayerCameraManager->ViewPitchMax;
    if (_playerController->PlayerCameraManager)
        _pitchMin = _playerController->PlayerCameraManager->ViewPitchMin;

    _myGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AGMS_MyGameStateBase>() : nullptr;
}

void UInventoryComponent::TickComponent(float deltaTime, enum ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
    Super::TickComponent(deltaTime, tickType, thisTickFunction);

    // ITEMS 
    // Calculations to know the distance and in what point the player has to look to detect an Item.
    if (_cameraComponent != nullptr)
        _start = _cameraComponent->GetComponentLocation();
    if (_cameraComponent != nullptr)
        _forwardVector = _cameraComponent->GetForwardVector();
    _end = ((_forwardVector * 200.f) + _start);

    // CHECK FOR ITEMS
    if (GetWorld()->LineTraceSingleByChannel(_hit, _start, _end, ECC_Visibility, FComponentQueryParams::DefaultQueryParam, FCollisionResponseParams::DefaultResponseParam)) {
        // If the player has in sight a Current Change Camera Item saves a pointer to that Actor.
        if (_hit.GetActor()->GetClass()->ImplementsInterface(UActorBlendInterface::StaticClass())) {
            _currentChangeCameraItem = _hit.GetActor();

            if (_myGameState->_hudWidget != nullptr) {
                _myGameState->_hudWidget->GetWidgetFromName("NoVisibleItem")->SetVisibility(ESlateVisibility::Hidden);
                _myGameState->_hudWidget->GetWidgetFromName("PickUpItem")->SetVisibility(ESlateVisibility::Hidden);
                _myGameState->_hudWidget->GetWidgetFromName("VisibleItem")->SetVisibility(ESlateVisibility::Visible);
            }

        // If the player has in sight an Item saves a pointer to that Actor.
        } else if (_hit.GetActor()->GetClass()->ImplementsInterface(UInteractiveInterface::StaticClass()) ||
            _hit.GetActor()->GetClass()->ImplementsInterface(UPickUpInterface::StaticClass()) ||
            _hit.GetActor()->GetClass()->ImplementsInterface(UOnActionInterface::StaticClass())) {
            _currentItemInSight = Cast<AItem>(_hit.GetActor());
            _currentAnimatedItemInSight = Cast<IInteractiveInterface>(_hit.GetActor());

            if (_myGameState->_hudWidget != nullptr) {
                _myGameState->_hudWidget->GetWidgetFromName("NoVisibleItem")->SetVisibility(ESlateVisibility::Hidden);

                if (_hit.GetActor()->GetClass()->ImplementsInterface(UPickUpInterface::StaticClass())) {
					_myGameState->_hudWidget->GetWidgetFromName("PickUpItem")->SetVisibility(ESlateVisibility::Visible);
					_myGameState->_hudWidget->GetWidgetFromName("VisibleItem")->SetVisibility(ESlateVisibility::Hidden);
				} else {
					_myGameState->_hudWidget->GetWidgetFromName("PickUpItem")->SetVisibility(ESlateVisibility::Hidden);
					_myGameState->_hudWidget->GetWidgetFromName("VisibleItem")->SetVisibility(ESlateVisibility::Visible);
				}
            }

        // If the player doesn't has in sight any item, all the items are null.
        } else {
            _currentChangeCameraItem = nullptr;
            _currentItemInSight = nullptr;
            _currentAnimatedItemInSight = nullptr;

            if (_myGameState->_hudWidget != nullptr) {
                _myGameState->_hudWidget->GetWidgetFromName("NoVisibleItem")->SetVisibility(ESlateVisibility::Visible);
                _myGameState->_hudWidget->GetWidgetFromName("VisibleItem")->SetVisibility(ESlateVisibility::Hidden);
				_myGameState->_hudWidget->GetWidgetFromName("PickUpItem")->SetVisibility(ESlateVisibility::Hidden);
            }
        }
    // If the player doesn't has in sight any item, all the items are null
    } else {
        _currentChangeCameraItem = nullptr;
        _currentItemInSight = nullptr;
        _currentAnimatedItemInSight = nullptr;

        if (_myGameState->_hudWidget != nullptr) {
            _myGameState->_hudWidget->GetWidgetFromName("NoVisibleItem")->SetVisibility(ESlateVisibility::Visible);
            _myGameState->_hudWidget->GetWidgetFromName("VisibleItem")->SetVisibility(ESlateVisibility::Hidden);
			_myGameState->_hudWidget->GetWidgetFromName("PickUpItem")->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    // INSPECTING ITEMS
    if (_bInspecting) {
        if (_bHoldingItem) {
            // If it's pressing the inspecting key and it's holding any item, it will put the field of view to the screen.
            _cameraComponent->SetFieldOfView(FMath::Lerp(_cameraComponent->FieldOfView, 90.f, 0.1f));

            // Also we get a vector calculating where to put the item. In this case, in the middle of the screen.
            if (_currentHandItem != nullptr) {
                FVector holdingComponentVector = (_cameraComponent->GetComponentLocation() + (_cameraComponent->GetForwardVector() * _currentHandItem->_inspectDistance));
                _holdingComponent->SetWorldLocation(holdingComponentVector);

                // We set the maximum and the minimum pith view to the highest possible values to be able to rotate the object 360�.
                GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax = 179.9000002f;
                GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin = -179.9000002f;

                // If we are holding an Item, we will call his rotate function.
                if (Cast<IPickUpInterface>(_currentHandItem))
                    Cast<IPickUpInterface>(_currentHandItem)->RotateItem(_currentHandItem);

                if (_currentHandItem->IsA<ANotes>() && !_myGameState->_keysHudWidget->IsInViewport() && !_bShowNoteHUDOnce) {
                    _myGameState->_keysHudWidget->AddKeyToHorizontalBox(FText::FromString("E"), FText::FromString("Read"), true);
                    _myGameState->_keysHudWidget->ShowHUD(3.f, true);
                    _bShowNoteHUDOnce = true;
                }
            }
        }
    } else {
        // If it's not pressing the inspecting key we will put the field of view to the screen.
        _cameraComponent->SetFieldOfView(FMath::Lerp(_cameraComponent->FieldOfView, 90.f, 0.1f));

        // If it's not pressing the inspecting key and it's holding any item, it will put the item in the hand position.
        if (_bHoldingItem) {
            if (_currentHandItem != nullptr) {
                _currentHandItem->_itemComponent->SetRelativeRotation(_currentHandItem->_pickUpRotation);
                _holdingComponent->SetRelativeLocation(_currentHandItem->_pickUpLocation);
            }
        }
    }

    if (_currentHandItem != nullptr) {
        // NO SWITCHEABLE ITEMS
        if (_currentHandItem->_bNoSwitchableItem) {
            if (AStair* stair = Cast<AStair>(_currentHandItem)) {
                if (stair->_triggered)
                    stair->_finalPosition->SetActorHiddenInGame(false);
                else
                    stair->_finalPosition->SetActorHiddenInGame(true);
            }
        }
    }
}

bool UInventoryComponent::AddItem(AItem* item, bool toggleHoldingItem)
{
	for (int i = 0; i < _capacity; i++) {
		if (_items[i] == nullptr) {
			_items[i] = item;
			break;
		}
	}
		
	_onInventoryUpdated.Broadcast();

    if (toggleHoldingItem)
		_bHoldingItem = !_bHoldingItem;
    
    if (_currentHandItem != nullptr) {
        if (_currentHandItem->IsA(AFlashlight::StaticClass())) {
            _flashlightCrankComponent->SetVisibility(false);
            _flashlightCrankHandleComponent->SetVisibility(false);
        }
    }

    _currentHandItem = item;

	if (_currentHandItem->IsA(AFlashlight::StaticClass())) {
		_flashlightCrankComponent->SetVisibility(true);
		_flashlightCrankHandleComponent->SetVisibility(true);
    } 

	return true;
}

bool UInventoryComponent::RemoveItem(AItem* item, bool deleteCurrentHandItem)
{
	if (item) {
		for (int i = 0; i < _capacity; i++) {
			if (_items[i] == item) {
				_items[i] = nullptr;
				break;
			}
		}
        
        // Order the items in the inventory
		for (int i = 0; i < _capacity; i++) {
			if (_items[i] == nullptr) {
                for (int j = i + 1; j < _capacity; j++) {
                    if (_items[j] != nullptr) {
                        _items[i] = _items[j];
                        _items[j] = nullptr;
                        break;
                    }
                }
            }
        }

        _onInventoryUpdated.Broadcast();

        _bHoldingItem = !_bHoldingItem;

        if (deleteCurrentHandItem) {
            _currentHandItem = nullptr;
            _holdingComponent->SetStaticMesh(nullptr);
        }

        _currentItemSlotIndex = 0;
        if (_items[_currentItemSlotIndex] != nullptr) {
            _currentHandItem = _items[_currentItemSlotIndex];
            _currentHandItem->SetActorHiddenInGame(false);
            _holdingComponent->SetStaticMesh(_currentHandItem->_meshComponent->GetStaticMesh());
            _bHoldingItem = true;
        }

        return true;
    }

    return false;
}

void UInventoryComponent::ChangeCurrentHandItem(const FInputActionValue& value, int index)
{
    if (_currentHandItem)
        if (_currentHandItem->_bNoSwitchableItem) return;

    // Hide the actual item in the _inventory and in the hand
    if (_items[_currentItemSlotIndex] != nullptr && _currentHandItem != nullptr) {
        _items[_currentItemSlotIndex]->SetActorHiddenInGame(true);
        _currentHandItem->SetActorHiddenInGame(true);
        _holdingComponent->SetStaticMesh(nullptr);

        if (_currentHandItem->IsA(AFlashlight::StaticClass())) {
            _flashlightCrankComponent->SetVisibility(false);
            _flashlightCrankHandleComponent->SetVisibility(false);
        }
    }

    // Check if the input is from a number key (0-9)
    if (index >= 0 && index <= 9) {
        if (_items[index] != nullptr)
            _currentItemSlotIndex = index;
        // Check if the input is from the mouse wheel axis
    }
    else {
        float mouseAxis = value.Get<float>();

        if (mouseAxis > 0.f) {
            if ((_currentItemSlotIndex < 9)) {
                if (_items[_currentItemSlotIndex + 1] != nullptr)
                    _currentItemSlotIndex++;
            }
        }
        else if (mouseAxis < 0.f) {
            if ((_currentItemSlotIndex > 0)) {
                if (_items[_currentItemSlotIndex - 1] != nullptr)
                    _currentItemSlotIndex--;
            }
        }
    }

    // Show the new item in the _inventory and in the hand
    if (_items[_currentItemSlotIndex] != nullptr) {
        _items[_currentItemSlotIndex]->SetActorHiddenInGame(false);
        _currentHandItem = _items[_currentItemSlotIndex];
        _currentHandItem->SetActorHiddenInGame(false);
        _holdingComponent->SetStaticMesh(_currentHandItem->_meshComponent->GetStaticMesh());
        _bHoldingItem = true;

        if (_currentHandItem->IsA(AFlashlight::StaticClass())) {
            _flashlightCrankComponent->SetVisibility(true);
            _flashlightCrankHandleComponent->SetVisibility(true);
        }
    }
}

void UInventoryComponent::InspectItem(const FInputActionValue& value)
{
    _bInspectingPressed = !_bInspectingPressed;

    if (_currentHandItem) {
        if (_currentHandItem->_bNoSwitchableItem) return;

        if (_bInspectingPressed) {
            if (_bHoldingItem) {
                if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(_playerController->GetLocalPlayer())) {
                    subsystem->RemoveMappingContext(_sophia->GetMainMappingContext());
                    subsystem->AddMappingContext(_sophia->GetInspectMappingContext(), 0);
                }

                if (_sophia->GetFlashlightComponent()->IsVisible()) {
                    _sophia->GetFlashlightComponent()->SetVisibility(false);
					_bChangeFlashlightVisible = true;
                }

                _sophia->ToggleMovement(_bInspecting);
                _lastRotation = _playerController->GetPawn()->GetControlRotation();
               
                if (_myGameState->_hudWidget)
					_myGameState->_hudWidget->SetVisibility(ESlateVisibility::Hidden);

                _bShowNoteHUDOnce = false;
            } else {
                _bInspecting = true;
            }
        } else {
            if (_bInspecting && _bHoldingItem) {
                if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(_playerController->GetLocalPlayer())) {
                    subsystem->RemoveMappingContext(_sophia->GetInspectMappingContext());
                    subsystem->AddMappingContext(_sophia->GetMainMappingContext(), 0);
                }

                if (!_sophia->GetFlashlightComponent()->IsVisible() && _bChangeFlashlightVisible) {
                    _sophia->GetFlashlightComponent()->SetVisibility(true);
					_bChangeFlashlightVisible = false;
				}

                _sophia->ToggleMovement(_bInspecting);
                _playerController->SetControlRotation(_lastRotation);
                _playerController->PlayerCameraManager->ViewPitchMax = _pitchMax;
                _playerController->PlayerCameraManager->ViewPitchMin = _pitchMin;
               
                if (_myGameState->_hudWidget) 
					_myGameState->_hudWidget->SetVisibility(ESlateVisibility::Visible);
            } else {
                _bInspecting = false;
            }
        }
    }
}