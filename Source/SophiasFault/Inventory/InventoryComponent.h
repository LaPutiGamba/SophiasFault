#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOPHIASFAULT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	virtual void BeginPlay() override;

	bool AddItem(class AItemPhysic* item);
	bool RemoveItem(class AItemPhysic* item);

	UPROPERTY(EditAnywhere)
	UStaticMesh* prueba;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 _capacity;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated _onInventoryUpdated;

	UPROPERTY(EditAnywhere, Category = "Items")
	TArray<class AItemPhysic*> _items;
};