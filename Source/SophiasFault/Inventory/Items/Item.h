#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Item.generated.h"

/**
 *
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInLineNew, DefaultToInstanced)
class SOPHIASFAULT_API UItem : public UObject
{
	GENERATED_BODY()

public:
	UItem();

	virtual class UWorld* GetWorld() const { return _world; };

	UPROPERTY(Transient)
	class UWorld* _world;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FText _useActionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	class UStaticMesh* _pickUpMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	class UTexture2D* _thumbnail;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FText _itemDisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (MultiLine = true))
	FText _itemDescription;

	UPROPERTY()
	class UInventoryComponent* _owningInventory;

	virtual void Use(class ASophia* character);

	UFUNCTION(BlueprintImplementableEvent)
	void OnUse(class ASophia* character);
};