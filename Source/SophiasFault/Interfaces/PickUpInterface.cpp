#include "PickUpInterface.h"
#include "../Inventory/Item.h"
#include "../Inventory/InventoryComponent.h"
#include "Camera/CameraComponent.h"

void IPickUpInterface::PickUpItem(AItem* item)
{
	if (item->_itemComponent) {
		item->_itemComponent->SetEnableGravity(false);
		item->_itemComponent->SetSimulatePhysics(false);
		item->_itemComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		item->_itemComponent->SetStaticMesh(item->_meshComponent->GetStaticMesh());
	}

	item->_owningInventory->AddItem(item);
	item->_meshComponent->SetVisibility(false);
}

void IPickUpInterface::DropItem(AItem* item)
{
	item->_meshComponent->SetVisibility(true);
	item->_meshComponent->SetEnableGravity(true);
	item->_meshComponent->SetSimulatePhysics(true);
	item->_meshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	item->_meshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	FVector forwardVector = item->_playerCamera->GetForwardVector();

	item->_meshComponent->AddForce(forwardVector * 40000 * item->_meshComponent->GetMass());

	item->_owningInventory->RemoveItem(item);
}

void IPickUpInterface::RotateItem(AItem* item)
{
	item->_itemComponent->AddWorldRotation(FQuat(_controlRotation));
	_controlRotation = FRotator::ZeroRotator;
}