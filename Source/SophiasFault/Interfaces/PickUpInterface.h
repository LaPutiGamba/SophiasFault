#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickUpInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPickUpInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SOPHIASFAULT_API IPickUpInterface
{
	GENERATED_BODY()

public:
	// Set _meshComponent->SetSimulatePhysics(true) on the Constructor

	FRotator _controlRotation;

	virtual void PickUpItem(class AItem* item);
	virtual void DropItem(class AItem* item, class UCameraComponent* cameraComponent);
	virtual void RotateItem(class AItem* item);
};
