#pragma once

#include "CoreMinimal.h"
#include "../../Item.h"
#include "GramophoneShape.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API AGramophoneShape : public AItem
{
	GENERATED_BODY()
	
public:
	AGramophoneShape();

	void UseInteraction();
};
