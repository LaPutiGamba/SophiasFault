#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TriggerInterface.generated.h"

UINTERFACE(MinimalAPI)
class UTriggerInterface : public UInterface
{
	GENERATED_BODY()
};

class SOPHIASFAULT_API ITriggerInterface
{
	GENERATED_BODY()

public:
	virtual void OnTriggerStart();
	virtual void OnTriggerEnd();
};
