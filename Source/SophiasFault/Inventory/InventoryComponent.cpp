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