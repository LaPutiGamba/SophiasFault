#pragma once

#include "GameFramework/Actor.h"
#include "Item.generated.h"

/**
 *
 */
UCLASS(Abstract)
class SOPHIASFAULT_API AItem : public AActor
{
	GENERATED_BODY()

public:
	AItem();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* _meshComponent;

	UPROPERTY(EditAnywhere)
	class USceneComponent* _itemComponent;

	class UInventoryComponent* _owningInventory;

	class ACharacter* _myCharacter;
	class UCameraComponent* _playerCamera;
};