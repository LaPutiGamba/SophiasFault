#include "InventoryComponent.h"
#include "Items/ItemPhysic.h"
#include "../Macros.h"

UInventoryComponent::UInventoryComponent()
{
	_capacity = 10;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < _capacity; i++) {
		_items.Add(nullptr);
	}
}

bool UInventoryComponent::AddItem(AItemPhysic* item)
{
	if (_items.Num() > _capacity || !item) return false;

	item->_owningInventory = this;

	for (int i = 0; i < _capacity; i++) {
		if (_items[i] == nullptr) {
			_items[i] = item;
			break;
		}
	}
		
	_onInventoryUpdated.Broadcast();

	return true;
}

bool UInventoryComponent::RemoveItem(AItemPhysic* item)
{
	if (item) {
		item->_owningInventory = nullptr;
		
		for (int i = 0; i < _capacity; i++) {
			if (_items[i] == item) {
				_items[i] = nullptr;
				break;
			}
		}

		_onInventoryUpdated.Broadcast();

		return true;
	}

	return false;
}

void UInventoryComponent::ChangeCurrentHandItem(const FInputActionValue& value, int index)
{
    if (_bNoSwitchableItem) return;

    // Hide the actual item in the _inventory and in the hand
    if (_items[_currentItemSlotIndex] != nullptr) {
        _items[_currentItemSlotIndex]->SetVisibility(false);
        _currentHandItem->SetVisibility(false);
    }

    // Check if the input is from a number key (0-9)
    if (index != 10) {
        // If the index is valid, change the item to that slot
        if (index >= 0 && index <= 9) {
            _currentItemSlotIndex = index;
        }
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
        _items[_currentItemSlotIndex]->SetVisibility(true);
        _currentHandItem = _items[_currentItemSlotIndex];
        _currentHandItem->SetVisibility(true);
        _bHoldingItem = true;
    // If the slot is empty, we will set the _currentHandItem to nullptr
    } else {
        _currentHandItem = nullptr;
        _bHoldingItem = false;
    }
}