#pragma once

#include "CoreMinimal.h"
#include "../../Item.h"
#include "PadlockNumber.generated.h"

UCLASS()
class SOPHIASFAULT_API APadlockNumber : public AItem
{
	GENERATED_BODY()
	
public:
	APadlockNumber();

	void UseInteraction();
};
