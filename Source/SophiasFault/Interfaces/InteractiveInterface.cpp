#include "InteractiveInterface.h"
#include "../Inventory/Item.h"
#include "Kismet/GameplayStatics.h"

void IInteractiveInterface::UseInteraction(AItem* item)
{
	if (item == nullptr)
		return;

	if (item->_interactSound)
		UGameplayStatics::PlaySoundAtLocation(item, item->_interactSound, item->GetActorLocation());
}