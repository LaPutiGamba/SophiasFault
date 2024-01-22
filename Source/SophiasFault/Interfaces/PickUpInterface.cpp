#include "PickUpInterface.h"
#include "../Inventory/Item.h"
#include "../Inventory/InventoryComponent.h"
#include "Camera/CameraComponent.h"

void IPickUpInterface::PickUpItem(AItem* item)
{
	item->_meshComponent->SetEnableGravity(false);
	item->_meshComponent->SetSimulatePhysics(false);
	item->_meshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (item->_itemComponent) {
		item->_meshComponent->AttachToComponent(item->_itemComponent, FAttachmentTransformRules::KeepWorldTransform);
		item->SetActorLocation(item->_itemComponent->GetComponentLocation());
	}

	item->_owningInventory->AddItem(item);
}

void IPickUpInterface::DropItem(AItem* item, UCameraComponent* cameraComponent)
{
	item->_meshComponent->SetEnableGravity(true);
	item->_meshComponent->SetSimulatePhysics(true);
	item->_meshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	item->_meshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	FVector _forwardVector = cameraComponent->GetForwardVector();

	item->_meshComponent->AddForce(_forwardVector * 40000 * item->_meshComponent->GetMass());

	item->_owningInventory->RemoveItem(item);
}

void IPickUpInterface::RotateItem(AItem* item)
{
	item->AddActorWorldRotation(FQuat(_controlRotation));
	_controlRotation = FRotator::ZeroRotator;
}