#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractiveInterface.generated.h"

UINTERFACE(MinimalAPI)
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
	virtual void UseInteraction() {};
};
