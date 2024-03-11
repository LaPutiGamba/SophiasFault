#include "PickUpInterface.h"
#include "../Inventory/Item.h"
#include "../Inventory/InventoryComponent.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"

void IPickUpInterface::PickUpItem(AItem* item)
{
	item->_meshComponent->SetVisibility(false);
	item->_meshComponent->SetEnableGravity(false);
	item->_meshComponent->SetSimulatePhysics(false);
	item->_meshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (UBoxComponent* boxCollision = Cast<UBoxComponent>(item->GetComponentByClass(UBoxComponent::StaticClass()))) {
		boxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (item->_itemComponent) {
		item->_itemComponent->SetEnableGravity(false);
		item->_itemComponent->SetSimulatePhysics(false);
		item->_itemComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		item->_itemComponent->SetStaticMesh(item->_meshComponent->GetStaticMesh());
		item->_itemComponent->SetRelativeRotation(item->_pickUpRotation);
		item->_itemComponent->SetRelativeLocation(item->_pickUpLocation);

		for (int i = 0; i < item->_itemComponent->GetNumChildrenComponents(); i++) {
			if (UBoxComponent* boxCollision = Cast<UBoxComponent>(item->_itemComponent->GetChildComponent(i))) {
				boxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				break;
			}
		}
	}

	if (item->_owningInventory->_currentHandItem)
		item->_owningInventory->AddItem(item, false);
	else 
		item->_owningInventory->AddItem(item);
}

void IPickUpInterface::DropItem(AItem* item)
{
	item->SetActorLocation(item->_itemComponent->GetComponentLocation());
	item->_meshComponent->SetVisibility(true);
	item->_meshComponent->SetEnableGravity(true);
	item->_meshComponent->SetSimulatePhysics(true);
	item->_meshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	if (UBoxComponent* boxCollision = Cast<UBoxComponent>(item->GetComponentByClass(UBoxComponent::StaticClass()))) {
		boxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

	FVector forwardVector = item->_playerCamera->GetForwardVector();
	item->_meshComponent->AddForce(forwardVector * 40000 * item->_meshComponent->GetMass());

	for (int i = 0; i < item->_itemComponent->GetNumChildrenComponents(); i++) {
		if (UBoxComponent* boxCollision = Cast<UBoxComponent>(item->_itemComponent->GetChildComponent(i))) {
			boxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;
		}
	}

	item->_owningInventory->RemoveItem(item);
}

void IPickUpInterface::RotateItem(AItem* item)
{
	item->_itemComponent->AddWorldRotation(FQuat(_controlRotation));
	_controlRotation = FRotator::ZeroRotator;
}