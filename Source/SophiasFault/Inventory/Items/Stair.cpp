#include "Stair.h"
#include "Components/ShapeComponent.h" 
#include "../InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../../Sophia.h"

AStair::AStair()
{
	_name = "Stair";
	_triggered = false;

	_bNoSwitchableItem = true;
}

void AStair::BeginPlay()
{
	Super::BeginPlay();

	if (_finalPosition) {
		_finalPosition->SetActorHiddenInGame(true);
		_finalPosition->SetActorEnableCollision(false);
	}
}

void AStair::OnAction()
{
	if (_triggered) {
		_owningInventory->RemoveItem(_owningInventory->_currentHandItem, false);
		_owningInventory->_currentHandItem->Destroy();
		_owningInventory->_currentHandItem = nullptr;
		_owningInventory->_bHoldingItem = !_owningInventory->_bHoldingItem;

		_finalPosition->SetActorHiddenInGame(false);
		_finalPosition->SetActorEnableCollision(true);
		UStaticMeshComponent* staticMesh = _finalPosition->GetComponentByClass<UStaticMeshComponent>();
		staticMesh->SetMaterial(0, _defaultMaterial);
		_bNoSwitchableItem = false;
	}
}