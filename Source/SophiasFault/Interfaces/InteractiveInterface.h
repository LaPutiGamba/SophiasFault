#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractiveInterface.generated.h"

UINTERFACE(MinimalAPI, NotBlueprintable)
class UInteractiveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SOPHIASFAULT_API IInteractiveInterface
{
	GENERATED_BODY()

public:
	// INTERACTION
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void UseInteraction() {};
};
