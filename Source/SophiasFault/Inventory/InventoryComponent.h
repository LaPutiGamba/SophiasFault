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

	class UCameraComponent* _cameraComponent;
	class USceneComponent* _holdingComponent;

	float _itemInspectDistance;

	FHitResult _hit;
	FVector _start;
	FVector _forwardVector;
	FVector _end;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 _capacity;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated _onInventoryUpdated;

	UPROPERTY(EditAnywhere, Category = "Items")
	TArray<class AItem*> _items;

	int _currentItemSlotIndex;

	class AItem* _currentHandItem;
	class AItem* _currentItemInSight;
	class AActorBlendCamera* _currentChangeCameraItem;
	class AFlashlight* _flashlightItem;

	bool _bHoldingItem;
	bool _bInspecting;

	virtual void BeginPlay() override;
	virtual void TickComponent(float deltaTime, enum ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;
	bool AddItem(class AItem* item);
	bool RemoveItem(class AItem* item, bool deleteCurrentHandItem = true);

	void SetCurrentHandItem(class AItem* item) { _currentHandItem = item; };
	class AItem* GetCurrentHandItem() { return _currentHandItem; };

	void ChangeCurrentHandItem(const FInputActionValue& value, int index);
};