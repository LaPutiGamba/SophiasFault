#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OnActionInterface.generated.h"

UINTERFACE(MinimalAPI)
class UOnActionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SOPHIASFAULT_API IOnActionInterface
{
	GENERATED_BODY()

public:
	virtual void OnAction() {};
};
