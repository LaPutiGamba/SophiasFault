#pragma once

#include "Item.h"
#include "ItemPhysic.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API AItemPhysic : public AItem
{
	GENERATED_BODY()
	
public:
	AItemPhysic();

	UPROPERTY(EditAnywhere)
	FString _name;

	FRotator _controlRotation;
	FVector _forwardVector;

	void RotateItem();

	virtual void PickUpItem();
	void DropItem();
	void SetVisibility(bool value);
};
