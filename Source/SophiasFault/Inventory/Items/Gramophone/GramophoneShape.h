#pragma once

#include "CoreMinimal.h"
#include "../../Item.h"
#include "../../../Interfaces/InteractiveInterface.h"
#include "GramophoneShape.generated.h"

/**
 * 
 */
UCLASS()
class SOPHIASFAULT_API AGramophoneShape : public AItem, public IInteractiveInterface
{
	GENERATED_BODY()
	
public:
	AGramophoneShape();

	virtual void UseInteraction() override;
};
