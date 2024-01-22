#include "InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Items/MirrorLight.h"
#include "Items/Stair.h"
#include "Items/Flashlight.h"
#include "Items/ActorBlendCamera.h"
#include "../Interfaces/InteractiveInterface.h"
#include "../Interfaces/PickUpInterface.h"
#include "../Interfaces/OnActionInterface.h"
#include "../Core/GMS_MyGameStateBase.h"
#include "../Sophia.h"
#include "../Macros.h"

UInventoryComponent::UInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
	_capacity = 10;

    _itemInspectDistance = 120.f;

    _bHoldingItem = false;
    _bInspecting = false;

	_currentItemSlotIndex = 0;
	_currentItemInSight = nullptr;
    _flashlightItem = nullptr;
	_currentChangeCameraItem = nullptr;
	_currentHandItem = nullptr;
	_cameraComponent = nullptr;
	_holdingComponent = nullptr;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

    for (int i = 0; i < _capacity; i++) {
        _items.Add(nullptr);
    }

    _cameraComponent = Cast<ASophia>(GetWorld()->GetFirstPlayerController()->GetPawn())->GetCameraComponent();
    _holdingComponent = Cast<ASophia>(GetWorld()->GetFirstPlayerController()->GetPawn())->GetHoldingComponent();
}

void UInventoryComponent::TickComponent(float deltaTime, enum ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
    Super::TickComponent(deltaTime, tickType, thisTickFunction);

    // ITEMS (PHYSICS AND STATICS)
    // Calculations to know the distance and in what point the player has to look to detect an Item.
    if (_cameraComponent != nullptr)
        _start = _cameraComponent->GetComponentLocation();
    if (_cameraComponent != nullptr)
        _forwardVector = _cameraComponent->GetForwardVector();
    _end = ((_forwardVector * 200.f) + _start);

    // CHECK FOR ITEMS
    if (!_bHoldingItem) {
        if (GetWorld()->LineTraceSingleByChannel(_hit, _start, _end, ECC_Visibility, FComponentQueryParams::DefaultQueryParam, FCollisionResponseParams::DefaultResponseParam)) {
            // If the player has in sight the Flashlight saves a pointer to that Actor.
            if (_hit.GetActor()->IsA<AFlashlight>()) {
                _flashlightItem = Cast<AFlashlight>(_hit.GetActor());
                return;
            }

            // If the player has in sight the Piano Seat or the Earth Ball, the _currentChangeCameraItem will save a pointer to that Actor.
            if (_hit.GetActor()->IsA<AActorBlendCamera>()) {
                _currentChangeCameraItem = Cast<AActorBlendCamera>(_hit.GetActor());
                return;
            }

            // If the player has in sight an Item saves a pointer to that Actor.
            if (_hit.GetActor())
                _currentItemInSight = Cast<AItem>(_hit.GetActor());
        } else {
            // If the player doesn't has in sight any item, all the items are null.
            _flashlightItem = nullptr;
            _currentChangeCameraItem = nullptr;
            _currentItemInSight = nullptr;
        }
    }

    // INSPECTING ITEMS
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
                if (Cast<IPickUpInterface>(_currentHandItem))
                    Cast<IPickUpInterface>(_currentHandItem)->RotateItem(_currentHandItem);
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

    // NO SWITCHEABLE ITEMS
    if (_currentHandItem != nullptr) {
        if (_currentHandItem->_bNoSwitchableItem) {
            if (Cast<AStair>(_currentHandItem)) {
                AStair* stair = Cast<AStair>(_currentHandItem);

                if (stair->_triggered)
                    stair->_finalPoistion->SetActorHiddenInGame(false);
                else
                    stair->_finalPoistion->SetActorHiddenInGame(true);
            }
        }
    }
}

bool UInventoryComponent::AddItem(AItem* item)
{
	//if (_items.Num() > _capacity || !item) return false;

	for (int i = 0; i < _capacity; i++) {
		if (_items[i] == nullptr) {
			_items[i] = item;
			break;
		}
	}
		
	_onInventoryUpdated.Broadcast();

    _bHoldingItem = !_bHoldingItem;
    _currentHandItem = item;

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

		_onInventoryUpdated.Broadcast();

        _bHoldingItem = !_bHoldingItem;

        if (deleteCurrentHandItem)
            _currentHandItem = nullptr;

		return true;
	}

	return false;
}

void UInventoryComponent::ChangeCurrentHandItem(const FInputActionValue& value, int index)
{
    if (_currentHandItem)
        if (_currentHandItem->_bNoSwitchableItem) return;

    // Hide the actual item in the _inventory and in the hand
    if (_items[_currentItemSlotIndex] != nullptr) {
        _items[_currentItemSlotIndex]->SetActorHiddenInGame(true);
        _currentHandItem->SetActorHiddenInGame(true);
    }

    // Check if the input is from a number key (0-9)
    if (index >= 0 && index <= 9) {
        _currentItemSlotIndex = index;
    // Check if the input is from the mouse wheel axis
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
    if (_items[_currentItemSlotIndex] != nullptr) {
        _items[_currentItemSlotIndex]->SetActorHiddenInGame(false);
        _currentHandItem = _items[_currentItemSlotIndex];
        _currentHandItem->SetActorHiddenInGame(false);
        _bHoldingItem = true;
    // If the slot is empty, we will set the _currentHandItem to nullptr
    } else {
        _currentHandItem = nullptr;
        _bHoldingItem = false;
    }
}