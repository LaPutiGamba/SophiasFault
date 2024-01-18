#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h" 
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOPHIASFAULT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	UPROPERTY(EditAnywhere)
	UStaticMesh* prueba;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 _capacity;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated _onInventoryUpdated;

	UPROPERTY(EditAnywhere, Category = "Items")
	TArray<class AItemPhysic*> _items;

	int _currentItemSlotIndex;

	class AItemPhysic* _currentHandItem;

	bool _bHoldingItem;
	bool _bNoSwitchableItem;

	virtual void BeginPlay() override;

	bool AddItem(class AItemPhysic* item);
	bool RemoveItem(class AItemPhysic* item);

	void SetCurrentHandItem(class AItemPhysic* item) { _currentHandItem = item; };
	class AItemPhysic* GetCurrentHandItem() { return _currentHandItem; };

	void ChangeCurrentHandItem(const FInputActionValue& value, int index);
};