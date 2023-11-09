#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
//#include "InputActionValue.h"
#include "InventoryWidget.generated.h"

// We make the class abstract, as we don't want to create
// instances of this, instead we want to create instances
// of our UMG Blueprint subclass.
UCLASS(Abstract)
class SOPHIASFAULT_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// Doing setup in the C++ constructor is not as useful as using NativeConstruct.
	virtual void NativeConstruct() override;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	//class UInputAction* _inventoryAction;

	//UPROPERTY(EditAnywhere, meta = (BindWidget))
	//class UInventoryComponent* _inventory;
};
