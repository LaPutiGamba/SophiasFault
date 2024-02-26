#include "Stair.h"
#include "Components/ShapeComponent.h" 
#include "../InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../../Core/GMS_MyGameStateBase.h"
#include "../../Sophia.h"

AStair::AStair()
{
	_name = "Stair";

	_bNoSwitchableItem = true;
	_triggered = false;
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
		_owningInventory->RemoveItem(this);
		Destroy();
		_owningInventory->_currentHandItem = nullptr;

		_finalPosition->SetActorHiddenInGame(false);
		_finalPosition->SetActorEnableCollision(true);
		_bNoSwitchableItem = false;

		_myGameState->_bStairPositioned = true;
	}
}

void AStair::OnTriggerStart()
{
	_triggered = true;
}

void AStair::OnTriggerEnd()
{
	_triggered = false;
}