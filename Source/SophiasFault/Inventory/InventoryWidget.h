#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

// We make the class abstract, as we don't want to create
// instances of this, instead we want to create instances
// of our UMG Blueprint subclass.
UCLASS(Abstract)
class SOPHIASFAULT_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	class ASophia* _playerCharacter;

	TSubclassOf<class UUserWidget> _itemsWidgetClass;

public:
	UFUNCTION()
	void UpdateInventory();
};
