#include "InventoryComponent.h"
#include "Items/Item.h"

UInventoryComponent::UInventoryComponent()
{
	_capacity = 10;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	for (size_t i = 0; i < _defaultItems.Num(); i++)
	{
		AddItem(_defaultItems[i]);
	}
}

bool UInventoryComponent::AddItem(UItem* item)
{
	if (_items.Num() >= _capacity || !item) return false;


	item->_owningInventory = this;
	item->_world = GetWorld();
	_items.Add(item);

	_onInventoryUpdated.Broadcast();

	return true;
}

bool UInventoryComponent::RemoveItem(UItem* item)
{
	if (item) {
		item->_owningInventory = nullptr;
		item->_world = nullptr;
		_items.RemoveSingle(item);
		_onInventoryUpdated.Broadcast();
		return true;
	}
	return false;
}