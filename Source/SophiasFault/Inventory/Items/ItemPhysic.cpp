#include "ItemPhysic.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "../../Macros.h"

AItemPhysic::AItemPhysic()
{
	_meshComponent->SetSimulatePhysics(true);
	_name = "Item Name";
}

void AItemPhysic::RotateItem()
{
	AddActorWorldRotation(FQuat(_controlRotation));
	_controlRotation = FRotator::ZeroRotator;
}

void AItemPhysic::PickUpItem()
{
	_meshComponent->SetEnableGravity(false);
	_meshComponent->SetSimulatePhysics(false);
	_meshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (_itemComponent) {
		_meshComponent->AttachToComponent(_itemComponent, FAttachmentTransformRules::KeepWorldTransform);
		SetActorLocation(_itemComponent->GetComponentLocation());
	}
}

void AItemPhysic::DropItem()
{
	_meshComponent->SetEnableGravity(true);
	_meshComponent->SetSimulatePhysics(true);
	_meshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	_meshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	_forwardVector = _playerCamera->GetForwardVector();

	_meshComponent->AddForce(_forwardVector * 40000 * _meshComponent->GetMass());
}

void AItemPhysic::SetVisibility(bool value)
{
	SetActorHiddenInGame(!value);
}