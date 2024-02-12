#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

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
